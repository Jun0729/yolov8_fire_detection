#include <UTFT.h> // UTFT 라이브러리를 포함, TFT LCD 제어에 사용
#include <URTouch.h> // URTouch 라이브러리를 포함, 터치스크린 제어에 사용
#include <TinyGPS.h> // TinyGPS 라이브러리를 포함, GPS 데이터 수신에 사용

TinyGPS gps1; // gps1 객체 생성, GPS 데이터를 처리할 객체
float latitude, longitude; // 위도와 경도를 저장할 변수 선언
unsigned long lastUpdate = 0; // 마지막으로 GPS 데이터가 업데이트된 시간을 저장할 변수

extern uint8_t SmallFont[]; // UTFT 라이브러리에서 제공하는 작은 폰트를 외부에서 참조
extern unsigned short Micon[0xff00]; // 비트맵 이미지 Micon 선언 (크기: 0xff00)
extern unsigned short home[0x400]; // 비트맵 이미지 home 선언 (크기: 0x400)
extern unsigned short gps[0x400]; // 비트맵 이미지 gps 선언 (크기: 0x400)

// GPS 좌표와 TFT 화면상의 좌표를 매핑하기 위한 실제 지리 좌표 설정
float lat_min = 37.8728; // TFT 화면의 좌하단에 대응하는 위도
float lat_max = 37.8753; // TFT 화면의 좌상단에 대응하는 위도
float lon_min = 127.1536; // TFT 화면의 좌하단에 대응하는 경도
float lon_max = 127.1578; // TFT 화면의 우하단에 대응하는 경도
bool gpsstate = false; // GPS모드가 활성화되었는지 여부를 나타내는 플래그

UTFT myGLCD(ITDB32WC, 38, 39, 40, 41); // UTFT 객체 초기화, LCD 디스플레이 핀 설정
URTouch myTouch(6, 5, 4, 3, 2); // URTouch 객체 초기화, 터치스크린 핀 설정
String windSpeed = "0.00"; // 초기 풍속 값을 "0.00"으로 설정

// 각 동 건물(공대 가동, 나동, 다동)의 깜빡임 상태 플래그
bool blinkGa = false; 
bool blinkNa = false; 
bool blinkDa = false; 

unsigned long previousMillisGa = 0; // 공대 가동의 마지막 깜빡임 시간을 기록
unsigned long previousMillisNa = 0; // 공대 나동의 마지막 깜빡임 시간을 기록
unsigned long previousMillisDa = 0; // 공대 다동의 마지막 깜빡임 시간을 기록
const long interval = 500; // 깜빡임 간격을 500ms로 설정

bool imageDisplayed = false; // 이미지가 표시되었는지 여부를 추적하는 변수(라즈베리파이에서 화재감지시 화재이미지 전송)

// setup 함수는 프로그램 시작 시 한 번만 실행됨
void setup() {
  Serial.begin(9600); // 시리얼 통신을 9600bps로 설정 (시리얼 모니터)
  Serial3.begin(9600); // GPS 모듈과의 시리얼 통신 설정 (9600bps)
  Serial2.begin(115200); // HC-12 무선 통신 모듈과의 시리얼 통신 설정 (115200bps)

  myGLCD.InitLCD(); // LCD 디스플레이 초기화
  myGLCD.clrScr(); // LCD 화면 지우기
  myTouch.InitTouch(); // 터치스크린 초기화
  myTouch.setPrecision(PREC_MEDIUM); // 터치스크린의 정밀도를 중간 수준으로 설정
  MA(); // 메인 화면을 출력하는 사용자 정의 함수 호출
}

// loop 함수는 프로그램이 종료될 때까지 반복 실행됨
void loop() {
  checkSerial(); // 시리얼 통신 데이터 확인 함수 호출
  updateBlink(); // 각 동의 깜빡임 상태를 업데이트하는 함수 호출
  checkTouch(); // 터치 입력을 확인하는 함수 호출
  if (gpsstate == true) { // GPS가 활성화되었을 때만
    checkGPS(); // GPS 데이터를 확인하는 함수 호출
  }
}

// 경도(lon) 값을 TFT X 좌표로 변환하는 함수
int mapLongitudeToX(float lon) {
  return map(lon * 1000000, lon_min * 1000000, lon_max * 1000000, 0, 320); // 실제 경도를 TFT의 X 좌표로 매핑
}

// 위도(lat) 값을 TFT Y 좌표로 변환하는 함수
int mapLatitudeToY(float lat) {
  return map(lat * 1000000, lat_min * 1000000, lat_max * 1000000, 240, 36); // 실제 위도를 TFT의 Y 좌표로 매핑
}

// (x, y) 좌표에 원을 그리는 함수
void displayCircle(int x, int y) {
  myGLCD.setColor(255, 127, 0); // 원의 색상을 오렌지색으로 설정
  myGLCD.fillCircle(x, y, 6); // 중심 좌표 (x, y)에서 반지름 6인 원을 그림
}

// GPS 데이터를 확인하고 화면에 좌표를 표시하는 함수
void checkGPS() {
  while (Serial3.available()) { // GPS 모듈에서 데이터가 들어올 때
    int c = Serial3.read(); // GPS 데이터 하나씩 읽음
    Serial.print((char)c); // 원본 GPS 데이터를 시리얼 모니터에 출력

    if (gps1.encode(c)) { // GPS 데이터가 유효하면
      gps1.f_get_position(&latitude, &longitude); // 위도와 경도를 받아옴
      if (latitude == TinyGPS::GPS_INVALID_F_ANGLE || longitude == TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.println("수신 불가 - Invalid angle"); // 유효하지 않은 GPS 각도일 경우 출력
      } else {
        Serial.print("Latitude: "); 
        Serial.println(latitude, 6); // 소수점 6자리까지 위도 출력
        Serial.print("Longitude: "); 
        Serial.println(longitude, 6); // 소수점 6자리까지 경도 출력
        lastUpdate = millis(); // 마지막 유효한 데이터를 받은 시간 기록

        // GPS 좌표를 TFT 좌표로 변환하여 원을 화면에 표시
        int x = mapLongitudeToX(longitude); // 경도를 TFT의 X 좌표로 변환
        int y = mapLatitudeToY(latitude); // 위도를 TFT의 Y 좌표로 변환
        displayCircle(x, y); // 변환된 좌표에 원을 그림
      }
    }
  }

  // 5초 동안 유효한 데이터를 받지 못한 경우, "수신 불가" 메시지 출력
  if (millis() - lastUpdate > 5000) {
    Serial.println("수신 불가 - No valid data for 5 seconds"); // 메시지 출력
    lastUpdate = millis(); // 중복 출력을 방지하기 위해 마지막 시간을 갱신
  }
}
