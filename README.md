Broken
======

Lightweight Unit Testing for C++.

  * [Official Repository (kobalicek/broken)](https://github.com/kobalicek/broken)
  * [Public Domain](http://unlicense.org/)

Introduction
------------

`Broken` is a lightweight framework that can be used to unit-test your C++ code. It has been designed to be lightweight so it can be shipped with the source code it tests. A typical use-case of the framework is to compile an extra version of your library or executable that is statically compiled with the code it tests. This provides some advantages - internals of the library can be easily tested and all issues related to a static compilation of the code can be eliminated.

`Broken` test-set is a single executable that can run one or multiple tests, which can be specified as command line options. This can be handy when one test corrupts some data that automatically breaks another test. Running all tests together simplifies continuous integration, running tests individually provides more control and checks.

Design Goals
------------

  - Lightweight, to be distributed with the source code it tests.
  - Do not use C++ STL library or any APIs that allocate memory.
  - Do not depend on C++ exception handling or dynamic casts.
  - Do tests your way, the framework just helps with the boilerplate.
  - Minimum command line interface to help listing and running tests.

Documentation
-------------

`Broken` defines only 3 macros:

  - `UNIT(_Name_)` - Use this to create a new unit test. `_Name_` can only contain ASCII characters, numbers and underscore. It has the same rules as identifiers in C and C++. Dashes `-` are automatically converted to underscores `_` when specifying a test from command line.

  - `INFO(...)` - Print an informative message to `stdout` or other `FILE*` if configured.

  - `EXPECT(_Ext_, ...)` - Expect `_Ext_` to be truthy, fail otherwise. This is the main macro used during testing. If the expression `_Exp_` converts to `false` your unit test failed and you will receive a report.

Installation
------------

There is nothing to be installed to use `Broken`. Just copy the files `broken.h`, `broken.cpp` and optionally `broken_main.cpp` somewhere to your project and use it in your unit-test enabled build target. The preferred location of these files is in a source subdirectory called `test`, but it's not required, just a convention.

Beginners Guide
---------------

Let's start with a simple function that you plan to test and assume that it's stored in a `.cpp` file in our project.

```C++
static bool safeToMul(uint32_t a, uint32_t b) {
  uint64_t result = static_cast<uint64_t>(a) * b;
  return result < 0xFFFFFFFFu;
}
```

This function contains a bug of course, but we want to explore the bug by running the unit-test. `Broken` doesn't dictate the way how the tests should be written, but there are in general two possibilities - put your test into the `.cpp` implementation file or create a new file with `_test.cpp` suffix (you can use any other suffix, `_test` is just used in other projects). I would go with a test inside the implementation file as it allows to test also internals available only in the file itself.

As a convention there should be defined macro when building the unit-test itself. The name is by convention `YOUR_PROJECT_NAME_TEST`, but can be anything else. The example below is a complete example where `BROKEN_EXAMPLE_TEST` is used to indicate it's a unit-test build:

```C++
#include <stdint.h>
#include "./broken.h"

static bool safeToMul(uint32_t a, uint32_t b) {
  uint64_t result = static_cast<uint64_t>(a) * b;
  return result < 0xFFFFFFFFu;
}

#if defined(BROKEN_EXAMPLE_TEST)
UNIT(base_safe_to_mul) {
  EXPECT_TRUE(safeToMul(0, 0)); // Safe to mul 0 * 0.
  EXPECT_TRUE(safeToMul(1, 1)); // Safe to mul 1 * 1.

  EXPECT_FALSE(safeToMul(0xFFFFFFFF, 2)); // Unsafe as it overflows, expected FALSE.

  EXPECT_TRUE(safeToMul(0xFFFFFFFF, 0)); // Doesn't overflow.
  EXPECT_TRUE(safeToMul(0xFFFFFFFF, 1)); // This test fails (there is a bug in the code)!
}
#endif // BROKEN_EXAMPLE_TEST
```

Well, to make the test executable working it's required to provide an entry point. If you copied also `broken_main.cpp` there is nothing to do, otherwise you have to create a file that will define an application entry point and call `BrokenAPI::run()`:

```C++
#include "./broken.h"

int main(int argc, const char* argv[]) {
  return BrokenAPI::run(argc, argv);
}
```

That's it. If you compile the unit-test executable you should be able to run it. The output should look like:

```
Running base_safe_to_mul
  Failed! Doesn't overflow
  File: ..\broken_example.cpp (Line: 15)
```

Since there is a bug in the sample test-case it shows it, including the message associated with `EXPECT` and file/line information. If you fix the code (quiz for you) it should print something similar to:

```
Running base_safe_to_mul

Success:
  All tests passed!
```

`Broken` package contains a simple `CMakeLists.txt` that can be used by a CMake to generate project files to run the example shown above. Integration with your project and build system is purely on you.

Command Line
------------

The unit-test executable provides several command line arguments:

  - `--help` - Display all command line arguments supported.
  - `--list` - List all tests.
  - `--run-some-test` - Run `some_test` only, specifying more tests works as expected.
  - `--run-some-*` - Run all tests matching `some-*`, for simplicity wildcard matching is only supported if it is specified at the end of the string.
  - `--run-all` - Run all tests (enabled by default).
