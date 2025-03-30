#pragma once

#include "radioData.hpp"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Update.h>
#include <AsyncTCP.h>

extern const uint8_t index_html_start[] asm("_binary_data_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_data_index_html_end");
extern const uint8_t style_css_start[] asm("_binary_data_style_css_start");
extern const uint8_t style_css_end[] asm("_binary_data_style_css_end");
extern const uint8_t script_js_start[] asm("_binary_data_script_js_start");
extern const uint8_t script_js_end[] asm("_binary_data_script_js_end");

extern RadioData radioData;
AsyncWebServer server(80);

void serveStaticFile(AsyncWebServerRequest *request, const char* mimeType, const uint8_t* fileData, size_t fileSize) {
    request->send_P(200, mimeType, fileData, fileSize);
}

void initWeb(void *pvParameters)
{
    WiFi.begin(radioData.webData.ssid, radioData.webData.password);

    Serial.printf("Verbinde mit WLAN: %s\n", radioData.webData.ssid);

    const unsigned long timeoutMs = 5000;
    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeoutMs) {
      vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nVerbindung fehlgeschlagen. Starte Access Point...");
      Serial.printf("Start Access Point: %s, %s\n", "TxOneMove", "12345678");
      WiFi.softAP("TxOneMove", "12345678");
    } else {
        Serial.printf("\nVerbunden! IP-Adresse: %s\n", WiFi.localIP().toString().c_str());
    }

    // HTML-Datei
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("GET /");
        serveStaticFile(request, "text/html", index_html_start, index_html_end - index_html_start);
        Serial.println("GET / done");
    });

    // // CSS-Datei
    // server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     Serial.println("GET /style.css");
    //     serveStaticFile(request, "text/css", style_css_start, style_css_end - style_css_start);
    //     Serial.println("GET /style.css done");
    // });

    // // JavaScript-Datei
    // server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     Serial.println("GET /script.js");
    //     serveStaticFile(request, "application/javascript", script_js_start, script_js_end - script_js_start);
    //     Serial.println("GET /script.js done");
    // });

    // server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    //     if (!Update.hasError()) {
    //         request->send_P(200, "text/plain", "Update erfolgreich! Starte neu...");
    //         delay(1000);
    //         ESP.restart();
    //     } else {
    //         request->send_P(500, "text/plain", "Update fehlgeschlagen.");
    //     }
    // }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    //     if (!index) {
    //         Serial.printf("Update gestartet: %s\n", filename.c_str());
    //         if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
    //             Serial.println("Update Start fehlgeschlagen!");
    //         }
    //     }
    //     if (Update.write(data, len) != len) {
    //         Serial.println("Fehler beim Schreiben!");
    //     }
    //     if (final) {
    //         if (Update.end(true)) {
    //             Serial.println("Update abgeschlossen!");
    //         } else {
    //             Serial.println("Update fehlgeschlagen!");
    //         }
    //     }
    // });


    //   // ✅ REST GET/POST für alle Bereiche
    // server.on("/global", HTTP_GET, [](AsyncWebServerRequest *r) {
    //     Serial.println("GET /global");
    //     DynamicJsonDocument doc(256);
    //     doc["ssid"] = radioData.webData.ssid;
    //     doc["password"] = radioData.webData.password;
    //     String json; serializeJson(doc, json);
    //     r->send_P(200, "application/json", json.c_str());
    //     Serial.println("GET /global done");
    // });

    // server.on("/global", HTTP_POST, [](AsyncWebServerRequest *r){}, NULL, [](AsyncWebServerRequest *r, uint8_t *data, size_t len, size_t, size_t){
    //     Serial.println("POST /global");
    //     DynamicJsonDocument doc(256);
    //     deserializeJson(doc, data);
    //     strlcpy(radioData.webData.ssid, doc["ssid"] | "", sizeof(radioData.webData.ssid));
    //     strlcpy(radioData.webData.password, doc["password"] | "", sizeof(radioData.webData.password));
    //     r->send_P(200, "text/plain", "OK");
    //     Serial.println("POST /global done");
    // });

    // // 🔘 Store/Load Buttons
    // server.on("/saveGlobal", HTTP_POST, [](AsyncWebServerRequest *r) {
    //     Serial.println("POST /saveGlobal");
    //     radioData.storeGlobalData();
    //     r->send_P(200, "text/plain", "OK");
    //     Serial.println("POST /saveGlobal done");
    // });

    server.on("/liveView", HTTP_GET, [](AsyncWebServerRequest *r) {
        Serial.println("GET /liveView");
        DynamicJsonDocument doc(512);
        doc["batteryVoltage"] = radioData.analogData.battery;
        doc["pitch"] = radioData.functionData.pitch;
        doc["roll"] = radioData.functionData.roll;
        doc["throttle"] = radioData.functionData.throttle;
        doc["armed"] = radioData.functionData.armed;
        doc["orientation"] = radioData.orientationNames[radioData.digitalData.orientation];
        doc["stickLeftRight"] = radioData.digitalData.stickLeftRight;
        doc["stickUpDown"] = radioData.digitalData.stickUpDown;
        doc["trimRoll"] = radioData.trimData.roll;
        doc["trimPitch"] = radioData.trimData.pitch;    
        String json; serializeJson(doc, json);
        r->send_P(200, "application/json", json.c_str());
        Serial.println("GET /liveView done");
    });
    server.begin();

    vTaskDelete(NULL);
}