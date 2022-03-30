# Serial Call and Response Analog Sensors Example

This example shows the necessary python scripts, DATs, and CHOPs to set up Serial Communication with an Arduino. This example uses the the free version of TouchDesigner 2021.16410 and the Arduino Nano 33 IoT.

## Arduino Circuit:
For this example I'm using a circut with 3 analog inputs.  This can be adjusted for any number of analog or digital inputs.


![Breadboard of Arduino Nano 33 IoT with Sensors connected to A0, A1, A2](./imgs/analogsensors.png?raw=true "Breadboard Sensors")


![Breadboard of Arduino Nano 33 IoT with Buttons connected to D2, D3, D4](./imgs/arduinoButtons.png?raw=true "Breadboard Switches")

## Arduino Code:
Because Arduino and Touchdesigner are communicating asynchronously, we want to set up a Call and Response system to ensure that Arduino only sends values at a rate that Touchdesigner can accommodate. We'll write code that tells arduino to print sensor values to Touchdesigner, but only if Touchdesigner has first sent a byte to arduino. We later tell touchdesigner to respond to Arduino, only if it has received values. In this way they will exchange information evenly without either ever sending before the other is ready to receive.


To begin, let's set up variables for our sensor values:

    int sensor1;
    int sensor2;
    int sensor3;

Next inside of your ```setup()``` make sure you use ```Serial.begin()```.  Remember the baudrate, this will be used later in your touchdesigner network.  I'll use 9600.  
    
    void setup(){
        Serial.begin(9600);
    }
    
Inside of your ```loop()``` read your sensors using ```analogRead()```:
    
    void loop(){
        sensor1 = analogRead(A0);
        sensor2 = analogRead(A1);
        sensor3 = analogRead(A2);
    }

After reading your sensors (and while still inside the ```loop()```). Create and `if` statement that only executes if `Serial.available()` is greater than `0`.  

This means whatever you put inside of this statement will only execute if the Arduino has received information on the serial port (from Touchdesigner).

    if (Serial.available() > 0) {
    
    }

If Arduino receives information from TouchDesigner, read that information, remove it from the serial buffer by assigning it to a variable `int incoming`.  Then print out the sensor values.  Format the communication by separating values with a delimiter (in this case a comma ",") and ending all communication with a newline character `'\n'`.  The newline will indicate to Touchdesigner that Arduino finished sending all the values for that turn.


    if (Serial.available() > 0) {
        int incoming = Serial.read();
        Serial.print(sensor1);
        Serial.print(",");
        Serial.print(sensor2);
        Serial.print(",");
        Serial.print(sensor3);
        Serial.print('\n');
    }

Full Code below:

    int sensor1;
    int sensor2;
    int sensor3;

    void setup() {
        Serial.begin(9600);
    }

    void loop() {
        sensor1 = analogRead(A0);
        sensor2 = analogRead(A1);
        sensor3 = analogRead(A2);

        if (Serial.available() > 0) {
            int incoming = Serial.read();
            Serial.print(sensor1);
            Serial.print(",");
            Serial.print(sensor2);
            Serial.print(",");
            Serial.print(sensor3);
            Serial.print('\n');
        }
    }


## The TouchDesigner Network:
![Serial Communication Network](https://github.com/RiosITP/DILP2022/blob/main/In%20Class%20Examples/Serial/imgs/network.png?raw=true "Network")

## Make a button:
Lets use a button to open and close the Serial port.  Create a ```button``` component (COMP), make sure the "Button Type" parameter is set to Toggle Up. Connect it to a ```null``` CHOP. 

## Make the button control your port opening:
![Serial Communication Network](./imgs/buttonNull.png?raw=true "Toggle Button to Null")

Make the ```null``` active and export it to the "Active" parameter of the ```serial``` DAT.

![Serial Communication Network](./imgs/serialDATexport2.png?raw=true "Toggle Button to Null")

Make your button active and click it to see if the serial DAT toggles on and off.


## Send a byte using a script:

Create a ``` CHOP Execute``` DAT.  

The ```CHOP Execute``` DAT is designed to trigger specific functions whenever the target CHOP channel vlaues has exhibited any of the parameter changes listed (```Off to On```,```While On```,```On to Off```, ```While Off ```,```Value Change```)

For this case we will only use ```Off to On```.  

Make sure the ```Off to On``` parameter is enabled.  

![Serial Communication Network](https://github.com/RiosITP/DILP2022/blob/main/In%20Class%20Examples/Serial/imgs/chopExecSettings.png?raw=true "ChopExecute Script")

Make the chopExec DAT Active and write a python script that will send one byte to Arduino when the button goes from off to on (which is simultaneously opening the and closing the serial port):

    def onOffToOn(channel, sampleIndex, val, prev):
        op('serial1').send('x',terminator='=')
        # print("TD SENDING")
        return


![Serial Communication Network](./imgs/chopExecActive.png?raw=true "ChopExecute Script")

To ensure only one byte gets sent, I am setting the terminator to "=" which means no terminator in TD.  For more info on formatting serial messages, review the [serial DAT reference](https://docs.derivative.ca/SerialDAT_Class)
Simultaneously use a CHOPExecute DAT to send a byte to arduino when the port is activated.

## Setup your Serial DAT

Click on the `serial` DAT.  I've set the parameters as such:

- Row/Callback Format: "One Per Message"
- Port: Portnames will differ by computer, it should match what you see in your Arduino IDE (e.g. COM4, usbmodem14201, etc)
- Baud Rate: 9600
- Data Bits: 8
- DTR: Enable
- RTS: Disable

![Serial DAT Settings](./imgs/serialDATsettings.png?raw=true "Serial DAT Settings")


If you already uploaded the Arduino code, and set the DATs up correctly, your DAT should look something like this:


![Serial DAT](./imgs/serialDAT.png?raw=true "Serial DAT")

Once the sensor data is coming in reliably we can connect the output of our `serial` DAT to the input of a `convert` DAT and use the `convert` to separate the values.

![Serial To Convert](./imgs/serialtoConvert.png?raw=true "Serial to Convert")

The `convert` parameters define how you will reformat the incoming values.  Make sure that you are converting to a table, this will take values and split them into individual cells.  Input a comma ',' in the `split cells at` field to define which delimiters you are using in between values.  You do not need to input '\n', the `serial` DAT takes care of the ending control character.

![Serial To Convert](./imgs/convertSettings.png?raw=true "Convert Settings")

Once we have the values separated, the output of your `convert` to a `DAT to` CHOP. This will turn the split DAT values into a channel (CHOP) format.  Once you have values in a CHOP you can then export and connect as we have in other examples.


