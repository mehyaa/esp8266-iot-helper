# ESP8266 IoT Helper

This library contains a simple helper that you can orchestrate WiFi connection via SmartConfig and Arduino IDE + HTTP OTA update features all together.

## How to use

Instantiate **helper** with valid HTTP OTA information:

`ESP8266IoTHelper iotHelper("<HTTP OTA Server Host>", <HTTP OTA Server Port>, "<HTTP OTA Server Path>")`

or with only SmartConfig and Arduino IDE OTA updates capability:

`ESP8266IoTHelper iotHelper`

Then in your setup function `initialize` it:

`iotHelper.initialize()`

And on your loop function call `execute` (preferably the first line):

`iotHelper.execute()`

## What does it do

### Initialize

* Tries to connect WiFi with your previosly saved WiFi configuration. If can not connect WiFi, starts SmartConfig.

* Sets up Arduino IDE OTA and HTTP OTA updates and checks for OTA updates if valid configuration supported.

### Execute

* Checks if the SmartConfig input pin is active (HIGH or LOW as your config) if enabled by configuration and if pin is active then starts SmartConfig.

* Checks if Arduino IDE OTA updates available.

* Checks HTTP OTA updates if valid configuration supported.

## Functions

**`iotHelper.initialize`:** Initialize the WiFi connection and OTA features.

**`iotHelper.execute`:** Check SmartConfig trigger and OTA updates.

**`iotHelper.wifiConnectionTimeout`:** Set or get timeout in seconds for WiFi connection.

**`iotHelper.smartConfigTimeout`:**  Set or get timeout in seconds for SmartConfig.

**`iotHelper.smartConfigTriggerEnabled`:** Set or get SmartConfig trigger enabled. Must be set before `initialize`.

**`iotHelper.smartConfigTriggerPin`:** Set or get SmartConfig trigger pin. Must be set before `initialize`.

**`iotHelper.smartConfigTriggerPinMode`:** Set or get SmartConfig trigger pin mode. Must be set before `initialize`.

**`iotHelper.smartConfigTriggerPinPullup`:** Set or get SmartConfig trigger is pull up or not. Must be set before `initialize`.

**`iotHelper.httpOtaEnabled`:** Set or get HTTP OTA updates enabled.

**`iotHelper.httpOtaHost`:** Set or get server host name or IP address for HTTP OTA updates.

**`iotHelper.httpOtaPort`:** Set or get server port for HTTP OTA updates.

**`iotHelper.httpOtaPath`:** Set or get server path for HTTP OTA updates.

**`iotHelper.httpOtaCheckPeriod`:** Set or get HTTP OTA updates check period in milliseconds.

**`iotHelper.lastHttpOtaCheckTime`:** Get last HTTP OTA updates checked time.

**`iotHelper.rebootOnUpdate`:** Set if reboot on HTTP OTA updates.

**`iotHelper.followRedirects`:** Get if follow HTTP redirects on HTTP OTA updates.
