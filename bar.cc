#include "bar.h"

#include "foo.h"

using addon::Bar;
using addon::Foo;

Nan::Persistent<v8::Function> Bar::constructor;
std::atomic<int> Bar::_last_id = {0};

Bar::Bar()
  : _id(_last_id.fetch_add(1))
  , _loop(uv_default_loop())
{
  uv_async_init(_loop, &_async, [](uv_async_t* handle) {
    auto self = static_cast<Bar*>(handle->data);
    self->Run();
  });
  _async.data = this;
}

void Bar::Run() {
  Nan::HandleScope scope;
  if (_should_stop) {
    uv_close(reinterpret_cast<uv_handle_t*>(&_async), [](uv_handle_t* handle) {
      auto self = static_cast<Bar*>(handle->data);
      self->Unref();
    });
    return;
  }
  while (_foos_to_create.fetch_sub(1)) {
    v8::Local<v8::Value> foo = Nan::NewInstance(Nan::New(Foo::constructor), 0, nullptr).ToLocalChecked();
    _async_resource->runInAsyncScope(handle(), "onfoo", 1, &foo);
  }
}

NAN_METHOD(Bar::New) {
  auto self = new Bar();
  self->Wrap(info.This());
  self->Ref();
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Bar::CreateFoo) {
  auto self = Nan::ObjectWrap::Unwrap<Bar>(info.Holder());
  self->_foos_to_create.fetch_add(1);
  uv_async_send(&self->_async);
}

NAN_METHOD(Bar::Stop) {
  auto self = Nan::ObjectWrap::Unwrap<Bar>(info.Holder());
  self->_should_stop = true;
  uv_async_send(&self->_async);
}

NAN_GETTER(Bar::Id) {
  auto self = Nan::ObjectWrap::Unwrap<Bar>(info.Holder());
  info.GetReturnValue().Set(Nan::New(self->_id));
}

void Bar::Unref() {
  Nan::HandleScope scope;
  delete _async_resource;
  _async_resource = nullptr;
  Nan::ObjectWrap::Unref();
}

void Bar::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Bar").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("id").ToLocalChecked(), Id, nullptr);
  Nan::SetPrototypeMethod(tpl, "createFoo", CreateFoo);
  Nan::SetPrototypeMethod(tpl, "stop", Stop);
  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Bar").ToLocalChecked(), tpl->GetFunction());
}
