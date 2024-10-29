#include <M5Core2.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <PubSubClient.h>

// Wi-Fi 설정
const char* ssid = "KSM";
const char* password = "ksm!@1419";

// MQTT 브로커 설정
const char* mqtt_server = "test.mosquitto.org";
const char* tempTopic =  "m5stack/temperature";
const char* gasTopic = "m5stack/gas";

const int temp_threshold = 45;

// ntp설정 (시간)
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 9 * 3600); // UTC+9 (대한민국)


// 재난 경고 기준치 설정
float tempValue = 0.0;  // 온도 값
String gasValue =" ";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Connecting to WiFi...");

  setup_wifi();               // Wi-Fi 연결 설정
  timeClient.begin();        // NTPClient 시작
  timeClient.update();       // 초기 시간 업데이트
  
  client.setServer(mqtt_server, 1883);  // MQTT 서버 설정
  client.setCallback(callback);         // 콜백 함수 설정

  while (!client.connected()) {     // MQTT클라이언트가 서버에 연결 될 때 까지 반복 
    if (client.connect("M5Core2Client")) {  //mqtt서버에 연결 시도 
      client.subscribe(tempTopic);
      client.subscribe(gasTopic);
    } else {
      delay(2000);
    }
  }
}

void setup_wifi() {
  WiFi.begin(ssid, password);  // Wi-Fi 연결 시작

  while (WiFi.status() != WL_CONNECTED) {  // 연결될 때까지 대기
    delay(500);
    M5.Lcd.print(".");
  }

  M5.Lcd.println("WiFi connected");
}

void displayTime() {
    timeClient.update(); // 시간을 업데이트
    String formattedTime = timeClient.getFormattedTime(); // HH:MM:SS 형식으로 시간 가져오기

    M5.Lcd.setCursor(50, 10); // 상단에 시간 출력
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print("Time: ");
    M5.Lcd.println(formattedTime); // 시간 출력
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];  // 수신된 메시지를 문자열로 변환
  }

  Serial.print("Received raw message: ");
  Serial.print("Topic:");
  Serial.print(topic);
  Serial.print("massge: ");
  Serial.println(message);  // 수신된 원본 메시지 출력

  
  //주제에 따라 처리
  if (String(topic) == tempTopic) {
    tempValue = message.toFloat();
    Serial.print("temp value:");
    Serial.println(tempValue);
  }else if (String(topic) == gasTopic) {
     gasValue = message;
     Serial.print("gas value:");
     Serial.println(gasValue);  
  }
  
  M5.Lcd.fillScreen(BLACK); // 화면 지우기
  displayTime(); // 시간 표시

  M5.Lcd.setCursor(50, 80);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Temperature: " + String(tempValue)); // 온도 값 출력

  M5.Lcd.setCursor(50, 100);
  M5.Lcd.println("Gas Level: ");

  M5.Lcd.setCursor(170, 100);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.println(gasValue);  // 가스 값 출력  
 
  if (tempValue >= temp_threshold) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(50, 110);
    M5.Lcd.setTextColor(RED);   
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("High Temperature Alert!");
    M5.Lcd.setCursor(50, 120);
    M5.Lcd.println("Temperature: " + String(tempValue)); // 온도 값 출력
    M5.Axp.SetLDOEnable(3, true); // 진동 모터 활성화
    delay(2000);                  // 진동 시간 조절
    M5.Axp.SetLDOEnable(3, false);// 진동 모터 비활성화
  }  

  if (gasValue == "Denger") {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(50, 130);
    M5.Lcd.setTextColor(RED); 
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("Gas Leak Alert!");
    M5.Lcd.setCursor(50, 150);
    M5.Lcd.println("Gas Level: " + String(gasValue)); // 가스 값 출력
    M5.Axp.SetLDOEnable(3, true); // 진동 모터 활성화
    delay(2000);                  // 진동 시간 조절
    M5.Axp.SetLDOEnable(3, false);// 진동 모터 비활성화
  }
}

void loop() {
  if (!client.connected()) {
    while (!client.connected()) {
      if (client.connect("M5Core2Client")) {
        client.subscribe(tempTopic);  // 주제 재구독
        client.subscribe(gasTopic);
      }
      delay(2000);
    }
  }
  client.loop();  // MQTT 클라이언트 루프 실행

  // 시간 표시 업데이트
    displayTime();
    delay(1000); // 1초마다 시간 업데이트
}