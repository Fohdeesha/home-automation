## Framework for an industrial PLC-based Home Automation System. Full supervisory measurement and control of power, HVAC, UPS systems, lighting, early smoke + fire detection, IT infrastructure, etc. 
There are many consumer automation packages available to a home owner, and also many home-spun projects using commodity hardware such as Arduinos and ESP8266 modules. Sadly they are quite limited in scope and flexibility, and more importantly, reliability. The advantage of an industrial PLC-centered automation design is several fold: device control and automation (fire detection, intrusion detection, power monitoring, HVAC control, etc) is not reliant on any computer systems, external software, services, or consumer grade hardware.  
You also get very rugged hardware with orders of magnitude more I/O protection and reliability over something like an Arduino or off the shelf consumer automation system, coupled with a robust RTOS running your logic. These devices are typically designed to be installed and forgotten in the bottom of lift stations running pump sequences, factory floors running boilers and chemical processes, elevators, etc - in a typical home environment they should last forever with no user intervention.

![PLC](https://i.imgur.com/5P6aPBs.jpg)

## Reliability	
The main design goal was reliability and high availability. Everything that can be reasonably duplicated for redundancy has been. Any configuration changes or modifications are first tested offline and are version controlled via Git. There are multiple power supply rails for the system fed from multiple power sources as well as a [DC UPS system](https://www.phoenixcontact.com/online/portal/us/?uri=pxc-oc-itemdetail:pid=2320238)   that will keep the entire system powered for 72 hours if all supplies fail simultaneously. The PLC system is connected via ethernet to a redundant ring of switches running Brocade's  L2 Metro-E protocol with 10gbe uplinks - any one switch or one link can fail entirely with no dropped packets or traffic interruption between the PLC and it's virtualized software parent (OpenHab, Grafana, and Node-Red). 

These ancillary services are not critical as all important logic is handled natively in the PLC, so they can be lost entirely with no serious repercussions. However they add easy to use interfaces to the system as well as feed the PLC data that would otherwise not be natively available (network information and status, external factors such as weather and dawn/dusk times, energy pricing, etc).  
All software is virtualized within a XenServer pool with two separate server hosts, meaning the hardware running the ancillary software is also redundant. An entire host server can be lost and Xen will hot-migrate the virtual machine to the remaining host with zero dropped traffic or service interruption. it is also automated to hot-migrate important services (not necessarily PLC related) to a colocated server in a chicago datacenter if multiple fire and smoke sensors have been tripped, indicating both local hosts are likely to melt soon. Data is always regularly synchronized between the two locations via ZFS pool replication, but hot migration ensures there is no downtime during the transition.  
Does this mean there is no single point of failure within the entire system? No, there are some remaining that are hard to avoid - thankfully they are the most reliable bits (mainly the PLC itself, which are designed for applications where a failure or fault potentially means killing employees)

## Usability 
All of these processes are useless if they are not succesfully hidden and abstracted from any potential users. Any user interface must be easy to use and require no explanation to a potential new user, a goal I am admittedly still working on. [HabPanel](http://docs.openhab.org/configuration/habpanel.html) has made this much easier to achieve than any other HMI offering, with plenty of community themes and custom UI elements available. It's also just a web page, so the main interface to the entire automation system can be pulled up on any device - a phone, tablet, laptop, etc.  
It's hosted locally and is not externally accessible on the internet for obvious reasons, but with a VPN connection to your home it can be if you so desire. 

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
That's a high level overview of how the system works - there are many other aspects not covered here and this doesn't even begin to cover the hardware/industrial sensor side. It has proven to be the most reliable system I've personally used, and the power and integration is unmatched. Having things like your air conditioning, fire detection system, servers, and lighting all aware of each other allows for some serious logic to be built. Most (software) moving parts and configuration sources are available in this repo for you to build upon if you so wish. 