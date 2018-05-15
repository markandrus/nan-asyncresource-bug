#ifndef BAR_H_
#define BAR_H_

#include <nan.h>

#include <atomic>

namespace addon {

class Bar
  : public Nan::AsyncResource
  , public Nan::ObjectWrap {
 public:
   Bar();

   static void Init(v8::Handle<v8::Object>);
   static Nan::Persistent<v8::Function> constructor;
   static NAN_METHOD(New);

   static NAN_METHOD(CreateFoo);
   static NAN_METHOD(Stop);

   static NAN_GETTER(Id);

   void Run();

 private:
   static std::atomic<int> _last_id;

   int _id;
   uv_async_t _async;
   uv_loop_t* _loop;
   std::atomic<int> _foos_to_create = {0};
   std::atomic<bool> _should_stop = {false};
};

}  // namespace addon

#endif  // BAR_H_
