# ESP32-C2 Weather & MQTT Display

A compact 240×240 weather and MQTT information display based on the **ESP32-C2** and an **ST7789V TFT**.

The project combines a live weather screen with an integrated MQTT/Tasmota energy monitor. Configuration is handled through a built-in web interface, settings are stored in NVS, and firmware updates can be installed over Wi-Fi using OTA.

> Recommended repository name: `ESP32-C2-Weather-MQTT-Display`

---

## Features

- ESP32-C2 based
- ST7789V 240×240 TFT
- Hardware SPI
- SPI Mode 3
- Display CS permanently connected to GND
- Inverted backlight control
- PWM brightness control
- OpenWeather current weather data
- Temperature in °C
- Humidity in %
- Air pressure in hPa
- Air pressure additionally displayed in cmHg
- Day/night weather icons
- Large digital clock
- Date and weekday display
- NTP time synchronization
- Automatic CET/CEST daylight-saving handling
- Current weather cached in ESP32 NVS
- Automatic weather refresh
- Integrated web configuration
- Wi-Fi network scan
- Setup access point for first-time configuration
- Settings stored permanently in NVS
- Live display-mode switching from the web interface
- Integrated **PicoMQTT broker**
- Tasmota MQTT support
- Tasmota power status display
- Current power in watts
- Energy consumption today
- Energy consumption yesterday
- Total energy consumption
- MQTT online/offline state via LWT
- Optional MQTT username/password authentication
- Configurable MQTT display name
- OTA firmware updates over Wi-Fi
- OTA progress shown directly on the TFT
- Wi-Fi watchdog with automatic reboot on connection loss
- Linux-style boot screen with status messages
- Factory-reset support in the configuration interface

---

# Display Modes

The project has two main display modes.

## Weather Mode

The weather screen displays:

- Date
- Weekday
- Current time
- OpenWeather weather icon
- Temperature
- Humidity
- Air pressure in hPa
- Air pressure in cmHg

The temperature and humidity values are also visualized using horizontal bars.

The weather data is cached in NVS, so the last successful values remain available after a reboot.

---

## MQTT / Tasmota Mode

The ESP32-C2 itself acts as an **MQTT broker**.

No external Mosquitto server is required for this mode.

The display subscribes to common Tasmota topics:

```text
tele/+/SENSOR
tele/+/STATE
stat/+/RESULT
stat/+/POWER
tele/+/LWT
```

The MQTT screen can display:

```text
Power       W
Today       kWh
Yesterday   kWh
Total       kWh
ON / OFF
Clock
Date
```

The Tasmota device name is detected from the MQTT topic. A custom fallback display name can also be configured through the web interface.

The broker starts lazily when MQTT mode is first activated, reducing resource usage while the display is used only in weather mode.

---

# Hardware

## Display

```text
Controller: ST7789V
Resolution: 240 × 240
Interface:  Hardware SPI
SPI mode:   SPI_MODE3
Rotation:   2
```

## Pin Assignment

The pin configuration is located in:

```text
include/pins.h
```

Current assignment:

| Function | GPIO |
|---|---:|
| TFT SCLK / CLK | GPIO 4 |
| TFT MOSI | GPIO 6 |
| TFT DC | GPIO 5 |
| TFT RESET | GPIO 1 |
| TFT Backlight | GPIO 18 |
| TFT CS | GND / no GPIO |
| TFT MISO | not used |

Important:

```cpp
#define TFT_CS -1
```

The display CS line is permanently connected to **GND** on this hardware.

There is no MISO connection.

---

# Backlight

The backlight uses an inverted transistor circuit:

```text
LOW = backlight ON
HIGH = backlight OFF
```

Brightness is controlled using ESP32 LEDC PWM.

The brightness can be changed directly from the web interface.

The configured brightness value is stored in NVS and restored after reboot.

---

# First Start / Setup Mode

If the ESP32 does not contain a valid Wi-Fi configuration, it automatically starts a setup access point.

The access-point name begins with:

```text
ESP32-WeatherTV-
```

