from ultralytics import YOLO
import cv2
import time
import serial
from PIL import Image

# 이미지의 크기를 조정하고, 상하 반전한 후 BMP 포맷으로 저장하는 함수
def resize_image(input_path, output_path, width, height):
    img = Image.open(input_path)  # 입력 경로에서 이미지를 열기
    img = img.resize((width, height), Image.LANCZOS)  # 지정된 너비와 높이로 이미지 크기 조정 (LANCZOS 필터 사용)
    img = img.transpose(Image.FLIP_TOP_BOTTOM)  # 이미지 상하 반전
    img.save(output_path, format='BMP')  # BMP 포맷으로 출력 경로에 이미지 저장

# 시리얼 포트를 통해 'f1' 신호를 전송하는 함수
def send_f1_signal():
    try:
        ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=1)  # 시리얼 포트 열기 ('/dev/ttyAMA0' 포트, 115200bps, 1초 타임아웃)
        ser.flush()  # 시리얼 포트 버퍼 비우기
        print("Sending f1 signal...")  # 신호 전송 시작 알림 출력
        ser.write(b'f1')  # 'f1' 신호 전송
        ser.close()  # 시리얼 포트 닫기
        print("f1 signal sent.")  # 신호 전송 완료 알림 출력
    except Exception as e:
        print(f"Error sending f1 signal: {e}")  # 예외 발생 시 에러 메시지 출력

# 시리얼 포트를 통해 이미지를 전송하는 함수
def send_image(filename):
    try:
        ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=1)  # 시리얼 포트 열기
        ser.flush()  # 시리얼 포트 버퍼 비우기
        print("Sending image...")  # 이미지 전송 시작 알림 출력
        with open(filename, 'rb') as f:  # 파일을 바이너리 모드로 열기
            f.seek(54)  # BMP 헤더를 건너뛰기 (54바이트 헤더)
            data = f.read(1024)  # 1024바이트씩 데이터 읽기
            while data:  # 데이터가 남아있는 동안
                ser.write(data)  # 시리얼 포트로 데이터 전송
                time.sleep(0.05)  # 50ms 대기
                data = f.read(1024)  # 다음 1024바이트 데이터 읽기
        ser.write(b'IMAGE_SENT')  # 이미지 전송 완료 신호 전송
        print("Image sent.")  # 이미지 전송 완료 알림 출력
        ser.close()  # 시리얼 포트 닫기
    except Exception as e:
        print(f"Error sending image: {e}")  # 예외 발생 시 에러 메시지 출력

# 화재를 감지하고 이미지를 캡처하는 함수
def detect_fire_and_capture(model, camera_index=0):
    cap = cv2.VideoCapture(camera_index)  # 카메라를 통해 비디오 캡처 객체 생성
    fire_detected = False  # 화재 감지 상태를 나타내는 플래그 (초기값: False)
    fire_start_time = 0  # 화재 감지 시작 시간을 기록하는 변수
    image_sent = False  # 이미지 전송 여부를 나타내는 플래그 (초기값: False)

    while True:  # 무한 루프 (종료 조건이 없으면 계속 반복)
        ret, frame = cap.read()  # 카메라에서 프레임 읽기
        if not ret:  # 프레임 읽기에 실패한 경우
            break  # 루프 종료

        results = model.predict(source=frame, show=True)  # 모델을 통해 프레임에서 객체 감지 (결과 표시)

        # 감지된 객체가 있고, 감지된 객체의 클래스 ID가 'fire'인 경우
        if results and results[0].boxes is not None and len(results[0].boxes) > 0:
            for box in results[0].boxes:  # 감지된 각 박스에 대해
                class_id = int(box.cls[0])  # 클래스 ID 가져오기
                if class_id in model.names and model.names[class_id] == 'fire':  # 클래스가 'fire'인 경우
                    if not fire_detected:  # 화재가 아직 감지되지 않았을 경우
                        fire_detected = True  # 화재 감지 상태를 True로 설정
                        fire_start_time = time.time()  # 화재 감지 시작 시간 기록
                    break
            else:
                fire_detected = False  # 감지된 객체가 없거나 클래스가 'fire'가 아닐 경우 화재 감지 상태를 False로 설정
        else:
            fire_detected = False  # 감지된 객체가 없는 경우 화재 감지 상태를 False로 설정

        # 화재가 감지되고, 3초 이상 경과했으며, 이미지를 전송하지 않은 경우
        if fire_detected and (time.time() - fire_start_time >= 3) and not image_sent:
            send_f1_signal()  # 'f1' 신호 전송
            time.sleep(1)  # 1초 대기
            img_filename = '/home/robo/captured_fire.jpg'  # 저장할 이미지 파일 이름
            cv2.imwrite(img_filename, frame)  # 현재 프레임을 파일로 저장
            print("Fire detected for 3 seconds, capturing image...")  # 화재 감지 및 이미지 캡처 알림 출력
            resize_image(img_filename, '/home/robo/converted_image.bmp', 160, 120)  # 이미지 크기 조정 및 저장
            send_image('/home/robo/converted_image.bmp')  # 조정된 이미지를 전송
            image_sent = True  # 이미지가 전송되었음을 기록

        if not fire_detected:  # 화재가 감지되지 않은 경우
            fire_start_time = 0  # 화재 감지 시작 시간 초기화
            image_sent = False  # 이미지 전송 상태 초기화

        if cv2.waitKey(1) & 0xFF == ord('q'):  # 'q' 키가 눌렸는지 확인
            break  # 루프 종료

    cap.release()  # 카메라 자원 해제
    cv2.destroyAllWindows()  # 모든 OpenCV 창 닫기

# 스크립트 실행 시 YOLO 모델을 로드하고 화재 감지 및 캡처 함수 호출
if __name__ == "__main__":
    model = YOLO("/home/robo/Desktop/best_full_integer_quant_edgetpu.tflite", task="detect")  # YOLO 모델 로드
    detect_fire_and_capture(model, camera_index=0)  # 화재 감지 및 캡처 함수 호출
