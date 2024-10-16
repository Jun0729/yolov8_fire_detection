void checkSerial() {
  // Serial2에서 수신된 데이터가 있을 경우 처리
  if (Serial2.available() > 0) { 
    String receivedData = Serial2.readString(); // Serial2로부터 수신된 데이터를 문자열로 저장
    Serial.println(receivedData); // 수신된 데이터를 시리얼 모니터에 출력

    // "f1"이 수신된 경우 (공대 가동에서 불이 난 상황)
    if (receivedData.indexOf("f1") >= 0) { 
      blinkGa = true; // 공대 가동 건물이 깜빡이도록 설정
      receiveAndDisplayImage(); // 이미지를 수신하여 화면에 표시하는 함수 호출
    }
    
    // "f2"가 수신된 경우 (공대 나동에서 불이 난 상황)
    if (receivedData.indexOf("f2") >= 0) {
      blinkNa = true; // 공대 나동 건물이 깜빡이도록 설정
    }
    
    // "f3"이 수신된 경우 (공대 다동에서 불이 난 상황)
    if (receivedData.indexOf("f3") >= 0) {
      blinkDa = true; // 공대 다동 건물이 깜빡이도록 설정
    }

    // 풍속 데이터를 수신하는 경우 (문자열에 "wind_speed:"가 포함되어 있을 때)
    if (receivedData.indexOf("wind_speed:") >= 0) {
      int startIndex = receivedData.indexOf("wind_speed:") + 11; // 풍속 값의 시작 인덱스 설정
      int endIndex = receivedData.indexOf("\n", startIndex); // 풍속 값의 끝 인덱스 설정 (다음 줄 바꿈까지)
      windSpeed = receivedData.substring(startIndex, endIndex); // 풍속 값을 추출하여 windSpeed 변수에 저장
      updateWindSpeedDisplay(); // 풍속 값을 화면에 업데이트하는 함수 호출
    }
  }
}

void updateWindSpeedDisplay() {
  // 기존의 풍속 값을 지우기 위해 배경색(검정색)으로 설정
  myGLCD.setColor(0, 0, 0);
  // 기존 풍속 값이 출력된 영역을 지움 (좌표로 사각형 영역을 설정)
  myGLCD.fillRect(LEFT + 90, 204, LEFT + 190, 220);

  // 새로운 풍속 값을 출력하기 위해 글자 색상을 흰색으로 설정
  myGLCD.setColor(255, 255, 255);
  // 새로운 풍속 값을 지정된 좌표에 출력
  myGLCD.print(windSpeed, LEFT + 90, 204);
}