The last four characters are generated from the device MAC address.

Example:

```text
ESP32-WeatherTV-A1B2
```

Default setup Wi-Fi password:

```text
12345678
```

The TFT shows:

- Setup mode
- Wi-Fi SSID
- Setup password
- Setup IP address

Connect a phone, tablet, or computer to the setup Wi-Fi and open the IP address shown on the TFT.

---

# Web Configuration

The ESP32 runs an HTTP configuration server on port 80.

After the device is connected to your normal Wi-Fi network, open:

```text
http://DEVICE-IP/
```

Example:

```text
http://192.168.0.41/
```

The web interface allows configuration of:

- Wi-Fi SSID
- Wi-Fi password
- OpenWeather API key
- OpenWeather city
- OpenWeather country
- MQTT port
- MQTT username
- MQTT password
- MQTT display name
- Display mode
- Display brightness

A Wi-Fi scan is available directly in the setup page.

---

# Live Mode Switching

The active display can be switched directly from the web interface.

Available modes:

```text
Weather
MQTT
```

The switch happens immediately without requiring a reboot.

The selected mode is stored in NVS, so the ESP32 returns to the same display mode after power loss or restart.

---

# OpenWeather Configuration

The weather screen uses the OpenWeather current-weather API.

Required values:

```text
API key
City
Country
```

Example:

```text
City:    Köln
Country: DE
```

The project is configured for:

```text
Units: metric
Language: German
```

The API request uses the OpenWeather current-weather endpoint.

Weather values are cached in NVS after a successful download.

---

# Time / NTP

Time synchronization uses:

```text
pool.ntp.org
time.nist.gov
```

Timezone configuration:

```text
CET / CEST
```

The firmware automatically handles daylight-saving and standard time for Central Europe.

The clock is updated locally after NTP synchronization and does not require a continuous connection to the NTP server.

---

# MQTT / Tasmota Setup

The ESP32-C2 acts as the MQTT broker.

The default MQTT port is:

```text
1883
```

In Tasmota, configure the MQTT host to the IP address of this display.

Example:

```text
Host:     192.168.0.41
Port:     1883
User:     configured MQTT username
Password: configured MQTT password
```

If no MQTT username is configured on the ESP32, the broker accepts MQTT clients without authentication.

Supported information includes:

- Relay state ON/OFF
- Current power
- Energy today
- Energy yesterday
- Total energy
- LWT online/offline information

---

# NVS Storage

Persistent device settings are stored using ESP32 `Preferences`.

The project stores, among other things:

```text
Wi-Fi configuration
OpenWeather configuration
Brightness
MQTT port
MQTT username/password
MQTT display name
Selected display mode
Last weather data
```

This allows the device to restore its configuration after reboot or power loss.

---

# Factory Reset

The web interface includes a factory-reset function.

A factory reset clears the stored configuration and cached application data from NVS and restarts the ESP32.

Afterward, the device starts again in setup access-point mode.

The source code also contains an optional boot-cycle based hardware-reset mechanism. It is disabled by default.

---

# Wi-Fi Watchdog

The firmware periodically checks the Wi-Fi connection.

If Wi-Fi is lost, the watchdog automatically restarts the ESP32 so that it can reconnect cleanly.

---

# OTA Firmware Updates

The project supports OTA updates using `ArduinoOTA`.

Two PlatformIO environments are included:

```text
esp32-c2-usb
esp32-c2-ota
```

## USB Upload

```bash
pio run -e esp32-c2-usb -t upload
```

## OTA Upload

Set the current device IP in:

```text
platformio.ini
```

Example:

```ini
[env:esp32-c2-ota]
upload_protocol = espota
upload_port = 192.168.0.41
```

Then upload with:

```bash
pio run -e esp32-c2-ota -t upload
```

The OTA service uses port:

```text
3232
```

During an OTA update, the TFT displays:

- OTA update screen
- Progress bar
- Percentage
- Reboot status
- Error information if the update fails

---

# OTA Partition Scheme

The firmware requires sufficiently large OTA application slots.

The project uses:

