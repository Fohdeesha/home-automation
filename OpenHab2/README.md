# home-automation
OpenHab2 configuration files. While the PLC handles all the safety, interlocking, and 99% of the automation, OpenHab is being used as a frontend UI (HabPanel) to the PLC as well as providing data to the PLC that would not otherwise be available (sunrise/sunset times, weather, calendar data, Asterisk call information, web service data, etc).

Due to its huge amount of available [Bindings](http://docs.openhab.org/addons/bindings.html), it is also being used as an arbitrary protocol converter. It communicates with the PLC via Modbus, but also grabs data from external equipment and other sensors via SNMP and MQTT, as well information from services like Asterisk, PfSense, Xen, etc. It then sends these values to the PLC via Modbus and vice versa. Thanks to OH's binding architecture you can exchange data between arbitrary protocols and devices very easily.

OpenHab is also being used for historical data logging, logging all values of all sensors and PLC automation data to an InfluxDB database, which is then displayed via Grafana. 

<p align="center">
  <img src="http://i.imgur.com/Motqw7s.png">
</p>

