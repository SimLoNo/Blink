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
void GreenLightSwitch(void * pvParameters);
void YellowLightSwitch(void * parameter);
void LightSwitchMethod(int pinParameter, int delayTime);

SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(lightPin, OUTPUT);
  pinMode(yellowLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(triggerPin, INPUT_PULLUP);
  timer = time(NULL);


  xTaskCreate(
      GreenLightSwitch, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      NULL);  /* Task handle. */

      delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreate(
      GreenLightSwitch,   /* Task function. */
      "Task2",     /* name of task. */
      10000,       /* Stack size of task */
      NULL,        /* parameter of the task */
      0,           /* priority of the task */
      NULL);      /* Task handle to keep track of created task */
    delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  int updateSpeed = analogRead(analogPin) / 500;

  /* Serial.print(analogRead(analogPin) * 0.0008058608058);
  Serial.println(" Volt.");
  

  Serial.print("Update speed is: ");
  Serial.print(updateSpeed);
  Serial.println(" seconds.");
  
  Serial.print("Triggerpin is :"); 
  Serial.println(digitalRead(triggerPin));*/
  if (digitalRead(triggerPin) == LOW  && updateSpeed != 0)
  {
    //digitalWrite(lightPin, HIGH);
    time_t currentTimer = time(NULL);
    int timeDifference = currentTimer - timer; 
    /* Serial.print("Current time is: ");
    Serial.print(currentTimer);
    Serial.print(". Old time is: ");
    Serial.print(timer);
    Serial.print(". Difference is: ");
    Serial.print(timeDifference);
    Serial.print(" Seconds. Update speed is: ");
    Serial.print(updateSpeed);
    Serial.println(" seconds."); */
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

void GreenLightSwitch(void * parameter){
  LightSwitchMethod(greenLightPin, 1000);
}

void YellowLightSwitch(void * parameter){
  LightSwitchMethod(yellowLightPin, 1000);
}
void LightSwitchMethod(int pinParameter, int delayTime)
{   /* See if we can obtain the semaphore.  If the semaphore is not
        available wait 10 ticks to see if it becomes free. */
  
      xSemaphoreTake( xSemaphore, delayTime * 3);
      Serial.print("Light switch running on core ");
      Serial.println(xPortGetCoreID());

      for(;;){
        Serial.println("Hello From The Task.");
        digitalWrite(pinParameter, HIGH);
        vTaskDelay(delayTime);
        digitalWrite(pinParameter, LOW);
        vTaskDelay(delayTime);
        xSemaphoreGive( xSemaphore );
      } 

      /* We have finished accessing the shared resource.  Release the
      semaphore. */
}
// void Task2Code(void * pvParameters)
// {
//   if( xSemaphore != NULL )
//    {
//     /* See if we can obtain the semaphore.  If the semaphore is not
//         available wait 10 ticks to see if it becomes free. */
//         if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
//         {
//             /* We were able to obtain the semaphore and can now access the
//             shared resource. */

//             Serial.print("Task2 running on core ");
//             Serial.println(xPortGetCoreID());

//             for(;;){
//               digitalWrite(greenLightPin, HIGH);
//               delay(700);
//               digitalWrite(greenLightPin, LOW);
//               delay(700);
//             } 

//             /* We have finished accessing the shared resource.  Release the
//             semaphore. */
//             xSemaphoreGive( xSemaphore );
//         }
//         else
//         {
//             /* We could not obtain the semaphore and can therefore not access
//             the shared resource safely. */
//         }
//    }
  
// }