```ini
board_build.partitions = min_spiffs.csv
```

If OTA fails with:

```text
OTA_BEGIN_ERROR
```

make sure the correct partition table has been flashed.

After changing the partition layout, perform at least one USB upload so the new partition table is written to flash.

---

# PlatformIO

Main configuration:

```ini
platform  = espressif32
board     = esp32-c2-devkitm-1
framework = arduino
```

Flash size:

```text
4 MB
```

Serial monitor:

```text
115200 baud
```

Important libraries:

```text
Adafruit ST7735 and ST7789 Library
Adafruit GFX Library
ArduinoJson
PicoMQTT
```

Build:

```bash
pio run
```

USB upload:

```bash
pio run -e esp32-c2-usb -t upload
```

Serial monitor:

```bash
pio device monitor
```

---

# Project Structure

```text
src/
  main.cpp

include/
  config_portal.h
  credentials.h
  mqtt.h
  ota.h
  pins.h

fonts/
  ...

Icons/
  ...

platformio.ini
```

---

# Security Before Publishing on GitHub

Before publishing the project publicly, check the complete project for private credentials.

Do not publish real:

- Wi-Fi passwords
- OpenWeather API keys
- MQTT passwords
- OTA passwords
- API tokens
- credentials stored in fallback configuration
- private information you do not want visible

The project currently supports optional fallback credentials in:

```text
include/credentials.h
```

For a public repository, keep:

```cpp
#define USE_FALLBACK_CREDENTIALS 0
```

and replace private example values with placeholders.

Example:

```cpp
#define FALLBACK_WIFI_SSID  "YOUR_WIFI_SSID"
#define FALLBACK_WIFI_PASS  "YOUR_WIFI_PASSWORD"
#define FALLBACK_OW_KEY     "YOUR_OPENWEATHER_API_KEY"
```

The OTA password should also be changed or moved to a local/private configuration file before publishing the repository.

---

# License

This project is licensed under the **MIT License**.

See the `LICENSE` file for details.

---

# Deutsche Version

# ESP32-C2 Wetter- & MQTT-Display

Ein kompaktes 240×240 Wetter- und MQTT-Informationsdisplay auf Basis des **ESP32-C2** und eines **ST7789V TFT**.

Das Projekt kombiniert eine Wetteranzeige mit einem integrierten MQTT-/Tasmota-Energiemonitor. Die Konfiguration erfolgt über eine eingebaute Webseite, Einstellungen werden dauerhaft im NVS gespeichert und Firmware-Updates können per OTA über WLAN eingespielt werden.

> Empfohlener Repository-Name: `ESP32-C2-Weather-MQTT-Display`

---

## Funktionen

- ESP32-C2
- ST7789V 240×240 TFT
- Hardware-SPI
- SPI Mode 3
- Display-CS fest mit GND verbunden
- invertierte Backlight-Ansteuerung
- PWM-Helligkeitsregelung
- aktuelle Wetterdaten über OpenWeather
- Temperatur in °C
- Luftfeuchtigkeit in %
- Luftdruck in hPa
- zusätzliche Luftdruckanzeige in cmHg
- Tag-/Nacht-Wettersymbole
- große Digitaluhr
- Datum und Wochentag
- Zeitsynchronisation über NTP
- automatische CET-/CEST-Sommerzeitumschaltung
- Wetterdaten-Cache im ESP32-NVS
- automatische Wetteraktualisierung
- integrierte Web-Konfiguration
- WLAN-Scan
- Setup-Access-Point für die Erstkonfiguration
- dauerhafte Speicherung der Einstellungen im NVS
- Live-Umschaltung des Anzeigemodus über das Webinterface
- integrierter **PicoMQTT-Broker**
- Tasmota-MQTT-Unterstützung
- Anzeige des Tasmota-Schaltzustands
- aktuelle Leistung in Watt
- Energieverbrauch heute
- Energieverbrauch gestern
- Gesamtverbrauch
- MQTT-Online-/Offline-Status über LWT
- optionale MQTT-Benutzername-/Passwort-Anmeldung
- frei einstellbarer MQTT-Anzeigename
- OTA-Firmwareupdates über WLAN
- OTA-Fortschritt direkt auf dem TFT
- WLAN-Watchdog mit automatischem Neustart bei Verbindungsverlust
- Linux-artiger Bootscreen mit Statusmeldungen
- Werksreset über das Konfigurationsinterface

