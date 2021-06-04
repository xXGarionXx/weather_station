// -- include header ----------------------------------

#include "db_query.h"

db_query::db_query()
{

}

bool db_query::weatherquery()
{
    HTTPClient http;                                // client for httptask
    http.begin(URL);                                // assign URL to client
    httpcode = http.GET();                          // methode to query

    if (httpcode == 200)                            // httpcode check
    {
        Serial.println("httpcode == 200");
    } 
    else 
    {
        Serial.println("httpcode == not 200");
    }

    httpdata = http.getString();                    // save incomming data in String 

    // -- the weather data is parsed -------------------------------------------------------

    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(6) + 100;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, httpdata);

    if(error) 
    {
        Serial.println(F("deserializeJson() fehlgeschlagen: "));
        vTaskDelay(5000);
        return false;
    }

    // transfer in string

    String _ID              = doc[0]["ID"];
    String _temp_out        = doc[0]["temp_out"];
    String _humidity_out    = doc[0]["humidity_out"];
    String _temp_in         = doc[0]["temp_in"];
    String _humidity_in     = doc[0]["humidity_in"];
    String _date            = doc[0]["humidity_in"];

    // tranfer in class variables 

    ID              = _ID;
    temp_out        = _temp_out;
    humidity_out    = _humidity_out;
    temp_in         = _temp_in;
    humidity_in     = _humidity_in;
    date            = _date; 

    return true;

}

// -- function for the statements of the weather data -------------------------------

String db_query::Weather_tmp_out()
{
    weatherquery();
    return temp_out;
}

String db_query::Weather_hum_Out()
{
    weatherquery();
    return humidity_out;
}

String db_query::Weather_tmp_in()
{
    weatherquery();
    return temp_in;
}

String db_query::Weather_hum_in()
{
    weatherquery();
    return humidity_in;
}

String db_query::Weather_date()
{
    weatherquery();
    return date;
}

void db_query::all_Weather_data(wbuffer &weatherdata)
{
    bool controll{false};

    while (controll == false)
    {
        controll = weatherquery();
    }

    weatherdata[0] = temp_out;
    weatherdata[1] = humidity_out;
    weatherdata[2] = temp_in;
    weatherdata[3] = humidity_in;
    weatherdata[4] = date;
    
    
    /*do
    {
        controll = weatherquery();

        weatherdata[0] = temp_out;
        weatherdata[1] = humidity_out;
        weatherdata[2] = temp_in;
        weatherdata[3] = humidity_in;
        weatherdata[4] = date;

    } while (controll == false);
    */
}

// ---------------------------------------------------------------
