/******************************************************
* DeviceVault Demo Hardware
* Arduino sketch for Grove Beginner Kit
* https://wiki.seeedstudio.com/Grove-Beginner-Kit-For-Arduino/
* Note: Works with other Arduino if you comment out the Available Sensors configuration block
* Note: Sketch is compatible with DHT20 humidity sensor (black housing) and will need modification for use with the earlier DHT11 (blue housing)
*
* Serial: Menu options are displayed when a serial port is connected or '?' is entered
* Button toggles the red LED, beeps the buzzer and updates the OLED contents
* Potentiometer controls the Uno LED blink rate (can be overridden from the serial port)
* Covering the light sensor will beep the buzzer
******************************************************/

/******************************************************************************
* CONFIGURATION
******************************************************************************/

// AVAILABLE SENSORS for the Arduino Grove Beginner Kit
// Comment out this block when using other Arduino
// Note: if sensors are not available then the compilation may fail, initialisation hangs, or use is very slow

// Only available on Grove Beginner Kit
#define USE_TEMP_HUMIDITY_DHT20 true
#define USE_PRESSURE_BMP280 true
#define USE_ACCEL_LIS3DHTR true
#define USE_OLED_U8X8 true

// Only available on Uno R3
#define USE_LIGHT_SENSOR_A6 true

// Only available on Uno R4
// #define USE_LED_MATRIX true

// DEMO FEATURES
const int DEFAULT_SPEED = 1000;            // Initial speed, 0 uses analog input to control speed
const bool USE_LIGHT_SENSOR_BEEP = false;  // Beeps when the light sensor is covered; Default false as this can get a bit annoying

// PIN MAPPING
#define BUILTIN_LED_OUTPUT_PIN 13
#define SENSE_LED_OUTPUT_PIN 4
#define SENSE_BUZZER_OUTPUT_PIN 5
#define SENSE_BUTTON_INPUT_PIN 6
#define SENSE_ROTARY_INPUT_PIN A0
#define SENSE_SOUND_INPUT_PIN A2
#if defined(USE_LIGHT_SENSOR_A6)
#define SENSE_LIGHT_INPUT_PIN A6
#endif

/******************************************************************************
* IMPLEMENTATION
******************************************************************************/

#define DEMO_VERSION "v1.01"

// BOARD TYPE
#if defined(ARDUINO_AVR_UNO)
#define BOARD_TYPE "Arduino Uno"
#elif defined(ARDUINO_UNOR4_MINIMA)
#define BOARD_TYPE "Arduino Uno R4 Minima"
#elif defined(ARDUINO_UNOR4_WIFI)
#define BOARD_TYPE "Arduino Uno R4 WIFI"
#endif

// Sensor libraries
#include <Arduino.h>
#include <Wire.h>
#if defined(USE_TEMP_HUMIDITY_DHT20)
#include <DHT.h>
#endif
#if defined(USE_PRESSURE_BMP280)
#include <Seeed_BMP280.h>
#endif
#if defined(USE_ACCEL_LIS3DHTR)
#include <LIS3DHTR.h>
#endif
#if defined(USE_OLED_U8X8)
#include <U8x8lib.h>
#endif
#if defined(USE_LED_MATRIX)
#include "ArduinoGraphics.h"
#include <Arduino_LED_Matrix.h>
#endif

// Sensor utilities
#if defined(USE_TEMP_HUMIDITY_DHT20)
DHT dht(DHT20);
#endif
#if defined(USE_PRESSURE_BMP280)
BMP280 bmp280;
#endif
#if defined(USE_ACCEL_LIS3DHTR)
LIS3DHTR<TwoWire> LIS;
#endif
#if defined(USE_OLED_U8X8)
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE);
#endif
#if defined(USE_LED_MATRIX)
ArduinoLEDMatrix matrix;
#endif

// State
int manual_speed_value = DEFAULT_SPEED;
long last_blink_time = 0;
int last_button_state = 0;
bool last_light_covered = false;
bool periodic_print_state = true;
bool periodic_print_temp = false;
bool periodic_print_pressure = false;
bool periodic_print_acceleration = false;
bool periodic_print_time = false;

void toggle_periodic_print_state(void) {
  periodic_print_state = !periodic_print_state;
  last_blink_time = 0;
}

void toggle_periodic_print_temp(void) {
  periodic_print_temp = !periodic_print_temp;
  last_blink_time = 0;
}

void toggle_periodic_print_pressure(void) {
  periodic_print_pressure = !periodic_print_pressure;
  last_blink_time = 0;
}

