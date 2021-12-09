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
 - [ ] Write PWM value
 - [x] Read analog value (D4, D5 only)
 
Servo pin (SV1 - SV3)

 - [ ] Set servo angle
 - [ ] Config timing of servo
 - [ ] Set speed and direction of 360 angle servo
 
Special function

 - [ ] UART transmission and receive
 - [ ] Read distant from Ultrasonic sensor 

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
  </tbody>
</table>
