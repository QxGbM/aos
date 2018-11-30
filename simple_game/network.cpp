/* 
 *  Project: Simple Game
 *  Team: 810
 *  Part: Liang Wei
 *  
 *  Warning:
 *  
 *  1) Don't forget to replace the ssid, password and server variables with your true WiFi ssid and password.
 *  2) Don't forget to chage the server[] variant to the real IP address of your server.
 *  3) I set the default port number on server as 2468, if you like you can change it.
 *  
 *  
 *  Usage:
 *  
 *  WiFiTest():
 *    Just a test function, meaningless.
 * 
 *  int WiFiConnection():
 *    You can use it to establish a WiFi connection. *    
 *    Those variables can be found in the front of this source code.
 *    It will return 0.
 *    And I set the port number to 2468.
 *    You can change it, though, just make sure it's identical with the value in the server program.
 * 
 *  double getHistory():
 *    It can be used to get the best score from server.
 *    I initialed the best score as -1 on the server.
 *    Because the signed integer -1 is the maximum in unsigned integers.
 *    It can make the comparison simpler.
 *    This function returns a double float number indicating seconds.
 *  
 *  int updateScore(double timeinseconds)
 *    When user completes a game, it can be used to take the current score ( time in seconds )
 *    as argument, and compare the best score on server.
 *    If the curent score is the most fast ever, server will return 1, otherwise 0.
 *    And this function takes the return value of server as its return value.
 */
#include <WiFi.h>
#include <M5Stack.h>
#include <SPI.h>
#include <stdio.h>
#include "network.h"

const char* ssid     = "shudo-lab";
const char* password = "shudowireless";


byte server[] = {172,16,0,22};
const int serverPortNo = 2468;  


char sendBuffer[256];
char recvBuffer[256];


WiFiClient client;

int WiFiTest(){
    WiFiConnect();
    Serial.println(fetchHistory());
    updateScore(13.141);
    Serial.println(fetchHistory());
    updateScore(26.282);
    Serial.println(fetchHistory());
}

int WiFiConnect(){
    //delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // begin connection
    WiFi.begin(ssid, password);

    // wait untile it's connected
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    return 0;
}


double fetchHistory(){
    if( client.connect(server, serverPortNo)){
      //Serial.println("connected.");
      
      char sendBuffer[256];
      
      strcpy(sendBuffer, "history");
      client.write(sendBuffer, sizeof(sendBuffer));
      //Serial.println("sended.");

      // Read data from server to recvBuffer
      memset(recvBuffer, 0, sizeof(recvBuffer));

      while( !client.available() );    // wait untile M5Stack can be read
      client.read((uint8_t*)recvBuffer, sizeof(recvBuffer)); // read to recvBuffer
      //Serial.println("End receiving history ...");

      // conver the history score char array to integer
      int historyResult;
      sscanf(recvBuffer, "%d", &historyResult); // convert the char array to integer

      // Because -1 is the default value, so if is -1, display 0 in screen
      return historyResult==-1 ? 0 : historyResult/1000.0; 
    }else{
      Serial.println("connection failed.");  
      return -1;
    }
}

int updateScore(double timeinseconds){

    // take the time in seconds as argument
    // time it with 1000, convert it to integer
    int timeinmilli = timeinseconds*1000;
    itoa(timeinmilli, sendBuffer, 10);   // convert it to char array
    

    // connect to server program
    if( client.connect(server, serverPortNo)){  
      client.write(sendBuffer, 256);  // write the time data to server
      

      // receive the update result
      // if it is the fastest score ever
      // server will return 1 back
      // if not
      // server will return 0 back
      memset(recvBuffer, 0, sizeof(recvBuffer));

      while( !client.available() ) ; // wait until server send the data back
      client.read((uint8_t*)recvBuffer, sizeof(recvBuffer));
      


      // convert the update result char array to integer
      int updateResult;
      sscanf(recvBuffer, "%d", &updateResult);

      Serial.print(timeinseconds);
      if( updateResult )  Serial.println(", updated.");
      else Serial.println(", not updated.");
      
      return updateResult;         
      
    }else{
      Serial.println("connection failed.");  
      return -1;
    }
}
