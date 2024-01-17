#include <Arduino.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb_common.h>
#include <proto/base_message.pb.h>

uint64_t id = 3;

void HandleMessage(const RemoteTestSite_Message message);
bool EncodeUnionmessage(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message);
void EncodeJoin(pb_ostream_t *stream, uint64_t new_id);
void EncodeMeasurement(pb_ostream_t *stream, RemoteTestSite_MeasurementType type, float value);
void EncodeUpdate(pb_ostream_t *stream, RemoteTestSite_MeasurementType type, RemoteTestSite_Timestamp frequency);
void EncodePing(pb_ostream_t *stream, RemoteTestSite_MeasurementType type, RemoteTestSite_Timestamp frequency);
void EncodeResponse(pb_ostream_t *stream, RemoteTestSite_ErrorCode code, String message);

void setup()
{
  Serial.begin(9600);
  uint8_t buffer[128];
  size_t message_length;
  bool status;

  /* Encode message */
  {
    /* Create base message. */
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.id = 3;
    message.has_id = true;

    /* Create output stream. */
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    /* Encode the message. */
    status = pb_encode(&stream, RemoteTestSite_Message_fields, &message);
    /* Check for any errors. */
    if (!status)
    {
      Serial.print("Encoding failed on base message: ");
      Serial.println(PB_GET_ERROR(&stream));
    }

    // EncodeJoin(&stream, 2);
    EncodeMeasurement(&stream, RemoteTestSite_MeasurementType_MEASUREMENT_TYPE_TEMPERATURE, 17.1);

    /* Get size of written bytes. */
    message_length = stream.bytes_written;
  }

  /* Decode message */
  {
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, RemoteTestSite_Message_fields, &message);

    /* Check for errors... */
    if (!status)
    {
      Serial.print("Decoding failed: ");
      Serial.println(PB_GET_ERROR(&stream));
    }

    HandleMessage(message);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}

void HandleMessage(const RemoteTestSite_Message message)
{
  /* Check if message is meant for this node */
  if (message.has_id)
  {
    Serial.print("Message id: ");
    Serial.println((int)message.id);
    if (id != message.id)
    {
      return;
    }
  }

  switch (message.which_function_info)
  {
  case RemoteTestSite_Message_join_tag:
    if (message.function_info.join.has_new_id)
    {
      Serial.print("Join message new id: ");
      Serial.println((int)message.function_info.join.new_id);
      id = message.function_info.join.new_id;
    }

    break;
  case RemoteTestSite_Message_measurement_tag:
    if (message.function_info.measurement.has_type && message.function_info.measurement.has_value)
    {
      Serial.print("Measurement of type: ");
      Serial.println((int)message.function_info.measurement.type);
      Serial.print("Value: ");
      Serial.println((float)message.function_info.measurement.value);
    }

    break;

  default:
    Serial.print("Function unknown: ");
    Serial.println((int)message.which_function_info);
    break;
  }
}

void EncodeJoin(pb_ostream_t *stream, uint64_t new_id)
{
  /* Create join message type. */
  RemoteTestSite_Join join = RemoteTestSite_Join_init_zero;
  join.new_id = new_id;
  join.has_new_id = true;

  bool status = EncodeUnionmessage(stream, RemoteTestSite_Join_fields, &join);

  /* Check for any errors. */
  if (!status)
  {
    Serial.print("Encoding failed on union: ");
    Serial.println(PB_GET_ERROR(stream));
  }
}

void EncodeMeasurement(pb_ostream_t *stream, RemoteTestSite_MeasurementType type, float value)
{
  /* Create join message type. */
  RemoteTestSite_Measurement measurement = RemoteTestSite_Measurement_init_zero;
  measurement.type = type;
  measurement.has_type = true;
  measurement.value = value;
  measurement.has_value = true;

  bool status = EncodeUnionmessage(stream, RemoteTestSite_Measurement_fields, &measurement);

  /* Check for any errors. */
  if (!status)
  {
    Serial.print("Encoding failed on union: ");
    Serial.println(PB_GET_ERROR(stream));
  }
}

void EncodeUpdate(pb_ostream_t *stream, RemoteTestSite_MeasurementType type, RemoteTestSite_Timestamp frequency);
void EncodePing(pb_ostream_t *stream, RemoteTestSite_MeasurementType type, RemoteTestSite_Timestamp frequency);
void EncodeResponse(pb_ostream_t *stream, RemoteTestSite_ErrorCode code, String message);

bool EncodeUnionmessage(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message)
{
  pb_field_iter_t iter;

  if (!pb_field_iter_begin(&iter, RemoteTestSite_Message_fields, message))
    return false;

  do
  {
    if (iter.submsg_desc == messagetype)
    {
      /* This is our field, encode the message using it. */
      if (!pb_encode_tag_for_field(stream, &iter))
        return false;

      return pb_encode_submessage(stream, messagetype, message);
    }
  } while (pb_field_iter_next(&iter));

  /* Didn't find the field for messagetype */
  return false;
}