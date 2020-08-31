/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"
#include "DataManager.h"


bool radioNumber = 1;   // Set this radio as radio number 0 or 1
bool role = 1;          // Used to control whether this node is sending or receiving


// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8
RF24 radio(7,8);

byte addresses[][6] = {"1Node","2Node"};

DataManager dm = DataManager();

void setup() {
        Serial.begin(115200);
        Serial.println(F("RF24/examples/GettingStarted"));
        Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
        radio.begin();

        // Set the PA Level low to prevent power supply related issues since this is a
        // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
        radio.setPALevel(RF24_PA_LOW);
  
        // Open a writing and reading pipe on each radio, with opposite addresses
        if (radioNumber) {
                radio.openWritingPipe((uint64_t)addresses[1]);
                radio.openReadingPipe(1,(uint64_t)addresses[0]);
        } else {
                radio.openWritingPipe((uint64_t)addresses[0]);
                radio.openReadingPipe(1,(uint64_t)addresses[1]);
        }
  
        // Start the radio listening for data
        radio.startListening();
}

void loop() {
  
        // Ping Out Role
        if (role == 1) {
                // First, stop listening so we can talk.
            radio.stopListening();                               
                Serial.println(F("Now sending"));
                
                // Take the time, and send it.  This will block until complete

                float receivedData[16] = {0};

                // 센서로 받은 데이터 저장하는 부분
                float gyro[3] = { 1.0, 2.0, 3.0};
                float accel[3] = {4.0, 5.0, 6.0};
                float pressure[2] = {7.0, 8.0};
                float ypr[3] = {10.0, 11.0, 12.0};

                float latitude = 13.0;
                float longitude = 14.0;
                float altitude = 15.0;
                float speed = 16.0;


                // 데이터를 데이터 매니저에 저장하는 부분
                dm.setAccel(accel);                               // 가속도 저장
                dm.setGyro(gyro);                                 // 자이로 저장
                dm.setYPR(ypr);                                   // 요 피치 롤 저장
                dm.setPressure(pressure);                         // 압력 저장
                dm.setGPS(latitude, longitude, altitude, speed);  // GPS 데이터 저장

                float* sendData = dm.getDataWithFloatArray();     // 송신용 데이터로 변환

                // 송신용 코드
                for(int i = 0; i < 2; i++){
                  if (!radio.write( sendData + 8*i, sizeof(float)*8 )) {
                          Serial.println(F("failed"));
                  }
                  else{
                    Serial.print("SEND_");
                    Serial.print(i+1);
                    for(int j = 0; j < 8; j++){
                      Serial.print(",");
                      Serial.print(sendData[j + 8*i]);
                    }
                    Serial.println("");
                  }
                  delay(50);
                }
        // Now, continue listening
                radio.startListening();                                            
   
                delay(1000);
        }


        // Pong Back Role
        if ( role == 0 ) {
                // Variable for the received timestamp
                float receivedData[16] = {0};
             
                if ( radio.available()) {
                        // While there is data ready
                        int count = 0;

                        // 두 번에 나눠 받는다.
                        while(count < 2){
                          // 다음 데이터가 들어올 때 까지 대기
                          while(!radio.available()){
                            
                          }
                          // 데이터를 받는다.
                          while (radio.available()) {                                
                                  // Get the payload
                                  radio.read( receivedData + 8*count, sizeof(float)*8 + 8*count );           
                          }
                          count++;
                          
                        }
                     // First, stop listening so we can talk   
                        radio.stopListening();          

                        // 데이터를 제대로 받았을 경우
                        if(dm.setDataWithFloatArray(receivedData)){
 
                        }
                        // 제대로 못받았을 때
                        else{
                          Serial.println("Failed, Wrong Data received");   
                        }

                          // 받은 데이터 출력
                        Serial.print("RECEIVED");
                        for(int i = 0; i < 16; i++){
                          Serial.print(",");
                          Serial.print(receivedData[i]);
                          
                        }
                        Serial.println("");
                        
                        radio.startListening();                                            
                         
                }
         }

        // Change Roles via Serial Commands
        if ( Serial.available() ) {
                char c = toupper(Serial.read());
                if ( c == 'T' && role == 0 ) {      
                        Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
                        // Become the primary transmitter (ping out)
                        role = 1; 
    
                } else {
                    if ( c == 'R' && role == 1 ) {
                                Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
                                // Become the primary receiver (pong back)
                                role = 0;
                                radio.startListening();
                        }
                }
        }
} 
