// [Broken]
// Lightweight Unit Testing for C++.
//
// [License]
// Public Domain (Unlicense)

// [Guard]
#ifndef BROKEN_INTERNAL_H
#define BROKEN_INTERNAL_H

// [Dependencies - C]
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// If using Doxygen to document a source-code hide everything. Ideally this
// can be also done by a macro, but there is no global and widely used one.

//! \internal
//! \{

// ============================================================================
// [Broken - API]
// ============================================================================

struct BrokenAPI {
  //! Test entry point.
  typedef void (*Entry)(void);

  //! Test unit.
  struct Unit {
    const char* name;
    Entry entry;
    size_t finished;
    Unit* next;
  };

  //! Automatic unit registration by using static initialization.
  struct AutoUnit : Unit {
    inline AutoUnit(const char* _name, Entry _entry) {
      name = _name;
      entry = _entry;
      finished = false;
      next = NULL;

      BrokenAPI::add(this);
    }
  };

  //! Register a new test (called automatically by `AutoUnit` and `UNIT`).
  static void add(Unit* unit);

  //! Set output file to `file`.
  static void setOutputFile(FILE* file);

  //! Initialize `Broken` framework.
  //!
  //! Returns `true` if `run()` should be called.
  static int run(int argc, const char* argv[],
    Entry onBeforeRun = (Entry)NULL,
    Entry onAfterRun = (Entry)NULL);

  //! Log message, adds automatically new line if not present.
  static void info(const char* fmt, ...);
  //! Called on `EXPECT()` failure.
  static void fail(const char* file, int line, const char* fmt, ...);
};

// ============================================================================
// [Broken - Macros]
// ============================================================================

//! Define a unit.
//!
//! `_Name_` can only contain ASCII characters, numbers and underscore. It has
//! the same rules as identifiers in C and C++.
#define UNIT(_Name_) \
  static void unit_##_Name_##_entry(void); \
  \
  static ::BrokenAPI::AutoUnit unit_##_Name_##_autoinit( \
    #_Name_, unit_##_Name_##_entry); \
  \
  static void unit_##_Name_##_entry(void)

//! Informative message printed to stdout.
#define INFO(...) \
  ::BrokenAPI::info(__VA_ARGS__)

//! Expect `_Exp_` to be truthy, fail otherwise.
#define EXPECT(_Exp_, ...) \
  do { \
    if (!(_Exp_)) ::BrokenAPI::fail(__FILE__, __LINE__, __VA_ARGS__); \
  } while(0)

//! Optional, can be used to create main() that initializes `Broken` and runs
//! all/specified tests.
#define UNIT_MAIN(_AppString_) \
  int main(int argc, char* argv[]) { \
    bool shouldRun = ::BrokenAPI::init(argc, argv); \
    \
    BrokenAPI::info("%s", _AppString_ \
    if (shouldRun) {\
      BrokenAPI::run(); \
    } \
    \
    return 0; \
  }

//! \}

// [Guard]
#endif // BROKEN_INTERNAL_H
