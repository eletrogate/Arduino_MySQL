/* 
 * Programa para ESP-01
 * Eletrogate
 * Arduino IDE versão 1.8.12
 */
#include <ESP8266WiFi.h>          
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

IPAddress server_addr(54, 39, 75, 7); // O IP DO SERVIDOR DA CLEVER CLOUD
char user[] = ">USUARIO CLEVER CLOUD<";              // Usuario MySQL
char password[] = ">COLOQUE AQUI A SENHA<";        //   Senha MySQL

char ssid[] = ">SEU USUARIO WIFI<";         //  Nome de rede Wifi
char pass[] = ">SENHA_WIFI<";     //            Senha Wi-Fi

char INSERT_DATA[] = "INSERT INTO >NOME_DO_BANCO_DE_DADOS<.Temperatura (ID_SENSOR, Temperatura) VALUES (%d,%s)"; 
//   EXEMPLO ACIMA:  "INSERT INTO b3d3z8bggux9ngtdusfw.Temperatura (ID_SENSOR, Temperatura) VALUES (%d,%s)"; 

WiFiClient client;             
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

void setup() {
  Serial.begin(9600);
  VerificaWiFi();
}

void loop() {
  int id;
  float temp;

  AguardaDados();
  Leitura(&id, &temp);
  delay(100);
  EnviaDados(id, temp);
  Serial.println();
}

void AguardaDados() {
  while (!(Serial.available() > 0)) {}
}
void Leitura(int *id, float *temp) {
  char  mensagem[20];
  byte atual, i = 0;
  atual = 255;

  if (Serial.available() > 0) {
    while (atual != 10) {
      if (Serial.available() > 0) {
        atual = Serial.read();
        //     Serial.print((char)leitura);
        mensagem[i] = (char)atual;
        i++;
      }
    }
    i = 0;
    *id = atoi(strtok(mensagem, "|"));
    *temp = atof(strtok(NULL, "|"));
  }

}
void EnviaDados(int id, float temp) {

  char query[128];
  char temperatura[10];
  VerificaWiFi();
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    dtostrf(temp, 1, 1, temperatura);
    sprintf(query, INSERT_DATA, id, temperatura);
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    Serial.println(); Serial.println("Data recorded."); //
  }
  else
    Serial.println(); Serial.println("Connection failed."); //
  conn.close();
}

void VerificaWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    //Serial.println("Sem conexão"); //
    WiFi.disconnect();
    delay(1000);
    WiFi.begin(ssid, pass);
    // Serial.println();Serial.println("Conectando ao WiFi."); //
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    // Serial.println();Serial.println("Conectado a rede!"); //
  }
}