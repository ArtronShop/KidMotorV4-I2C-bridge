#include <Arduino.h>
#include <Wire.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

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

uint8_t reg_data[30];

void on_i2c_receive_handle(int);
void on_i2c_request_handle();

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
   
  // ADC Read
  if ((reg_data[0x05] & 0x80) != 0) { // ADC FLAG is SET ?
    int vPin_to_real_pin[8] = { -1, -1, -1, A0, A1, -1, -1, -1 };
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
    Wire.write((digitalRead(D5) << 4) | (digitalRead(D4) << 3) | (digitalRead(D3) << 2) | (digitalRead(D2) << 1) | (digitalRead(D1) << 0));
  } else {
    Wire.write(reg_data[reg_offset]);
  }
  reg_offset++;
  if (reg_offset >= sizeof(reg_data)) {
    reg_offset = 0;
  }
}
