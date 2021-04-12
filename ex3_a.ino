#include <Arduino_FreeRTOS.h>
#include <semphr.h>

const byte ledPin = 13;
const byte interruptPin = 2;

void task1(void *pvParameters);
void task2(void *pvParameters);

SemaphoreHandle_t xSemaphore = NULL;

void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupcao, RISING);

  xTaskCreate(task1, "task1", 128, NULL, 3, NULL);
 
}

void interrupcao(){
 
  Serial.println("GERANDO INTERRUPÇÃO...");
  digitalWrite(ledPin, HIGH);
  
  //aproximadamente 500 milis 
  for(unsigned long i=0; i<1150000; i++){
    asm("nop");
  }
  
  digitalWrite(ledPin, LOW);
  Serial.println("SAINDO INTERRUPÇÃO...");

}

void task1(void *pvParameters){
  
  unsigned long timeStop;
  
  for(;;){
      TickType_t xLastWakeTime = xTaskGetTickCount();
      Serial.println("TASK 1");
      timeStop = millis() + 200;
      while (millis() < timeStop);
      xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(300));
  }
}

void loop() {

}
