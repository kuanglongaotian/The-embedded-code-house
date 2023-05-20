#include <Arduino.h>

void Mytask(void *pvParam)
{
  while(1)
  {
    printf("Task1\n");
  }
}

void setup()
{
  Serial.begin(115200);
  TaskHandle_t Taskhandle1=NULL;
  xTaskCreate(Mytask,"mytask",512,NULL,1,&Taskhandle1);
}
void loop()
{
    
}