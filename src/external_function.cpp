// -- include header --------------------------------

#include "external_function.h"

// -- include lib ----------------------------------

#include <WiFi.h>
#include <NtpClientLib.h>

// -- include new data -----------------------------

#include "db_query.h"


// -- variable -------------------------------------

bool weathercounter{false};
int counter{};

// -- function definition --------------------------

void wifi_connect(const char* ssid, const char* password)
{
    while(WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);
        Serial.println("connecting...");
        delay(7000);
        //timesetup();
    }   
    Serial.println("connected"); 
}

// -- Time function ----------------------------------

void setup_time()
{
    WiFiUDP ntpUDP;
    NTPClient timeClient(WiFiUDP);

    NTP.begin("pool.ntp.org",1,true);
}

// --------------------------------------------------

void current_weather(weatherbuffer &weather_buffer, String current_time)
{
    String minute{};

    minute.concat(current_time.charAt(3));
    minute.concat(current_time.charAt(4));

    int minute_int = minute.toInt();


    if(minute_int == 0 || minute_int == 15 || minute_int == 30 || minute_int == 45)
    {
        if(weathercounter == true)
        {   
            Serial.println("I am called to: " + counter);
            counter ++;

            db_query* weatherdata = new db_query;

            weatherbuffer get_data;
            weatherdata->all_Weather_data(weather_buffer);
            weathercounter = false;

            delete weatherdata;
        }
    }
    if(weathercounter != true)
    {
        
        if((minute_int >= 1 && minute_int <= 14) || (minute_int >= 16  && minute_int <= 29) ||
         (minute_int >= 31 && minute_int <= 44) || (minute_int >= 46 && minute_int <= 59))
        {
            weathercounter = true;
            Serial.println("weathercontrol: true ");
        }
    }
}