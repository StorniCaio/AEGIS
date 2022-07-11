#ifndef RTC_
#define RTC_


#include "Arduino.h"
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include "PubSubClient.h"


using namespace std;

class RTC{

    private:        
        struct timeval t;

        bool arrived, inicio;
        long epsilon;
        uint32_t sync_sec, sync_usec;
        byte cont,timeout;
        int i, j;
        void setTime(int _sec, int _usec);
        const char* syncTopic = "sync/answer";
        const char* resyncTopic = "resync";
        double lastSyncTime = 10000;

    public:
        RTC();
        void sync(PubSubClient &mqtt);
        void askUnix(PubSubClient &mqtt);
        void setUnix(char* packetBuffer, unsigned int size, long aux);
        String getDateTime();
        String getDateTimeToFileName();
        int getUnix_m();
        double getTime();  
        char* getCharArrayTime();
        const char* getSyncTopic();
        void setTimeMQTT(PubSubClient &mqtt, char* topic, byte* payload, unsigned int lenght);
        
        

        
};

#endif
