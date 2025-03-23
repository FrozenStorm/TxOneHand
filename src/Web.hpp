#pragma once

#include "radioData.hpp"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
// #include <Update.h>
// #include <AsyncTCP.h>

extern const uint8_t index_html_start[] asm("_binary_data_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_data_index_html_end");
extern const uint8_t style_css_start[] asm("_binary_data_style_css_start");
extern const uint8_t style_css_end[] asm("_binary_data_style_css_end");
extern const uint8_t script_js_start[] asm("_binary_data_script_js_start");
extern const uint8_t script_js_end[] asm("_binary_data_script_js_end");

extern RadioData radioData;
AsyncWebServer server(80);

void serveStaticFile(AsyncWebServerRequest *request, const char* mimeType, const uint8_t* fileData, size_t fileSize) {
    request->send(200, mimeType, fileData, fileSize);
}

void initWeb()
{
    WiFi.mode(WIFI_STA);
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
      IPAddress local_ip(192, 168, 1, 1);
      IPAddress gateway(192, 168, 1, 1);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.softAPConfig(local_ip, gateway, subnet);
      Serial.printf("Start Access Point: %s, %s\n", radioData.webData.apSsid, radioData.webData.apPassword);
      WiFi.softAP(radioData.webData.apSsid, radioData.webData.apPassword);
    } else {
        Serial.printf("\nVerbunden! IP-Adresse: %s\n", WiFi.localIP().toString().c_str());
    }

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


      // ✅ REST GET/POST für alle Bereiche
    server.on("/web", HTTP_GET, [](AsyncWebServerRequest *r) {
        JsonDocument doc(256);
        doc["ssid"] = radioData.webData.ssid;
        doc["password"] = radioData.webData.password;
        doc["apSsid"] = radioData.webData.apSsid;
        doc["apPassword"] = radioData.webData.apPassword;
        String json; serializeJson(doc, json);
        r->send(200, "application/json", json);
    });

    server.on("/web", HTTP_POST, [](AsyncWebServerRequest *r){}, NULL, [](AsyncWebServerRequest *r, uint8_t *data, size_t len, size_t, size_t){
        JsonDocument doc(256);
        deserializeJson(doc, data);
        strlcpy(radioData.webData.ssid, doc["ssid"] | "", sizeof(radioData.webData.ssid));
        strlcpy(radioData.webData.password, doc["password"] | "", sizeof(radioData.webData.password));
        strlcpy(radioData.webData.apSsid, doc["apSsid"] | "", sizeof(radioData.webData.apSsid));
        strlcpy(radioData.webData.apPassword, doc["apPassword"] | "", sizeof(radioData.webData.apPassword));
        r->send(200, "text/plain", "OK");
    });

    // 🔘 Store/Load Buttons
    server.on("/storeGlobal", HTTP_POST, [](AsyncWebServerRequest *r) {
        radioData.storeGlobalData();
        r->send(200, "text/plain", "OK");
    });

    server.begin();
}