//첫번째 슬레이브 (집게팔 제어부)
#include <Wire.h> //I2C 라이브러리
#include <Servo.h> // 서보 모터 라이브러리 임포트

// 집게팔 17 스텝 모터 (집게팔 회전 모터)
#define stepPin_17_J 2   // PUL + 핀
#define dirPin_17_J 3    // DIR + 핀
#define enPin_17_J 8     // ENA + 핀

// 명령 수신 관련 변수
volatile bool CommandReceived = false; // 명령 수신 여부를 나타내는 플래그
volatile char receivedData[9]; // I2C 통신으로 수신된 데이터를 저장할 배열
volatile bool taskInProgress = false;

Servo myServo;

// 모터 관련 변수
int stepstodegrees_17 = 400;       //45도 = 100/ 90도 = 200/ 180도 = 400/ 360도 = 800
int stepDelay_17 = 2000; 

void setup() {
  // I2C 통신 관련 세팅
  Wire.begin(8);  // 첫번째 슬레이브 주소 8번으로 지정

  Wire.onReceive(receiveEvent); // I2C 수신 이벤트 발생 시 호출될 함수 설정
  Wire.onRequest(requestEvent); // I2C 요청 이벤트 발생 시 호출될 함수 설정

  //핀 번호 설정
  myServo.attach(9);             // Servo motor (집게팔 모터)

  //핀모드 설정
  pinMode(stepPin_17_J, OUTPUT);  // 집게팔 17 Stepping motor
  pinMode(dirPin_17_J, OUTPUT);
  pinMode(enPin_17_J, OUTPUT);

  //서보모터 초기 위치 세팅
  myServo.write(0);

  Serial.begin(9600); //Serial 통신 시작
}

void loop() {
  if (CommandReceived) { // 명령이 수신된 경우
    Serial.print("Executing command: ");  // 명령 실행 시작 알림 메시지 출력
    Serial.println((const char*)receivedData);  // 수신된 명령을 Serial Monitor에 출력

    if (strcmp((const char*)receivedData, "command1") == 0) { // 수신된 명령이 "command1"인 경우
      
      //집게팔 집기
      for (int pos = 10; pos <= 100; pos += 2) {  // 10~100도까지 2도 단위로 회전 //180에 가까울수록 집게 조임 //0도에 가까울수록 집게 벌림
        myServo.write(pos);                       // 각도 설정
        delay(5);                                // 지연 시간 설정 (줄일 수록 회전속도 증가)
      }
      taskInProgress = true;   // 동작 처리 완료
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋
    }

    else if (strcmp((const char*)receivedData, "command2") == 0) { // 수신된 명령이 "command2"인 경우
      
      //집게팔 시계방향 회전하기
      digitalWrite(dirPin_17_J, HIGH);                    // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_17; i++) {       //시계방향으로 180도 회
        digitalWrite(stepPin_17_J, HIGH);
        delayMicroseconds(stepDelay_17);                     // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_J, LOW);
        delayMicroseconds(stepDelay_17);
      }
      taskInProgress = true;   //동작 처리 완료
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋
    }

    else if (strcmp((const char*)receivedData, "command3") == 0) { // 수신된 명령이 "command3"인 경우
      
      //집게팔 반시계방향 회전하기
      digitalWrite(dirPin_17_J, LOW);                    // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_17; i++) {       //시계방향으로 180도 회
        digitalWrite(stepPin_17_J, HIGH);
        delayMicroseconds(stepDelay_17);                     // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_J, LOW);
        delayMicroseconds(stepDelay_17);
      }

      taskInProgress = true;
      CommandReceived = false; // 명령 처리 완료, 플래그를 false로 리셋
    }

    else if (strcmp((const char*)receivedData, "command4") == 0) { // 수신된 명령이 "command3"인 경우
      
      myServo.write(0);

      taskInProgress = true;
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
}

void requestEvent() {
  if (taskInProgress) {
    Wire.write("Done"); // 마스터에게 작업 완료 신호 전송
    taskInProgress = false;
  }
}
