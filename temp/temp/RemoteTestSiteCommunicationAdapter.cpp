// #include "RemoteTestSiteCommunicationAdapter.hpp"
// #include "IRemoteTestSiteController.hpp"
// #include <pb_encode.h>
// #include <pb_decode.h>
// #include <pb_common.h>

// RemoteTestSiteCommunicationAdapter::RemoteTestSiteCommunicationAdapter(IRemoteTestSiteController *const controller)
// {
//     this->controller = controller;
//     // All ids must initialize with 0 and be given an id when joining network
//     this->id = 0;
// }

// pb_ostream_t RemoteTestSiteCommunicationAdapter::CreateOutputStream(uint8_t *const message_buffer, const size_t message_buffer_size)
// {
//     return pb_ostream_from_buffer(message_buffer, message_buffer_size);
// }
// pb_istream_t RemoteTestSiteCommunicationAdapter::CreateInputStream(const uint8_t *const message_buffer, const size_t message_length)
// {
//     return pb_istream_from_buffer(message_buffer, message_length);
// }

// bool RemoteTestSiteCommunicationAdapter::EncodeBaseMessage(pb_ostream_t *const stream, const uint64_t destination_id)
// {
//     /* Create base message. */
//     RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
//     message.sender_id = this->id;
//     message.has_sender_id = true;

//     /* Encode the message. */
//     return pb_encode(stream, RemoteTestSite_Message_fields, &message);
// }

// bool RemoteTestSiteCommunicationAdapter::HandleMessage(pb_istream_t *const stream)
// {
//     RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;

//     /* Now we are ready to decode the message. */
//     bool status = pb_decode(stream, RemoteTestSite_Message_fields, &message);

//     /* Check for errors... */
//     if (!status)
//     {
//         return false;
//     }

//     /* Check if message is meant for this node */
//     if (message.has_target_id)
//     {
//         if (this->id != message.target_id)
//         {
//             /* Message not meant for this node. */
//             return true;
//         }
//     }

//     switch (message.which_function_info)
//     {
//     case RemoteTestSite_Message_measurement_tag:
//         if (message.function_info.measurement.has_type && message.function_info.measurement.has_value)
//         {
//             return this->controller->Measurement(message.function_info.measurement.type, message.function_info.measurement.value);
//         }

//         break;
//     case RemoteTestSite_Message_update_tag:
//         // TODO: make sitch for update types
//         return this->controller->UpdateFrequency(message.function_info.update.UpdateType.update_frequency.type, message.function_info.update.UpdateType.update_frequency.frequency);
//         break;
//     case RemoteTestSite_Message_response_tag:
//         if (message.function_info.response.has_response_code)
//         {
//             return controller->Response(message.function_info.response.response_code);
//         }

//         break;

//     default:
//         return this->controller->UnknownMessageType(message.which_function_info);
//         break;
//     }
//     return false;
// }

// void RemoteTestSiteCommunicationAdapter::SetId(uint64_t new_id)
// {
//     this->id = new_id;
// }

// bool RemoteTestSiteCommunicationAdapter::EncodeMeasurement(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_MeasurementType type, const float value)
// {
//     EncodeBaseMessage(stream, destination_id);

//     /* Create measurement message type. */
//     RemoteTestSite_Measurement measurement = RemoteTestSite_Measurement_init_zero;
//     measurement.type = type;
//     measurement.has_type = true;
//     measurement.value = value;
//     measurement.has_value = true;

//     return EncodeUnionmessage(stream, RemoteTestSite_Measurement_fields, &measurement);
// }

// bool RemoteTestSiteCommunicationAdapter::EncodeUpdate(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_MeasurementType type, const RemoteTestSite_Timestamp frequency)
// {
//     EncodeBaseMessage(stream, destination_id);

//     /* Create update message type. */
//     RemoteTestSite_Update update = RemoteTestSite_Update_init_zero;
//     RemoteTestSite_Update_UpdateFrequency update_frequency = RemoteTestSite_Update_UpdateFrequency_init_zero;
//     update_frequency.type = type;
//     update_frequency.has_type = true;
//     update_frequency.frequency = frequency;
//     update_frequency.has_frequency = true;
//     EncodeUnionmessage(stream, RemoteTestSite_Update_UpdateFrequency_fields, &update_frequency);

//     return EncodeUnionmessage(stream, RemoteTestSite_Update_fields, &update);
// }

// bool RemoteTestSiteCommunicationAdapter::EncodeResponse(pb_ostream_t *const stream, const uint64_t destination_id, const RemoteTestSite_ResponseCode code)
// {
//     EncodeBaseMessage(stream, destination_id);

//     /* Create response message type. */
//     RemoteTestSite_Response response = RemoteTestSite_Response_init_zero;
//     response.response_code = code;
//     response.has_response_code = true;

//     return EncodeUnionmessage(stream, RemoteTestSite_Response_fields, &response);
// }

// bool RemoteTestSiteCommunicationAdapter::EncodeUnionmessage(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message)
// {

//     pb_field_iter_t iter;

//     if (!pb_field_iter_begin(&iter, RemoteTestSite_Message_fields, message))
//         return false;

//     do
//     {
//         if (iter.submsg_desc == messagetype)
//         {
//             /* This is our field, encode the message using it. */
//             if (!pb_encode_tag_for_field(stream, &iter))
//             {
//                 return false;
//             }

//             return pb_encode_submessage(stream, messagetype, message);
//         }
//     } while (pb_field_iter_next(&iter));

//     /* Didn't find the field for messagetype */
//     return false;
// }
