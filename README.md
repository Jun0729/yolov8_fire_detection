# yolov8_fire_detection

test.py는 라즈베리파이(화재 감지부에서의 제품 동작입니다)
best_full_integer_quant_edgetpu.tflite 은 불 사진을 학습시킨 YOLOV8 모델을 TPU 사용 가능한 모델로 변환시킨
TFLITE 파일입니다.
졸작 아두이노 폴더는 연동디바이스에 관한 코드입니다(기능 별로 c파일을 분리 해놨습니다.)
