#include <unity.h>
#include "proto_handlers/proto_handler.hpp"

ProtoHelper helper = ProtoHelper();
ProtoHandler handler = ProtoHandler(&helper);

void setUp(void)
{
    helper = ProtoHelper();
    handler = ProtoHandler(&helper);
}

void tearDown(void)
{
}

void test_get_sequence_number(void)
{
    TEST_ASSERT_EQUAL(0, helper.sequence_number);
    TEST_ASSERT_EQUAL(helper.sequence_number, handler.GetSequenceNumber());

    // Check when new sequencenumber is received
    helper.GetNewSequenceNumber();
    TEST_ASSERT_NOT_EQUAL(0, helper.sequence_number);
    TEST_ASSERT_EQUAL(helper.sequence_number, handler.GetSequenceNumber());
}

void test_get_id(void)
{
    TEST_ASSERT_EQUAL(0, helper.id);
    const uint64_t value = 10;
    handler.SetId(value);
    TEST_ASSERT_EQUAL_INT64(value, helper.id);
}

void RUN_UNITY_TESTS()
{
    UNITY_BEGIN();
    RUN_TEST(test_get_sequence_number);
    RUN_TEST(test_get_id);
    UNITY_END();
}

int main(int argc, char **argv)
{
    RUN_UNITY_TESTS();
    return 0;
}