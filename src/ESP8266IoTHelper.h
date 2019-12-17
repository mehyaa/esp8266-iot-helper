/**
 *
 * @file ESP8266IoTHelper.h
 * @date 16.12.2019
 * @author Mehmet Yasin AKAR
 *
 */

#ifndef ESP8266IOTHELPER_h
#define ESP8266IOTHELPER_h

#include <Arduino.h>

class ESP8266IoTHelper
{
public:
    ESP8266IoTHelper();
    ESP8266IoTHelper(const String &host, unsigned int port, const String &path);
    ~ESP8266IoTHelper(void);

    void wifiConnectionTimeout(unsigned int timeout)
    {
        _wifiConnectionTimeout = timeout;
    }

    unsigned int wifiConnectionTimeout(void)
    {
        return _wifiConnectionTimeout;
    }

    void smartConfigTimeout(unsigned int timeout)
    {
        _smartConfigTimeout = timeout;
    }

    unsigned int smartConfigTimeout(void)
    {
        return _smartConfigTimeout;
    }

    void smartConfigTriggerEnabled(bool enabled)
    {
        _smartConfigTriggerEnabled = enabled;
    }

    bool smartConfigTriggerEnabled(void)
    {
        return _smartConfigTriggerEnabled;
    }

    void smartConfigTriggerPin(unsigned int pin)
    {
        _smartConfigTriggerPin = pin;
    }

    unsigned int smartConfigTriggerPin(void)
    {
        return _smartConfigTriggerPin;
    }

    void smartConfigTriggerPinMode(int pinMode)
    {
        _smartConfigTriggerPinMode = pinMode;
    }

    int smartConfigTriggerPinMode(void)
    {
        return _smartConfigTriggerPinMode;
    }

    void smartConfigTriggerPinPullup(bool enabled)
    {
        _smartConfigTriggerPinPullup = enabled;
    }

    bool smartConfigTriggerPinPullup(void)
    {
        return _smartConfigTriggerPinPullup;
    }

    void httpOtaEnabled(bool enabled)
    {
        if (enabled) {
            _httpOtaEnabled == _httpOtaHost.length() > 0;
        }
        else {
            _httpOtaEnabled = false;
        }
    }

    bool httpOtaEnabled(void)
    {
        return _httpOtaEnabled;
    }

    void httpOtaHost(const String &host)
    {
        _httpOtaHost = host;
    }

    String httpOtaHost(void)
    {
        return _httpOtaHost;
    }

    void httpOtaPort(unsigned int port)
    {
        _httpOtaPort = port;
    }

    unsigned int httpOtaPort(void)
    {
        return _httpOtaPort;
    }

    void httpOtaPath(const String &path)
    {
        _httpOtaPath = path;
    }

    String httpOtaPath(void)
    {
        return _httpOtaPath;
    }

    void httpOtaCheckPeriod(unsigned long period)
    {
        _httpOtaCheckPeriod = period;
    }

    unsigned long httpOtaCheckPeriod(void)
    {
        return _httpOtaCheckPeriod;
    }

    unsigned long lastHttpOtaCheckTime(void)
    {
        return _lastHttpOtaCheckTime;
    }

    void rebootOnUpdate(bool);
    void followRedirects(bool);

    void initialize(void);
    void execute(void);

protected:

private:
    void checkSmartConfigTriggered(void);
    void checkOTA(void);

    unsigned int _wifiConnectionTimeout = 30;
    unsigned int _smartConfigTimeout = 180;
    bool _smartConfigTriggerEnabled = false;
    unsigned int _smartConfigTriggerPin = 2;
    int _smartConfigTriggerPinMode = INPUT;
    bool _smartConfigTriggerPinPullup = false;
    bool _httpOtaEnabled = false;
    String _httpOtaHost = "";
    unsigned int _httpOtaPort = 80;
    String _httpOtaPath = "/Update/ESP8266";
    unsigned long _httpOtaCheckPeriod = 3600000;
    unsigned long _lastHttpOtaCheckTime = 0;
};

#endif /* ESP8266IOTHELPER_h */