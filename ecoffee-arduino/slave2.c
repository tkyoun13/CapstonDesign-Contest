//두번째 슬레이브 (펌프 모터 제어부)
#include <Wire.h> //I2C 라이브러리
#include <Servo.h> //서보 라이브러

Servo myServo1;     //투입구쪽 서보모터(10)
Servo myServo2;     //로봇팔쪽 서보모터(11)

// DC 모터 관련 변수

int DC_clean_1 = 2;  //정수 모터
int DC_clean_2 = 3;
int DC_clean_en = 8;

int DC_pump_1 = 4;  //펌프 모터
int DC_pump_2 = 5;
int DC_pump_en = 9;

// 명령 수신 관련 변수
volatile bool CommandReceived = false; // 명령 수신 여부를 나타내는 플래그
volatile char receivedData[9]; // I2C 통신으로 수신된 데이터를 저장할 배열
volatile bool taskInProgress = false;

void setup() {
  // I2C 통신 관련 세팅
  Wire.begin(9);  // 첫번째 슬레이브 주소 8번으로 지정

  Wire.onReceive(receiveEvent); // I2C 수신 이벤트 발생 시 호출될 함수 설정
  Wire.onRequest(requestEvent); // I2C 요청 이벤트 발생 시 호출될 함수 설정

  pinMode(DC_clean_1, OUTPUT);  // 핀모드 설정
  pinMode(DC_clean_2, OUTPUT);
  pinMode(DC_clean_en, OUTPUT);

  pinMode(DC_pump_1, OUTPUT);
  pinMode(DC_pump_2, OUTPUT);
  pinMode(DC_pump_en, OUTPUT);

  myServo1.attach(10);          //투입구쪽
  myServo2.attach(11);          //로봇팔쪽 

  // 초기 상태: 모터 OFF
  digitalWrite(DC_clean_1, LOW);
  digitalWrite(DC_clean_2, LOW);

  digitalWrite(DC_pump_1, LOW);
  digitalWrite(DC_pump_2, LOW);

  // 초기 상태 : 사용자쪽 open, 로봇팔쪽 close
  myServo1.write(0);        //투입구쪽(102 = close, 0 = open)
  myServo2.write(6);          //로봇팔  (6 = close , 110 = open)

  // 속도 제어 없이 최대 속도로 동작시키기
  analogWrite(DC_clean_en, 255);  
  analogWrite(DC_pump_en, 255);  

  Serial.begin(9600); //Serial 통신 시작
}

void loop() {
  if (CommandReceived) { // 명령이 수신된 경우
    Serial.print("Executing command: ");  // 명령 실행 시작 알림 메시지 출력
    Serial.println((const char*)receivedData);  // 수신된 명령을 Serial Monitor에 출력

    if (strcmp((const char*)receivedData, "command1") == 0) { // 수신된 명령이 "command1"인 경우
      
      // 펌프 모터 2초간 작동
      // 모터 ON (정방향)
      digitalWrite(DC_pump_1, HIGH);
      digitalWrite(DC_pump_2, LOW);
      digitalWrite(DC_clean_1, HIGH);
      digitalWrite(DC_clean_2, LOW);
      delay(2000); // 2초간 작동

      // 모터 OFF
      digitalWrite(DC_pump_1, LOW);
      digitalWrite(DC_pump_2, LOW);
      digitalWrite(DC_clean_1, LOW);
      digitalWrite(DC_clean_2, LOW);

      taskInProgress = true;   //동작 처리 완료
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋

    }
    else if (strcmp((const char*)receivedData, "command2") == 0) { // 수신된 명령이 "command1"인 경우
      //사용자쪽 문 열기
      delay(1000);

      myServo1.write(0);

      
      taskInProgress = true;   //동작 처리 완료
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋
      
    }
    else if (strcmp((const char*)receivedData, "command5") == 0) { // 수신된 명령이 "command1"인 경우
      //사용자쪽 문 닫기
      delay(1000);

      myServo1.write(102);
      
      taskInProgress = true;   //동작 처리 완료
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋
      
    }
    else if (strcmp((const char*)receivedData, "command3") == 0) { 
      //로봇팔 쪽 문열기
      delay(1000);

      myServo2.write(110);

      taskInProgress = true;   //동작 처리 완료 
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋

    }
    else if (strcmp((const char*)receivedData, "command4") == 0) { 
      //로봇팔 쪽 문 닫기
      delay(1000);

      myServo2.write(6);

      taskInProgress = true;   //동작 처리 완료 
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋

    }
  }
}

void receiveEvent(int howMany) { // I2C 수신 이벤트 발생 시 호출되는 함수
  int i = 0;
  while (Wire.available() && i < sizeof(receivedData) - 1) {  // 수신된 데이터가 있는 동안
    receivedData[i++] = Wire.read(); // 데이터를 한 바이트씩 읽어서 배열에 저장
  }
  receivedData[i] = '\0';  // 문자열의 끝을 표시하기 위해 null 문자 추가

  Serial.print("Received from Master: "); // 수신된 데이터를 Serial Monitor에 출력
  Serial.println((const char*)receivedData); // 문자열로 변환하여 출력

  if (strcmp((const char*)receivedData, "command1") == 0) { // 수신된 명령이 "Pump_motor"인 경우
    CommandReceived = true; // 명령 수신 플래그를 true로 설정
  }
  else if (strcmp((const char*)receivedData, "command2") == 0) { // 수신된 명령이 "Pump_motor"인 경우
    CommandReceived = true; // 명령 수신 플래그를 true로 설정
  }
  else if (strcmp((const char*)receivedData, "command3") == 0) { // 수신된 명령이 "Pump_motor"인 경우
    CommandReceived = true; // 명령 수신 플래그를 true로 설정
  }
  else if (strcmp((const char*)receivedData, "command4") == 0) { // 수신된 명령이 "Pump_motor"인 경우
    CommandReceived = true; // 명령 수신 플래그를 true로 설정
  }
  else if (strcmp((const char*)receivedData, "command5") == 0) { // 수신된 명령이 "Pump_motor"인 경우
    CommandReceived = true; // 명령 수신 플래그를 true로 설정
  }
}

void requestEvent() {
  if (taskInProgress) {
    Wire.write("Done"); // 마스터에게 작업 완료 신호 전송
    taskInProgress = false;
  }
}
