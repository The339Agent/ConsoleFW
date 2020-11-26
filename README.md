# ConsoleFW

## Introduction

[ConsoleFW](https://github.com/UnnamedDeveloper/ConsoleFW) (*CFW*) is a C/C++ library for console application development using [nCurses](https://no.wikipedia.org/wiki/Ncurses).

*CFW* is written using the C99 standard. It is currently not being tested on compilers, so compatibility must be checked by yourself.

*CFW* supports Linux and other Unix-like systems.

*CFW* is licensed under the [Apache-2.0 license](LICENSE).

## Building ConsoleFW

### Dependencies

*CFW* requires the following to compile:
- [nCurses](https://no.wikipedia.org/wiki/Ncurses)

### Build ConsoleFW

After downloading the *CFW* sources to your machine, you will need to generate the Makefiles to build the library with CMake.

```
cd <cfw-root-dir>
mkdir build
cd build
cmake ..
```

If you want to compile *CFW* as a shared library, you need to set the `BUILD_SHARED_LIBS` flag.

```
cmake -DBUILD_SHARED_LIBS=ON ..
```

After generating the Makefiles, you're ready to compile *CFW*. To compile, run the following command.

```
make .
```

### Install ConsoleFW

To install *CFW*, you will need to download the source files of *CFW* to your machine. After downloading the source, you will need to generate the Makefiles with CMake.

```
cd <cfw-root-dir>
mkdir build
cd build
cmake -DCFW_INSTALL=ON ..
```

The `CFW_INSTALL` flag tells CMake to generate the Makefiles to install *CFW* as well.

After generating the Makefiles, you have to build and install *CFW*.

```
make install .
```

## Using ConsoleFW

### CMake

To use *CFW* with CMake, you will need do firstly download the source files of *CFW*.

Changing your own `CMakeLists.txt` file a little bit will make you able to link *CFW* to your own project.

Add the source directory of *CFW* as a subdirectory to your `CMakeLists.txt`.

```cmake
add_subdirectory(path/to/cfw)
```

After adding *CFW* to your `CMakeLists.txt` file, link your application to the `cfw` target.

```cmake
target_link_libraries(myapp cfw)
```

Now, you can add *CFW* to your source files like so.

```c
#include <CFW/cfw.h>
```

### Precompiled-binaries

To use *CFW* with the precompiled-binaries, you will need the header files as well as the binaries of *CFW*.

Add the binary to your application.

```cmake
target_sources(myapp PRIVATE path/to/libcfw.(a/so/exe/dll))
```

After adding the binary, add the header files of *CFW* as well.

```cmake
target_include_directories(myapp PRIVATE path/to/cfw/headers)
```

If the binaries are shared library binaries, you will also need to set the `CFW_DLL` definition to tell *CFW* that it is a shared library.

```cmake
target_compile_definitions(myapp CFW_DLL)
```
