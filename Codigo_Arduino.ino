#include <OneWire.h> // Biblioteca para o protocolo de comunicação 
#include <DallasTemperature.h> //Biblioteca das funções do sensor de temperatura
#include <SoftwareSerial.h> // A biblioteca para comunicar o Arduino com o ESP-01

SoftwareSerial SoftSerial(8,9); // RX Arduino 8, TX Arduino 9.

class Sensor { // Criando uma classe para ficar mais fácil usar os sensores
    uint8_t ID_num ;
    OneWire _Wire;
    DallasTemperature sensor;
  public:
    void SetPin(uint8_t pin1) {
        _Wire.begin(pin1);
        sensor.setOneWire(&_Wire);
        sensor.setResolution(12);
        sensor.begin(); //INICIA O SENSOR
        sensor.requestTemperatures(); }
    float Temp() {
        sensor.requestTemperatures();
        return (sensor.getTempCByIndex(0)); }
    void SetID(uint8_t n) {
        ID_num = n;}
    uint8_t ID() {
        return (ID_num);}
};
Sensor TempAmbiente; // Crio um tipo de sensor de temperatura que é para medir o ambiente

void setup() {
  delay(7000); // Aguarda um tempo para inicialização do ESP
  TempAmbiente.SetPin(2); // Defino o pino 2 como o pino do sensor de temperatura
  TempAmbiente.SetID(1);  // Seto o ID como o sensor de temperatura do ambiente como 1
  Serial.begin(9600); //INICIALIZA A SERIAL
  SoftSerial.begin(9600);
}

void loop() {

  
  EnviaESP(TempAmbiente.ID(),TempAmbiente.Temp()); // Envio para o ESP os dados do ID e da temperatura
  EscutandoESP(8000); // O Arduino ouve o ESP-01 por 8 segundos, para imprimir as mensagens seriais.
  
}

void EnviaESP(uint8_t ID, float Temperatura){
  
   SoftSerial.print(ID);SoftSerial.print("|");
   SoftSerial.print(Temperatura);SoftSerial.println("|");
}

void EscutandoESP(int tempo){
  unsigned long time1;
  char check[2];
  char resposta[25] = {"         "};
  byte leitura, i=0;

  time1 = millis();
while(abs((millis()-time1)<tempo)){   
  leitura=255;
    
if(SoftSerial.available()>0){
  while(leitura != 10){
    if(SoftSerial.available()>0){
      leitura = SoftSerial.read();
 //     Serial.print((char)leitura);
      
      resposta[i]= (char)leitura;
      i++; 
    }
  }
  i=0;

  for(int j=0; (int)resposta[j] != 10; j++)
  Serial.print(resposta[j]);
  Serial.println(); 
}
}
}