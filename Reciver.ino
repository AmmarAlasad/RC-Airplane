// Receptor 7 canais JC02AP Pisca Permaneente
 // Que pode ligar diretamente 3 dispositivos de baixo consumo
 // tipo LED
const int LEDpin = 15;
const long onDuration = 100;// OFF time for LED
const long offDuration = 900;// ON time for LED
int LEDState =HIGH; //Led
long rememberTime=0;// this is used by the code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
int ch_width_5 = 0;
int ch_width_6 = 0;
int ch_width_7 = 0;
Servo canal1; // Canal 1 Leme * Throttel
Servo canal2; // Canal 2 Elevador * Rudder 
Servo canal3; // Canal 3 Acelerador * 
Servo canal4; // Canal 4 Flap
Servo canal5; // Canal 5 Auxiliar 1
Servo canal6; // Canal 7 Auxiliar 3
Servo canal7; // Canal 7 Auxiliar 3
struct Signal {
 byte canal1; // Canal 1 Leme
 byte canal2; // Canal 2 Elevador
 byte canal3; // Canal 3 Acelerador
 byte canal4; // Canal 4 Flap
 byte canal5; // Canal 5 Auxiliar 1
 byte canal6; // Canal 6 Auxiliar 2
 byte canal7; // Canal 7 Auxiliar 3
};
Signal data;
const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);
void ResetData()
{
data.canal1 = 0; // Centro do Joystick
 data.canal2 = 127; // Centro do Joystick
 data.canal3 = 127; // Centro do Joystick
 data.canal4 = 127; // Centro do Joystick
 data.canal5 = 127; // Centro do Joystick
 data.canal6 = 127; // Centro do Joystick
 data.canal7 = 127; // Centro do Joystick
}
void setup()
{Serial.begin(9600);
 //Saida de capa pino no arduino
pinMode(LEDpin,OUTPUT);// define LEDpin as output
 digitalWrite(LEDpin,LEDState);// set initial state
 //nnn

 canal1.attach(4);
 canal2.attach(3);
 canal3.attach(2);
 canal4.attach(5);
 canal5.attach(6);
 canal6.attach(7);
 canal7.attach(19);
pinMode(16, OUTPUT);
pinMode(17, OUTPUT);
pinMode(18, OUTPUT);

 //Configure the NRF24 module
 ResetData();
 radio.begin();
 radio.openReadingPipe(1,pipeIn);
 radio.setAutoAck(false);
 radio.setDataRate(RF24_250KBPS);
 radio.setPALevel(RF24_PA_HIGH);
 radio.startListening();
 pinMode(6,OUTPUT);
}
unsigned long lastRecvTime = 0;
void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(Signal));
lastRecvTime = millis();
}
}
void loop()
{
{
 // Led intermitente()
if( LEDState ==HIGH )
{
 if( (millis()- rememberTime) >= onDuration){
 LEDState = LOW;// change the state of LED
 rememberTime=millis();// remember Current millis() time
 }
}
else
{
 if( (millis()- rememberTime) >= offDuration){
 LEDState =HIGH;// change the state of LED
 rememberTime=millis();// remember Current millis() time
 }
}
// LED blink
digitalWrite(LEDpin,LEDState);// turn the LED ON or OFF
}// loop ends

recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) 
{
  ResetData();
}


ch_width_1 = map(data.canal1, 0, 255, 1000, 2000); //throttel 
ch_width_2 = map(data.canal2, 0, 255, 1000, 2000);
ch_width_3 = map(data.canal3, 0, 255, 1000, 2000); // different angle replace 160 by 255 and 950 by 1000
ch_width_4 = map(data.canal4, 0, 255, 1000, 2000);
ch_width_5 = map(data.canal5, 0, 255, 1000, 2000);

analogWrite(16, data.canal5);
ch_width_6 = map(data.canal6, 0, 255, 1000, 2000);
analogWrite(17, data.canal6);
ch_width_7 = map(data.canal7, 0, 255, 1000, 2000);
analogWrite(18, data.canal7);


canal1.writeMicroseconds(ch_width_1); // throttel 
canal2.writeMicroseconds(ch_width_2); // rudder 
canal3.writeMicroseconds(ch_width_3); // Elevator
canal4.writeMicroseconds(ch_width_4); // flaps
canal5.writeMicroseconds(ch_width_4);
canal6.writeMicroseconds(ch_width_6);
canal7.writeMicroseconds(ch_width_7);

Serial.println(data.canal4);

}
// finish code
