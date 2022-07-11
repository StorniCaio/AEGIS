#include "RTC_.h"



#define DEBUG


RTC::RTC(){
    arrived = false;
    epsilon = 0;
    inicio = true;
    sync_sec = 0;
    sync_usec = 0;
    cont = 0;
    timeout = 25;
}

void RTC:: askUnix(PubSubClient &mqtt){

    mqtt.subscribe(resyncTopic);
    mqtt.subscribe(syncTopic,0);
    arrived = false;
    byte contaLoop = 0;

    while((!arrived) && ( timeout > contaLoop) ){
        if(!mqtt.connected()){
            if(mqtt.connect("ESP32_Test")) Serial.println("Connected");
            }
        epsilon = (long)micros();
        mqtt.publish("sync/request", "Unix");
        Serial.println("Enviado Pedido");
        mqtt.loop();
        delay(1);
        contaLoop++;
    }

}

void RTC::setUnix(char* packetBuffer, unsigned int size, long timeReceive){
    
    char char_sec[16], char_usec[8]; //Vetor maior que o dado para ter o \n
            memset(char_sec, 0, sizeof(char_sec));
            memset(char_usec, 0, sizeof(char_usec));

    #ifdef DEBUG
        Serial.print("Pacote Recebido ");
        for (int i = 0; i < size; i++) 
            Serial.print((char)packetBuffer[i]);
        Serial.println("");
            
    #endif
            for(i = 0; packetBuffer[i]!='.'; i++)
                char_sec[i] = packetBuffer[i];
            for (j = i + 1; j < size && j < 6 + i +1; j++) // Laço até acabar o packetBuffer ou até preencher 6 números (micro segundos)
               char_usec[j-(i+1)] = packetBuffer[j];            

            sync_sec = atoi(char_sec);
            sync_usec = atoi(char_usec);
            epsilon = 0.5*(timeReceive - epsilon);

            Serial.println("Contador: "+String(cont));
            arrived = true;
            setTime(sync_sec, sync_usec*pow(10, j-i-1) + epsilon);//+370000);

            

    #ifdef DEBUG 
            Serial.println("Epsilon: "+String(epsilon,6));
            Serial.println("Casa decimais: "+String(j-i-1));
            Serial.print("Convertido para Número "+String(sync_sec)+"."+String(sync_usec)+" s\n");
            Serial.println("Syncado com "+String(sync_sec)+"."+String((int)(sync_usec*pow(10, j-i-1) + epsilon))+" s\n");
    #endif
}

void RTC::setTime(int _sec, int _usec){
    struct timeval tv;
    tv.tv_sec = _sec;
    tv.tv_usec = _usec;
    settimeofday(&tv, NULL);
}

double RTC::getTime(){
    gettimeofday(&t, NULL);
    double time = double(t.tv_sec+(t.tv_usec/1.0E6));
    return time;
}

char* RTC::getCharArrayTime(){
  return (char*)String(getTime(),5).c_str();
}

String RTC::getDateTime(){
    time_t tt = time(NULL);//Obtem o tempo atual em segundos. Utilize isso sempre que precisar obter o tempo atual
    struct tm data = *gmtime(&tt);//Converte o tempo atual e atribui na estrutura
    char data_formatada[64];
    strftime(data_formatada, 64, "%d/%m/%Y %H:%M:%S", &data);//Cria uma String formatada da estrutura "data"
    
    return String(data_formatada);
}

String RTC::getDateTimeToFileName(){
    time_t tt = time(NULL);//Obtem o tempo atual em segundos. Utilize isso sempre que precisar obter o tempo atual
    struct tm data = *gmtime(&tt);//Converte o tempo atual e atribui na estrutura
    char data_formatada[64];
    strftime(data_formatada, 64, "%d_%m_%Y_%H_%M", &data);//Cria uma String formatada da estrutura "data"
    
    return String(data_formatada);
}
const char* RTC::getSyncTopic(){
    return syncTopic;
}

void RTC:: setTimeMQTT(PubSubClient &mqtt, char* topic, byte* payload, unsigned int lenght){
    if((String(topic) == String(getSyncTopic())) &&
     (abs(getTime() - lastSyncTime) >2)){

      Serial.println("Syncando");
      setUnix((char*) payload, lenght, micros()); 
      lastSyncTime = getTime();
    }
    if(String(topic) == String(resyncTopic)){
      Serial.println("ReSyncando");
      askUnix(mqtt);
    }
}

