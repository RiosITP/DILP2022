# Serial Call and Response Example

This example shows the necessary python scripts, DATs, and CHOPs to set up Serial Communication with an Arduino. This example uses the Arduino Nano 33 IoT

The Network:
![Serial Communication Network](https://github.com/RiosITP/DILP2022/blob/main/In%20Class%20Examples/Serial/imgs/network.png?raw=true "Network")

Use a button to open and close the Serial port by exporting the button value to the Active parameter of the Serial DAT.  Simultaneously use a 
CHOPExecute DAT to send a byte to arduino when the port is activated.
