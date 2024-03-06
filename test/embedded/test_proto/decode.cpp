#include "proto_handlers/proto_handler.hpp"
#include <unity.h>
#include <Arduino.h>

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void print_bytes(uint8_t *data, const size_t bytes_length)
{
    Serial.print('{');
    for (size_t i = 0; i < bytes_length; i++)
    {
        Serial.print("0x");
        Serial.print(data[i], HEX);
        if (i < bytes_length - 1)
        {
            Serial.print(',');
        }
    }
    Serial.println('}');
}

void test_decode_message()
{
    uint8_t data[] = {0x8, 0x0, 0x10, 0x0, 0x18, 0xF6, 0xD5, 0x9D, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0x28, 0xAD, 0xAB, 0xA9, 0xC8, 0x1, 0x30, 0x1};

    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    ProtoHandler handler = ProtoHandler(0, 0);
    bool succes = handler.DecodeMessage(message, data, sizeof(data) / sizeof(data[0]));

    TEST_ASSERT_TRUE(succes);
}

void test_encode_and_decode_measurement(void)
{
    const int value = 10;

    ProtoHandler handler = ProtoHandler(0, 0);
    const size_t data_size = 192;
    uint8_t data[data_size] = {0};
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_measurement_tag;

    RemoteTestSite_Measurement measurement = RemoteTestSite_Measurement_init_zero;
    measurement.has_value = true;
    measurement.value = value;

    message.function_info.measurement = measurement;

    size_t bytes = handler.EncodeMessage(data, data_size, message);
    TEST_ASSERT_TRUE(bytes);

    RemoteTestSite_Message message2 = RemoteTestSite_Message_init_zero;
    bool succes2 = handler.DecodeMessage(message2, data, bytes);
    TEST_ASSERT_TRUE(succes2);
    TEST_ASSERT_EQUAL(RemoteTestSite_Message_measurement_tag, message2.which_function_info);
    TEST_ASSERT_TRUE(message2.function_info.measurement.has_value);
    TEST_ASSERT_EQUAL(value, message2.function_info.measurement.value);
}

void test_encode_and_decode_syn(void)
{
    ProtoHandler handler = ProtoHandler(0, 0);
    const size_t data_size = 192;
    uint8_t data[data_size] = {0};
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_syn_tag;

    size_t bytes = handler.EncodeMessage(data, data_size, message);
    TEST_ASSERT_TRUE(bytes);

    RemoteTestSite_Message message2 = RemoteTestSite_Message_init_zero;
    bool succes2 = handler.DecodeMessage(message2, data, bytes);
    TEST_ASSERT_TRUE(succes2);
    TEST_ASSERT_EQUAL(RemoteTestSite_Message_syn_tag, message2.which_function_info);
}

void test_decode_syn(void)
{
    ProtoHandler handler = ProtoHandler(0, 0);

    uint8_t syn_message[] = {0x8, 0x0, 0x10, 0x0, 0x18, 0xF2, 0xF5, 0xD8, 0x86, 0x1, 0x28, 0xAD, 0xAB, 0xA9, 0xC8, 0x1, 0x30, 0x1};
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    bool succes = handler.DecodeMessage(message, syn_message, sizeof(syn_message) / sizeof(syn_message[0]));
    TEST_ASSERT_TRUE(succes);
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_decode_syn);
    RUN_TEST(test_encode_and_decode_syn);
    RUN_TEST(test_encode_and_decode_measurement);
    RUN_TEST(test_decode_message);
    UNITY_END();
}

void loop()
{
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}