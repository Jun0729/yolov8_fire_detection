#define BMP_WIDTH  160  // 비트맵 이미지의 너비를 160 픽셀로 정의
#define BMP_HEIGHT 120  // 비트맵 이미지의 높이를 120 픽셀로 정의
#define SCREEN_WIDTH 320  // LCD 스크린의 너비를 320 픽셀로 정의
#define SCREEN_HEIGHT 240  // LCD 스크린의 높이를 240 픽셀로 정의
#define ROW_SIZE ((BMP_WIDTH * 3 + 3) & (~3)) // 각 행의 크기를 4의 배수로 맞춤 (비트맵 패딩 처리) 패딩 처리 검색해봐
#define BUFFER_SIZE (ROW_SIZE * BMP_HEIGHT) // 전체 이미지 데이터를 저장할 버퍼 크기 계산
unsigned char imageData[BUFFER_SIZE]; // 비트맵 데이터를 저장할 배열 선언

// 라즈베리파이에서 비트맵 이미지를 수신하여 디스플레이하는 함수
void receiveAndDisplayImage() {
  // 이미지를 화면 중앙에 표시하기 위한 좌표 계산
  int x_center = (SCREEN_WIDTH - BMP_WIDTH) / 2; // 가로 중심 계산
  int y_center = (SCREEN_HEIGHT - BMP_HEIGHT) / 2; // 세로 중심 계산

  Serial.println("Waiting for image..."); // 시리얼 모니터에 이미지 수신 대기 메시지 출력

  // BMP 데이터 수신
  int index = 0; // 이미지 데이터를 저장할 배열의 인덱스 초기화
  while (index < BUFFER_SIZE) { // 전체 버퍼 크기만큼 데이터를 수신할 때까지 반복
    if (Serial2.available() > 0) { // 시리얼2(HC-12 모듈)로부터 데이터가 들어왔는지 확인
      imageData[index++] = Serial2.read(); // 데이터를 읽어 imageData 배열에 저장
    }
  }

  Serial.println("Image received. Displaying..."); // 이미지 수신 완료 후 시리얼 모니터에 메시지 출력

  // BMP 데이터를 화면에 그리기
  for (int y = 0; y < BMP_HEIGHT; y++) { // 이미지의 세로 높이만큼 반복
    int dataIndex = y * ROW_SIZE; // 패딩을 포함한 각 행의 시작 인덱스 계산
    for (int x = 0; x < BMP_WIDTH; x++) { // 이미지의 가로 너비만큼 반복
      if (dataIndex < BUFFER_SIZE - 2) { // 버퍼 크기를 초과하지 않도록 확인
        char b = imageData[dataIndex++]; // BMP 데이터에서 파란색(B) 값 읽기
        char g = imageData[dataIndex++]; // BMP 데이터에서 초록색(G) 값 읽기
        char r = imageData[dataIndex++]; // BMP 데이터에서 빨간색(R) 값 읽기
        
        // 16비트 색상으로 변환 (RGB565 형식) 비트맵은 24비트형식이라 16비트로 변경
        int color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

        // 계산된 색상으로 픽셀을 화면에 그리기
        myGLCD.setColor(color); // 그릴 색상 설정
        myGLCD.drawPixel(x_center + x, y_center + y); // 화면 중앙을 기준으로 픽셀 그리기
      }
    }
  }

  Serial.println("Image displayed."); // 이미지 출력 완료 후 시리얼 모니터에 메시지 출력
  imageDisplayed = true; // 이미지가 표시되었음을 기록하는 플래그 설정
}
