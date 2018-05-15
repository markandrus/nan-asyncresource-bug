nan-asyncresource-bug
=====================

I'm not actually sure if this is a bug, and perhaps I'm using the Nan and libuv
APIs incorrectly; however, this is what I observe: if

* I have some class Bar that asynchronously creates instances of Foo, and
* I a construct a Bar asynchronously (e.g., in some `async function`), and then
* I use that Bar to create many instances of Foo in a loop, and, finally, if
* I repeat this process enough times to trigger garbage collection,

I eventually get an `EXC_BAD_ACCESS` in the destructor for AsyncResource (using
Node v9.8.0). Try it out yourself:

```
./node_modules/node-gyp rebuild
npm test
```
