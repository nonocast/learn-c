hello.cc
```
#include <node.h>

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

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(addon, init)

}  // namespace __first__
```

逻辑上等同于,
```
function hello() {
  return "hello world";
}

exports.hello = hello;
```

编译 (target: hello.node)
```
node-gyp configure
node-gyp build
```

运行
```
$ node
> const addon = require('./')
> addon.hello()
```
前提的package.json中,  
```
"main": "build/Release/hello",
```


CLIENT
------
mkdir 02-nodecc-client && cd $_
yarn init
npm link ../02-nodecc

app.js
```
const addon = require('02-nodecc')

console.log(addon.hello());
```
> hello world

在04-hello-cc继续补充如何通过nodejs调用C的so
```
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
```

然后在binding.gyp设定library参数，其实懂了原理，这些就显而易见了
```
{
  "targets": [{
    "target_name": "hello",
    "sources": [
      "hello.cc"
    ],
    "libraries": [
      "-L/Users/nonocast/lib",
      "-lcalc"
    ]
  }]
}
```