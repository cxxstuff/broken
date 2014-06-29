#include <stdint.h>
#include "./broken.h"

static bool safeToMul(uint32_t a, uint32_t b) {
  uint64_t result = static_cast<uint64_t>(a) * b;
  return result < 0xFFFFFFFFU;
}

#if defined(BROKEN_EXAMPLE_TEST)
UNIT(base_safe_to_mul) {
  EXPECT(safeToMul(0, 0), "Should allow 0 * 0");
  EXPECT(safeToMul(1, 1), "Should allow 1 * 1");

  EXPECT(safeToMul(0xFFFFFFFF, 0), "Doesn't overflow");
  EXPECT(safeToMul(0xFFFFFFFF, 1), "Doesn't overflow"); // This test fails!

  EXPECT(!safeToMul(0xFFFFFFFF, 2), "Unsafe as it overflows");
}
#endif // BROKEN_EXAMPLE_TEST
