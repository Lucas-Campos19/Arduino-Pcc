#include <SPI.h>
#include <Ethernet.h>

// Definições de IP, máscara de rede e gateway
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,10);
IPAddress server(192, 168, 1, 4);
int port = 5078;
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetClient client;

#define ldrPin A1
int vldr = 0;

void setup() {
  pinMode(ldrPin, INPUT);
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
}

void loop() {
  vldr = analogRead(ldrPin);
  Serial.print("Lux ambiente: ");
  Serial.println(vldr);


  if (client.connect(server,port)) { // Conecta ao servidor ASP.NET Core
    String url = "/ldr?values=" + String(vldr);

    
    // Fazendo a requisição HTTP GET
   // Fazendo a requisição HTTP GET
    client.print("GET ");
    client.print(url);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println("192.168.1.4");
    client.println("Connection: close");
    client.println(); // Fim dos cabeçalhos


    // Aguarda a resposta do servidor
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
  } else {
    Serial.println("Connection failed");
  }

  delay(100); // Envia a cada 10 segundos
}