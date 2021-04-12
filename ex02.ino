#include <Arduino_FreeRTOS.h>
#include <semphr.h>

void taskBaixaPrioridade(void *pvParameters);
void taskMediaPrioridade(void *pvParameters);
void taskAltaPrioridade(void *pvParameters);

SemaphoreHandle_t xSemaphore = NULL;
void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  xSemaphore = xSemaphoreCreateBinary();
  xTaskCreate(taskBaixaPrioridade, "taskBaixaPrioridade", 128, NULL, 1, NULL);
  xTaskCreate(taskMediaPrioridade, "taskMediaPrioridade", 128, NULL, 3, NULL);
  xTaskCreate(taskAltaPrioridade, "taskAltaPrioridade", 128, NULL, 5, NULL);
  
}

void taskBaixaPrioridade(void *pvParameters){
  
  unsigned long timeStop;
  
  for(;;){
    Serial.println("TASK 1 BAIXA PRIORIDADE - PEGANDO SEMAFORO");
    if(xSemaphoreTake(xSemaphore, portMAX_DELAY)){
      timeStop = millis() + 500;
      while (millis() < timeStop);
      digitalWrite(13, LOW);
      Serial.println("TASK 1 BAIXA PRIORIDADE - APAGANDO LED");
    } else {
      Serial.println("Semaforo Ocupado");
    }
  }
  
}

void taskMediaPrioridade(void *pvParameters){
  
  unsigned long timeStop;
  unsigned long lastPrint=0;
  
  for(;;){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    timeStop = millis() + 200;
    while (millis() < timeStop);
    Serial.println("TASK 2 MÉDIA PRIORIDADE");
    Serial.print("Ultima impressão em ");
    Serial.println(millis() - lastPrint);
    lastPrint = millis();
    xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
  } 
  
}

void taskAltaPrioridade(void *pvParameters){
  
  for(;;){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    digitalWrite(13, HIGH);
    Serial.println("TASK 3 ALTA PRIORIDADE - ACENDENDO LED");
    xSemaphoreGive(xSemaphore);
    xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
  }
  
}

void loop() {

}
