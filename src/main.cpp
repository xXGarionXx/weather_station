// -- include lib -------------------------------------

#include <Arduino.h>
#include <Wifi.h>

// -- include new data --------------------------------

#include "external_function.h"
#include "LED_running_text.h"
#include "webserver.h"
#include "db_query.h"

// -- pinout ------------------------------------------

int INTERRUPTPIN1 = 32;     // pin für Interrupts
int INTERRUPTPIN2 = 35;     // pin für Interrupts 

// -- create datatyp ----------------------------------

// -- variables ---------------------------------------

// wifi connect
const char* ssid      {"Wlan Name"};                     // for wifi_connect
const char* password  {"Netzwerkschlüssel"};             // for wifi_connect

// Interrupts 
volatile int    counter_server {0};                         // butten click counter
volatile int    buttonstatus   {0};                         // State of buttes                        
volatile int    buttonstatus1  {1};                         // State of buttes 2
volatile bool   auswahl        {false};                     // how to switch

// led_data

wbuffer weatheroutput{};
String currenttime{};
// -- objects -----------------------------------------

WiFiServer server(80);                                              // object for webserver

Led_running_text* led       = new Led_running_text{12,14,27,8};     // object for running text display one 
Led_running_text* led2      = new Led_running_text(26,25,33,8);     // object for stand text display two 



// -- function ----------------------------------------

void task1(void * pvParameters);
void task2(void * pvParameters);
void task3(void * pvParameters);

void IRAM_ATTR isr1();                                              // Interrupt 1 declaration
void IRAM_ATTR isr2();                                              // Interrupt 2 declaration

String Output_matrix_one();
String Output_matrix_tow();
void initialize_data(wbuffer &w_data);

// -- programm ---------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
    // ------------------------------------------------------------------------
    xTaskCreate(task1, "task1", 10000 ,NULL, 1 ,NULL);      // initialize task1
    xTaskCreate(task2, "task2", 10000, NULL, 1, NULL);      // initialize task2
    xTaskCreate(task3, "task3", 10000, NULL, 1, NULL);      // initialize task2
    // ------------------------------------------------------------------------

    Serial.begin(9600);

    wifi_connect(ssid, password);                           // Wifi connecten 
    server.begin();                                         // start webserver
    setup_time();                                           // timeserver start 

    // Interrupt------------------------------------------------------
    pinMode(32,INPUT_PULLDOWN);                                         // for debouncing switches
    pinMode(35,INPUT_PULLDOWN);                                         
    attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN1),isr1, HIGH);   // Interrupt setup
    attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN2),isr2, HIGH);   
    //----------------------------------------------------------------
    
    initialize_data(weatheroutput);

    led->Ledsetup();                                                    // ledrunningtext setup
    led2->Ledsetup();                                                      
}

// -----------------------------------------------------------------------------------------------
void loop() 
{
    webserver(server, counter_server);
}

// -----------------------------------------------------------------------------------------------

void task1(void* pvParameters)
{
    for(;;)
    {
        led->Textloop(Output_matrix_one());
    }
}

// -----------------------------------------------------------------------------------------------

void task2(void* pvParameters)
{
    for(;;)
    {
        led2->Stand_text(Output_matrix_tow());
        vTaskDelay(200);
    }
}

void task3(void* pvParameters)
{
    for(;;)
    {
        current_weather(weatheroutput, currenttime);
        currenttime = NTP.getTimeStr();
        vTaskDelay(400);
    }
}

// -- function definition -------------------------------------------------------------

void IRAM_ATTR isr1()
{
    auswahl = true; 
    buttonstatus = digitalRead(INTERRUPTPIN1);

    if(buttonstatus == 1)
    {
        ++counter_server;
    }
  
    if(counter_server >= 6)
    {
        counter_server = 1;
    }
}

void IRAM_ATTR isr2()
{
    auswahl = true; 
    buttonstatus1 = digitalRead(INTERRUPTPIN2);

    if(buttonstatus1 == 1)
    {
        --counter_server;
    }
  
    if(counter_server <= 0)
    {
        counter_server = 5;
    }
}

String Output_matrix_one()
{
    String output{};

    output = " ***Wetterausgabe*** outside temperature: " + weatheroutput[0] + "C " + 
    " outside humidity: " + weatheroutput[1] + "% " +  " inside temperature: " + weatheroutput[2] + 
    + "C " " inside humidity: " + weatheroutput[3] + "%        ";

    
    return output; 
}

String Output_matrix_tow()
{
    // 0 = temp_out, 1 = humidity out, 2 = temp in, 3 = humidity in, 4 = date

    String output{};

    switch (counter_server)
    {
        case 0:
        {
            output = currenttime;
        }
        break;

        case 1:
        {
            output = "t.o." +  weatheroutput[0];
        }
        break;

        case 2:
        {
            output = "h.o." + weatheroutput[1];
        }
        break; 

        case 3:
        {
            output = "t.i." + weatheroutput[2];
        }

    default:
        {
            output = "default ";
        }
        break;
    }

    return output;
}

void initialize_data(wbuffer &w_data)
{
    db_query* weatherdata = new db_query;
    weatherdata->all_Weather_data(w_data);
    delete weatherdata;
}
