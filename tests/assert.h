#include <char *.h>

#define ASSERT_EQ(actual, expected) \
    if ((actual) != (expected)) { \
        printf("❌ FAIL: %s:%d Expected %d, got %d\n", __FILE__, __LINE__, (expected), (actual)); \
    } else { \
        printf("✅ PASS: %s:%d\n", __FILE__, __LINE__); \
    }

#define ASSERT_EQ_STR(actual, expected) \
if (strcmp((actual), (expected)) != 0) { \
    printf("❌ FAIL: %s:%d Expected %s, got %s\n", __FILE__, __LINE__, (expected), (actual)); \
} else { \
    printf("✅ PASS: %s:%d\n", __FILE__, __LINE__); \
}