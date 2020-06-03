const int playerNumber = 0; //speaker 0, 1st speaker


#include <SPI.h>
#include <nRF24L01.h>  //https://github.com/nRF24/RF24
#include <RF24.h>

#include <DFPlayerMini_Fast.h>  //https://github.com/PowerBroker2/DFPlayerMini_Fast

DFPlayerMini_Fast myMP3; 

RF24 radio(8,9); //CE and CSN

const byte address[6] = "00001";
const int motionSensorPin = 10;
const int mp3PowerPin = 7;
const int led = 13;

//poem info... indicates the SD card file number for each poem.. 
int poemInfo [][8] = {  {1,2,3,4,5,6,0,0}, //time for bed - 6 lines 
                        {7,8,9,10,11,0,0,0}, //oleander - 5 lines 
                        {12,13,14,15,0,0,0,0}, // its cool - 4 lines 
                        {16,17,18,19,20,21,0,0}, //tonight I could do anything - 6 lines
                        {22, 23, 24, 25, 26, 27, 0, 0}, //you're a bird to the treasure - 6 lines
                        {28,29,30,31,32,33, 0, 0}, //tamborine - 6 lines
                        {34,35,36,37,38,39, 0, 0}, //wildfire
                        {40,41,42,43,44,45, 0, 0}, //william berry
                        
};

int poemSectionDelays [][8] = { {3000,3000,3000,3000,3000,3000,0,0}, //time for bed - 6 lines 
                                {3000,3000,3000,3000,3000,0,0,0}, //oleander - 5 lines 
                                {3000,3000,3000,3000,0,0,0,0}, // its cool - 4 lines 
                                {12000,8000,7000,5000,4000,4000,0,0}, //tonight I could do anything
                                {2000,5000,5000,6000,6000,5000,0,0}, //you're a bird to the treasure
                                {8000,3000,7000,12000,11000,8000,0,0}, //tamborine
                                {4000,15000,12000,23000,4000,11000,0,0}, //wildfire
                                {11000,13000,8000,8000,8000,12000,0,0}, //william berry
};

//                                     1     2     3     4   5
byte poemSectionStartDelays [][6] = { {0x4, 0x7, 0x0A, 0x0D, 0x10, 0x0}, //time for bed - 6 lines 
                                      {0x4, 0x7, 0x8, 0x9, 0x0, 0x0}, //oleander - 5 lines 
                                      {0X4, 0x6, 0x8, 0x0, 0x0, 0x0}, // its cool - 4 lines 
                                      {0x0B, 0x13, 0x18, 0x1B, 0x1E, 0x0}, //tonight I could do anything
                                      {0x3, 0x7, 0x0A, 0x0E, 0x13, 0x0}, //you're a bird to the treasure
                                      {0x8, 0x0A, 0x10, 0x19, 0x22, 0x0}, //tamborine                                      
                                      {0x4, 0x11, 0x1B, 0x31, 0x33, 0x0}, //wildfire
                                      
                                      {0x0B, 0x15, 0x1C, 0x23, 0x2A, 0x0}, //william berry
};

int poemLength [] = { 20000, //time for bed
                      12000, //oleander
                      11000, //it's cool, its cool
                      35000, //tonight I could do anything
                      25000, // you're a bird to the treasure
                      43000, //tamborine
                      62000, //wildfire
                      57000, //william berry
                      };

byte poemNumber = 0;
byte messageNumber = 1;


void setup() {
  Serial.begin(115200);

  Serial.println("starting");
  pinMode(led, OUTPUT);
  pinMode(motionSensorPin, INPUT);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  Serial1.begin(9600); //mp3 player communication
  myMP3.begin(Serial1);
  pinMode(mp3PowerPin, OUTPUT); //power pin
  digitalWrite(mp3PowerPin, HIGH);
  delay(800);
  myMP3.volume(30);
  delay(50);
  digitalWrite(mp3PowerPin, LOW);

  randomSeed(analogRead(0));
  poemNumber = random(0,4);

  
}


long lastTimeSent = -30000;
int fileToPlay = 0;

void loop() 
{
  long currentTime = millis();

  int motionSensor = digitalRead(motionSensorPin);

  int delayBeforeNextPoem = poemLength[poemNumber];
  
  if(currentTime - lastTimeSent > delayBeforeNextPoem && motionSensor)
  {
    lastTimeSent = currentTime;

    messageNumber++;
    if(messageNumber == 254) messageNumber = 1;

    
    if(poemNumber == 4) poemNumber = 1;
    else if(poemNumber == 1) poemNumber = 3;
    else if(poemNumber == 3) poemNumber = 7;
    else if(poemNumber == 7) poemNumber = 5;
    else if(poemNumber == 5) poemNumber = 0;
    else if(poemNumber == 0) poemNumber = 2;
    else if(poemNumber == 2) poemNumber = 6;
    else if(poemNumber == 6) poemNumber = 4;
    
    //poemNumber = 7;
    const char message[] = {messageNumber, poemNumber, poemSectionStartDelays[poemNumber][0], poemSectionStartDelays[poemNumber][1], poemSectionStartDelays[poemNumber][2], poemSectionStartDelays[poemNumber][3], poemSectionStartDelays[poemNumber][4], poemSectionStartDelays[poemNumber][5]} ;
    
    for(int i = 0; i < sizeof(message); i++)
    {
      Serial.print(message[i], HEX);
      Serial.print(", ");
    }
    Serial.println();
    radio.write(&message, sizeof(message));
    delay(50);
    radio.write(&message, sizeof(message));
    delay(50);
    radio.write(&message, sizeof(message));
    delay(50);
    
    digitalWrite(mp3PowerPin, HIGH);    
    fileToPlay = poemInfo [poemNumber][playerNumber];
    delay(800);
    myMP3.volume(30);
    delay(50);
    myMP3.play(fileToPlay);
    delay(poemSectionDelays [poemNumber][playerNumber]);
    digitalWrite(mp3PowerPin, LOW);
  }

}
