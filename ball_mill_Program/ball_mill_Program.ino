#include <LiquidCrystal.h>
#include <Plex.h>

double currentspeed,nextspeed,finalspeed;

LiquidCrystal lcd(2, 4, 5, 6, 7, 8);

#define en 10
#define PulsePin 3

#define upbutton 5
#define selectbutton 6
#define downbutton 7

#define OutPin 19

int unsigned millspeed = 90;
int unsigned milltime = 1;
bool millrun = 1;

void setup() 
{
  lcd.begin(16, 2);
  
  pinMode(PulsePin, INPUT);
  pinMode(en, OUTPUT);
  pinMode(OutPin, OUTPUT);
  digitalWrite(en, HIGH);
  setupPlex();
  digitalWrite(OutPin,LOW);
  selectB(3);
}

void loop() 
{
  millrun = 1;
  digitalWrite(en, HIGH);
  
  lcd.clear();
  lcd.print("Speed");  
  while(digitalReadC(selectbutton) == LOW)
  {  
    lcd.setCursor(0,1);
    lcd.print(millspeed);
    lcd.print(" rpm      ");
    delay(100);
    if(digitalReadC(upbutton) == HIGH)millspeed++;
    if(digitalReadC(downbutton) == HIGH)millspeed--;
    if(millspeed > 180) millspeed = 180;
    if(millspeed < 10) millspeed = 10;
  }

  delay(100);
  while(digitalReadC(selectbutton) == HIGH);
  delay(100);

  lcd.clear();
  lcd.print("Time");  
  while(digitalReadC(selectbutton) == LOW)
  {  
    lcd.setCursor(0,1);
    lcd.print(milltime);
    lcd.print(" mins      ");
    delay(100);
    if(digitalReadC(upbutton) == HIGH)milltime++;
    if(digitalReadC(downbutton) == HIGH)milltime--;
    if(milltime > 60) milltime = 60;
    if(milltime < 1) milltime = 1;
  }

  delay(100);
  while(digitalReadC(selectbutton) == HIGH);
  delay(100);

  lcd.clear();
  lcd.print("Start milling");  
  while(digitalReadC(selectbutton) == LOW)
  {  
    lcd.setCursor(0,1);
    if(millrun)
    {lcd.print("Yes");}
    else
    {lcd.print("No ");}
    
    delay(100);
    if(digitalReadC(upbutton) == HIGH)millrun = 1;
    if(digitalReadC(downbutton) == HIGH)millrun = 0;
  }

  delay(100);
  while(digitalReadC(selectbutton) == HIGH);
  delay(100);

  if(!millrun){return;}

  lcd.clear();
  lcd.print("Runnning");
  
  long unsigned timefromstart = millis() + (milltime*60000);
  
  digitalWrite(OutPin, HIGH);

  delay(50);
  finalspeed = millspeed;

  while(millis() < timefromstart)
  {
    unsigned long Pulselow = pulseIn(PulsePin, LOW);
    unsigned long Pulsehigh = pulseIn(PulsePin, HIGH);
    unsigned long Pulsetotal = (Pulselow+Pulsehigh);

    float PulsetotalSeconds = (long)Pulsetotal*0.000001f;
    //unsigned long f = 1/PulsetotalSeconds;
    currentspeed = 1/(PulsetotalSeconds*15)*12;

    if(currentspeed<finalspeed)
    {
      digitalWrite(OutPin, HIGH);
    }
    else
    {
      digitalWrite(OutPin, LOW);
    }
    if(digitalReadC(selectbutton) == HIGH)break;

    
    long unsigned timeleft =  timefromstart - millis();
    lcd.setCursor(0,0);
    lcd.print(timeleft/60000);
    lcd.print(":");
    lcd.print(timeleft/1000%60);
    lcd.print("      ");

    lcd.setCursor(0,1);
    lcd.print("RPM: ");
    if(currentspeed<100) lcd.print(" ");
    lcd.print((int)currentspeed);
    lcd.print("       ");
  }
  digitalWrite(OutPin, LOW);

  lcd.clear();
  lcd.print("Finished"); 
  
  delay(100);
  while(digitalReadC(selectbutton) == HIGH);
  delay(100);
  while(digitalReadC(selectbutton) == LOW) {}
  delay(100);
  while(digitalReadC(selectbutton) == HIGH);
  delay(100);
}