---

# Anzeigemodi

Das Projekt besitzt zwei Hauptansichten.

## Wettermodus

Die Wetteranzeige zeigt:

- Datum
- Wochentag
- aktuelle Uhrzeit
- OpenWeather-Wettersymbol
- Temperatur
- Luftfeuchtigkeit
- Luftdruck in hPa
- Luftdruck in cmHg

Temperatur und Luftfeuchtigkeit werden zusätzlich als horizontale Balken dargestellt.

Die Wetterdaten werden im NVS gespeichert. Dadurch stehen nach einem Neustart zunächst die zuletzt erfolgreich empfangenen Werte zur Verfügung.

---

## MQTT-/Tasmota-Modus

Der ESP32-C2 arbeitet selbst als **MQTT-Broker**.

Für diesen Modus wird kein zusätzlicher Mosquitto-Server benötigt.

Abonniert werden unter anderem typische Tasmota-Topics:

```text
tele/+/SENSOR
tele/+/STATE
stat/+/RESULT
stat/+/POWER
tele/+/LWT
```

Die MQTT-Anzeige kann darstellen:

```text
Leistung
Verbrauch heute
Verbrauch gestern
Gesamtverbrauch
ON / OFF
Uhrzeit
Datum
```

Der Gerätename kann automatisch aus dem MQTT-Topic erkannt werden. Zusätzlich kann im Webinterface ein eigener Ersatz-Anzeigename eingestellt werden.

Der MQTT-Broker wird erst beim ersten Wechsel in den MQTT-Modus gestartet. Dadurch werden im reinen Wetterbetrieb RAM und CPU geschont.

---

# Hardware

## Display

```text
Controller: ST7789V
Auflösung:  240 × 240
Interface:  Hardware-SPI
SPI-Modus:  SPI_MODE3
Rotation:   2
```

## Pinbelegung

Die Pinbelegung befindet sich in:

```text
include/pins.h
```

Aktuelle Belegung:

| Funktion | GPIO |
|---|---:|
| TFT SCLK / CLK | GPIO 4 |
| TFT MOSI | GPIO 6 |
| TFT DC | GPIO 5 |
| TFT RESET | GPIO 1 |
| TFT Backlight | GPIO 18 |
| TFT CS | GND / kein GPIO |
| TFT MISO | nicht verwendet |

Wichtig:

```cpp
#define TFT_CS -1
```

Die CS-Leitung des Displays ist bei dieser Hardware dauerhaft mit **GND** verbunden.

Eine MISO-Leitung wird nicht verwendet.

---

# Hintergrundbeleuchtung

Die Hintergrundbeleuchtung wird über eine invertierte Transistorstufe angesteuert:

```text
LOW = Hintergrundbeleuchtung EIN
HIGH = Hintergrundbeleuchtung AUS
```

Die Helligkeit wird über ESP32-LEDC-PWM geregelt.

Sie kann direkt über das Webinterface verändert werden.

Der eingestellte Wert wird im NVS gespeichert und nach einem Neustart wiederhergestellt.

---

# Erster Start / Setup-Modus

Wenn noch keine gültige WLAN-Konfiguration im ESP32 gespeichert ist, startet das Gerät automatisch einen eigenen Setup-Access-Point.

Der WLAN-Name beginnt mit:

```text
ESP32-WeatherTV-
```

Die letzten vier Zeichen werden aus der MAC-Adresse des Geräts erzeugt.

Beispiel:

```text
ESP32-WeatherTV-A1B2
```

Standardpasswort des Setup-WLANs:

```text
12345678
```

Auf dem TFT werden angezeigt:

- Setup-Modus
- WLAN-SSID
- Setup-Passwort
- Setup-IP-Adresse

