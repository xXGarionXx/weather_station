// -- include lib -----------------------------------

#include <Arduino.h>
#include <array>
#include <NtpClientLib.h>


// -- include new daten -------------------------------

using weatherbuffer    = std::array<String,5>;              // new datatyp with five strings 

// -- create datatyp ----------------------------------

struct twoString
{
    String currenttime;
    String currentdate;
};

// -- function declaration ----------------------------

void wifi_connect(const char* ssid, const char* password);

void setup_time();

void current_weather(weatherbuffer &weather_buffer, String cureent_time);