#pragma once

#include "radioData.hpp"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
// #include <Update.h>
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

    server.begin();
}