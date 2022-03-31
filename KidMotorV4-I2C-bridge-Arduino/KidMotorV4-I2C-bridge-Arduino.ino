#include <Arduino.h>
#include <Wire.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <Servo.h>

#define SELF_I2C_ADDR 0x08
#define SCL_PIN 5
#define SDA_PIN 4

#define IN1A_PIN 13
#define IN1B_PIN 14
#define IN2A_PIN 16
#define IN2B_PIN 17

#define D1 10
#define D2 11
#define D3 12
#define D4 26
#define D5 27
#define SV1 18
#define SV2 19
#define SV3 20

int vPin_to_real_pin[8] = { -1, -1, -1, A0, A1, -1, -1, -1 };
int vPin_to_digital_pin[8] = { D1, D2, D3, D4, D5, SV1, SV2, SV3 };

uint8_t reg_data[256];

void on_i2c_receive_handle(int);
void on_i2c_request_handle();

bool pwm_update[8] = { 
  false, false, false, false, false, // D1 - D5, PWM
  false, false, false // SV1 - SV3, Servo
};

Servo *myServo[3] = { NULL, NULL, NULL };
// #define DEBUG_REGISTER

void setup() {
  Serial.begin(9600);

  // Clean reg
  memset(reg_data, 0, sizeof reg_data);

  // Config PWM
  analogWriteFreq(1E3); // 1 kHz
  analogWriteResolution(16); // 16 bit

  // Configs All I/O
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  pinMode(SV3, OUTPUT);

  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin(SELF_I2C_ADDR);
  Wire.onReceive(on_i2c_receive_handle);
  Wire.onRequest(on_i2c_request_handle);
}

void loop() {
#ifdef DEBUG_REGISTER
  for (int i=0;i<sizeof reg_data;i++) {
    Serial.print("0x" + String(reg_data[i], HEX));
    Serial.print("\t");
  }
  Serial.println();
  delay(300);
#endif

  // Motor output
  analogWrite(IN1A_PIN, (reg_data[0x00] & 0x80) == 0 ? 65535 : (65535 - ((reg_data[0x00] & 0x7F) / 100.0 * 65535)));
  analogWrite(IN1B_PIN, (reg_data[0x00] & 0x80) == 0 ? (65535 - ((reg_data[0x00] & 0x7F) / 100.0 * 65535)) : 65535);
  analogWrite(IN2A_PIN, (reg_data[0x01] & 0x80) == 0 ? 65535 : (65535 - ((reg_data[0x01] & 0x7F) / 100.0 * 65535)));
  analogWrite(IN2B_PIN, (reg_data[0x01] & 0x80) == 0 ? (65535 - ((reg_data[0x01] & 0x7F) / 100.0 * 65535)) : 65535);

  // PWM update
  for (uint8_t ch=0;ch<5;ch++) {
    if (pwm_update[ch]) {
      analogWrite(vPin_to_digital_pin[ch], (reg_data[0x10 + (ch * 2)] << 8) | reg_data[0x11 + (ch * 2)]);
      pwm_update[ch] = false;
    }
  }

  // Servo update
  for (uint8_t ch=5;ch<8;ch++) {
    if (pwm_update[ch]) {
      uint8_t angle = reg_data[0x20 + (ch - 5)];
      if ((!myServo[ch]) && angle != 255) {
        myServo[ch] = new Servo();
        myServo[ch]->attach(vPin_to_digital_pin[ch]);
      }
      if (angle < 255) {
        myServo[ch]->write(angle);
      } else {
        delete myServo[ch];
        myServo[ch] = NULL;
      }
      pwm_update[ch] = false;
    }
  }

  // Ultrasonic sensor
  if ((reg_data[0x30] & 0x80) != 0) { // START flag is SET
    int trigPin = vPin_to_digital_pin[(reg_data[0x30] >> 3) & 0x07];
    int echoPin = vPin_to_digital_pin[(reg_data[0x30] >> 0) & 0x07];
    
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    uint16_t duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    uint16_t distance_cm = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");
    reg_data[0x31] = (distance_cm >> 8) & 0xFF;
    reg_data[0x32] = distance_cm & 0xFF;
    if ((reg_data[0x30] & 0x40) == 0) { // CONTI flag RESET
      reg_data[0x30] &= ~0x80; // Set START flag to RESET
    }
  }

  // UART

  // ADC Read
  if ((reg_data[0x05] & 0x80) != 0) { // ADC FLAG is SET ?
    int pin = vPin_to_real_pin[reg_data[0x05] & 0x07];
    uint16_t raw = 0;
    if (pin != -1) {
      raw = analogRead(pin);
      // Serial.println("Analog pin: " + String(pin));
    }
    reg_data[0x06] = (raw >> 8) & 0xFF;
    reg_data[0x07] = raw & 0xFF;
    reg_data[0x05] &= ~0x80; // Reset FLAG
  }

  delay(1); // Stop for USB work
}

uint8_t reg_offset = 0;

void on_i2c_receive_handle(int n) {
  for (int i=0;i<n;i++) {
    if (i == 0) {
      reg_offset = Wire.read();
    } else {
      uint8_t dataIn = Wire.read();
      if (reg_offset == 0x02) { // GPIO_DIR
        pinMode(D1, dataIn & 0x01 ? OUTPUT : INPUT);
        pinMode(D2, dataIn & 0x02 ? OUTPUT : INPUT);
        pinMode(D3, dataIn & 0x04 ? OUTPUT : INPUT);
        pinMode(D4, dataIn & 0x08 ? OUTPUT : INPUT);
        pinMode(D5, dataIn & 0x10 ? OUTPUT : INPUT);
      } else if (reg_offset == 0x04) { // GPIO_OUT
        digitalWrite(D1, dataIn & 0x01 ? HIGH : LOW);
        digitalWrite(D2, dataIn & 0x02 ? HIGH : LOW);
        digitalWrite(D3, dataIn & 0x04 ? HIGH : LOW);
        digitalWrite(D4, dataIn & 0x08 ? HIGH : LOW);
        digitalWrite(D5, dataIn & 0x10 ? HIGH : LOW);
      } else if ((reg_offset >= 0x10) && (reg_offset <= 0x19)) {
        pwm_update[(int)((reg_offset - 0x10) / 2)] = true;
      } else if ((reg_offset >= 0x20) && (reg_offset <= 0x22)) {
        pwm_update[(int)(reg_offset - 0x20) + 5] = true;
      }
      reg_data[reg_offset] = dataIn;
      reg_offset++;
      if (reg_offset >= sizeof(reg_data)) {
        reg_offset = 0;
      }
    }
  }
}

void on_i2c_request_handle() {
  if (reg_offset == 0x03) {
    reg_data[reg_offset] = (digitalRead(D5) << 4) | (digitalRead(D4) << 3) | (digitalRead(D3) << 2) | (digitalRead(D2) << 1) | (digitalRead(D1) << 0);
  }
  Wire.write(reg_data[reg_offset]);
  reg_offset++;
  if (reg_offset >= sizeof(reg_data)) {
    reg_offset = 0;
  }
}
