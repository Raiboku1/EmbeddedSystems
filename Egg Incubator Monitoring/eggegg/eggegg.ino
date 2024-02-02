#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

#include <SoftwareSerial.h>
SoftwareSerial SIM900A(14,15); //rx tx

//dht Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
//lcd
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

//ultraso1
const int trigPin1 = 3;
const int echoPin1 = 4;
long duration1;
int distance1;
int prevdistance1;
//ultraso2
const int trigPin2 = 5;
const int echoPin2 = 6;
long duration2;
int distance2;
int prevdistance2;
//ultraso3
const int trigPin3 = 7;
const int echoPin3 = 8;
long duration3;
int distance3; 
int prevdistance3; 
int detected;
int time;

void setup()
{
  Serial.begin(19200);
	dht.begin();
  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(trigPin1, OUTPUT); // Sets the trigPin1 as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin1 as an Input
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  SIM900A.begin(19200);   // GSM Module Baud rate - communication speed

  Serial.println ("On na sya"); 

  delay(100);
}

void loop()
{
  Serial.print("distance1: ");
  Serial.println(ultraso1());
  Serial.print("prevdistance1: ");
  Serial.println(prevdistance1);

  if(ultraso1()!=prevdistance1||ultraso2()!=prevdistance2||ultraso3()!=prevdistance3){
    detected++;
  }else{
    detected = 0;
  }
  
  if(detected>2){
    dispmvmnt();
    detected=0;
    time = millis();
    if(time>5000){
      SendMessage();
      time = 0;
    }
    delay(2000);
  }else{
    disphumtemp();
  }
  delay(1000);   
}
//methods below
void disphumtemp(){
  //with display
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    lcd.clear();                 // clear display
    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print("Humidity: ");
    lcd.print(hum); 
    lcd.print("%");        // print message at (0, 0)
    lcd.setCursor(1, 1);         // move cursor to   (2, 1)
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" C");// print message at (2, 1)
}

int ultraso1(){
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  
  distance1 = duration1 * 0.034 / 2;
  prevdistance1 = distance1;
  return distance1;
}

int ultraso2(){
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  prevdistance2 = distance2;
  distance2 = duration1 * 0.034 / 2;

  return distance2;
}

int ultraso3(){
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration1 = pulseIn(echoPin3, HIGH);
  prevdistance3 = distance3;
  distance1 = duration1 * 0.034 / 2;

  return distance3;
}

void dispmvmnt(){
    lcd.clear();                 // clear display
    lcd.setCursor(0, 0);         
    lcd.print("Movement");
    lcd.setCursor(2, 1);         
    lcd.print("detected!!");
    Serial.println("mvmnt dtected");
}

void SendMessage(){
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
