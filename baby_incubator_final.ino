#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>
#include <TM1638_Arduino.h>

TM1638 afficheur(PB3, PB4, PB5); // strobe, clock, data pins

int humWasOn = 0;
int nombre[8];
int tmod=0;
int hmod=0;


#define DHTPIN PC0     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define COOLING PC5 
#define HEATING PC6
#define HUMPIN PC9 //Humidifier pin


//i2c connected to 5v, gnd, pb8 and pb9
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    pinMode(HUMPIN, OUTPUT);
    pinMode(HEATING, OUTPUT); 
    pinMode(COOLING, OUTPUT);
    pinMode(DHTPIN, INPUT_PULLUP);
    dht.begin();
    lcd.init();                      // initialize the lcd 
    afficheur.init(2);
    Serial.begin(115200);
    lcd.backlight();
    lcd.init();
    lcd.setCursor(3,0);
    lcd.print("HOOIEVAREN IND");
    
    lcd.setCursor(3,1);
    lcd.print("BABY INCUBATOR");

    lcd.setCursor(3,2);
    lcd.print("VER 0.6.9 BETA");
    lcd.setCursor(3,3);
    lcd.print("BOOTING...");
    
          

    delay(4000);


    digitalWrite(HEATING, LOW);
        digitalWrite(COOLING, HIGH);
    //splash screen that is being shown at the start of the incubator.
  for (int i = 0; i < 8; i++) {
    nombre[i] = 0;
  }

  for (int i = 0; i < 8; i++) {
    afficheur.setDigit(i, nombre[i]);
  }
  lcd.init();
  humWasOn = 0;
}
void loop() {
sensorPrint(); //print sensor values


}


void humOff(){
    if(humWasOn == 1){
      delay(50);
      digitalWrite(HUMPIN, LOW);
      delay(50);
      digitalWrite(HUMPIN, HIGH);
      delay(50);
      digitalWrite(HUMPIN, LOW);
      delay(50);
      digitalWrite(HUMPIN, HIGH);
      delay(50);
      humWasOn = 0;
      return;
  }
    else{
      return;
  }
}

void humOn(){
    if(humWasOn == 0){
      delay(50);
      digitalWrite(HUMPIN, LOW);
      delay(50);
      digitalWrite(HUMPIN, HIGH);
      delay(50);
      humWasOn = 1;
       return;
    }
    else{
      return;
    }
}
void sensorPrint(){
  // Wait a few seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
  float h = dht.readHumidity();
  int h1 =dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  int t1=dht.readTemperature();
  afficheur.setDigit(0, tmod/10%10);
  afficheur.setDigit(1, tmod%10);
  afficheur.setDigit(2, 23);
  afficheur.setDigit(3, 12);
  afficheur.setDigit(4, hmod/10%10);
  afficheur.setDigit(5, hmod%10);
  afficheur.setDigit(6, 23);
  afficheur.setDigit(7, 22);
  //Sets the values that will be shown on the TM1638 board
  //The board only displays the desired temperature values, not the real time ones.
  if(t1>=10)
  {
    afficheur.setLED(0, ON);
    if(t1>=20)
    afficheur.setLED(1, ON);
      else
      afficheur.setLED(1, OFF);
    if(t1>=30)
       afficheur.setLED(2, ON);
      else
      afficheur.setLED(2, OFF);
    if(t1>=40)
    afficheur.setLED(3, ON);
      else
      afficheur.setLED(3, OFF);
  }
  //Temperature led on and off module, it lights up leds if the temperature is above 10,20,30,40 degrees celsius 
  else
  afficheur.setLED(0, OFF);

  
    if(h1>25)
  {
    afficheur.setLED(4, ON);
    if(h1>=50)
    afficheur.setLED(5, ON);
      else
      afficheur.setLED(5, OFF);
    if(h1>=75)
       afficheur.setLED(6, ON);
      else
      afficheur.setLED(6, OFF);
    if(h1>=99)
    afficheur.setLED(7, ON);
      else
      afficheur.setLED(7, OFF);
  }
  //Humidity led on and off module, it lights up leds if the humidity is above 25,50,75,99 % 
  else
  afficheur.setLED(7, OFF);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    lcd.backlight();
    lcd.setCursor(0,3);
      lcd.print("Error reading sensor");

    return;
  }
  // Compute heat index in Celsius 
  float hic = dht.computeHeatIndex(t, h, false);
  afficheur.readButtons();
  if(afficheur.hasBeenPressed(0)) // checks if button 0 has been pressed
  {
     int x=1;
     lcd.init();
     lcd.backlight();
     lcd.setCursor(4,0);
     lcd.print("NOW CHANGING");
     lcd.setCursor(0,1);
     lcd.print("Temp & Humidity");
     lcd.setCursor(0,2);
     lcd.print("B5&B6 for temp");
     lcd.setCursor(0,3);
     lcd.print("B7&B8 for humidity  ");
     delay(5000);
     lcd.init();
     // main screen for the menu that changes the values of desired temperature and humidity levels
   while(x!=0)
   {
    
    afficheur.readButtons();
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.print(tmod);
    lcd.print(" C");
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(hmod);
    lcd.print(" %");
    if(afficheur.hasBeenPressed(4)) // checks if button 4 has been pressed
    tmod=tmod+10;
    if(afficheur.hasBeenPressed(5))
    tmod=tmod+1;
    if(afficheur.hasBeenPressed(6))
    hmod=hmod+10;
    if(afficheur.hasBeenPressed(7))
    hmod=hmod+1;
    if(tmod>99 || hmod>99)
    {
      tmod=0;
      hmod=0;
    }
    //changes the values of the standard/set values of the desired temperature and humidity levels. it increments them either with 1 or 10
    lcd.setCursor(0,2);
    lcd.print("Press B2 to confirm ");
    lcd.setCursor(0,3);
    lcd.print("Press B3 to cancel  ");
    if(afficheur.hasBeenPressed(1))
      x=0;
     if(afficheur.hasBeenPressed(2))
     {
      x=0;
      tmod=0;
      hmod=0;
     }
     //the values can either be confirmed, and set in the incubator, or restored back to their original values (0).
    afficheur.setDigit(0, tmod/10%10);
    afficheur.setDigit(1, tmod%10);
    afficheur.setDigit(4, hmod/10%10);
    afficheur.setDigit(5, hmod%10);
  } 
  }
  lcd.backlight();
  lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.print(t);
    lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print(" %");

 lcd.setCursor(0,2);
    lcd.print("Heat index: ");
    lcd.print(hic);
    lcd.print(" C");
   if((hmod && tmod )!= 0)
    {
      
      if (t1<tmod)
      {
        digitalWrite(HEATING, HIGH);
        digitalWrite(COOLING, HIGH);
         lcd.setCursor(0,3);
         lcd.print(" WARMING            ");
         
      }
      else
      {
        digitalWrite(HEATING, LOW);
        digitalWrite(COOLING, HIGH);
        lcd.setCursor(0,3);
        lcd.print("COOLING             ");
      }
      if(t1==tmod)
      {
        digitalWrite(HEATING, LOW);
        digitalWrite(COOLING, LOW);
        lcd.setCursor(0,3);
        lcd.print("ALL's GOOD!         ");
      }
      if(h1<hmod)
      {
        humOn();
      }
      if(h1>hmod)
        {
        humOff();
        }
    }     
    else
    {
      lcd.setCursor(0,3);
      lcd.print("Baby might be alive ");
    }

   //the main screen that shows the temperature, humidity and heat index values of the incubator
   //it also showcases a brief information about the status of the infant based on the current values of heat and humidity.
}
