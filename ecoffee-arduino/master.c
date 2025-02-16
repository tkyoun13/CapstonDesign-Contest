// MASETER CODE

////////////////////// 패키지 임포트 //////////////////////////
#include <Wire.h>  // I2C 라이브러리 임포트

///////////////////// 핀 번호 설정 ////////////////////////////

// 23 Stepping Motor (원판 회전 모터)
#define stepPin_23 2  // PUL + 핀
#define dirPin_23 3   // DIR + 핀
#define enPin_23 8    // ENA + 핀

// 좌측 17 스텝 모터 (집게팔 앞/뒤 움직임 조절)
#define stepPin_17_L 4  // PUL + 핀
#define dirPin_17_L 5   // DIR + 핀
#define enPin_17_L 9    // ENA + 핀

// 우측 17 스텝 모터 (로봇팔 높이 조절)
#define stepPin_17_R 6  // PUL + 핀
#define dirPin_17_R 7   // DIR + 핀
#define enPin_17_R 10   // ENA + 핀

/////////////////////////  변수 설정  ///////////////////////

int stepstodegrees_23 = 533;  // 30도 = 533/ 90도 = 1600/ 180도 3200 / 360도 = 6400
int stepstodegrees_17 = 50;   //45도 = 100/ 90도 = 200/ 180도 = 400/ 360도 = 800
int stepDelay_23 = 400;       // 각 스텝 사이의 지연 시간 (마이크로초 단위)
int stepDelay_17_R = 600;
int stepDelay_17_L = 800;

const char Case1 = '2';  // 시리얼 트리거 명령어
const char Case2 = '3';  // 시리얼 트리거 명령어
const char Case3 = '1';  // 시리얼 트리거 명령어

volatile bool taskCompleted = false;

void setup() {
  //I2C 통신 Master
  Wire.begin();

  // 핀 모드 설정
  pinMode(stepPin_23, OUTPUT);  // 23 Stepping motor (원판 회전 모터)
  pinMode(dirPin_23, OUTPUT);
  pinMode(enPin_23, OUTPUT);

  pinMode(stepPin_17_L, OUTPUT);  // 좌측 17 Stepping motor
  pinMode(dirPin_17_L, OUTPUT);
  pinMode(enPin_17_L, OUTPUT);

  pinMode(stepPin_17_R, OUTPUT);  // 우측 17 Stepping motor
  pinMode(dirPin_17_R, OUTPUT);
  pinMode(enPin_17_R, OUTPUT);

  // Stepping 모터 활성화
  digitalWrite(enPin_23, LOW);  // LOW로 설정하여 모터 활성화
  digitalWrite(enPin_17_L, LOW);
  digitalWrite(enPin_17_R, LOW);

  // 시리얼 통신 지정
  Serial.begin(9600);
  
  // 로봇팔 초기위치 세팅
  digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
  for (int i = 0; i < 1200; i++) {
    digitalWrite(stepPin_17_R, HIGH);
    delayMicroseconds(stepDelay_17_R);
    digitalWrite(stepPin_17_R, LOW);
    delayMicroseconds(stepDelay_17_R);
  }

  digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
  for (int i = 0; i < 1200; i++) {
    digitalWrite(stepPin_17_L, HIGH);
    digitalWrite(stepPin_17_R, HIGH);
    delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
    digitalWrite(stepPin_17_R, LOW);
    digitalWrite(stepPin_17_L, LOW);
    delayMicroseconds(stepDelay_17_L);
  }
  
}

