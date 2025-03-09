#ifndef WEB_HPP
#define WEB_HPP

#include "radioData.hpp"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

extern const uint8_t index_html_start[] asm("_binary_data_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_data_index_html_end");

extern const uint8_t style_css_start[] asm("_binary_data_style_css_start");
extern const uint8_t style_css_end[] asm("_binary_data_style_css_end");

extern const uint8_t script_js_start[] asm("_binary_data_script_js_start");
extern const uint8_t script_js_end[] asm("_binary_data_script_js_end");

extern RadioData radioData;

AsyncWebServer server(80);

bool apMode = false; // Speichert, ob der ESP als Access Point läuft

String generateSensorData() {
    Serial.printf("Freier Heap = %u Bytes\n", ESP.getFreeHeap());
    DynamicJsonDocument doc(1024 * 5);
    Serial.printf("Freier Heap = %u Bytes\n", ESP.getFreeHeap());

    // AnalogToDigitalData
    doc["analogToDigitalData.stickLimitUpDown.min"] = radioData.analogToDigitalData.stickLimitUpDown.min;
    doc["analogToDigitalData.stickLimitUpDown.max"] = radioData.analogToDigitalData.stickLimitUpDown.max;
    doc["analogToDigitalData.stickLimitUpDown.center"] = radioData.analogToDigitalData.stickLimitUpDown.center;
    doc["analogToDigitalData.stickLimitUpDown.invert"] = radioData.analogToDigitalData.stickLimitUpDown.invert;

    doc["analogToDigitalData.stickLimitLeftRight.min"] = radioData.analogToDigitalData.stickLimitLeftRight.min;
    doc["analogToDigitalData.stickLimitLeftRight.max"] = radioData.analogToDigitalData.stickLimitLeftRight.max;
    doc["analogToDigitalData.stickLimitLeftRight.center"] = radioData.analogToDigitalData.stickLimitLeftRight.center;
    doc["analogToDigitalData.stickLimitLeftRight.invert"] = radioData.analogToDigitalData.stickLimitLeftRight.invert;

    doc["analogToDigitalData.longPressDurationMs"] = radioData.analogToDigitalData.longPressDurationMs;

    // // SensorToDigitalData
    // doc["sensorToDigitalData.angleLimitPitch.delta"] = radioData.sensorToDigitalData.angleLimitPitch.delta;
    // doc["sensorToDigitalData.angleLimitPitch.center"] = radioData.sensorToDigitalData.angleLimitPitch.center;
    // doc["sensorToDigitalData.angleLimitRoll.delta"] = radioData.sensorToDigitalData.angleLimitRoll.delta;
    // doc["sensorToDigitalData.angleLimitRoll.center"] = radioData.sensorToDigitalData.angleLimitRoll.center;
    // doc["sensorToDigitalData.seaLevelPressure"] = radioData.sensorToDigitalData.seaLevelPressure;

    // // ExpoData
    // doc["expoData.roll"] = radioData.expoData.roll;
    // doc["expoData.pitch"] = radioData.expoData.pitch;
    // doc["expoData.throttle"] = radioData.expoData.throttle;

    // // DualRateData
    // doc["dualRateData.roll"] = radioData.dualRateData.roll;
    // doc["dualRateData.pitch"] = radioData.dualRateData.pitch;
    // doc["dualRateData.throttle"] = radioData.dualRateData.throttle;

    // // TrimData
    // doc["trimData.roll"] = radioData.trimData.roll;
    // doc["trimData.pitch"] = radioData.trimData.pitch;

    // // MixerData
    // doc["mixerData.throttleToPitch"] = radioData.mixerData.throttleToPitch;

    // // FunctionToChannelData
    // for (int i = 0; i < SUPPORTED_CHANNELS; i++) {
    //     char key[32];
    //     sprintf(key, "functionToChannelData.invertChannel.%d", i);
    //     doc[key] = radioData.functionToChannelData.invertChannel[i];

    //     sprintf(key, "functionToChannelData.functionOnChannel.%d", i);
    //     doc[key] = radioData.functionToChannelData.functionOnChannel[i];

    //     sprintf(key, "functionToChannelData.upperLimitChannel.%d", i);
    //     doc[key] = radioData.functionToChannelData.upperLimitChannel[i];

    //     sprintf(key, "functionToChannelData.lowerLimitChannel.%d", i);
    //     doc[key] = radioData.functionToChannelData.lowerLimitChannel[i];
    // }

    // // RawData
    // doc["rawData.stickUpDown"] = radioData.rawData.stickUpDown;
    // doc["rawData.stickLeftRight"] = radioData.rawData.stickLeftRight;
    // doc["rawData.battery"] = radioData.rawData.battery;
    // doc["rawData.gyroPitch"] = radioData.rawData.gyroPitch;
    // doc["rawData.gyroRoll"] = radioData.rawData.gyroRoll;
    // doc["rawData.gyroYaw"] = radioData.rawData.gyroYaw;
    // doc["rawData.accelPitch"] = radioData.rawData.accelPitch;
    // doc["rawData.accelRoll"] = radioData.rawData.accelRoll;
    // doc["rawData.accelYaw"] = radioData.rawData.accelYaw;

    // AnalogData
    doc["analogData.stickUpDown"] = radioData.analogData.stickUpDown;
    doc["analogData.stickLeftRight"] = radioData.analogData.stickLeftRight;
    doc["analogData.battery"] = radioData.analogData.battery;
    doc["analogData.gyroPitch"] = radioData.analogData.gyroPitch;
    doc["analogData.gyroRoll"] = radioData.analogData.gyroRoll;
    doc["analogData.gyroYaw"] = radioData.analogData.gyroYaw;
    doc["analogData.accelPitch"] = radioData.analogData.accelPitch;
    doc["analogData.accelRoll"] = radioData.analogData.accelRoll;
    doc["analogData.accelYaw"] = radioData.analogData.accelYaw;
    doc["analogData.pitch"] = radioData.analogData.pitch;
    doc["analogData.roll"] = radioData.analogData.roll;
    doc["analogData.yaw"] = radioData.analogData.yaw;

    // // DigitalData
    // doc["digitalData.stickUpDown"] = radioData.digitalData.stickUpDown;
    // doc["digitalData.stickLeftRight"] = radioData.digitalData.stickLeftRight;
    // doc["digitalData.arm"] = radioData.digitalData.arm;
    // doc["digitalData.trim"] = radioData.digitalData.trim;
    // doc["digitalData.armEvent"] = radioData.digitalData.armEvent;
    // doc["digitalData.trimEvent"] = radioData.digitalData.trimEvent;
    // doc["digitalData.armLongPressEvent"] = radioData.digitalData.armLongPressEvent;
    // doc["digitalData.trimLongPressEvent"] = radioData.digitalData.trimLongPressEvent;
    // doc["digitalData.pitch"] = radioData.digitalData.pitch;
    // doc["digitalData.roll"] = radioData.digitalData.roll;
    // doc["digitalData.yaw"] = radioData.digitalData.yaw;
    // doc["digitalData.altitude"] = radioData.digitalData.altitude;
    // doc["digitalData.temperature"] = radioData.digitalData.temperature;

    // FunctionData
    doc["functionData.pitch"] = radioData.functionData.pitch;
    doc["functionData.roll"] = radioData.functionData.roll;
    doc["functionData.throttle"] = radioData.functionData.throttle;
    doc["functionData.armed"] = radioData.functionData.armed;
    doc["functionData.vTailLeft"] = radioData.functionData.vTailLeft;
    doc["functionData.vTailRight"] = radioData.functionData.vTailRight;

    // // ChannelData
    // for (int i = 0; i < 16; i++) {
    //     char key[32];
    //     sprintf(key, "channelData.channel.%d", i);
    //     doc[key] = radioData.channelData.channel[i];
    // }

    // TransmitterData
    doc["transmitterData.bindingState"] = radioData.transmitterData.bindingState;
    doc["transmitterData.selectedProtocol"] = radioData.transmitterData.selectedProtocol;
    doc["transmitterData.selectedSubProtocol"] = radioData.transmitterData.selectedSubProtocol;
    doc["transmitterData.rangeCheck"] = radioData.transmitterData.rangeCheck;
    doc["transmitterData.rxNum"] = radioData.transmitterData.rxNum;
    doc["transmitterData.powerValue"] = radioData.transmitterData.powerValue;

    // // Model Data
    // char modelNameStr[MODEL_NAME_LENGTH + 1];
    // memcpy(modelNameStr, radioData.modelData.modelName, MODEL_NAME_LENGTH);
    // modelNameStr[MODEL_NAME_LENGTH] = '\0';
    // doc["modelData.modelName"] = modelNameStr;

    String json;
    serializeJson(doc, json);

    // Log the generated JSON
    Serial.println("Generated JSON:");
    Serial.println(json);

    return json;
}
/*
// API-Anfragen verarbeiten
void handleSetRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, data);

    // AnalogToDigitalData
    if (doc.containsKey("analogToDigitalData.stickLimitUpDown.min")) radioData.analogToDigitalData.stickLimitUpDown.min = doc["analogToDigitalData.stickLimitUpDown.min"];
    if (doc.containsKey("analogToDigitalData.stickLimitUpDown.max")) radioData.analogToDigitalData.stickLimitUpDown.max = doc["analogToDigitalData.stickLimitUpDown.max"];
    if (doc.containsKey("analogToDigitalData.stickLimitUpDown.center")) radioData.analogToDigitalData.stickLimitUpDown.center = doc["analogToDigitalData.stickLimitUpDown.center"];
    if (doc.containsKey("analogToDigitalData.stickLimitUpDown.invert")) radioData.analogToDigitalData.stickLimitUpDown.invert = doc["analogToDigitalData.stickLimitUpDown.invert"];

    if (doc.containsKey("analogToDigitalData.stickLimitLeftRight.min")) radioData.analogToDigitalData.stickLimitLeftRight.min = doc["analogToDigitalData.stickLimitLeftRight.min"];
    if (doc.containsKey("analogToDigitalData.stickLimitLeftRight.max")) radioData.analogToDigitalData.stickLimitLeftRight.max = doc["analogToDigitalData.stickLimitLeftRight.max"];
    if (doc.containsKey("analogToDigitalData.stickLimitLeftRight.center")) radioData.analogToDigitalData.stickLimitLeftRight.center = doc["analogToDigitalData.stickLimitLeftRight.center"];
    if (doc.containsKey("analogToDigitalData.stickLimitLeftRight.invert")) radioData.analogToDigitalData.stickLimitLeftRight.invert = doc["analogToDigitalData.stickLimitLeftRight.invert"];

    if (doc.containsKey("analogToDigitalData.longPressDurationMs")) radioData.analogToDigitalData.longPressDurationMs = doc["analogToDigitalData.longPressDurationMs"];

    // SensorToDigitalData
    if (doc.containsKey("sensorToDigitalData.angleLimitPitch.delta")) radioData.sensorToDigitalData.angleLimitPitch.delta = doc["sensorToDigitalData.angleLimitPitch.delta"];
    if (doc.containsKey("sensorToDigitalData.angleLimitPitch.center")) radioData.sensorToDigitalData.angleLimitPitch.center = doc["sensorToDigitalData.angleLimitPitch.center"];
    if (doc.containsKey("sensorToDigitalData.angleLimitRoll.delta")) radioData.sensorToDigitalData.angleLimitRoll.delta = doc["sensorToDigitalData.angleLimitRoll.delta"];
    if (doc.containsKey("sensorToDigitalData.angleLimitRoll.center")) radioData.sensorToDigitalData.angleLimitRoll.center = doc["sensorToDigitalData.angleLimitRoll.center"];
    if (doc.containsKey("sensorToDigitalData.seaLevelPressure")) radioData.sensorToDigitalData.seaLevelPressure = doc["sensorToDigitalData.seaLevelPressure"];

    // ExpoData
    if (doc.containsKey("expoData.roll")) radioData.expoData.roll = doc["expoData.roll"];
    if (doc.containsKey("expoData.pitch")) radioData.expoData.pitch = doc["expoData.pitch"];
    if (doc.containsKey("expoData.throttle")) radioData.expoData.throttle = doc["expoData.throttle"];

    // DualRateData
    if (doc.containsKey("dualRateData.roll")) radioData.dualRateData.roll = doc["dualRateData.roll"];
    if (doc.containsKey("dualRateData.pitch")) radioData.dualRateData.pitch = doc["dualRateData.pitch"];
    if (doc.containsKey("dualRateData.throttle")) radioData.dualRateData.throttle = doc["dualRateData.throttle"];

    // TrimData
    if (doc.containsKey("trimData.roll")) radioData.trimData.roll = doc["trimData.roll"];
    if (doc.containsKey("trimData.pitch")) radioData.trimData.pitch = doc["trimData.pitch"];

    // MixerData
    if (doc.containsKey("mixerData.throttleToPitch")) radioData.mixerData.throttleToPitch = doc["mixerData.throttleToPitch"];

    // FunctionToChannelData
    for (int i = 0; i < SUPPORTED_CHANNELS; i++) {
        char key[32];
        sprintf(key, "functionToChannelData.invertChannel.%d", i);
        if (doc.containsKey(key)) radioData.functionToChannelData.invertChannel[i] = doc[key];

        sprintf(key, "functionToChannelData.functionOnChannel.%d", i);
        if (doc.containsKey(key)) radioData.functionToChannelData.functionOnChannel[i] = (RadioData::Function)doc[key];

        sprintf(key, "functionToChannelData.upperLimitChannel.%d", i);
        if (doc.containsKey(key)) radioData.functionToChannelData.upperLimitChannel[i] = doc[key];

        sprintf(key, "functionToChannelData.lowerLimitChannel.%d", i);
        if (doc.containsKey(key)) radioData.functionToChannelData.lowerLimitChannel[i] = doc[key];
    }

 
    // TransmitterData
    if (doc.containsKey("transmitterData.selectedProtocol")) radioData.transmitterData.selectedProtocol = doc["transmitterData.selectedProtocol"];
    if (doc.containsKey("transmitterData.selectedSubProtocol")) radioData.transmitterData.selectedSubProtocol = doc["transmitterData.selectedSubProtocol"];
    if (doc.containsKey("transmitterData.rangeCheck")) radioData.transmitterData.rangeCheck = doc["transmitterData.rangeCheck"];
    if (doc.containsKey("transmitterData.rxNum")) radioData.transmitterData.rxNum = doc["transmitterData.rxNum"];
    if (doc.containsKey("transmitterData.powerValue")) radioData.transmitterData.powerValue = (RadioData::PowerValue)doc["transmitterData.powerValue"];

    // Model Data
    if (doc.containsKey("modelData.modelName")) {
        strncpy(radioData.modelData.modelName, doc["modelData.modelName"], MODEL_NAME_LENGTH);
    }

    Serial.println("Einstellung aktualisiert:");
    serializeJson(doc, Serial);
    Serial.println();

    request->send(200, "application/json", "{\"status\":\"OK\"}");
}*/

