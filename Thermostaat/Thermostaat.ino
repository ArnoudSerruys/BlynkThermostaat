
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>

#define dhtpin 4
#define DHTTYPE DHT11

#define PotPin 32
#define LED 33

DHT dht (dhtpin, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

float Temp = 0;
float Wished = 0;

// blynk
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "-9mL-jTNnr_iVO4o7tI6DULh03ZGjCWz";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MSI 6691";
char pass[] = "123456789";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(LED, OUTPUT);
  pinMode(PotPin, INPUT);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
}

void loop() {
  Blynk.run();
  // put your main code here, to run repeatedly:
  Temp = dht.readTemperature();

  BLYNK_WRITE(V1);
  
  Blynk.virtualWrite(V0, Temp);

  lcd.setCursor(0, 0);
  lcd.print("Wished:");
  lcd.setCursor(8, 0);
  lcd.print(Wished);
  lcd.setCursor(14, 0);
  lcd.print((char)223);
  lcd.setCursor(15, 0);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.setCursor(8, 1);
  lcd.print(Temp);
  lcd.setCursor(14, 1);
  lcd.print((char)223);
  lcd.setCursor(15, 1);
  lcd.print("C");

  if (Wished > Temp)
  {
    digitalWrite(LED, HIGH);
    Blynk.virtualWrite(V2, 1023);
  }

  if (Wished <= Temp)
  {
    digitalWrite(LED, LOW);
    Blynk.virtualWrite(V2, 0);
  }
}

BLYNK_WRITE(V1)
{
  Wished = param.asInt();
}