void loop() {
  if (Serial.available() > 0) {    // 시리얼 데이터가 수신되었는지 확인
    char command = Serial.read();  // 시리얼 데이터 읽기

    if (command == '2') {  // (큰 컵)
      
      delay(1000);

      // 2번 슬레이브로 신호 전송
      // 사용자쪽 문 닫기
      /*
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command5");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋
      

      delay(3000);  // 1초 대기
      */
      // 2번 슬레이브로 신호 전송
      // 로봇팔쪽 문 열기 
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command3");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기
      
      /*
      // 로봇팔 초기위치 세팅
      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 1200; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 1200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);
      */
      

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 0.33; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);
      ///////////////////////////////
      
      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 1100; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);
      
      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 150; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }
      
      delay(2000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command1");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);

      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 900; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 2번 슬레이브로 신호 전송
      // 로봇팔쪽 문 열기 
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command4");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, HIGH);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 7.33; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);

      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 700; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, HIGH);  // LOW = 앞으로 밀기
      for (int i = 0; i < 800; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command2");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      digitalWrite(dirPin_17_R, HIGH);  // LOW = 앞으로 밀기
      for (int i = 0; i < 300; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_R);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_R);
        
      }

      delay(1000);
  
      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 2.7; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);  // 1초 대기

      digitalWrite(dirPin_17_L, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 220; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 2번 슬레이브로 신호 전송
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command1");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(3000);  // 1초 대기

      digitalWrite(dirPin_17_L, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 220; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 2.1; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);

      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command4");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command3");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 2.2; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);  // 1초 대기

      //초기값 리셋
      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      digitalWrite(dirPin_17_L, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 62; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);
      
      // 2번 슬레이브로 신호 전송
      // 사용자쪽 문 열기
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command2");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋
      

      delay(1000);  // 1초 대기
    }

    else if (command == '3') {  // (작은컵)

      delay(1000);
      // 2번 슬레이브로 신호 전송
      // 사용자쪽 문 닫기
      /*
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command5");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋
      

      delay(3000);  // 1초 대기
      */
      // 2번 슬레이브로 신호 전송
      // 로봇팔쪽 문 열기 
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command3");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      /*
      // 로봇팔 초기위치 세팅
      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 1200; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 1200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);
      */

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 0.33; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);
      ///////////////////////////////
      
      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 1100; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 50; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }
      
      delay(2000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command1");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);

      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 900; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_L);
      }
 
      delay(1000);

      // 2번 슬레이브로 신호 전송
      // 로봇팔쪽 문 열기 
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command4");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, HIGH);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 7.33; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);

      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 700; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, HIGH);  // LOW = 앞으로 밀기
      for (int i = 0; i < 800; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command2");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      digitalWrite(dirPin_17_R, HIGH);  // LOW = 앞으로 밀기
      for (int i = 0; i < 220; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_R);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 2.7; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);  // 1초 대기

      digitalWrite(dirPin_17_L, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 2번 슬레이브로 신호 전송
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command1");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(3000);  // 1초 대기

      digitalWrite(dirPin_17_L, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 2.25; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);

      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 450; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);
      

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command4");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 550; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command3");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 2.05; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);  // 1초 대기

      //초기값 리셋
      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      digitalWrite(dirPin_17_L, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 400; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 58; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);
      
      // 2번 슬레이브로 신호 전송
      // 사용자쪽 문 열기
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command2");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋
      

      delay(1000);  // 1초 대기

    }
    else if (command == '1') {  // (라벨링 작은 컵)
      
      delay(1000);
      // 2번 슬레이브로 신호 전송
      // 사용자쪽 문 닫기
      /*
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command5");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋
      

      delay(3000);  // 1초 대기
      */
      // 2번 슬레이브로 신호 전송
      // 로봇팔쪽 문 열기 
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command3");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      /*
      // 로봇팔 초기위치 세팅
      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 1200; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 1200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);
      */

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 0.33; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);
      ///////////////////////////////
      
      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 1100; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 50; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }
      
      delay(2000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command1");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);

      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 900; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_L);
      }
 
      delay(1000);

      // 2번 슬레이브로 신호 전송
      // 로봇팔쪽 문 열기 
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command4");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, HIGH);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 7.33; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);

      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 700; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, HIGH);  // LOW = 앞으로 밀기
      for (int i = 0; i < 800; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command2");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);

      digitalWrite(dirPin_17_L, LOW);  // LOW = 앞으로 밀기
      digitalWrite(dirPin_17_R, HIGH);  // LOW = 앞으로 밀기
      for (int i = 0; i < 220; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_R);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 2.7; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);  // 1초 대기

      digitalWrite(dirPin_17_L, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);

      // 2번 슬레이브로 신호 전송
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command1");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(3000);  // 1초 대기

      digitalWrite(dirPin_17_L, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 220; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 3; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);

      digitalWrite(dirPin_17_R, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 130; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command4");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      // 1번 슬레이브로 신호 전송
      Wire.beginTransmission(8);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command3");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(8, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋

      delay(1000);  // 1초 대기

      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      // 23 Stepping motor 회전
      digitalWrite(dirPin_23, LOW);  // HIGH = 시계방향 회전
      for (int i = 0; i < stepstodegrees_23 * 1.3; i++) {
        digitalWrite(stepPin_23, HIGH);
        delayMicroseconds(stepDelay_23);  // 스텝 사이의 지연 시간
        digitalWrite(stepPin_23, LOW);
        delayMicroseconds(stepDelay_23);
      }

      delay(1000);  // 1초 대기

      //초기값 리셋
      digitalWrite(dirPin_17_R, HIGH);  // HIGH = 들어올리기
      digitalWrite(dirPin_17_L, LOW);  // HIGH = 들어올리기
      for (int i = 0; i < 400; i++) {
        digitalWrite(stepPin_17_R, HIGH);
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_R);
        digitalWrite(stepPin_17_R, LOW);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_R);
      }

      delay(1000);

      digitalWrite(dirPin_17_L, HIGH);  // HIGH = 들어올리기
      for (int i = 0; i < 70; i++) {
        digitalWrite(stepPin_17_L, HIGH);
        delayMicroseconds(stepDelay_17_L);
        digitalWrite(stepPin_17_L, LOW);
        delayMicroseconds(stepDelay_17_L);
      }

      delay(1000);
      
      // 2번 슬레이브로 신호 전송
      // 사용자쪽 문 열기
      Wire.beginTransmission(9);  //I2C 통신 시작. 첫번째 슬레이브 (주소 8)번으로 데이터 전송 시작
      Wire.write("command2");     // "command1" 라는 문자열 slave로 전송
      Wire.endTransmission();     //I2C 통신종료

      delay(1000);

      // Slave로부터 데이터 수신
      while (!taskCompleted) {
        Wire.requestFrom(9, 4);         // 1번 슬레이브 (주소 8번) 에서 4바이트의 데이터("Done") 요청
        if (Wire.available() >= 4) {    // 데이터가 4바이트 이상 수신되었는지 확인
          char response[5];             // 응답을 저장할 배열 (4바이트 + null terminator)
          Wire.readBytes(response, 4);  // 4바이트 읽기
          response[4] = '\0';           // null terminator 추가

          if (strcmp(response, "Done") == 0) {  // 수신된 데이터가 "Done"인지 확인
            taskCompleted = true;
            Serial.println("Task completed");
          }
        }
      }

      taskCompleted = false;  // 플래그 리셋
      

      delay(1000);  // 1초 대기
    }

    else {
      // 케이스 분류에 속하지 않을 때는 아무것도 동작하지 않음
    }
  }
}
