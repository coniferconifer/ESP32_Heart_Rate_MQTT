## Heart rate monitor for MQTT server by ESP32

AD8323 ECG(Electronic CardiodiaGram) board is a worth buying device to observe
the electric activity of heart. 
Not only viewing ECG by monitor plot in Arduino IDE , heart beat rate can be transfered remotely by ESP32 to MQTT server
 like ThingsBoard running on Raspberry pi and charted by ThingsBoard.

- Heart beat rate is sent to MQTT server every 15sec by default.
- I recomment to use ThingsBoard as a MQTT server and run with 
  AIR BENTO to visualize heart beat rate vs co2 level 
  or temperature or air pressure.

### based on pcbreflux's art [ESP32_AD8232.ino](https://github.com/pcbreflux/espressif/tree/master/esp32/arduino/sketchbook/ESP32_AD8232)

### what's new
- May 12, 2018 

![ThingsBoard](https://github.com/coniferconifer/ESP32_Heart_Rate_MQTT/blob/master/ThingsBoardHR.png)

### Heart sensor AD8323
- analog output is connected to GPIO 34
- Lead off + : GPIO 16
- Lead off - : GPIO 17

### References
- SparkFun Single Lead Heart Rate Monitor - AD8232 <br>
[https://www.sparkfun.com/products/12650](https://www.sparkfun.com/products/12650)

- "ESP32 #50: Heart Rate / ECG Monitor with AD8232" by pcbreflux <br>
[https://www.youtube.com/watch?v=bPefUf0waoA](https://www.youtube.com/watch?v=bPefUf0waoA)

- "DIY ECG" <br>
[https://www.youtube.com/watch?v=0JPNoVXht3c](https://www.youtube.com/watch?v=0JPNoVXht3c)

- "ELI5:How do right leg drivers work?"<br>
[https://www.reddit.com/r/explainlikeimfive/comments/4oo7nl/eli5how_do_right_leg_drivers_work/](https://www.reddit.com/r/explainlikeimfive/comments/4oo7nl/eli5how_do_right_leg_drivers_work/)

- ThingsBoard<br>
[https://thingsboard.io/](https://thingsboard.io/)
