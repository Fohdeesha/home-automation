# home-automation
Main ladder logic program for the PLC. To use, you'll need a [Click PLC](https://www.automationdirect.com/adc/Shopping/Catalog/Programmable_Controllers/CLICK_Series_PLCs_(Stackable_Micro_Brick)/PLC_Units/C0-02DD2-D) and accompanying I/O modules from Automation Direct, as well as the [software](http://support.automationdirect.com/products/clickplcs.html) to edit and load it.

Substitute your own PLC model if desired, or don't use one at all. OpenHab is very flexible in what it will talk to, and powerful enough to run all automation on its own. 

However the advantage of a PLC-centered automation design is several fold: device control and automation (fire detection, intrusion detection, power monitoring, HVAC control, etc) is not reliant on any computer systems or external software and services. You also get very rugged hardware with orders of magnitude more I/O protection and reliability over something like an Arduino, coupled with a robust RTOS running your ladder logic. These devices are typically designed to be installed and forgotten in the bottom of lift stations running pump sequences, factory floors running boilers and chemical processes, conveyer systems, etc - in a typical home environment they should last forever. 

The downside being I/O hardware is not cheap. This can be offset by adding commodity hardware (Arduino, ESP8266, etc) for non-critical sensors and devices. In my use case an Arduino is being used to provide several PWM outputs to control RGB strip lighting, and various I/O for other decorative lighting. OpenHab converts the MQTT data to and from the Arduino into Modbus data to the PLC, so the PLC system just sees it as native I/O. This saves several hundred dollars in expensive I/O modules - and if the arduino fails, you've only lost decorative lighting control while the PLC logs the event and moves on keeping things like HVAC and intrusion detection running smoothly.

![PLC Screenshot](http://i.imgur.com/XjOHT22.png)