Mit Smartphone, Tablet oder PC mit diesem WLAN verbinden und anschließend die auf dem TFT angezeigte IP-Adresse im Browser öffnen.

---

# Web-Konfiguration

Der ESP32 stellt einen HTTP-Konfigurationsserver auf Port 80 bereit.

Nach erfolgreicher Verbindung mit dem normalen WLAN:

```text
http://GERAETE-IP/
```

Beispiel:

```text
http://192.168.0.41/
```

Im Webinterface können eingestellt werden:

- WLAN-SSID
- WLAN-Passwort
- OpenWeather-API-Key
- OpenWeather-Ort
- OpenWeather-Land
- MQTT-Port
- MQTT-Benutzername
- MQTT-Passwort
- MQTT-Anzeigename
- Anzeigemodus
- Displayhelligkeit

Ein WLAN-Scan ist direkt auf der Setup-Seite verfügbar.

---

# Live-Umschaltung

Die aktive Anzeige kann direkt über das Webinterface gewechselt werden.

Verfügbare Modi:

```text
Wetter
MQTT
```

Die Umschaltung erfolgt sofort und ohne Neustart.

Der ausgewählte Modus wird im NVS gespeichert. Nach Stromausfall oder Neustart startet das Gerät wieder mit der zuletzt gewählten Ansicht.

---

# OpenWeather-Konfiguration

Die Wetteranzeige verwendet die aktuelle Wetterabfrage von OpenWeather.

Benötigt werden:

```text
API-Key
Ort
Land
```

Beispiel:

```text
Ort:  Köln
Land: DE
```

Das Projekt ist eingestellt auf:

```text
Einheiten: metrisch
Sprache:   Deutsch
```

Nach erfolgreichem Abruf werden die Wetterdaten im NVS zwischengespeichert.

---

# Uhrzeit / NTP

Die Zeitsynchronisation erfolgt über:

```text
pool.ntp.org
time.nist.gov
```

Zeitzone:

```text
CET / CEST
```

Sommer- und Winterzeit für Mitteleuropa werden automatisch berücksichtigt.

Nach erfolgreicher NTP-Synchronisation läuft die Uhr lokal weiter.

---

# MQTT-/Tasmota-Einrichtung

Der ESP32-C2 selbst arbeitet als MQTT-Broker.

Standard-MQTT-Port:

```text
1883
```

In Tasmota wird als MQTT-Host die IP-Adresse dieses Displays eingetragen.

Beispiel:

```text
Host:     192.168.0.41
Port:     1883
Benutzer: wie im Webinterface eingestellt
Passwort: wie im Webinterface eingestellt
```

Wenn auf dem ESP32 kein MQTT-Benutzername eingetragen wurde, akzeptiert der Broker MQTT-Verbindungen ohne Benutzername und Passwort.

Unterstützte Informationen sind unter anderem:

- Relaisstatus ON/OFF
- aktuelle Leistung
- Verbrauch heute
- Verbrauch gestern
- Gesamtverbrauch
- Online-/Offline-Status über Tasmota LWT

---

# NVS-Speicherung

Dauerhafte Einstellungen werden mit ESP32 `Preferences` gespeichert.

Gespeichert werden unter anderem:

```text
WLAN-Konfiguration
OpenWeather-Konfiguration
Helligkeit
MQTT-Port
MQTT-Benutzername/Passwort
MQTT-Anzeigename
gewählter Anzeigemodus
letzte Wetterdaten
```

Dadurch bleiben Einstellungen auch nach einem Neustart oder Stromausfall erhalten.

---

# Werksreset

Im Webinterface befindet sich eine Funktion für den Werksreset.

Dabei werden die gespeicherte Konfiguration und die zwischengespeicherten Anwendungsdaten aus dem NVS gelöscht und der ESP32 wird neu gestartet.

Anschließend startet das Gerät wieder im Setup-Access-Point-Modus.

Im Quelltext befindet sich zusätzlich ein optionaler Werksreset über mehrere schnelle Bootvorgänge. Dieser Mechanismus ist standardmäßig deaktiviert.

