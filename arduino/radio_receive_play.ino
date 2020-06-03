

//07/30/19... x2 .. just added in all the hard delays to simply program... because some speakers seemed to be getting stuck...
//x3 needed to fix the radio.. it will store values if they are not read and discarded




const int playerNumber = 2; //starts at speaker 0

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


#include <SPI.h>
#include <nRF24L01.h>  //https://github.com/nRF24/RF24
#include <RF24.h>

#include <DFPlayerMini_Fast.h>  //https://github.com/PowerBroker2/DFPlayerMini_Fast

DFPlayerMini_Fast myMP3;
RF24 radio(8,9);

const int led = 13;
const byte address[6] = "00001";

const int mp3PowerPin = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.println("starting");
  pinMode(led, OUTPUT);
  
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  Serial1.begin(9600); //mp3 player communication
  myMP3.begin(Serial1);
  pinMode(mp3PowerPin, OUTPUT); //power pin
  digitalWrite(mp3PowerPin, HIGH);
  delay(800);
  myMP3.volume(30);
  delay(50);
  //myMP3.play(5);
  //delay(3000);
  digitalWrite(mp3PowerPin, LOW);
}

byte lastMessage = 0;

int repeatMessage = 0;
boolean listenFlag = 1;
char message[8] = "";

long poemStartTime = -8000;
boolean playPoemFlag = 0;
int delayPoemInterval = 0;
int fileToPlay = 0;
int poemNumber = 0;
int lastPoemNumber = 0;

void loop() {

  long currentTime = millis();  
  
  if (radio.available()) 
  {
    radio.read(&message, sizeof(message));
    for(int i = 0; i < sizeof(message); i++)
    {
      Serial.print(message[i], HEX);
      Serial.print(", ");
    }
    Serial.println();

    Serial.print("player number: ");
    Serial.println(playerNumber);
    Serial.print("poem number: ");
    poemNumber = message[1];
    Serial.println(poemNumber);

    if(poemNumber != lastPoemNumber)
    {
      Serial.println("new Poem");
      lastPoemNumber = poemNumber;
    
      delayPoemInterval = message[playerNumber + 1];
      delayPoemInterval = delayPoemInterval * 1000; //change to seconds 
      poemNumber = message[1];
      fileToPlay = poemInfo [poemNumber][playerNumber];
      if(fileToPlay > 0) 
      {
        digitalWrite(mp3PowerPin, HIGH); //turn on speaker
    
        lastMessage = message[0];
        radio.stopListening();
        radio.openWritingPipe(address);
        radio.setPALevel(RF24_PA_MAX);
        radio.setDataRate(RF24_250KBPS);
    
        radio.write(&message, sizeof(message));
        delay(100);
        radio.write(&message, sizeof(message));
        delay(100);
        radio.write(&message, sizeof(message));
        delay(100);
        
        Serial.println("starting to listen again");
        radio.openReadingPipe(0,address);
        radio.setPALevel(RF24_PA_MAX);
        radio.setDataRate(RF24_250KBPS);
        radio.startListening();
      }
      
  
      digitalWrite(led, HIGH);
      delay(200);
      digitalWrite(led, LOW);
      
      delay(delayPoemInterval);
      Serial.print("delayed: ");
      Serial.print(delayPoemInterval);
      Serial.print("ms, ");
      Serial.print("playing File: ");
      Serial.println(fileToPlay);
      int delaySectionLength = poemSectionDelays [poemNumber][playerNumber];
      myMP3.volume(30);
      delay(50);
      myMP3.play(fileToPlay);
      delay(delaySectionLength);
      Serial.print("turning off after: ");
      Serial.print(delaySectionLength);
      Serial.println();
      digitalWrite(mp3PowerPin, LOW);
    }  
  }
}
