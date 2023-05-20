#include <Arduino.h>
#include <BleMouse.h>
// #include <BleKeyboard.h>

BleMouse bleMouse("ESP32蓝牙鼠标","Espressif",100);//其中“ESP32蓝牙键盘”为键盘名称；"Espressif"为制造商
//BleKeyboard bleKeyboard("ESP32蓝牙键盘","Espressif",100);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() 
{
  if(bleMouse.isConnected()) 
  {
    Serial.println("右键单击");
    bleMouse.click(MOUSE_RIGHT);
    delay(500);
    Serial.println("鼠标移动");
    bleMouse.move(38,38);
    delay(500);
    Serial.println("左键点击");
    bleMouse.click(MOUSE_LEFT);
    delay(500);
    Serial.println("鼠标移动");
    bleMouse.move(-38,-38);
    delay(500);
  }
}

// void setup() 
// {
//   Serial.begin(115200);
//   Serial.println("Starting BLE work!");
//   bleKeyboard.begin();
// }

// void loop() 
// {
//   if(bleKeyboard.isConnected()) 
//   {
//     Serial.println("Sending 'Hello world'...");
//     bleKeyboard.print("Hello world");
//     delay(1000);
//     Serial.println("Sending Enter keyHelloworld...");
//     bleKeyboard.write(KEY_RETURN);
//     delay(1000);
//     Serial.println("Sending Play/Pause media key...");
//     bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
//     delay(1000);
//   }
//   Serial.println("Waiting 5 seconds...");
//   delay(5000);
// }
