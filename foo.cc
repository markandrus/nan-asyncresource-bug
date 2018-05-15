#include "foo.h"

using addon::Foo;

Nan::Persistent<v8::Function> Foo::constructor;
std::atomic<int> Foo::_last_id = {0};

Foo::Foo()
  : Nan::AsyncResource("Foo")
  , _id(_last_id.fetch_add(1))
  , _loop(uv_default_loop())
{
  uv_async_init(_loop, &_async, [](uv_async_t* handle) {
    auto self = static_cast<Foo*>(handle->data);
    self->Run();
  });
  _async.data = this;
}

void Foo::Run() {
  uv_close(reinterpret_cast<uv_handle_t*>(&_async), [](uv_handle_t* handle) {
    auto self = static_cast<Foo*>(handle->data);
    self->Unref();
  });
}

NAN_METHOD(Foo::New) {
  auto self = new Foo();
  self->Wrap(info.This());
  self->Ref();
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Foo::Stop) {
  auto self = Nan::ObjectWrap::Unwrap<Foo>(info.Holder());
  uv_async_send(&self->_async);
}

NAN_GETTER(Foo::Id) {
  auto self = Nan::ObjectWrap::Unwrap<Foo>(info.Holder());
  info.GetReturnValue().Set(Nan::New(self->_id));
}

void Foo::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Foo").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("id").ToLocalChecked(), Id, nullptr);
  Nan::SetPrototypeMethod(tpl, "stop", Stop);
  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Foo").ToLocalChecked(), tpl->GetFunction());
}
