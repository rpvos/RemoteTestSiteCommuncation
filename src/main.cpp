#include <Arduino.h>
#include "RemoteTestSiteBasestation.hpp"
#include "pb_encode.h"
#include "pb_decode.h"

RemoteTestSiteBasestation basestation = RemoteTestSiteBasestation();

void setup()
{
  Serial.begin(9600);
  uint8_t buffer[256] = {0};

  pb_ostream_t stream = basestation.communication_adapter.CreateOutputStream(buffer, 256);
  if (!basestation.communication_adapter.EncodePing(&stream, 0))
  {
    Serial.println("Encode failed");
    Serial.println(PB_GET_ERROR(&stream));
  }

  pb_istream_t istream = basestation.communication_adapter.CreateInputStream(buffer, stream.bytes_written);
  if (!basestation.communication_adapter.HandleMessage(&istream))
  {
    Serial.println("Handle failed");
    Serial.println(PB_GET_ERROR(&stream));
  }

  // pb_ostream_t stream = basestation.communication_adapter.CreateOutputStream(buffer);
  // if (!basestation.communication_adapter.EncodeJoin(&stream, 0, 5))
  // {
  //   Serial.println("Encode failed");
  // }

  // pb_istream_t istream = basestation.communication_adapter.CreateInputStream(buffer, strlen((char *)buffer));
  // if (!basestation.communication_adapter.HandleMessage(&istream))
  // {
  //   Serial.println("Decode failed");
  // }

  // basestation.communication_adapter.EncodeJoin(&stream, 5, 1);

  // EncodeJoin(&stream, 2);
  // EncodeMeasurement(&stream, RemoteTestSite_MeasurementType_MEASUREMENT_TYPE_TEMPERATURE, 17.1);
  // RemoteTestSite_Timestamp timestamp = RemoteTestSite_Timestamp_init_zero;
  // timestamp.seconds = 123;
  // timestamp.nanos = 456;
  // EncodeUpdate(&stream, RemoteTestSite_MeasurementType_MEASUREMENT_TYPE_TEMPERATURE, timestamp);
  // EncodePing(&stream);
  // EncodeResponse(&stream, RemoteTestSite_ErrorCode_ERROR_CODE_OK);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
