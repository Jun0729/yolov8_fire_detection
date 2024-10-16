void toggleCircle(int x, int y, char building) {
  // 각 건물의 원 색상 상태를 저장하기 위한 변수들
  static bool isRedGa = false; // 공대 가동의 원 색상 상태 (빨간색인지 여부)
  static bool isRedNa = false; // 공대 나동의 원 색상 상태 (빨간색인지 여부)
  static bool isRedDa = false; // 공대 다동의 원 색상 상태 (빨간색인지 여부)
  
  // 'G'는 공대 가동을 나타냄
  if (building == 'G') {
    // 공대 가동의 원 색상 상태에 따라 색상 설정
    if (isRedGa) {
      myGLCD.setColor(255, 255, 255);  // 현재 빨간색이면 배경색으로 설정 (흰색)
    } else {
      myGLCD.setColor(255, 0, 0); // 현재 흰색이면 빨간색으로 설정
    }
    myGLCD.drawCircle(x, y, 15); // (x, y) 좌표에 반지름 15인 원을 그림
    isRedGa = !isRedGa; // 색상 상태를 반전시킴
  }
  
  // 'N'은 공대 나동을 나타냄
  if (building == 'N') {
    // 공대 나동의 원 색상 상태에 따라 색상 설정
    if (isRedNa) {
      myGLCD.setColor(255, 255, 255);  // 현재 빨간색이면 배경색으로 설정 (흰색)
    } else {
      myGLCD.setColor(255, 0, 0); // 현재 흰색이면 빨간색으로 설정
    }
    myGLCD.drawCircle(x, y, 15); // (x, y) 좌표에 반지름 15인 원을 그림
    isRedNa = !isRedNa; // 색상 상태를 반전시킴
  }
  
  // 'D'는 공대 다동을 나타냄
  if (building == 'D') {
    // 공대 다동의 원 색상 상태에 따라 색상 설정
    if (isRedDa) {
      myGLCD.setColor(255, 255, 255);  // 현재 빨간색이면 배경색으로 설정 (흰색)
    } else {
      myGLCD.setColor(255, 0, 0); // 현재 흰색이면 빨간색으로 설정
    }
    myGLCD.drawCircle(x, y, 15); // (x, y) 좌표에 반지름 15인 원을 그림
    isRedDa = !isRedDa; // 색상 상태를 반전시킴
  }
}
