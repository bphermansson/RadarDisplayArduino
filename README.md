This device fetches temp and forecast from yr.no and time from a ntp-server. It the displays it on an epaper module, using a ESP32S3 as it's brain (but can use other ESP32-based boards as well).
It also uses a LD2420 precense detector and a RGB-ledstrip. The purpose of this is to work as a warning system. If someone enters the room, the ledstrip flashes.
All settings are made in platform.ini, except Wifi password. For this you have to create a file "private_settings.ini" with this content:
```
[env]
build_flags =
    -DWIFI_PASSWORD=\"<Wifi password>\"
```