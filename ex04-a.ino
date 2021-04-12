#include <Arduino_FreeRTOS.h>
#include <semphr.h>

const byte ledPin = 13;
const byte interruptPin = 2;

const int high = 1;
const int low = 0;

SemaphoreHandle_t xSemaphoreMutex = NULL;
SemaphoreHandle_t xSemaphoreBinary = NULL;

void myTask1(void *pvParameters);
void myTask2(void *pvParametres);
void GateKeeperTask(void *pvParameters);

QueueHandle_t xQueue;

void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupcao, RISING);

  xSemaphoreMutex = xSemaphoreCreateMutex();
  vSemaphoreCreateBinary(xSemaphoreBinary);
  
  xTaskCreate(myTask1, "myTask1", 128, NULL, 2, NULL);
  xTaskCreate(myTask2, "myTask2", 128, NULL, 1, NULL);
  xTaskCreate(GateKeeperTask, "GateKeeper", 128, NULL, 0, NULL);

  xQueue = xQueueCreate(5, sizeof(int));
  
}

void interrupcao(){
 
  Serial.println("GERANDO INTERRUPÇÃO...");
  xSemaphoreGiveFromISR(xSemaphoreBinary, NULL); //entregando semaforo para aplicação
  Serial.println("SAINDO INTERRUPÇÃO...");
  
}

void myTask1(void *pvParameters){
  
  unsigned long timeStop; 
  
  for(;;){
    
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //instruir gatekeeper a ligar o led
    xQueueSend(xQueue, (void *)&high, portMAX_DELAY); 
    
    timeStop = millis() + 500;
    while (millis() < timeStop);
    
    xSemaphoreTake(xSemaphoreMutex, portMAX_DELAY);
    Serial.println();
    Serial.println("Imprimindo mensagem da task 1...");
    Serial.println("Esta impressão deve ser correta...");
    Serial.println();
    xSemaphoreGive(xSemaphoreMutex);    
    
    xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(800));
    
  }
  
}

void myTask2(void *pvParameters){

  unsigned long timeStop; 

  for(;;){   
    if(xSemaphoreTake(xSemaphoreBinary, 100) == true){
      
      //instruir gatekeeper a desligar o led
      xQueueSend(xQueue, (void *)&low, portMAX_DELAY); 
      
      timeStop = millis() + 200;
      while (millis() < timeStop);

      xSemaphoreTake(xSemaphoreMutex, portMAX_DELAY);
      Serial.println();
      Serial.println("Imprimindo mensagem da task 2...");
      Serial.println("Esta impressão deve ser correta...");
      Serial.println();
      xSemaphoreGive(xSemaphoreMutex);    
    
    } else if(xSemaphoreTake(xSemaphoreBinary, 0) == NULL) {
      Serial.println("Semaforo ocupado");
    }
  }
  
}

void GateKeeperTask(void *pvParameters){
  
  int *queueMsg;
  
  for(;;){
    xQueueReceive(xQueue,&queueMsg,portMAX_DELAY);
    if(queueMsg == 0){
      digitalWrite(ledPin, LOW);
      Serial.println("Desigando Led");
    } else if(queueMsg == 1){
      digitalWrite(ledPin, HIGH);
      Serial.println("Ligando Led");
    }
  
  }
  
}

void loop() {

}
