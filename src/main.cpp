#include <Arduino.h>

int lightPin = 2;
int triggerPin = 5;
int analogPin = 4;
bool lightSwitch = true;
time_t timer;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(lightPin, OUTPUT);
  pinMode(triggerPin, INPUT_PULLUP);
  timer = time(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  int updateSpeed = analogRead(analogPin) / 500;

  Serial.print(analogRead(analogPin) * 0.0008058608058);
  Serial.println(" Volt.");
  

  Serial.print("Update speed is: ");
  Serial.print(updateSpeed);
  Serial.println(" seconds.");
  
  Serial.print("Triggerpin is :");
  Serial.println(digitalRead(triggerPin));
  if (digitalRead(triggerPin) == LOW  && updateSpeed != 0)
  {
    //digitalWrite(lightPin, HIGH);
    time_t currentTimer = time(NULL);
    int timeDifference = currentTimer - timer; 
    Serial.print("Current time is: ");
    Serial.print(currentTimer);
    Serial.print(". Old time is: ");
    Serial.print(timer);
    Serial.print(". Difference is: ");
    Serial.print(timeDifference);
    Serial.print(" Seconds. Update speed is: ");
    Serial.print(updateSpeed);
    Serial.println(" seconds.");
    if( timeDifference >= updateSpeed)
    {
      timer = currentTimer;
      if (lightSwitch == true)
      {
        Serial.println("Lightswitch turning on.");
        lightSwitch = !lightSwitch;
        digitalWrite(lightPin, HIGH);
      }
      else
      {
        Serial.println("Lightswitch turning off.");
        lightSwitch = !lightSwitch;
        digitalWrite(lightPin, LOW);
      }
      
    }
  }
  else
  {
    digitalWrite(lightPin, LOW);
    lightSwitch = true;
  }

}
