#include <Arduino_FreeRTOS.h>

void TaskBlinkLed(void *pvParameters);
void TaskCounter(void *pvParameters);

void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  xTaskCreate(TaskBlinkLed, "TaskBlinkLed", 128, NULL, 1, NULL);
  xTaskCreate(TaskCounter, "TaskCounter", 128, NULL, 1, NULL);
  
}

//Tarefa 1 - pisca led
void TaskBlinkLed(void *pvParameters){
  
  for(;;){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    digitalWrite(13, HIGH);//acendendo led
    Serial.println("Blink Led");
    xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(750));
    digitalWrite(13, LOW);
    Serial.println("Apagando Led");
    xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1500));
  }
  
}

//Tarefa 2 - contador
void TaskCounter(void *pvParameters){

  int contador = 0;

  for(;;){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    Serial.print("Timer : ");
    Serial.println(contador ++);
    xTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(1000));
  }
  
}

void loop() {

}
