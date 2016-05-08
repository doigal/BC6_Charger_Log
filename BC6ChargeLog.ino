/*
------------------------------------------------------------------------
  Arudino Log/Display for e-Station BC-6 Multichem battery charger
------------------------------------------------------------------------
An arduino based reader for the e-Station BC-6 battery charger, that when
 connected to a suitable display can graph the charge curve of the battery.

LICENCE:
This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
Feel free to share it, hack it apart, make it better, etc. 
Just don't make money off others work, and credit where credits due.

HARDWARE:
No external hardware required, only three wires to connect to the BC-6 charger.
If wanted a screen can be added for stand alone monitoring.

CREDITS:
Couldnt thank the "Long-Term Memory" blog enough for working out how to decipher the BC-6's output. 
http://blog.dest-unreach.be/2012/01/29/imax-b6-charger-protocol-reverse-engineered

FUTURE WORK:
 - Move to SoftwareSerial so that PC logging can be done without 
    a Arduino Mega
 - Add in TFT display (TJCTM24024-SPI) for graphing relvant items
 - Add in SD card logging on the TFT display
 - Some form of control for selecting screens

CHANGE LOG
 - V0.1 8th May 2016
    Development release. 
    Reads values from charger using Serial1 on Arduino Mega, displays them on PC via serial.
*/

// Variables
byte inByte;

double volt_pack;
int mAh;
double current;
double volt_cell_1;
double volt_cell_2;
double volt_cell_3;
double volt_cell_4;
double volt_cell_5;
double volt_cell_6;
int RunState;
int chem;
int time_mins;

void setup() {
  // initialize both serial ports to 9600baud
  Serial.begin(9600);
  Serial1.begin(9600);

  Serial.println("begin");
}

void loop() {
  {
  if (Serial1.available()) {

  //Looks for the start char of each packet:
  // This is 0x7B in hex or { in normal speak :)
    inByte = Serial1.read();
    if (inByte == 0x7B)
     {Serial.println("");
      Serial.println("New Packet ");

      //Packets are 76 bytes in total.
      //This includes the start and end chars: { }
      //Read all the remaining packet data into a buffer 
      // We have already read the first, so there are 75 remaining:
      unsigned char buf[75];
      Serial1.readBytesUntil(0x7D,buf,75);

      //Buffer address reference http://blog.dest-unreach.be/2012/01/29/imax-b6-charger-protocol-reverse-engineered
      //All the values seem to be shifted by dec 128 or hex 0x80.
      
      RunState = (buf[23]-128);                                   // Returns 1 for run and 0 for standby
      volt_pack = (buf[34]-128) + ((buf[35]-128)/100.0);          // Returns current pack voltage in V
      current = (buf[32]-128) + ((buf[33]-128)/100.0);            // Returns current charge/discharge rate in A
      mAh = ((buf[42]-128)*100) + (buf[43]-128);                  // Returns level of energy put in or taken from the pack
      chem = buf[22];                                             // Returns the current chemistry mode

      volt_cell_1 = (buf[44] - 128) + ((buf[45] - 128)/100.0);
      volt_cell_2 = (buf[46] - 128) + ((buf[47] - 128)/100.0);
      volt_cell_3 = (buf[48] - 128) + ((buf[49] - 128)/100.0);
      volt_cell_4 = (buf[50] - 128) + ((buf[51] - 128)/100.0);
      volt_cell_5 = (buf[52] - 128) + ((buf[53] - 128)/100.0);                  
      volt_cell_6 = (buf[54] - 128) + ((buf[55] - 128)/100.0);      

      time_mins=buf[69];
      
      Serial.print("Running State :");
      Serial.println(RunState);

      Serial.print("Battery Chemistry :");
      Serial.println(chem,HEX);
      
      Serial.print("Pack Voltage :");
      Serial.println(volt_pack);

      Serial.print("Charge Current :");
      Serial.println(current);

      Serial.print("mAh :");      
      Serial.println(mAh);     

      Serial.println("Cell Voltage :");
      Serial.println(volt_cell_1);     
      Serial.println(volt_cell_2);  
      Serial.println(volt_cell_3);     
      Serial.println(volt_cell_4);     
      Serial.println(volt_cell_5);     
      Serial.println(volt_cell_6);                 

      Serial.print("Time in minutes :");
      Serial.println(time_mins);
      
      }
     }
  }
  }

