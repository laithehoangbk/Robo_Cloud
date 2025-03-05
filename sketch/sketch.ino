// Thư viện cho điều khiển động cơ (sử dụng L298N driver)
#include <Arduino.h>

// Định nghĩa các chân kết nối với động cơ
const int motorLeftEnablePin = 9;  // Chân Enable của động cơ trái (PWM)
const int motorLeftForwardPin = 8; // Chân IN1 của động cơ trái
const int motorLeftBackwardPin = 7; // Chân IN2 của động cơ trái

const int motorRightEnablePin = 10; // Chân Enable của động cơ phải (PWM)
const int motorRightForwardPin = 11; // Chân IN3 của động cơ phải
const int motorRightBackwardPin = 12; // Chân IN4 của động cơ phải

// Định nghĩa tốc độ động cơ (0-255)
const int baseSpeed = 150; // Tốc độ cơ bản
const int turnSpeedOffset = 50; // Độ lệch tốc độ khi rẽ

// Bán kính vòng tròn (điều chỉnh giá trị này để thay đổi kích thước vòng tròn)
const int circleRadius = 10; // Chỉ là một giá trị tham khảo, cần điều chỉnh dựa trên thực tế

void setup() {
  // Cấu hình các chân là output
  pinMode(motorLeftEnablePin, OUTPUT);
  pinMode(motorLeftForwardPin, OUTPUT);
  pinMode(motorLeftBackwardPin, OUTPUT);

  pinMode(motorRightEnablePin, OUTPUT);
  pinMode(motorRightForwardPin, OUTPUT);
  pinMode(motorRightBackwardPin, OUTPUT);

  // Dừng động cơ khi bắt đầu
  stopMotors();

  // Khởi tạo Serial (optional, để debug)
  Serial.begin(9600);
  Serial.println("Robot is ready!");
}

void loop() {
  // Di chuyển theo vòng tròn liên tục
  moveInCircle();

  // (Optional) Thêm delay để kiểm soát tốc độ vòng tròn
  // delay(10);  // Delay nhỏ có thể giúp điều chỉnh độ mượt của vòng tròn
}

// Hàm điều khiển động cơ trái
void controlMotorLeft(int speed, boolean forward) {
  analogWrite(motorLeftEnablePin, speed);  // Điều khiển tốc độ bằng PWM
  digitalWrite(motorLeftForwardPin, forward ? HIGH : LOW);
  digitalWrite(motorLeftBackwardPin, forward ? LOW : HIGH);
}

// Hàm điều khiển động cơ phải
void controlMotorRight(int speed, boolean forward) {
  analogWrite(motorRightEnablePin, speed); // Điều khiển tốc độ bằng PWM
  digitalWrite(motorRightForwardPin, forward ? HIGH : LOW);
  digitalWrite(motorRightBackwardPin, forward ? LOW : HIGH);
}

// Hàm dừng cả hai động cơ
void stopMotors() {
  controlMotorLeft(0, true);  // Dừng động cơ trái
  controlMotorRight(0, true); // Dừng động cơ phải
}

// Hàm di chuyển robot về phía trước
void moveForward(int speed) {
  controlMotorLeft(speed, true);
  controlMotorRight(speed, true);
}

// Hàm di chuyển robot lùi lại
void moveBackward(int speed) {
  controlMotorLeft(speed, false);
  controlMotorRight(speed, false);
}

// Hàm xoay robot sang trái tại chỗ
void turnLeft(int speed) {
  controlMotorLeft(speed, false);
  controlMotorRight(speed, true);
}

// Hàm xoay robot sang phải tại chỗ
void turnRight(int speed) {
  controlMotorLeft(speed, true);
  controlMotorRight(speed, false);
}

// Hàm di chuyển robot theo vòng tròn
void moveInCircle() {
  // Điều chỉnh tốc độ động cơ để tạo vòng tròn
  // Động cơ bên trong vòng tròn (trong trường hợp này, động cơ trái) chạy chậm hơn
  // và động cơ bên ngoài vòng tròn (động cơ phải) chạy nhanh hơn.

  // Điều chỉnh các giá trị speed dựa trên thử nghiệm thực tế.
  // Giá trị turnSpeedOffset càng lớn, vòng tròn càng nhỏ.

  int leftSpeed = baseSpeed - turnSpeedOffset;
  int rightSpeed = baseSpeed + turnSpeedOffset;

  // Đảm bảo tốc độ không vượt quá giới hạn 255
  if (leftSpeed < 0) leftSpeed = 0;
  if (rightSpeed > 255) rightSpeed = 255;

  controlMotorLeft(leftSpeed, true); // Động cơ trái chậm hơn
  controlMotorRight(rightSpeed, true); // Động cơ phải nhanh hơn

  // (Optional) In ra tốc độ động cơ để debug
  Serial.print("Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print("  Right Speed: ");
  Serial.println(rightSpeed);
}