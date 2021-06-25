#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT22

#define h_up_pin 16
#define h_down_pin 15

#define relay_pin 6

Adafruit_SSD1306 display = Adafruit_SSD1306();
DHT dht(DHTPIN, DHTTYPE);

int button;
int t, h;
int h_set = 45;

unsigned long relay_previousMillis = 0;
const long relay_interval = 20000;

unsigned long h_set_previousMillis = 0;
const long h_set_interval = 1000;

void setup() {
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(1.9999999);
  display.setTextColor(WHITE);

  pinMode(h_up_pin, INPUT);
  pinMode(h_down_pin, INPUT);

  pinMode(relay_pin, OUTPUT);
}

void loop() {
  float newT = dht.readTemperature(true);
  float newH = dht.readHumidity();

  unsigned long currentMillis = millis();

  if (isnan(newT)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    t = newT;
  }
  if (isnan(newH)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    h = newH;
  }

  if (currentMillis - h_set_previousMillis >= h_set_interval) {
    if (digitalRead(h_up_pin) == HIGH) 
      h_set++;
     else if (digitalRead(h_down_pin) == HIGH) 
      h_set--;
    
    if(h_set > 99)
      h_set = 99;
    else if(h_set < 1)
      h_set = 1;
  }

  if(h >= h_set){
    if (currentMillis - relay_previousMillis >= relay_interval) {
      digitalWrite(relay_pin, HIGH);
      relay_previousMillis = currentMillis;
    }
  }
  else {
    if (currentMillis - relay_previousMillis >= relay_interval) {
      digitalWrite(relay_pin, LOW);
      relay_previousMillis = currentMillis;
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.print(String(t));
  display.print("F");
  display.setCursor(0, 6);
  display.print("------------------");
  display.setCursor(0, 12);
  display.print("Humidity: ");
  display.print(String(h));
  display.print("%");
  display.setCursor(0, 21);
  display.print("Set: ");
  display.print(String(h_set));
  display.print("%");
  display.display();

  delay(10);
}
