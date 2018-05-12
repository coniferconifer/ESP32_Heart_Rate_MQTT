## Heart rate monitor for MQTT server by ESP32
- Heart beat rate is sent to MQTT server every 15sec by default.
- I recomment to use ThingsBoard as a MQTT server and run with 
  AIR BENTO to visualize heart beat rate vs co2 level 
  or temperature or air pressure.

### based on pcbreflux's art [ESP32_AD8232.ino]( https://github.com/pcbreflux/espressif/tree/master/esp32/arduino/sketchbook/ESP32_AD8232)

### what's new
- May 12, 2018 

![ThingsBoard](https://github.com/coniferconifer/ESP32_Heart_Rate_MQTT/blob/master/ThingsBoardHR.png)

### Heart sensor AD8323
- analog output is connected to GPIO 34
- Lead off + : GPIO 16
- Lead off - : GPIO 17

### References
- "ESP32 #50: Heart Rate / ECG Monitor with AD8232" by pcbreflux
[Youtube https://www.youtube.com/watch?v=bPefUf0waoA](https://www.youtube.com/watch?v=bPefUf0waoA)

- "DIY ECG"
[Youtube https://www.youtube.com/watch?v=0JPNoVXht3c](https://www.youtube.com/watch?v=0JPNoVXht3c)
