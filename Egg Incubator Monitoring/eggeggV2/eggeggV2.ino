#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define TRIG_PIN1 3
#define ECHO_PIN1 4
#define TRIG_PIN2 5
#define ECHO_PIN2 6
#define TRIG_PIN3 7
#define ECHO_PIN3 8
#define MIN_DISTANCE_CHANGE 5 // Minimum distance change to consider as movement

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

#include <SoftwareSerial.h>
SoftwareSerial SIM900A(14,15); //rx tx

long previousDistance = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

void setup() {
  Serial.begin(9600);

  dht.begin();
  lcd.init(); // initialize the lcd
  lcd.backlight();

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);

  SIM900A.begin(19200);   // GSM Module Baud rate - communication speed

  Serial.println ("On na sya"); 

  delay(100);
}

void loop() {
  disphumtemp();
  
  ultrasonic1();
  ultrasonic2();
  ultrasonic3();
  
  delay(1000);
}

void disphumtemp(){
  //with display
    lcd.clear();                 // clear display
    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print("Humidity: ");
    lcd.print(dht.readHumidity()); 
    lcd.print("%");        // print message at (0, 0)
    lcd.setCursor(1, 1);         // move cursor to   (2, 1)
    lcd.print("Temp: ");
    lcd.print(dht.readTemperature());
    lcd.print(" C");// print message at (2, 1)
}

void ultrasonic1(){
  long duration, distance;
  digitalWrite(TRIG_PIN1, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN1, LOW);
  duration = pulseIn(ECHO_PIN1, HIGH);
  distance = (duration/2) / 29.1;

  if (abs(distance - previousDistance) > MIN_DISTANCE_CHANGE) {
    Serial.println("Movement detected");
    Message();
  }else{
    Serial.println("No Movement :)");
  }
  
  previousDistance = distance;
  delay(1000);
}
void ultrasonic2(){
  long duration, distance;
  digitalWrite(TRIG_PIN2, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN2, LOW);
  duration = pulseIn(ECHO_PIN2, HIGH);
  distance = (duration/2) / 29.1;

  if (abs(distance - previousDistance) > MIN_DISTANCE_CHANGE) {
    Serial.println("Movement detected");
    Message();
  }else{
    Serial.println("No Movement :)");
  }
  
  previousDistance = distance;
  delay(1000);
}
void ultrasonic3(){
  long duration, distance;
  digitalWrite(TRIG_PIN3, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN3, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN3, LOW);
  duration = pulseIn(ECHO_PIN3, HIGH);
  distance = (duration/2) / 29.1;

  if (abs(distance - previousDistance) > MIN_DISTANCE_CHANGE) {
    Serial.println("Movement detected");
    Message();
  }else{
    Serial.println("No Movement :)");
  }
  
  previousDistance = distance;
  delay(1000);
}

void Message(){
  Serial.println ("Sending Message please wait....");
  SIM900A.println("AT+CMGF=1");    //Text Mode initialisation 
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+639566047496\"\r"); // Receiver's Mobile Number
  delay(3000);
  Serial.println ("Set SMS Content");
  SIM900A.println("Movement Detected!");// Messsage content
  delay(3000);
  Serial.println ("Done");
  SIM900A.println((char)26);//   delay(1000);
  Serial.println ("Message sent succesfully");
}