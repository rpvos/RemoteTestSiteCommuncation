#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void simple_test(void)
{
    TEST_ASSERT_EQUAL(33, 33);
}

void RUN_UNITY_TESTS()
{
    UNITY_BEGIN();
    RUN_TEST(simple_test);
    UNITY_END();
}

int main(int argc, char **argv)
{
    RUN_UNITY_TESTS();
    return 0;
}
