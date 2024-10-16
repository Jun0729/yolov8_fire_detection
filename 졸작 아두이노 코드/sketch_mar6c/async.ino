void updateBlink() {
  unsigned long currentMillis = millis(); // 현재 시간을 밀리초 단위로 가져옴

  // 공대 가동이 깜빡임 상태일 때
  if (blinkGa && currentMillis - previousMillisGa >= interval) {
    previousMillisGa = currentMillis; // 현재 시간을 마지막 깜빡임 시간으로 갱신
    toggleCircle(108, 63, 'G'); // 공대 가동의 원을 토글 (그리거나 지움)
  }

  // 공대 나동이 깜빡임 상태일 때
  if (blinkNa && currentMillis - previousMillisNa >= interval) {
    previousMillisNa = currentMillis; // 현재 시간을 마지막 깜빡임 시간으로 갱신
    toggleCircle(166, 105, 'N'); // 공대 나동의 원을 토글 (그리거나 지움)
  }

  // 공대 다동이 깜빡임 상태일 때
  if (blinkDa && currentMillis - previousMillisDa >= interval) {
    previousMillisDa = currentMillis; // 현재 시간을 마지막 깜빡임 시간으로 갱신
    toggleCircle(207, 142, 'D'); // 공대 다동의 원을 토글 (그리거나 지움)
  }
}
