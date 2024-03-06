#include <Arduino.h>
#include "connection_handler.hpp"
#include "message_handlers/RemoteTestSiteBasestation.hpp"
#include "comunication_devices/usr_lg206_p_adapter.hpp"
#include "proto_handlers/proto_handler.hpp"

// #define SensorNode

RemoteTestSiteBasestation message_handler = RemoteTestSiteBasestation();
UsrLg206PAdapter communication_device = UsrLg206PAdapter();
ProtoHandler proto_handler = ProtoHandler();

ConnectionHandler connection_handler = ConnectionHandler(&message_handler, &communication_device, &proto_handler);

void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200, SERIAL_8N1);

    while (!connection_handler.Begin())
    {
        Serial.println("Could not setup communication module");
        delay(1000);
    }

#ifdef SensorNode
    Serial.println("Starting connection");
    while (!connection_handler.Connect(0))
    {
        Serial.println("Connection could not be established");
        delay(1000);
    }
    Serial.println("Connection established");
#endif

    Serial.println("Setup complete");
}

void loop()
{

#ifdef SensorNode
    RemoteTestSite_Message message = RemoteTestSite_Message_init_zero;
    message.which_function_info = RemoteTestSite_Message_measurement_tag;

    message.function_info.measurement.has_timestamp = true;
    message.function_info.measurement.has_type = true;
    message.function_info.measurement.has_value = true;
    message.function_info.measurement.timestamp.seconds = millis() / 1000;
    message.function_info.measurement.type = RemoteTestSite_MeasurementType_MEASUREMENT_TYPE_TEMPERATURE;
    message.function_info.measurement.value = 20;

    connection_handler.Write(message);
    delay(2000);
#else
    if (connection_handler.IsConnected())
    {
        if (connection_handler.Available())
        {
            connection_handler.Read();
        }
    }
    else
    {
        connection_handler.WaitForConnection();
    }

#endif
}