void toggle_periodic_print_acceleration(void) {
  periodic_print_acceleration = !periodic_print_acceleration;
  last_blink_time = 0;
}

void toggle_periodic_print_time(void) {
  periodic_print_time = !periodic_print_time;
  last_blink_time = 0;
}

void do_periodic_print(void) {
  bool has_content = false;

  // State
  if (periodic_print_state) {
    has_content = true;

    Serial.print("LED:");
    Serial.print(digitalRead(SENSE_LED_OUTPUT_PIN));

    Serial.print(", Rotary:");
    Serial.print(analogRead(SENSE_ROTARY_INPUT_PIN));

    Serial.print(", Button:");
    Serial.print(digitalRead(SENSE_BUTTON_INPUT_PIN));

    Serial.print(", Sound:");
    Serial.print(analogRead(SENSE_SOUND_INPUT_PIN));

#if defined(SENSE_LIGHT_INPUT_PIN)
    Serial.print(", Light:");
    Serial.print(analogRead(SENSE_LIGHT_INPUT_PIN));
#endif
  }

// Temperature and Humidity
#if defined(USE_TEMP_HUMIDITY_DHT20)
  if (periodic_print_temp) {
    if (has_content)
      Serial.print(", ");
    has_content = true;
    Serial.print("Temp:");
    Serial.print(dht.readTemperature());
    Serial.print(", Humidity:");
    Serial.print(dht.readHumidity());
  }
#endif

// Pressure
#if defined(USE_PRESSURE_BMP280)
  if (periodic_print_pressure) {
    if (has_content)
      Serial.print(", ");
    has_content = true;
    Serial.print("Altitude:");
    Serial.print(bmp280.calcAltitude(bmp280.getPressure()));
  }
#endif

// Acceleration
#if defined(USE_ACCEL_LIS3DHTR)
  if (periodic_print_acceleration) {
    if (has_content)
      Serial.print(", ");
    has_content = true;

    Serial.print("X:");
    Serial.print(LIS.getAccelerationX());
    Serial.print(", Y:");
    Serial.print(LIS.getAccelerationY());
    Serial.print(", Z:");
    Serial.print(LIS.getAccelerationZ());
  }
#endif

  // Time
  if (periodic_print_time) {
    if (has_content)
      Serial.print(", ");
    has_content = true;

    Serial.print("Speed:");
    Serial.print(get_speed());
    Serial.print(", Time:");
    Serial.print(millis());
  }

  if (has_content)
    Serial.println();
}

void print_status_formatted(void) {
  Serial.println("Sensor Values:");

  Serial.print("- Controls:");
  Serial.print(" LED=");
  Serial.print(digitalRead(SENSE_LED_OUTPUT_PIN));
  Serial.print(", Rotary=");
  Serial.print(analogRead(SENSE_ROTARY_INPUT_PIN));
  Serial.print(", Button=");
  Serial.print(digitalRead(SENSE_BUTTON_INPUT_PIN));
  Serial.print(", Sound=");
  Serial.print(analogRead(SENSE_SOUND_INPUT_PIN));
  Serial.print(", Light=");
#if defined(SENSE_LIGHT_INPUT_PIN)
  Serial.print(analogRead(SENSE_LIGHT_INPUT_PIN));
#else
  Serial.print("DISABLED");
#endif
  Serial.println();

  // Temp and Humidity
  Serial.print("- Temp and Humidity: ");
#if defined(USE_TEMP_HUMIDITY_DHT20)
  Serial.print("temp=");
  Serial.print(dht.readTemperature());
  Serial.print("C, humidity=");
  Serial.print(dht.readHumidity());
  Serial.print("%");
#else
  Serial.print("DISABLED");
#endif
  Serial.println();

  // Pressure
  Serial.print("- Pressure: ");
#if defined(USE_PRESSURE_BMP280)
  Serial.print(bmp280.getPressure());
  Serial.print("Pa");
  Serial.print(" (altitude=");
  Serial.print(bmp280.calcAltitude(bmp280.getPressure()));
  Serial.print("m), temp=");
  Serial.print(bmp280.getTemperature());
  Serial.print("C");
#else
  Serial.print("DISABLED");
#endif
  Serial.println();

  // Acceleration
  Serial.print("- Acceleration: ");
#if defined(USE_ACCEL_LIS3DHTR)
  Serial.print("X=");
  Serial.print(LIS.getAccelerationX());
  Serial.print(", Y=");
  Serial.print(LIS.getAccelerationY());
  Serial.print(", Z=");
  Serial.print(LIS.getAccelerationZ());
  Serial.print(", temp=");
  Serial.print(LIS.getTemperature());
  Serial.print("C");
#else
  Serial.print("DISABLED");
#endif
  Serial.println();

  // Time
  Serial.print("- Controls:");
  Serial.print(" Speed=");
  Serial.print(get_speed());
  Serial.print(", Time:");
  Serial.print(millis());
  Serial.println();
}

