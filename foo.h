#ifndef FOO_H_
#define FOO_H_

#include <nan.h>

#include <atomic>

namespace addon {

class Foo
  : public Nan::AsyncResource
  , public Nan::ObjectWrap {
 public:
  Foo();

  static void Init(v8::Handle<v8::Object>);
  static Nan::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);

  static NAN_METHOD(Stop);

  static NAN_GETTER(Id);

  void Run();

 private: 
  static std::atomic<int> _last_id;

  int _id;
  uv_async_t _async;
  uv_loop_t* _loop;
};

}  // namespace addon

#endif  // FOO_H_
