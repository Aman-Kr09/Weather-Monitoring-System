#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void setup1() {
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setTextSize(2);
  oled.setCursor(20, 20);
  oled.println("Weather Monitoring System");
  oled.display();
}

void loop1() {}


#define BLYNK_TEMPLATE_ID "TMPL3ozv3-FG_"
#define BLYNK_TEMPLATE_NAME "weather monitoring system"
#define BLYNK_AUTH_TOKEN "Ih5dTqeScrRD3UKHYi95_DNLBesgvb9e"

#include <OakOLED.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#define LDR 4
#define TH 5
#define Rain 36

//Created three variables for pressure
double T, P;
char status;

// Create an object for the BMP180 sensor
SFE_BMP180 bmp;

DHT dht(TH, DHT11);
BlynkTimer timer;

// Enter your Auth token from blynk
char auth[] = "Ih5dTqeScrRD3UKHYi95_DNLBesgvb9e";

//Enter your WIFI SSID and password (local wifi)
char ssid[] = "evilstash09";
char pass[] = "amanaman";

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  bmp.begin();
  dht.begin();
  oled.init();
  oled.backlight();
  pinMode(LDR, INPUT);
  pinMode(Rain, INPUT);
  analogReadResolution(12);

  oled.setCursor(0, 0);
  oled.print("System");
  oled.setCursor(4, 1);
  oled.print("Loading..");
  delay(4000);
  oled.clear();

}

//Get the DHT11 sensor values and updating on blynkIoT, oled
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  oled.setCursor(0, 0);
  oled.print("T:");
  oled.print(t);

  oled.setCursor(8, 0);
  oled.print("H:");
  oled.print(h);

}

//Get the rain sensor values
void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;
  Blynk.virtualWrite(V2, Rvalue);

  oled.setCursor(0, 1);
  oled.print("R:");
  oled.print(Rvalue);
  oled.print(" ");
  Serial.println(Rvalue);
}

//Get the pressure values
void pressure() {
  status =  bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);

    status = bmp.startPressure(3);// 0 to 3
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(P, T);
    }
  }

  Blynk.virtualWrite(V3, P);
  lcd.setCursor(8, 1);
  lcd.print("P:");
  lcd.print(P);
}

//Get the LDR sensor values
void LDRsensor() {
  bool value = digitalRead(LDR);
  if (value == 1) {
    WidgetLED LED(V4);
    LED.on();
  } else {
    WidgetLED LED(V4);
    LED.off();
  }

}

void loop() {//to call these function again and again untill we stop
  DHT11sensor();
  rainSensor();
  pressure();
  LDRsensor();
  Blynk.run();//Run the Blynk library
}