void sense_oled_write_status(void) {
#if defined(USE_OLED_U8X8)
  u8x8.clearDisplay();

  u8x8.setCursor(0, 0);
  u8x8.print("LED: ");
  u8x8.print(digitalRead(SENSE_LED_OUTPUT_PIN));
  u8x8.setCursor(0, 1);
  u8x8.print("Rotary: ");
  u8x8.print(analogRead(SENSE_ROTARY_INPUT_PIN));

#if defined(USE_TEMP_HUMIDITY_DHT20)
  u8x8.setCursor(0, 2);
  u8x8.print("Temp: ");
  u8x8.print(dht.readTemperature());
  u8x8.print("C");
  u8x8.setCursor(0, 3);
  u8x8.print("Humidity: ");
  u8x8.print(dht.readHumidity());
  u8x8.print("%");
#endif

#if defined(USE_PRESSURE_BMP280)
  u8x8.setCursor(0, 4);
  u8x8.print("Press: ");
  u8x8.print(bmp280.getPressure());
  u8x8.print("Pa");
#endif

#if defined(USE_ACCEL_LIS3DHTR)
  u8x8.setCursor(0, 5);
  u8x8.print("X: ");
  u8x8.print(LIS.getAccelerationX());
  u8x8.setCursor(0, 6);
  u8x8.print("Y: ");
  u8x8.print(LIS.getAccelerationY());
  u8x8.setCursor(0, 7);
  u8x8.print("Z: ");
  u8x8.print(LIS.getAccelerationZ());
#endif

  u8x8.refreshDisplay();
#endif
}

void update_led_matrix(void) {
#if defined(USE_LED_MATRIX)
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);

  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(digitalRead(SENSE_LED_OUTPUT_PIN));
  matrix.endText();

  matrix.endDraw();
#endif
}

void sense_led_toggle(void) {
  digitalWrite(SENSE_LED_OUTPUT_PIN, !digitalRead(SENSE_LED_OUTPUT_PIN));
  update_led_matrix();
  last_blink_time = 0;
}

void sense_buzzer_beep(void) {
  analogWrite(SENSE_BUZZER_OUTPUT_PIN, 128);
  delay(100);
  analogWrite(SENSE_BUZZER_OUTPUT_PIN, 0);
  last_blink_time = 0;
}

void set_speed(void) {
  // Get rid of any characters after the menu command, e.g. carriage return
  while (Serial.available())
    Serial.read();
  Serial.print("Enter Speed value (0 to disable): ");
  Serial.setTimeout(10000);  // Timeout after which the manual speed will be set to zero (disabled)
  manual_speed_value = Serial.parseInt();
  last_blink_time = 0;

  Serial.println();
  Serial.print("Speed = ");
  Serial.print(manual_speed_value);
  Serial.println();
}

int get_speed(void) {
  if (manual_speed_value > 0)
    return manual_speed_value;
  else
    return analogRead(SENSE_ROTARY_INPUT_PIN);
}

void reset_arduino() {
  asm volatile("jmp 0");
}

void menu_show(void) {
  // Header
  Serial.print("DeviceVault Demo Hardware ");
  Serial.println(DEMO_VERSION);

  // Board
#if defined(BOARD_TYPE)
  Serial.print(BOARD_TYPE);
#else
  Serial.print("Arduino (unknown type)");
#endif
  Serial.print(" - ");
  Serial.print(NUM_DIGITAL_PINS);
  Serial.print(" digital pins, ");
  Serial.print(NUM_ANALOG_INPUTS);
  Serial.println(" analog inputs");

  // Menu
  Serial.println("1: Toggle LED");
  Serial.println("2: Beep");
#if defined(USE_OLED_U8X8)
  Serial.println("3: OLED Status");
#endif
  Serial.println("4: Set Speed");
  Serial.println("5: Toggle periodic print state (LED, Speed, Sound, Light)");
#if defined(USE_TEMP_HUMIDITY_DHT20)
  Serial.println("6: Toggle periodic print Temperature and Humidity DHT20");
#endif
#if defined(USE_PRESSURE_BMP280)
  Serial.println("7: Toggle periodic print Pressure BMP280");
#endif
#if defined(USE_ACCEL_LIS3DHTR)
  Serial.println("8: Toggle periodic print Acceleration LIS3DHTR");
#endif
  Serial.println("9: Toggle periodic print Time");
  Serial.println("0: Print Status (Formatted)");
  Serial.println("x: Reset the Arduino");
  Serial.println("?: Show menu");
}