---

# WLAN-Watchdog

Die Firmware überprüft regelmäßig die WLAN-Verbindung.

Geht die Verbindung verloren, startet der ESP32 automatisch neu, damit eine saubere Wiederverbindung erfolgen kann.

---

# OTA-Firmwareupdate

Das Projekt unterstützt OTA-Updates mit `ArduinoOTA`.

Es existieren zwei PlatformIO-Environments:

```text
esp32-c2-usb
esp32-c2-ota
```

## USB-Upload

```bash
pio run -e esp32-c2-usb -t upload
```

## OTA-Upload

In:

```text
platformio.ini
```

die aktuelle IP-Adresse des Geräts eintragen.

Beispiel:

```ini
[env:esp32-c2-ota]
upload_protocol = espota
upload_port = 192.168.0.41
```

Anschließend:

```bash
pio run -e esp32-c2-ota -t upload
```

Der OTA-Dienst verwendet Port:

```text
3232
```

Während des Updates zeigt das TFT:

- OTA-Updatebildschirm
- Fortschrittsbalken
- Prozentanzeige
- Neustartstatus
- Fehlermeldungen bei Problemen

---

# OTA-Partitionsschema

Die Firmware benötigt ausreichend große OTA-App-Slots.

Das Projekt verwendet:

```ini
board_build.partitions = min_spiffs.csv
```

Falls OTA mit:

```text
OTA_BEGIN_ERROR
```

abbricht, sollte überprüft werden, ob die korrekte Partitionstabelle geflasht wurde.

Nach einer Änderung des Partitionsschemas muss mindestens einmal per USB geflasht werden, damit die neue Partitionstabelle auf den ESP32 geschrieben wird.

---

# PlatformIO

Grundkonfiguration:

```ini
platform  = espressif32
board     = esp32-c2-devkitm-1
framework = arduino
```

Flash:

```text
4 MB
```

Serieller Monitor:

```text
115200 Baud
```

Wichtige Bibliotheken:

```text
Adafruit ST7735 and ST7789 Library
Adafruit GFX Library
ArduinoJson
PicoMQTT
```

Projekt bauen:

```bash
pio run
```

Per USB flashen:

```bash
pio run -e esp32-c2-usb -t upload
```

Seriellen Monitor öffnen:

```bash
pio device monitor
```

---

# Projektstruktur

```text
src/
  main.cpp

include/
  config_portal.h
  credentials.h
  mqtt.h
  ota.h
  pins.h

fonts/
  ...

Icons/
  ...

platformio.ini
```

---

# Sicherheit vor dem GitHub-Upload

Vor einem öffentlichen GitHub-Upload sollte das gesamte Projekt auf private Zugangsdaten überprüft werden.

Nicht öffentlich hochladen:

- echte WLAN-Passwörter
- echte OpenWeather-API-Keys
- MQTT-Passwörter
- OTA-Passwörter
- API-Tokens
- private Zugangsdaten in der Fallback-Konfiguration
- sonstige Daten, die nicht öffentlich sichtbar sein sollen

Das Projekt unterstützt optionale Fallback-Zugangsdaten in:

```text
include/credentials.h
```

Für ein öffentliches Repository sollte:

```cpp
#define USE_FALLBACK_CREDENTIALS 0
```

aktiv bleiben und private Beispielwerte sollten durch Platzhalter ersetzt werden.

Beispiel:

```cpp
#define FALLBACK_WIFI_SSID  "YOUR_WIFI_SSID"
#define FALLBACK_WIFI_PASS  "YOUR_WIFI_PASSWORD"
#define FALLBACK_OW_KEY     "YOUR_OPENWEATHER_API_KEY"
```

Auch das OTA-Passwort sollte vor dem öffentlichen Upload geändert oder in eine lokale/private Konfigurationsdatei ausgelagert werden.

---

# Lizenz

Dieses Projekt steht unter der **MIT-Lizenz**.

Weitere Informationen findest du in der Datei `LICENSE`.
