// Broken - Lightweight unit testing for C++
//
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org>

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
