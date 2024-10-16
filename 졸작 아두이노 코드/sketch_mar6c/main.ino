unsigned int MA() {
  myGLCD.setFont(SmallFont);  // 작은 폰트를 사용하여 텍스트를 출력하도록 설정
  myGLCD.fillScr(0, 0, 0);    // 화면 전체를 검은색으로 채워 배경을 초기화

  // 메인 화면 비트맵 이미지를 화면에 그리기
  // 비트맵 이미지 Micon을 (0, 0) 좌표에서 시작하여 320x204 크기로 화면에 표시
  myGLCD.drawBitmap(0, 0, 320, 204, Micon);

  myGLCD.setColor(255, 255, 255);  // 텍스트와 도형의 색상을 흰색으로 설정

  // 각 공대 건물의 위치에 원을 그리기
  // (207, 142) 좌표에 반지름 15의 원을 그려 공대 다동을 나타냄
  myGLCD.drawCircle(207, 142, 15);
  
  // (166, 105) 좌표에 반지름 15의 원을 그려 공대 나동을 나타냄
  myGLCD.drawCircle(166, 105, 15);
  
  // (108, 63) 좌표에 반지름 15의 원을 그려 공대 가동을 나타냄
  myGLCD.drawCircle(108, 63, 15);

  // 홈키와 GPS 모드 버튼의 비트맵 이미지 표시
  // (280, 205) 좌표에 32x32 크기의 홈키 비트맵 이미지 표시
  myGLCD.drawBitmap(280, 205, 32, 32, home);
  
  // (240, 205) 좌표에 32x32 크기의 GPS 모드 비트맵 이미지 표시
  myGLCD.drawBitmap(240, 205, 32, 32, gps);

  // GPS 모드가 비활성화된 경우에만 풍속을 화면에 표시
  if(gpsstate == false) {
    // "WindSpeed:" 라는 텍스트를 화면 좌측에서 (LEFT, 204) 좌표에 출력
    myGLCD.print("WindSpeed:", LEFT, 204);
    
    // 현재 풍속 값을 화면에 표시
    // 풍속 값을 tftlcd (LEFT + 90, 204) 좌표에 출력
    myGLCD.print(windSpeed, LEFT + 90, 204);
  }

  return 1;  // 함수가 1을 반환 (특별한 의미는 없으며, 함수 종료 시 사용됨)
}
