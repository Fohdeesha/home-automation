# home-automation
A ruby script for wind simulation in the racing simulator rFactor2, because why not.
While the simulator is running, it the script pulls the current car speed by polling a file RF2 writes, containing car data. It then scales this speed to 0-100% fan output, and sends that speed command out to the PLC via serial Modbus. 
The PLC then sends that 0-100% output to a large SCR-based 120v dimmer, which a large box fan is connected to. The result is a large fan blowing on the driver varying amounts of wind in sync with the car speed. Extremely useless? yes