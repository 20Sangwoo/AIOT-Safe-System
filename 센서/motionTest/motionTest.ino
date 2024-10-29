#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int Pin = D5;                // HC-SR501 센서 핀
int pirState = LOW;          // 센서 초기 상태 저장

const char* ssid = "핫스팟";           // WiFi 이름
const char* password = "1234567890q";  // WiFi 비밀번호

const char* mqtt_server = "test.mosquitto.org"; // 공용 MQTT 브로커 주소

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;

void setup() {
  pinMode(Pin, INPUT);         // 센서를 입력 모드로 설정
  Serial.begin(115200);        // 시리얼 모니터 초기화
  setup_wifi();                // WiFi 연결 설정

  client.setServer(mqtt_server, 1883); // MQTT 서버 설정
  reconnect();                 // 초기 MQTT 연결 시도
}

void setup_wifi() {
  WiFi.begin(ssid, password); // WiFi 연결 시작
  Serial.print("WiFi 연결 중");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi 연결 완료");
  Serial.print("IP 주소: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT 서버에 연결 중...");
    if (client.connect("NodeMCUClient")) {
      Serial.println("연결 성공");
    } else {
      Serial.print("오류 코드: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsgTime > 1000) { // 1초마다 체크
    lastMsgTime = now;

    int motionDetected = digitalRead(Pin);
    const char* motionTopic = "m5stack/motion";

    // 센서 상태 변경 시 메시지 발행
    if (motionDetected != pirState) {
      if (motionDetected == HIGH) {
        Serial.println("움직임 감지!");
        client.publish(motionTopic, "O"); // danger 메시지 발송
      } else {
        Serial.println("움직임 없음");
        client.publish(motionTopic, "X"); // safe 메시지 발송
      }
      pirState = motionDetected; // 상태 업데이트
    }
  }
}