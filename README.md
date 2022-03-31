# KidMotorV4-I2C-bridge

Use KidMotor V4 as i/o extender, I2C communication

 > Use Arduino IDE for build uf2 file, needs [RP2 core for Arduino](https://github.com/earlephilhower/arduino-pico)

## Future

Motor

 - [x] Move forward
 - [x] Move backword
 - [x] Turn left
 - [x] Turn right
 - [x] Stop moving
 
D1 to D5 pin

 - [x] Write digital value
 - [x] Read digital value
 - [x] Write PWM value
 - [x] Read analog value (D4, D5 only)
 
Servo pin (SV1 - SV3)

 - [x] Set servo angle
 - [ ] Config timing of servo
 - [ ] Set speed and direction of 360 angle servo
 
Special function

 - [ ] UART transmission and receive
 - [x] Read distant from Ultrasonic sensor 

## Development support

 - [microBlock IDE](https://microblock.app) using with [KidMotor V3 extension](https://github.com/ArtronShop/KidMotorV3-extension)
 - [KidBrightIDE](https://www.kid-bright.org/) using with [KidMotor V3 plugin](https://store.kidbright.info/plugin/46/KidMotor+V3)
 - [KBIDE](https://kbide.org/) using with [kb-KidMotorV3](https://github.com/ArtronShop/kbide-KidMotorV3)
 - [Arduino IDE]() not support yet

## Register Map

<table>
  <thead>
    <tr>
      <th>Address</th>
      <th>Label</th>
      <th>BIT 7</th>
      <th>BIT 6</th>
      <th>BIT 5</th>
      <th>BIT 4</th>
      <th>BIT 3</th>
      <th>BIT 2</th>
      <th>BIT 1</th>
      <th>BIT 0</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td align=center>0x00</td>
      <td>Motor 1 control direction and speed</td>
      <td align=center>DIR1</td>
      <td align=center colspan=7>SPEED1</td>
    </tr>
    <tr>
      <td align=center>0x01</td>
      <td>Motor 2 control direction and speed</td>
      <td align=center>DIR2</td>
      <td align=center colspan=7>SPEED2</td>
    </tr>
    <tr>
      <td align=center>0x02</td>
      <td>D1 to D5 direction config</td>
      <td align=center colspan=3>Reserved.</td>
      <td align=center>DR5</td>
      <td align=center>DR4</td>
      <td align=center>DR3</td>
      <td align=center>DR2</td>
      <td align=center>DR1</td>
    </tr>
    <tr>
      <td align=center>0x03</td>
      <td>D1 to D5 digital input value</td>
      <td align=center colspan=3>Reserved.</td>
      <td align=center>DI5</td>
      <td align=center>DI4</td>
      <td align=center>DI3</td>
      <td align=center>DI2</td>
      <td align=center>DI1</td>
    </tr>
    <tr>
      <td align=center>0x04</td>
      <td>D1 to D5 digital output value</td>
      <td align=center colspan=3>Reserved.</td>
      <td align=center>DO5</td>
      <td align=center>DO4</td>
      <td align=center>DO3</td>
      <td align=center>DO2</td>
      <td align=center>DO1</td>
    </tr>
    <tr>
      <td align=center>0x05</td>
      <td>ADC control</td>
      <td align=center>START</td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=3>A_CH</td>
    </tr>
    <tr>
      <td align=center>0x06</td>
      <td>ADC data high</td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=4>ADC_VALUE_MSB</td>
    </tr>
    <tr>
      <td align=center>0x07</td>
      <td>ADC data low</td>
      <td align=center colspan=8>ADC_VALUE_LSB</td>
    </tr>
    <tr>
      <td align=center>0x08 - 0x0F</td>
      <td>Reserved.</td>
      <td align=center colspan=8>Reserved.</td>
    </tr>
    <tr>
      <td align=center>0x10</td>
      <td>PWM1 value hight<sup>1</sup></td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=4>PWM1_VALUE_MSB</td>
    </tr>
    <tr>
      <td align=center>0x11</td>
      <td>PWM1 value low<sup>1</sup></td>
      <td align=center colspan=8>PWM1_VALUE_LSB</td>
    </tr>
    <tr>
      <td align=center>0x12</td>
      <td>PWM2 value high<sup>1</sup></td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=4>PWM2_VALUE_MSB</td>
    </tr>
    <tr>
      <td align=center>0x13</td>
      <td>PWM2 value low<sup>1</sup></td>
      <td align=center colspan=8>PWM2_VALUE_LSB</td>
    </tr>
   <tr>
      <td align=center>0x14</td>
      <td>PWM3 value high<sup>1</sup></td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=4>PWM3_VALUE_MSB</td>
    </tr>
    <tr>
      <td align=center>0x15</td>
      <td>PWM3 value low<sup>1</sup></td>
      <td align=center colspan=8>PWM3_VALUE_LSB</td>
    </tr>
   <tr>
      <td align=center>0x16</td>
      <td>PWM4 value high<sup>1</sup></td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=4>PWM4_VALUE_MSB</td>
    </tr>
    <tr>
      <td align=center>0x17</td>
      <td>PWM4 value low<sup>1</sup></td>
      <td align=center colspan=8>PWM4_VALUE_LSB</td>
    </tr>
   <tr>
      <td align=center>0x18</td>
      <td>PWM5 value high<sup>1</sup></td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=4>PWM5_VALUE_MSB</td>
    </tr>
    <tr>
      <td align=center>0x19</td>
      <td>PWM5 value low<sup>1</sup></td>
      <td align=center colspan=8>PWM5_VALUE_LSB</td>
    </tr>
    <tr>
      <td align=center>0x1A - 0x1F</td>
      <td>Reserved.</td>
      <td align=center colspan=8>Reserved.</td>
    </tr>
    <tr>
      <td align=center>0x20</td>
      <td>Servo1 angle<sup>2</sup></td>
      <td align=center colspan=8>SERVO1_ANGLE</td>
    </tr>
    <tr>
      <td align=center>0x21</td>
      <td>Servo2 angle<sup>2</sup></td>
      <td align=center colspan=8>SERVO2_ANGLE</td>
    </tr>
    <tr>
      <td align=center>0x22</td>
      <td>Servo3 angle<sup>2</sup></td>
      <td align=center colspan=8>SERVO3_ANGLE</td>
    </tr>
    <tr>
      <td align=center>0x23 - 0x2F</td>
      <td>Reserved.</td>
      <td align=center colspan=8>Reserved.</td>
    </tr>
    <tr>
      <td align=center>0x30</td>
      <td>Ultrasonic sensor pin select</td>
      <td align=center colspan=1>START</td>
      <td align=center colspan=1>CONTI</td>
      <td align=center colspan=3>TRIG_PIN</td>
      <td align=center colspan=3>ECHO_PIN</td>
    </tr>
    <tr>
      <td align=center>0x31</td>
      <td>Ultrasonic Measurement result high</td>
      <td align=center colspan=8>DISTANCE_IN_CM_MSB</td>
    </tr>
    <tr>
      <td align=center>0x32</td>
      <td>Ultrasonic Measurement result low</td>
      <td align=center colspan=8>DISTANCE_IN_CM_LSB</td>
    </tr>
    <tr>
      <td align=center>0x33 - 0x3F</td>
      <td>Reserved.</td>
      <td align=center colspan=8>Reserved.</td>
    </tr>
    <tr>
      <td align=center>0x40</td>
      <td>UART pin select</td>
      <td align=center colspan=1>EN</td>
      <td align=center colspan=1>Reserved.</td>
      <td align=center colspan=3>TX_PIN</td>
      <td align=center colspan=3>RX_PIN</td>
    </tr>
    <tr>
      <td align=center>0x41</td>
      <td>UART baud rate</td>
      <td align=center colspan=4>Reserved.</td>
      <td align=center colspan=4>BAUD_RATE</td>
    </tr>
    <tr>
      <td align=center>0x42</td>
      <td>UART config</td>
      <td align=center colspan=3>Reserved.</td>
      <td align=center colspan=1>STOP_BIT</td>
      <td align=center colspan=2>PARITY_BIT</td>
      <td align=center colspan=3>DATA_BIT_LENGTH</td>
    </tr>
    <tr>
      <td align=center>0x43 - 0x83</td>
      <td>UART rx buffer (64 byte)</td>
      <td align=center colspan=8>DATA_RX1 ... DATA_RX64</td>
    </tr>
    <tr>
      <td align=center>0x84 - 0xC4</td>
      <td>UART tx buffer (64 byte)</td>
      <td align=center colspan=8>DATA_TX1 ... DATA_TX64</td>
    </tr>
  </tbody>
</table>

<sup>1</sup> Write PWM value will auto write digital output value to 0

<sup>2</sup> Write 255 to stop PWM output, Write other will set servo angle
