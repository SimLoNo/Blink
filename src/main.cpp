#include <Arduino.h>

int lightPin = 2;
int triggerPin = 5;
int analogPin = 4;
int yellowLightPin = 22;
int greenLightPin = 23;
bool lightSwitch = true;
time_t timer;

TaskHandle_t task1;
TaskHandle_t task2;

void Task1Code(void * pvParameters);
void Task2Code(void * pvParameters);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(lightPin, OUTPUT);
  pinMode(yellowLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(triggerPin, INPUT_PULLUP);
  timer = time(NULL);

  xTaskCreatePinnedToCore(
      Task1Code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &task1,  /* Task handle. */
      0); /* Core where the task should run */

      delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2Code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500);
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
void Task1Code(void * pvParameters){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(greenLightPin, HIGH);
    delay(1000);
    digitalWrite(greenLightPin, LOW);
    delay(1000);
  } 
}
void Task2Code(void * pvParameters){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(yellowLightPin, HIGH);
    delay(700);
    digitalWrite(yellowLightPin, LOW);
    delay(700);
  }
}
