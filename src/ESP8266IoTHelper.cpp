/**
 *
 * @file ESP8266IoTHelper.cpp
 * @date 16.12.2019
 * @author Mehmet Yasin AKAR
 *
 */

#include "ESP8266IoTHelper.h"

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>

ESP8266IoTHelper::ESP8266IoTHelper(void)
{
    ESPhttpUpdate.rebootOnUpdate(true);
    ESPhttpUpdate.followRedirects(true);
}

ESP8266IoTHelper::ESP8266IoTHelper(const String &host, unsigned int port, const String &path)
{
    ESPhttpUpdate.rebootOnUpdate(true);
    ESPhttpUpdate.followRedirects(true);

    httpOtaHost(host);
    httpOtaPort(port);
    httpOtaPath(path);
    httpOtaEnabled(true);
}

ESP8266IoTHelper::~ESP8266IoTHelper(void)
{
}

void ESP8266IoTHelper::rebootOnUpdate(bool reboot)
{
    ESPhttpUpdate.rebootOnUpdate(reboot);
}

void ESP8266IoTHelper::followRedirects(bool follow)
{
    ESPhttpUpdate.followRedirects(follow);
}

void ESP8266IoTHelper::initialize(void)
{
    if (_smartConfigTriggerEnabled)
    {
        pinMode(_smartConfigTriggerPin, _smartConfigTriggerPinMode);
    }

    WiFi.mode(WIFI_STA);

    WiFi.begin();

    Serial.println("Connecting WiFi.");

    for (int i = 0; i < _wifiConnectionTimeout && WiFi.status() != WL_CONNECTED; i++)
    {
        Serial.print("|");

        delay(1000);
    }

    Serial.println();

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Couldn't connect to WiFi, waiting SmartConfig.");

        WiFi.disconnect();

        WiFi.beginSmartConfig();
    }

    for (int i = 0; i < _smartConfigTimeout && WiFi.status() != WL_CONNECTED; i++)
    {
        Serial.print("|");

        if (WiFi.smartConfigDone())
        {
            Serial.println();

            Serial.println("SmartConfig successful.");

            break;
        }

        delay(1000);
    }

    Serial.println();

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("SmartConfig not completed, restarting.");

        ESP.restart();
    }

    WiFi.setAutoConnect(true);

    WiFi.setAutoReconnect(true);

    ArduinoOTA.onStart([]() {
        Serial.println("Arduino IDE OTA started.");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println();
        Serial.println("Arduino IDE OTA completed.");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Arduino IDE OTA progress: %u%%", (progress / (total / 100)));
        Serial.println();
    });

    ArduinoOTA.onError([](ota_error_t error) {
        switch (error)
        {
        case OTA_AUTH_ERROR:
            Serial.println("Arduino IDE OTA Error: Authentication failed.");
            break;

        case OTA_BEGIN_ERROR:
            Serial.println("Arduino IDE OTA Error: Couldn't begin.");
            break;

        case OTA_CONNECT_ERROR:
            Serial.println("Arduino IDE OTA Error: Couldn't connect.");
            break;

        case OTA_RECEIVE_ERROR:
            Serial.println("Arduino IDE OTA Error: Couldn't receive.");
            break;

        case OTA_END_ERROR:
            Serial.println("Arduino IDE OTA Error: Couldn't end.");
            break;
        }
    });

    ArduinoOTA.begin();

    ESPhttpUpdate.onStart([]() {
        Serial.println("HTTP OTA started.");
    });

    ESPhttpUpdate.onEnd([]() {
        Serial.println();
        Serial.println("HTTP OTA completed.");
    });

    ESPhttpUpdate.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("HTTP OTA progress: %u%%", (progress / (total / 100)));
        Serial.println();
    });

    ESPhttpUpdate.onError([](int error) {
        switch (error)
        {
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP OTA: No update available.");
            break;

        case HTTP_UE_TOO_LESS_SPACE:
            Serial.println("HTTP OTA: Not enough space.");
            break;

        case HTTP_UE_SERVER_NOT_REPORT_SIZE:
            Serial.println("HTTP OTA: Server did not report size.");
            break;

        case HTTP_UE_SERVER_FILE_NOT_FOUND:
            Serial.println("HTTP OTA: File not found (404).");
            break;

        case HTTP_UE_SERVER_FORBIDDEN:
            Serial.println("HTTP OTA: Forbidden (403).");
            break;

        case HTTP_UE_SERVER_WRONG_HTTP_CODE:
            Serial.println("HTTP OTA: Wrong HTTP code.");
            break;

        case HTTP_UE_SERVER_FAULTY_MD5:
            Serial.println("HTTP OTA: Wrong MD5.");
            break;

        case HTTP_UE_BIN_VERIFY_HEADER_FAILED:
            Serial.println("HTTP OTA: Verify bin header failed.");
            break;

        case HTTP_UE_BIN_FOR_WRONG_FLASH:
            Serial.println("HTTP OTA: New binary does not fit flash size.");
            break;

        default:
            Serial.println("HTTP OTA: Update failed.");
            break;
        }
    });

    if (_httpOtaEnabled)
    {
        ESPhttpUpdate.update(_httpOtaHost, _httpOtaPort, _httpOtaPath);
    }

    Serial.print("IP address: ");

    Serial.println(WiFi.localIP());
}

void ESP8266IoTHelper::execute(void)
{
    checkSmartConfigTriggered();

    if (WiFi.status() == WL_CONNECTED)
    {
        checkOTA();
    }
}

void ESP8266IoTHelper::checkSmartConfigTriggered(void)
{
    if (!_smartConfigTriggerEnabled)
    {
        return;
    }

    int isSmartConfig = digitalRead(_smartConfigTriggerPin);

    if ((_smartConfigTriggerPinPullup && isSmartConfig == LOW) ||
        (!_smartConfigTriggerPinPullup && isSmartConfig == HIGH))
    {
        Serial.println("SmartConfig triggered, waiting config.");

        WiFi.disconnect();

        WiFi.beginSmartConfig();

        for (int i = 0; i < _smartConfigTimeout && WiFi.status() != WL_CONNECTED; i++)
        {
            Serial.print("|");

            if (WiFi.smartConfigDone())
            {
                Serial.println();

                Serial.println("SmartConfig successful, restarting");

                ESP.restart();
            }

            delay(1000);
        }

        Serial.println("No config received, reconnecting.");

        WiFi.reconnect();

        for (int i = 0; i < _wifiConnectionTimeout && WiFi.status() != WL_CONNECTED; i++)
        {
            Serial.print("|");

            delay(1000);
        }

        Serial.println();

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Couldn't connect to WiFi, restarting.");

            ESP.restart();
        }
    }
}

void ESP8266IoTHelper::checkOTA(void)
{
    ArduinoOTA.handle();

    if (_httpOtaEnabled)
    {
        unsigned long now = millis();

        if (now > _lastHttpOtaCheckTime + _httpOtaCheckPeriod || now < _lastHttpOtaCheckTime)
        {
            Serial.println("Checking HTTP OTA.");

            t_httpUpdate_return result = ESPhttpUpdate.update(_httpOtaHost, _httpOtaPort, _httpOtaPath);

            _lastHttpOtaCheckTime = now;

            if (result == HTTP_UPDATE_NO_UPDATES)
            {
                Serial.println("HTTP OTA: No update available.");
            }
        }
    }
}