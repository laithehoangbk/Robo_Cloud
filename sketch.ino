#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Cấu hình Màn hình OLED
#define SCREEN_WIDTH 128   // Độ rộng OLED display, in pixels
#define SCREEN_HEIGHT 64   // Chiều cao OLED display, in pixels

// Khai báo cho màn hình OLED
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_ADDR   0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Cấu hình Wifi
const char* ssid = "YOUR_WIFI_SSID";         // Thay thế bằng SSID WiFi của bạn
const char* password = "YOUR_WIFI_PASSWORD"; // Thay thế bằng mật khẩu WiFi của bạn

// Cấu hình OTA
const char* hostname = "esp32-hello-ota"; // Tên Hostname cho OTA, dễ dàng nhận biết

void setup() {
  Serial.begin(115200);

  // Khởi tạo màn hình OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Không làm gì cả, dừng lại.
  }

  // Xóa màn hình
  display.clearDisplay();

  // Hiển thị thông báo khởi tạo
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Starting...");
  display.display();

  // Kết nối WiFi
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname); // Set hostname để dễ nhận biết trong mạng
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connecting WiFi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WiFi Connected!");
  display.println("IP: " + WiFi.localIP().toString());
  display.display();

  // Khởi tạo ArduinoOTA
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS
      // using SPIFFS.end()
      Serial.println("Start updating " + type);
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("OTA Update...");
      display.println("Starting...");
      display.display();
    })
    .onEnd([]() {
      Serial.println("\nEnd");
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("OTA Update...");
      display.println("Finished!");
      display.display();
      delay(1000);
      ESP.restart(); // Khởi động lại sau khi cập nhật
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("OTA Update...");
      display.print("Progress: ");
      display.print(progress / (total / 100));
      display.println("%");
      display.display();
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("OTA Update...");
      display.println("Error!");
      display.display();
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, World!");
  display.display();
}

void loop() {
  ArduinoOTA.handle();
  delay(10); // Thêm một độ trễ nhỏ
}