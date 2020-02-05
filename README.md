# ext-fs
Extensions to nodejs's fs
## Build steps
In Command line, enter

```npm install```

### Debugging
Build addon with the following command:

```npm install --debug```

Open ```dev/index.js``` and type ```F5```

Open Visual Studio and attach to process ```node.exe```

## TODO:
* Exceptions:
```
ThrowException(Exception::TypeError(String::New("This is an error, oh yes."))) return Undefined(); 
```
1. Code (Directory Not found, No authorization, ...)
2. Text
3. Stacktrace
* In README describe steps to access methods like ```getFiles```. 
* Test: fileItem constructor, first item wstring data pointer -> results in Worker: is it the same pointer?