void menu_check(void) {
  if (!Serial.available())
    return;
  char c = Serial.read();
  if (c == '1')
    sense_led_toggle();
  else if (c == '2')
    sense_buzzer_beep();
  else if (c == '3')
    sense_oled_write_status();
  else if (c == '4')
    set_speed();
  else if (c == '5')
    toggle_periodic_print_state();
  else if (c == '6')
    toggle_periodic_print_temp();
  else if (c == '7')
    toggle_periodic_print_pressure();
  else if (c == '8')
    toggle_periodic_print_acceleration();
  else if (c == '9')
    toggle_periodic_print_time();
  else if (c == '0')
    print_status_formatted();
  else if (c == 'x')
    reset_arduino();
  else if (c == '?')
    menu_show();
  else if (c != '\n') {
    Serial.print(c);
    Serial.println(": Unrecognized menu option");
  }
}

void setup() {
  pinMode(BUILTIN_LED_OUTPUT_PIN, OUTPUT);
  pinMode(SENSE_LED_OUTPUT_PIN, OUTPUT);
  pinMode(SENSE_BUZZER_OUTPUT_PIN, OUTPUT);
  pinMode(SENSE_BUTTON_INPUT_PIN, INPUT);
  pinMode(SENSE_ROTARY_INPUT_PIN, INPUT);
  pinMode(SENSE_SOUND_INPUT_PIN, INPUT);
#if defined(SENSE_LIGHT_INPUT_PIN)
  pinMode(SENSE_LIGHT_INPUT_PIN, INPUT);
#endif

  Serial.begin(9600);

  Wire.begin();
#if defined(USE_TEMP_HUMIDITY_DHT20)
  dht.begin();
#endif
#if defined(USE_PRESSURE_BMP280)
  if (!bmp280.init())
    Serial.println("Baro not connected or broken!");
#endif
#if defined(USE_ACCEL_LIS3DHTR)
  LIS.begin(Wire, 0x19);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  LIS.openTemp();
  if (!LIS)
    Serial.println("LIS3DHTR didn't connect.");
#endif
#if defined(USE_OLED_U8X8)
  u8x8.begin();
  u8x8.setFlipMode(1);
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFont(u8x8_font_artosserif8_r);

  // Initialisation message
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.print("DeviceVault");
  u8x8.setCursor(0, 2);
  u8x8.print("Demo Hardware");
  u8x8.setCursor(0, 4);
  u8x8.print(DEMO_VERSION);
#endif
#if defined(USE_LED_MATRIX)
  matrix.begin();

  // Scroll
  const String text = String("  DeviceVault Demo HW " + String(DEMO_VERSION));
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

  // Initial state
  update_led_matrix();
#endif

  menu_show();
}

void loop() {
  menu_check();

  // Periodic actions
  const long now = millis();
  const long blink_duration = 50 + (long)(4 * get_speed());
  if (now > last_blink_time + blink_duration) {
    last_blink_time = now;
    // Toggle LED
    digitalWrite(BUILTIN_LED_OUTPUT_PIN, !digitalRead(BUILTIN_LED_OUTPUT_PIN));
    // Periodic print
    do_periodic_print();
  }

  // Sense button press
  int button_state = digitalRead(SENSE_BUTTON_INPUT_PIN);
  if (button_state != last_button_state) {
    last_button_state = button_state;
    if (button_state) {
      sense_led_toggle();
      sense_buzzer_beep();
      sense_oled_write_status();
    }
  }

// Sense cover light
#if defined(SENSE_LIGHT_INPUT_PIN)
  if (USE_LIGHT_SENSOR_BEEP) {
    bool light_covered = analogRead(SENSE_LIGHT_INPUT_PIN) < 1;
    if (light_covered != last_light_covered) {
      last_light_covered = light_covered;
      if (light_covered) {
        sense_buzzer_beep();
      }
    }
  }
#endif

  delay(10);
}
