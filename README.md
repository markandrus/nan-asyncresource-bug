nan-asyncresource-bug
=====================

I'm not exactly sure if this is a bug, but here is what I observe:

```
$ ./node_modules/node-gyp rebuild
$ lldb -e node
(lldb) run test
# ...
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x0)
    frame #0: 0x0000000100009322 node`node::EmitAsyncDestroy(v8::Isolate*, node::async_context) + 18
node`node::EmitAsyncDestroy:
->  0x100009322 <+18>: movq   (%rax), %rax
    0x100009325 <+21>: movq   0x37(%rax), %rax
    0x100009329 <+25>: movq   0x10f(%rax), %rdi
    0x100009330 <+32>: movsd  -0x8(%rbp), %xmm0         ; xmm0 = mem[0],zero
Target 0: (node) stopped.
(lldb) bt
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x0)
  * frame #0: 0x0000000100009322 node`node::EmitAsyncDestroy(v8::Isolate*, node::async_context) + 18
    frame #1: 0x0000000101eb713b addon.node`addon::Foo::~Foo() at nan.h:513 [opt]
    frame #2: 0x0000000101eb7124 addon.node`addon::Foo::~Foo() [inlined] addon::Foo::~Foo(this=0x0000000102fad4f0) at foo.h:10 [opt]
    frame #3: 0x0000000101eb711f addon.node`addon::Foo::~Foo() [inlined] addon::Foo::~Foo(this=0x0000000102fad4f0) at foo.h:10 [opt]
    frame #4: 0x0000000101eb711f addon.node`addon::Foo::~Foo(this=0x0000000102fad4f0) at foo.h:10 [opt]
    frame #5: 0x00000001004f3384 node`v8::internal::GlobalHandles::DispatchPendingPhantomCallbacks(bool) + 180
    frame #6: 0x00000001004f36b1 node`v8::internal::GlobalHandles::PostGarbageCollectionProcessing(v8::internal::GarbageCollector, v8::GCCallbackFlags) + 49
    frame #7: 0x0000000100509085 node`v8::internal::Heap::PerformGarbageCollection(v8::internal::GarbageCollector, v8::GCCallbackFlags) + 2533
    frame #8: 0x000000010050800d node`v8::internal::Heap::CollectGarbage(v8::internal::AllocationSpace, v8::internal::GarbageCollectionReason, v8::GCCallbackFlags) + 733
    frame #9: 0x00000001004c9c92 node`v8::internal::Factory::NewFillerObject(int, bool, v8::internal::AllocationSpace) + 82
    frame #10: 0x0000000100774125 node`v8::internal::Runtime_AllocateInNewSpace(int, v8::internal::Object**, v8::internal::Isolate*) + 101
    frame #11: 0x000026aa60b842fd
    frame #12: 0x000026aa60c381a1
    frame #13: 0x000026aa60d11d2d
    frame #14: 0x000026aa60c38016
    frame #15: 0x000026aa60c09cfc
    frame #16: 0x000026aa60b84239
    frame #17: 0x000026aa60b84101
    frame #18: 0x00000001004bf669 node`v8::internal::(anonymous namespace)::Invoke(v8::internal::Isolate*, bool, v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>, int, v8::internal::Handle<v8::internal::Object>*, v8::internal::Handle<v8::internal::Object>, v8::internal::Execution::MessageHandling) + 633
    frame #19: 0x00000001004bf893 node`v8::internal::Execution::TryCall(v8::internal::Isolate*, v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>, int, v8::internal::Handle<v8::internal::Object>*, v8::internal::Execution::MessageHandling, v8::internal::MaybeHandle<v8::internal::Object>*) + 227
    frame #20: 0x00000001005e1c9e node`v8::internal::Isolate::PromiseReactionJob(v8::internal::Handle<v8::internal::PromiseReactionJobInfo>, v8::internal::MaybeHandle<v8::internal::Object>*, v8::internal::MaybeHandle<v8::internal::Object>*) + 654
    frame #21: 0x00000001005e273c node`v8::internal::Isolate::RunMicrotasksInternal() + 1180
    frame #22: 0x00000001005e15e8 node`v8::internal::Isolate::RunMicrotasks() + 56
    frame #23: 0x000026aa60d06747
    frame #24: 0x000026aa60d06ee2
    frame #25: 0x000026aa60b84239
    frame #26: 0x000026aa60b84101
    frame #27: 0x00000001004bf669 node`v8::internal::(anonymous namespace)::Invoke(v8::internal::Isolate*, bool, v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>, int, v8::internal::Handle<v8::internal::Object>*, v8::internal::Handle<v8::internal::Object>, v8::internal::Execution::MessageHandling) + 633
    frame #28: 0x00000001004bf37c node`v8::internal::Execution::Call(v8::internal::Isolate*, v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>, int, v8::internal::Handle<v8::internal::Object>*) + 140
    frame #29: 0x000000010019b710 node`v8::Function::Call(v8::Local<v8::Context>, v8::Local<v8::Value>, int, v8::Local<v8::Value>*) + 384
    frame #30: 0x0000000100026bcd node`node::InternalCallbackScope::Close() + 253
    frame #31: 0x0000000100026ec3 node`node::InternalMakeCallback(node::Environment*, v8::Local<v8::Object>, v8::Local<v8::Function>, int, v8::Local<v8::Value>*, node::async_context) + 675
    frame #32: 0x0000000100026fb9 node`node::MakeCallback(v8::Isolate*, v8::Local<v8::Object>, char const*, int, v8::Local<v8::Value>*, node::async_context) + 169
    frame #33: 0x0000000101eb6077 addon.node`addon::Bar::Run() [inlined] Nan::AsyncResource::runInAsyncScope(this=<unavailable>, target=(val_ = 0x0000000102048c28), method=<unavailable>, argc=1, argv=0x0000000102048c20) at nan.h:551 [opt]
    frame #34: 0x0000000101eb604a addon.node`addon::Bar::Run(this=0x0000000104aaf9c0) at bar.cc:35 [opt]
    frame #35: 0x00000001019e77cb libuv.1.dylib`uv__async_io + 331
    frame #36: 0x00000001019fa41d libuv.1.dylib`uv__io_poll + 2061
    frame #37: 0x00000001019e7d43 libuv.1.dylib`uv_run + 371
    frame #38: 0x0000000100034b75 node`node::Start(v8::Isolate*, node::IsolateData*, int, char const* const*, int, char const* const*) + 661
    frame #39: 0x0000000100031e83 node`node::Start(uv_loop_s*, int, char const* const*, int, char const* const*) + 435
    frame #40: 0x0000000100031988 node`node::Start(int, char**) + 536
    frame #41: 0x00000001000012c4 node`start + 52
```
