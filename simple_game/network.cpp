#include <WiFi.h>
#include <M5Stack.h>
#include <SPI.h>
#include <stdio.h>


const char* ssid     = "shudo-lab";
const char* password = "shudowireless";


byte server[] = {172,16,0,36};
const int serverPortNo = 2468;


WiFiClient client;

int test(){
    Serial.println("\nHello world!");
}

int updateSocre(double time){

    
    

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

  
    // Connect to server   
  
    char sendBuffer[256];
    

    int thousandTime = time*1000;
    itoa(thousandTime, sendBuffer, 10);
    Serial.println(sendBuffer);

    // Connect to server program 
    // and write data
    
    if( client.connect(server, serverPortNo)){
      Serial.println("connected.");
      client.write(sendBuffer, 256);
      
      int size = 10;
      uint8_t* recvBuffer = (uint8_t*)malloc(size);
      memset(recvBuffer, 0, sizeof(recvBuffer));
      
  
      while( !client.available() );    
      size = client.read(recvBuffer, size);  
      
      Serial.println("End receiving...");
      Serial.println(recvBuffer[0]);
      
      free(recvBuffer); 

      return recvBuffer[0] == 49 ? 1:0;
      
    }else{
      Serial.println("connection failed.");  
      return -1;
    }
}
