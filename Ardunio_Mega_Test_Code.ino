// Motor sürücü pinleri
const int PWMA = 2;     // PWM pini
const int AIN1 = 22;     // Yön kontrol pini 1
const int AIN2 = 23;     // Yön kontrol pini 2
const int STBY = 34;     // Sürücü aktif pini
const int M1_CS = A0;    // ACS712 çıkışı

// Kontrol parametreleri
float Kp = 1000.0;
float Ki = 150.0;
float dt = 0.01; // 10 ms döngü süresi
float integral = 0;
const int MAX_PWM = 200;

// Akım hedefi (Amper)
float targetCurrent = 0.4;

// ACS712 ayarları
float currentOffset = 2.5;  // Kalibrasyon için güncellenecek
const float sensitivity = 0.185;  // 185 mV/A (5A modeli)

// Filtre ayarları
#define FILTER_SIZE 10
float currentBuffer[FILTER_SIZE];
int bufferIndex = 0;

// İlk kurulum
void setup() {
  analogReference(DEFAULT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH); // Sürücüyü aktif et

  Serial.begin(9600);
  delay(1000);

  // ACS712 sıfır akım kalibrasyonu
  float sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += analogRead(M1_CS) * (5.0 / 1023.0);
    delay(5);
  }
  currentOffset = sum / 100.0;
  Serial.print("Offset voltaj: ");
  Serial.println(currentOffset);
}

// Tek akım ölçüm fonksiyonu
float readCurrent() {
  int raw = analogRead(M1_CS);
  float voltage = raw * (5.0 / 1023.0);
  float current = (voltage - currentOffset) / sensitivity;
  return current;
}

// Filtreli akım ölçümü (hareketli ortalama)
float readFilteredCurrent() {
  currentBuffer[bufferIndex] = readCurrent();
  bufferIndex = (bufferIndex + 1) % FILTER_SIZE;

  float sum = 0;
  for (int i = 0; i < FILTER_SIZE; i++) sum += currentBuffer[i];
  return sum / FILTER_SIZE;
}

// Ana döngü
void loop() {
  float current = readFilteredCurrent();
  float error = targetCurrent - current;
  integral += error * dt;

  float controlOutput = Kp * error + Ki * integral;

  // Yön kontrolü
  if (controlOutput >= 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    controlOutput = -controlOutput;
  }

  int pwm = constrain(controlOutput, 0, MAX_PWM);
  analogWrite(PWMA, pwm);

  // Seri monitöre yazdır
  Serial.print("Hedef: "); Serial.print(targetCurrent);
  Serial.print(" A | Ölçüm: "); Serial.print(current);
  Serial.print(" A | PWM: "); Serial.println(pwm);

  delay(10); // 10 ms döngü süresi
}
