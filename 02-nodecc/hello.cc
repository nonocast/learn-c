#include <node.h>
extern "C" int add(int, int);

namespace __first__ {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "hello world"));
}

void calc(const FunctionCallbackInfo<Value>& args) {
  args.GetReturnValue().Set(add(1, 2));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
  NODE_SET_METHOD(exports, "calc", calc);

}

NODE_MODULE(addon, init)

}  // namespace __first__