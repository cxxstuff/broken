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

// Hide everything when using Doxygen. Ideally this can be protected by a macro,
// but there is not globally and widely used one across multiple projects.

//! \internal
//! \{

// ============================================================================
// [Broken - API]
// ============================================================================

struct BrokenAPI {
  //! Entry point of a unit test defined by `UNIT` macro.
  typedef void (*Entry)(void);

  //! Test defined by `UNIT` macro.
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

  //! Register a new unit test (called automatically by `AutoUnit` and `UNIT`).
  static void add(Unit* unit);

  //! Set output file to a `file`.
  static void setOutputFile(FILE* file);

  //! Set the current context to `file` and `line`.
  //!
  //! This is called by `EXPECT` macro to set the correct `file` and `line`,
  //! because `EXPECT` macro internally calls `expect()` function, which does
  //! change the original file & line to non-interesting `broken.h`.
  static void setContext(const char* file, int line);

  //! Initialize `Broken` framework.
  //!
  //! Returns `true` if `run()` should be called.
  static int run(int argc, const char* argv[],
    Entry onBeforeRun = (Entry)NULL,
    Entry onAfterRun = (Entry)NULL);

  //! Used internally by `EXPECT` macro.
  template<typename T>
  static void expect(const T& exp, const char* fmt = NULL, ...) {
    if (exp)
      return;

    va_list ap;
    va_start(ap, fmt);
    fail(fmt, ap);
    va_end(ap);
  }

  //! Log message, adds automatically new line if not present.
  static void info(const char* fmt, ...);
  //! Called on `EXPECT()` failure.
  static void fail(const char* fmt, va_list ap);
};

// ============================================================================
// [Broken - Macros]
// ============================================================================

//! Define a unit test.
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

//! Informative message printed to `stdout`.
#define INFO(...) \
  ::BrokenAPI::info(__VA_ARGS__)

//! Expect `_Exp_` to be true or evaluates to true, fail otherwise.
#define EXPECT(...) \
  do { \
    ::BrokenAPI::setContext(__FILE__, __LINE__); \
    ::BrokenAPI::expect(__VA_ARGS__); \
  } while(0)

//! \}

// [Guard]
#endif // BROKEN_INTERNAL_H
