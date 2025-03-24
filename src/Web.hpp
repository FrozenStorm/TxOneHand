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

void initWeb()
{
    // WiFi.begin(radioData.webData.ssid, radioData.webData.password);
    // ToDo WLAN verbinden geht noch nicht

    // Serial.printf("Verbinde mit WLAN: %s\n", radioData.webData.ssid);

    // const unsigned long timeoutMs = 5000;
    // unsigned long startAttemptTime = millis();

    // while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeoutMs) {
    //   vTaskDelay(500 / portTICK_PERIOD_MS);
    //     Serial.print(".");
    // }

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

    // CSS-Datei
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("GET /style.css");
        serveStaticFile(request, "text/css", style_css_start, style_css_end - style_css_start);
        Serial.println("GET /style.css done");
    });

    // JavaScript-Datei
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("GET /script.js");
        serveStaticFile(request, "application/javascript", script_js_start, script_js_end - script_js_start);
        Serial.println("GET /script.js done");
    });

    server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!Update.hasError()) {
            request->send_P(200, "text/plain", "Update erfolgreich! Starte neu...");
            delay(1000);
            ESP.restart();
        } else {
            request->send_P(500, "text/plain", "Update fehlgeschlagen.");
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


      // ✅ REST GET/POST für alle Bereiche
    server.on("/web", HTTP_GET, [](AsyncWebServerRequest *r) {
        Serial.println("GET /web");
        DynamicJsonDocument doc(256);
        doc["ssid"] = radioData.webData.ssid;
        doc["password"] = radioData.webData.password;
        doc["apSsid"] = radioData.webData.apSsid;
        doc["apPassword"] = radioData.webData.apPassword;
        String json; serializeJson(doc, json);
        r->send_P(200, "application/json", json.c_str());
        Serial.println("GET /web done");
    });

    server.on("/web", HTTP_POST, [](AsyncWebServerRequest *r){}, NULL, [](AsyncWebServerRequest *r, uint8_t *data, size_t len, size_t, size_t){
        Serial.println("POST /web");
        DynamicJsonDocument doc(256);
        deserializeJson(doc, data);
        strlcpy(radioData.webData.ssid, doc["ssid"] | "", sizeof(radioData.webData.ssid));
        strlcpy(radioData.webData.password, doc["password"] | "", sizeof(radioData.webData.password));
        strlcpy(radioData.webData.apSsid, doc["apSsid"] | "", sizeof(radioData.webData.apSsid));
        strlcpy(radioData.webData.apPassword, doc["apPassword"] | "", sizeof(radioData.webData.apPassword));
        r->send_P(200, "text/plain", "OK");
        Serial.println("POST /web done");
    });

    // 🔘 Store/Load Buttons
    server.on("/storeGlobal", HTTP_POST, [](AsyncWebServerRequest *r) {
        Serial.println("POST /storeGlobal");
        radioData.storeGlobalData();
        r->send_P(200, "text/plain", "OK");
        Serial.println("POST /storeGlobal done");
    });

    server.begin();
}