void serveStaticFile(AsyncWebServerRequest *request, const char* mimeType, const uint8_t* fileData, size_t fileSize) {
    request->send_P(200, mimeType, fileData, fileSize);
}

void startAPMode() {
    Serial.println("\nStarte Access Point...");
    WiFi.softAP("ESP32_Setup", "12345678");
    Serial.print("AP IP-Adresse: ");
    Serial.println(WiFi.softAPIP());
    apMode = true;
}

void taskInitWeb()
{
    startAPMode();

    // HTML-Datei
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        serveStaticFile(request, "text/html", index_html_start, index_html_end - index_html_start);
    });

    // CSS-Datei
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        serveStaticFile(request, "text/css", style_css_start, style_css_end - style_css_start);
    });

    // JavaScript-Datei
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        serveStaticFile(request, "application/javascript", script_js_start, script_js_end - script_js_start);
    });

    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(200, "application/json", generateSensorData()); });
    //server.on("/set", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, handleSetRequest);


    server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!Update.hasError()) {
            request->send(200, "text/plain", "Update erfolgreich! Starte neu...");
            delay(1000);
            ESP.restart();
        } else {
            request->send(500, "text/plain", "Update fehlgeschlagen.");
        }
    }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!index) {
            Serial.printf("Update gestartet: %s\n", filename.c_str());
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
                Serial.println("Update Start fehlgeschlagen!");
            }
        }
        if (Update.write(data, len) != len) {
            Serial.println("Fehler beim Schreiben!");
        }
        if (final) {
            if (Update.end(true)) {
                Serial.println("Update abgeschlossen!");
            } else {
                Serial.println("Update fehlgeschlagen!");
            }
        }
    });

    server.begin();

}

#endif