#include <DHT.h>
#define DHTPIN D5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
WiFiClient client;
#include <SoftwareSerial.h>//
#include <stdlib.h>

String apiKey22 = "*************"; //내 서버의 주소
String apiKey360 = "*************";
String apiKey1800 = "*************";
String apiKey3960 = "**************";
const char* ssid = "*************";
const char* password = "************";
const char* server = "api.thingspeak.com";

#define DEBUG true

extern volatile unsigned long timer0_millis;

int i, j, first, erCount, twenty_second, six_minute, thirty_minute, sixtysix_minute;
int twenty_second_Count=7;
int six_minute_Count=134;
int thirty_minute_Count=672;
int sixtysix_minute_Count=1479;
int reset=D7;

float h, t, tumbH, tumbT;
float twenty_second_TotalH, twenty_second_TotalT, twenty_second_ResultT, twenty_second_ResultH;
float six_minute_TotalH, six_minute_TotalT, six_minute_ResultT, six_minute_ResultH;
float thirty_minute_TotalH,thirty_minute_TotalT, thirty_minute_ResultT, thirty_minute_ResultH;
float sixtysix_minute_TotalH, sixtysix_minute_TotalT, sixtysix_minute_ResultT, sixtysix_minute_ResultH;

unsigned long previous_time = 0;
unsigned long interval_send = 2662; //실제로 2.708일떄 실제로2.754쯤  101.6987%늘어짐 //98.3297%

void setup() 
{
  Serial.begin(115200);
  delay(10);
  dht.begin();
 
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("와이파이 ");
  Serial.print(ssid);
  Serial.print("와 연결중");
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print(".");
  delay(500);
  }
  Serial.println();
  Serial.println("연결 완료");
  digitalWrite(reset,HIGH);
  pinMode(reset,OUTPUT);
}
 
