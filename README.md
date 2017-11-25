## Framework for an industrial PLC-based Home Automation System. Full supervisory control of power, HVAC, UPS systems, lighting, early smoke + fire detection, IT infrastructure, etc. 
There's a lot of consumer automation packages available, as well as a lot of home-brew projects (Arduino, ESP8266). However they're quite limited in scope and flexibility, and more importantly, reliability. The advantage of an industrial PLC-centered design is several fold: device control and automation (fire detection, intrusion detection, HVAC control, etc) is not reliant on any computer systems, external software, services, or consumer grade hardware.  
You also get very rugged hardware with orders of magnitude more I/O protection and reliability over something like an Arduino or off the shelf consumer automation system, coupled with a robust RTOS running your logic. These devices are designed for long term use in harsh industrial environments - in a typical home environment they should last forever with no user intervention.

![PLC](https://i.imgur.com/5P6aPBs.jpg)

## Reliability	
The main design goal was high availability. Everything that can be reasonably duplicated for redundancy has been. There are multiple power supplies for the system fed from multiple power sources as well as a [DC UPS system](https://www.phoenixcontact.com/online/portal/us/?uri=pxc-oc-itemdetail:pid=2320238) that will keep the entire system powered for 72 hours if all supplies fail simultaneously. The PLC is networked via ethernet to a redundant ring (Brocade MRP) - any one switch or link can fail with no dropped packets or traffic interruption between the PLC and it's virtualized software parent (OpenHab, Grafana, and Node-Red). 

These ancillary services are not critical as all important logic is handled natively in the PLC. They do however add easy to use interfaces to the system as well as feed the PLC data that would otherwise not be natively available (network information and status, external factors such as weather and dawn/dusk times, energy pricing, etc).  
All software is virtualized within a XenServer pool with two physically separate hosts, meaning the hardware running the ancillary software is also redundant. An entire host can be lost and Xen will hot-migrate the virtual machine to the remaining host with no service interruption.  
There are still single points of failure - thankfully they are the most reliable bits (The PLC itself).

## Usability 
All of these processes are useless if they are not abstracted from users. The user interface must be easy to use and require no explanation to a potential new user, a goal I am admittedly still working on. [HabPanel](http://docs.openhab.org/configuration/habpanel.html) has made this much easier to achieve than any other HMI offering, with plenty of community themes and custom UI elements available. It's also just a web page, so the main interface to the entire automation system can be pulled up on any device - a phone, tablet, laptop, etc.

![HabPanel Screenshot](https://i.imgur.com/IGEvSM4.jpg)

There are several other pages accessible for more specific device control - 

![HabPanel Screenshot2](https://i.imgur.com/TINXqKa.jpg)

Grafana is used for graphing and visualizing trends/historical data - 

![Grafana Screenshot](https://i.imgur.com/SNKC5Aw.jpg)

Those are the only interfaces normal users will ever need. However there are also some services for management running on the backend: 

Node-Red is used for additional glue logic, as well as SMS alerts (about fire, smoke, intrusion, loss of power, backup battery statuses, lost services etc) via the Twilio API - 

![Node-Red Screenshot](https://i.imgur.com/OCQeE9i.png)

LibreNMS is used for network monitoring and alerts - 

![LibreNMS Screenshot](https://i.imgur.com/MiPaFvh.png)

Xen Orchestra is used for XenServer/Virtualization platform management - 

![XO Screenshot](https://i.imgur.com/yGWiCEI.png)

## Conclusions
That's a high level overview of how the system works - there are many other aspects not covered here and this doesn't even begin to cover the hardware/industrial sensor side. It's been quite reliable (quite a bit more reliable than things like the power feed to the house, sadly). Having things like your air conditioning, fire detection system, servers, and lighting all aware of each other allows for some serious logic to be built. Most (software) moving parts and configuration sources are available in this repo for you to build upon if you so wish. 