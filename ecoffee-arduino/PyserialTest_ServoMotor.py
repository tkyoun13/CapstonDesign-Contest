import serial
import time

# 시리얼 포트 설정 (Jetson Orin Nano의 시리얼 포트에 맞춰 수정 필요)
port = '/dev/ttyACM0'  # Jetson의 시리얼 포트
baud_rate = 9600

# 시리얼 포트 열기
ser = serial.Serial(port, baud_rate, timeout=1)

# 아두이노에 'Start_Process' 명령 전송
def send_start_command():
    ser.write(b'Start_Process\n')  # 명령 전송
    print("Sent: Start_Process")

# 아두이노로부터 응답 수신
def receive_done_response():
    while True:
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8').strip()
            if response == "Done_Process":
                print("Received: Done_Process")
                break

if __name__ == "__main__":
    try:
        send_start_command()
        receive_done_response()
    except KeyboardInterrupt:
        print("Communication stopped")
    finally:
        ser.close()