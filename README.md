# IOT-Safe-System <br><br>

IoT 기반 안전 알림 시스템<br><br>

IoT 기반 안전 알림 시스템은 실내에서 발생할 수 있는 가스 누출, 화재, 무단침입 같은 사건과 사고를<br>
예방하고 방지하는 것을 목표로 기획 및 설계, 진행된 프로젝트입니다.<br><br>

특히 노약자, 어린이, 청각 또는 시각에 불편함을 가지고 계신 분들 등, 사회적, 신체적 약자분들도 큰 어려움과 <br>
불편함 없이 이용할 수 있는 것을 목표로 설계했고 현재는 초기 목표치로 잡았던 시스템의 구성 중<br>
7~80% 정도의 완성도에 도달했습니다. UI와 디자인 부분에선 아직 보완할 점이 많이 남았기에<br>
앞으로 수정 및 보완해나가며 완성도를 높일 예정입니다.<br><br>

이 시스템은 여러 센서들과 세이프워치로 구성되어있습니다.<br>
각 센서 실내의 적절한 위치에 설치한 후, M5 core 기반 세이프워치를 사용자가 손목에 장착하고 전원을 켜면<br>
IoT 안전 알림 시스템이 작동합니다.<br><br>

각 센서와 세이프워치는 nodeMCU를 통해 WIFI를 통해 인터넷 연결을 하고<br>
MQTT 프로토콜을 이용해 같은 서버 안에서 실시간으로 정보를 주고 받으며 처리합니다.<br>
그래서 거리가 멀리 떨어져 있거나, 만약 M5기기와 각 센서가 다른 WIFI 신호를 사용해야하는 환경이라고 해도<br>
서로 동일한 MQTT 프로토콜 서버를 이용해 통신한다면 거리 제약 없이 이용할 수 있습니다.<br><br>

평소에는 안전 상태를 알리고 현재 시각을 표시하는 화면이지만,<br>
센서에서 감지한 데이터에 이상 징후가 있다면 세이프워치는 그 데이터를 분석해, 경고 메시지를 출력하고<br>
기기 내의 진동 모터를 제어해 진동 및 경고 메시지로 시각과 청각에 불편함을 가지신 분들도 위험을 즉각 감지하고<br>
그에 대처할 수 있도록 설계했습니다.<br><br>

세부 센서 기능을 설명드리겠습니다.<br><br>

온습도 센서는 0–50℃ 범위를 감지하며, 45–48℃ 이상을 경고 온도로 설정했습니다.<br> 
오늘 시연에서는 경고 온도를 35℃로 낮추었습니다.<br><br>

가연성 가스 센서는 공기 저항값과 공기 중 각 가스 성분비를 계산하는 공식을 사용해 가연성 가스의 성분비 분석을 진행하고<br>
실생활에서 인체에 부정적인 영향을 끼칠 수 있는 공기 중 가스 성분의 비율은 메탄 약 10,000ppm, LPG 5,000ppm, 일산화탄소 200ppm이지만,<br>
오늘은 시연을 위해 각각 400, 400, 150ppm으로 설정해 두었습니다.<br><br>

적외선 센서는 120° 의 감지각과 7M 내의 거리의 움직임을 탐지하여 무단 침입 발생 시<br>
세이프워치에서 경고하도록 설정했습니다.<br><br>

앞서 설명드린 위험 상황을 판별하는 데이터 기준은 센서의 데이터 시트를 참고하고<br>
실제 재난 기사문과 인터넷 자료(재난센터 등)를 참고해서 로직의 위험 기준을 설정했습니다.<br><br>

세이프워치는 M5 core 기기를 활용해, LCD 모니터, 진동 모터, 버튼을 통한 밝기 조정과 진동 제어 기능을 적용하여<br>
사용자의 편의성을 높였습니다. 그리고 스트랩을 결합해서 손목에 착용해 간편하게 사용할 수 있게 했습니다.<br><br>

시스템 개발에 있어서 가장 어려웠던 점은 안정적이고 실시간으로 통신을 주고 받을 수 있고 저전력 장치를 사용하는 데에<br>
적당한 통신을 설계하는 점이었는데, 그래서 통신 설계에는 가벼우면서 실시간 통신에 적합한 MQTT 프로토콜 방식을 택했습니다.<br><br>

또, MQTT 통신을 사용하기 위해서 인터넷의 사용도 필수였는데 비용 대비 좋은 성능을 가졌고 프로그래밍 편의성이 뛰어나며<br>
접근하기 쉬운 WIFI를 이용할 수 있는 nodeMCU 플랫폼을 활용했습니다.<br><br>

각 센서에 아두이노를 이용한 이유는 학교에서 공부했던 개발 툴이라 친숙하기도 하고 다양한 라이브러리를 지원하며 또 호환성이 높기 때문에 택했고<br>
세이프워치는 M5 core와 호환이 되고 블록 코딩과 마이크로파이썬을 제공해서 더 가볍고 간단한 문법을 사용하기 때문에,<br>
작은 IoT 장치에 적합하다고 생각해 이용했습니다.