void loop() 
{

  unsigned long current_time = millis();
  if(current_time - previous_time > interval_send){
    previous_time = current_time;
    t = dht.readTemperature();
    h = dht.readHumidity();
    if (isnan(h) || isnan(t)) 
    {
      erCount+=1;
      Serial.print("Failed to read from DHT sensor! erCount : ");
      Serial.println(erCount);
      return;
    }
    twenty_second_TotalT=twenty_second_TotalT+t;  
    six_minute_TotalT=six_minute_TotalT+t;
    thirty_minute_TotalT=thirty_minute_TotalT+t;
    sixtysix_minute_TotalT= sixtysix_minute_TotalT+t;
     
    twenty_second_TotalH=twenty_second_TotalH+h;
    six_minute_TotalH=six_minute_TotalH+h;
    thirty_minute_TotalH=thirty_minute_TotalH+h;
    sixtysix_minute_TotalH=sixtysix_minute_TotalH+h;
     
    i=i+1; 
    twenty_second=twenty_second+1;
    six_minute=six_minute+1;
    thirty_minute=thirty_minute+1;
    sixtysix_minute=sixtysix_minute+1;
    
    /*Serial.print("twenty_second : ");
    Serial.print(twenty_second);
    Serial.print("/");
    Serial.print(twenty_second_Count);
    Serial.print(",  ");
     
    Serial.print("six_minute : ");
    Serial.print(six_minute);
    Serial.print("/");
    Serial.print(six_minute_Count);
    Serial.print(",  ");
     
    Serial.print("thirty_minute : ");
    Serial.print(thirty_minute);
    Serial.print("/");
    Serial.print(thirty_minute_Count);
    Serial.print(",  ");

    Serial.print("sixtysix_minute : ");
    Serial.print(sixtysix_minute);
    Serial.print("/");
    Serial.print(sixtysix_minute_Count);
    Serial.print(",  ");

    Serial.print("current T/H : ");
    Serial.print(t);
    Serial.print("/");
    Serial.print(h);
    Serial.print(",  ");

    Serial.print("erCount : ");
    Serial.print(erCount);
    Serial.print(",  6: ");
    Serial.print(six_minute_TotalT);
    Serial.print(" / ");
    Serial.print(six_minute_TotalH);
    Serial.print(",  ");
    Serial.print(six_minute_ResultT);
    Serial.print(" / ");
    Serial.print(six_minute_ResultH);
    Serial.print(",  30: ");

    Serial.print(thirty_minute_TotalT);
    Serial.print(" / ");
    Serial.print(thirty_minute_TotalH);
    Serial.print(",  ");
    Serial.print(thirty_minute_ResultT);
    Serial.print(" / ");
    Serial.print(thirty_minute_ResultH); 
    Serial.print(",  66: ");

    Serial.print(sixtysix_minute_TotalT);
    Serial.print(" / ");
    Serial.print(sixtysix_minute_TotalH);
    Serial.print(",  ");
    Serial.print(sixtysix_minute_ResultT);
    Serial.print(" / ");
    Serial.println(sixtysix_minute_ResultH); 
*/
  }
  
  if(twenty_second==twenty_second_Count&&j==0){
    if(first==0){ // tumbH값으로 앞의 값도 영향을 끼치는데 처음에는 앞의 값이 없으므로
      twenty_second_ResultH=twenty_second_TotalH/twenty_second;
      twenty_second_ResultT=twenty_second_TotalT/twenty_second;
      twenty_second_TotalH=0;
      twenty_second_TotalT=0;
      first=first+1;
    }
    else{
      twenty_second_TotalH=twenty_second_TotalH+(tumbH*47);
      twenty_second_TotalT=twenty_second_TotalT+(tumbT*47);
      twenty_second_ResultH=twenty_second_TotalH/(47+twenty_second);
      twenty_second_ResultT=twenty_second_TotalT/(47+twenty_second);
    }
    tumbH=twenty_second_ResultH;
    tumbT=twenty_second_ResultT;
    twenty_second_TotalH=0;
    twenty_second_TotalT=0;
    j=j+1;
  }
  
  if(six_minute==six_minute_Count){
    six_minute_ResultH=six_minute_TotalH/six_minute_Count;
    six_minute_ResultT=six_minute_TotalT/six_minute_Count;
  }
  if(thirty_minute==thirty_minute_Count){
    thirty_minute_ResultH=thirty_minute_TotalH/thirty_minute_Count;
    thirty_minute_ResultT=thirty_minute_TotalT/thirty_minute_Count;
  }
  if(sixtysix_minute==sixtysix_minute_Count){
    sixtysix_minute_ResultH=sixtysix_minute_TotalH/sixtysix_minute_Count;
    sixtysix_minute_ResultT=sixtysix_minute_TotalT/sixtysix_minute_Count;
  }

  String postStr22;
  String postStr360;
  String postStr1800;
  String postStr3960;


 if(twenty_second>twenty_second_Count){
    if (client.connect(server,80)) {
      postStr22 += apiKey22;
      postStr22 +="&field1=";
      postStr22 += String(twenty_second_ResultT);
      postStr22 +="&field2=";
      postStr22 += String(twenty_second_ResultH);
      postStr22 += "\r\n\r\n";
      
      Serial.print("T/20s : ");
      Serial.println(twenty_second_ResultT);
      Serial.print("H/20s : ");
      Serial.println(twenty_second_ResultH);
     
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey22+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr22.length());
      client.print("\n\n");
      client.print(postStr22);
      twenty_second=0;
      twenty_second_TotalH=0;  //이걸 위에두면 six_minute==138이 2번 작동해서 안됨 get방식에서 쓴 주석이라 잘 모르겠음
      twenty_second_TotalT=0;
      erCount=0; 
      j=0;
    }
    else{
      erCount+=1;
      Serial.println("20초때 에러");
      delay(5000);
      if (client.connect(server,80)) {
        postStr22 += apiKey22;
        postStr22 +="&field1=";
        postStr22 += String(twenty_second_ResultT);
        postStr22 +="&field2=";
        postStr22 += String(twenty_second_ResultH);
        postStr22 += "\r\n\r\n";
      
        Serial.print("T/20s : ");
        Serial.println(twenty_second_ResultT);
        Serial.print("H/20s : ");
        Serial.println(twenty_second_ResultH);
     
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey360+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr22.length());
        client.print("\n\n");
        client.print(postStr22);
        twenty_second=0;
        twenty_second_TotalH=0;  //이걸 위에두면 six_minute==138이 2번 작동해서 안됨 get방식에서 쓴 주석이라 잘 모르겠음
        twenty_second_TotalT=0;
        erCount=0;
        j=0;       
      }
    }
    client.stop();
  }  







  if(six_minute>six_minute_Count){
    if (client.connect(server,80)) {
      postStr360 += apiKey360;
      postStr360 +="&field1=";
      postStr360 += String(six_minute_ResultT);
      postStr360 +="&field2=";
      postStr360 += String(six_minute_ResultH);
      postStr360 += "\r\n\r\n";
      
      Serial.print("T/6m : ");
      Serial.println(six_minute_ResultT);
      Serial.print("H/6m : ");
      Serial.println(six_minute_ResultH);
     
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey360+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr360.length());
      client.print("\n\n");
      client.print(postStr360);
      six_minute=0;
      six_minute_TotalH=0;  //이걸 위에두면 six_minute==138이 2번 작동해서 안됨 get방식에서 쓴 주석이라 잘 모르겠음
      six_minute_TotalT=0;
      erCount=0; 
    }
    else{
      erCount+=1;
      Serial.println("6분때 에러");
      delay(5000);
      if (client.connect(server,80)) {
        postStr360 += apiKey360;
        postStr360 +="&field1=";
        postStr360 += String(six_minute_ResultT);
        postStr360 +="&field2=";
        postStr360 += String(six_minute_ResultH);
        postStr360 += "\r\n\r\n";
      
        Serial.print("T/6m : ");
        Serial.println(six_minute_ResultT);
        Serial.print("H/6m : ");
        Serial.println(six_minute_ResultH);
     
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey360+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr360.length());
        client.print("\n\n");
        client.print(postStr360);
        six_minute=0;
        six_minute_TotalH=0;  //이걸 위에두면 six_minute==138이 2번 작동해서 안됨 get방식에서 쓴 주석이라 잘 모르겠음
        six_minute_TotalT=0;
        erCount=0;       
      }
    }
    client.stop();
  }  
    
  if(thirty_minute>thirty_minute_Count){
    if (client.connect(server,80)) {   
      postStr1800 += apiKey1800;
      postStr1800 +="&field1=";
      postStr1800 += String(thirty_minute_ResultT);
      postStr1800 +="&field2=";
      postStr1800 += String(thirty_minute_ResultH);
      postStr1800 += "\r\n\r\n"; 
            
      Serial.print("T/30m : ");
      Serial.println(thirty_minute_ResultT);
      Serial.print("H/30m : ");
      Serial.println(thirty_minute_ResultH);
     
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey1800+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr1800.length());
      client.print("\n\n");
      client.print(postStr1800);
      thirty_minute=0;
      thirty_minute_TotalH=0;  
      thirty_minute_TotalT=0;
      erCount=0;
    }
    else{
      erCount+=1;
      Serial.println("30분때 에러");
      delay(5000);
      if (client.connect(server,80)) {   
        postStr1800 += apiKey1800;
        postStr1800 +="&field1=";
        postStr1800 += String(thirty_minute_ResultT);
        postStr1800 +="&field2=";
        postStr1800 += String(thirty_minute_ResultH);
        postStr1800 += "\r\n\r\n"; 
            
        Serial.print("T/30m : ");
        Serial.println(thirty_minute_ResultT);
        Serial.print("H/30m : ");
        Serial.println(thirty_minute_ResultH);
     
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey1800+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr1800.length());
        client.print("\n\n");
        client.print(postStr1800);
        thirty_minute=0;
        thirty_minute_TotalH=0;  
        thirty_minute_TotalT=0;
        erCount=0;      
        }
    }
    client.stop();
  }

  if(sixtysix_minute>sixtysix_minute_Count){
    if (client.connect(server,80)) {
      postStr3960 += apiKey3960;
      postStr3960 +="&field1=";
      postStr3960 += String(sixtysix_minute_ResultT);
      postStr3960 +="&field2=";
      postStr3960 += String(sixtysix_minute_ResultH);
      postStr3960 += "\r\n\r\n";
      
      Serial.print("H/66m : ");
      Serial.println(sixtysix_minute_ResultH);
      Serial.print("T/66m : ");
      Serial.println(sixtysix_minute_ResultT);
     
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey3960+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr3960.length());
      client.print("\n\n");
      client.print(postStr3960);
      sixtysix_minute=0;
      sixtysix_minute_TotalH=0;  
      sixtysix_minute_TotalT=0;
      erCount=0;
    }
    else{
      delay(5000);
      Serial.println("66분때 에러");
      erCount+=1;
      if (client.connect(server,80)) {
        postStr3960 += apiKey3960;
        postStr3960 +="&field1=";
        postStr3960 += String(sixtysix_minute_ResultT);
        postStr3960 +="&field2=";
        postStr3960 += String(sixtysix_minute_ResultH);
        postStr3960 += "\r\n\r\n";
      
        Serial.print("H/66m : ");
        Serial.println(sixtysix_minute_ResultH);
        Serial.print("T/66m : ");
        Serial.println(sixtysix_minute_ResultT);
     
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey3960+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr3960.length());
        client.print("\n\n");
        client.print(postStr3960);
        sixtysix_minute=0;
        sixtysix_minute_TotalH=0;  
        sixtysix_minute_TotalT=0;
        erCount=0;
      }
    }
    client.stop();
  }

  if(current_time>4294957294){  //49일후 타이머 초기화
    digitalWrite(reset,LOW);
  }
    
  if(erCount>50){    //에러 누적시 초기화
    digitalWrite(reset,LOW);
  }
}
