// Transmissor RC de 7 Canais 
 #include <SPI.h> //Inclui biblioteca SPI
 #include <nRF24L01.h>
 #include <RF24.h>
 const uint64_t pipeOut = 0xE8E8F0F0E1LL; //Este codigo 0xE8E8F0F0E1LL deve ser igual ao do Receptor
 RF24 radio(9, 10); // Os pinos de radio devem ser indicado neste caso usei o 9 e o 10
 struct Signal { // Configuracao ou extrutura do transmissor
 byte canal1; // Canal 1 Leme
 byte canal2; // Canal 2 Elevador
 byte canal3; // Canal 3 Acelerador 
 byte canal4; // Canal 4 Flap
 byte canal5; // Canal 5 Auxiliar 1
 byte canal6; // Canal 6 Auxiliar 2
 byte canal7; // Canal 7 Auxiliar 3
};
 Signal data;
 void ResetData() 
{
 data.canal1 = 127; // Centro do Joystick 
 data.canal2 = 127; // Centro do Joystick 
 data.canal3 = 127; // Centro do Joystick 
 data.canal4 = 127; // Centro do Joystick 
 data.canal6 = 127; // Centro do Joystick 
 data.canal5 = 127; // Centro do Joystick 
 data.canal7 = 127; // Centro do Joystick 
}
 void setup()
{
  Serial.begin(19200);
 //Comunicacao com o radio frequencia Receptor
 radio.begin(); // Abre ligacao com o Receptor 
 radio.openWritingPipe(pipeOut); // Comeca ligacao com o Receptor 
 radio.setAutoAck(false);
 radio.setDataRate(RF24_250KBPS); // Velocidade de ligacao deve ser a mesma no Receptor
 radio.setPALevel(RF24_PA_HIGH); // Quanto mais alto mais longo o alcance e mais bateria consome
 // Quanto mais baixo menor o alcance menos consumo de bateria
 ResetData();}
 // Comando do Joystick
 // Comando do Joystick
 
 int mapJoystickValues(int val, int baixo, int medio, int alto)
{
 val = constrain(val, 0, 4000);
 if ( val < 2000 )
 val = map(val, 0, 2000, 0, 128); // Mapeia os valores para o potenciometro
 else
 val = map(val,0, 4000,0, 255);
 return (val);
}

 void loop()
{
 
 // Valores de calibracao 
 data.canal1 = mapJoystickValues( analogRead(A3), 0, 510, 1020); // "true" or "false" direccao do sinal | Acelerador
 data.canal2 = mapJoystickValues( analogRead(A0), 12, 510, 1020); // "true" or "false" direccao do servo | Leme 0
 data.canal3 = mapJoystickValues( analogRead(A1), 12, 510, 1020); // "true" or "false" direccao do servo | Elevador
 data.canal4 = mapJoystickValues( analogRead(A4), 12, 510, 1020); // "true" or "false" direccao do servo | Flaps
 data.canal5 = mapJoystickValues( analogRead(A6), 12, 510, 1020); // "true" or "false" direccao do servo | Axiliar 1
 data.canal6 = mapJoystickValues( analogRead(A7), 12, 510, 1020); // "true" or "false" direccao do servo | Auxiliar 2
 data.canal7 = mapJoystickValues( analogRead(A2), 12, 510, 1020); // "true" or "false" direccao do servo | Auxiliar 3 
 radio.write(&data, sizeof(Signal));

  
}
// finish code
