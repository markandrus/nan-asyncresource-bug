#include <nan.h>

#include "bar.h"
#include "foo.h"

void init(v8::Handle<v8::Object> exports) {
  addon::Bar::Init(exports);
  addon::Foo::Init(exports);
}

NODE_MODULE(addon, init)
