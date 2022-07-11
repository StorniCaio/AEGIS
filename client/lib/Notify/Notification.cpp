#include "Arduino.h"
#include "Notification.h"

Notification::Notification()
{
    
}

void Notification::wifiNotConnected(){
  digitalWrite(led,HIGH);
  delay(100);
  digitalWrite(led,LOW);
}