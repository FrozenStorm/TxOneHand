#ifndef WEB_HPP
#define WEB_HPP

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

extern const uint8_t index_html_start[] asm("_binary_data_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_data_index_html_end");

extern const uint8_t style_css_start[] asm("_binary_data_style_css_start");
extern const uint8_t style_css_end[] asm("_binary_data_style_css_end");

extern const uint8_t script_js_start[] asm("_binary_data_script_js_start");
extern const uint8_t script_js_end[] asm("_binary_data_script_js_end");


AsyncWebServer server(80);

bool apMode = false; // Speichert, ob der ESP als Access Point läuft

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