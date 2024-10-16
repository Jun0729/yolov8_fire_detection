//     if ((y >= 50) && (y <= 75) && (x >= 190) && (x <= 215)) {
//       f1_active = !f1_active;
//       updateDisplay();
//     } else if ((y >= 90) && (y <= 120) && (x >= 140) && (x <= 165)) {
//       f2_active = !f2_active;
//       updateDisplay();
//     } else if ((y >= 120) && (y <= 150) && (x >= 95) && (x <= 130)) {
//       f3_active = !f3_active;
//       updateDisplay();
//     } else if ((y >= 195) && (y <= 220) && (x >= 280) && (x <= 305)) {




//       void receiveAndDisplayImage() {
//   int x_center = (SCREEN_WIDTH - BMP_WIDTH) / 2;
//   int y_center = (SCREEN_HEIGHT - BMP_HEIGHT) / 2;

//   Serial.println("Waiting for image...");

//   // 데이터 수신
//   int index = 0;
//   while (index < BUFFER_SIZE) {
//     if (Serial2.available() > 0) {
//       imageData[index++] = Serial2.read();
//     }
//   }

//   Serial.println("Image received. Displaying...");

//   // BMP 데이터 배열에 저장 후 한꺼번에 그리기
//   for (int y = 0; y < BMP_HEIGHT; y++) {
//     int dataIndex = y * ROW_SIZE; // 패딩을 포함한 행 시작 인덱스 계산
//     for (int x = 0; x < BMP_WIDTH; x++) {
//       if (dataIndex < BUFFER_SIZE - 2) {
//         char b = imageData[dataIndex++];
//         char g = imageData[dataIndex++];
//         char r = imageData[dataIndex++];
//         int color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
//         myGLCD.setColor(color);
//         myGLCD.drawPixel(x_center + x, y_center + y); // 좌우 반전 없이 원래대로 그리기
//       }
//     }
//   }

//   Serial.println("Image displayed.");
// }
