// -- include lib ----------------------------------

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// -- new datatyp ----------------------------------

using wbuffer    = std::array<String,5>;

// -- declaration -------------------------------------------------------------------------------------------------------------------------------------

class db_query
{
    private:
        // -- variables -------------------------

        String ID;
        String temp_out;
        String humidity_out;
        String temp_in;
        String humidity_in;
        String date;

        // for weatherquery ---------------------------------------------------------------------------

        const String URL {"Link zum abrufen einer JSON_Datei"}; // URL for weather json data 
        int httpcode{};                                                 
        String httpdata{};                                              // for incomming data
        
        // --------------------------------------------------------------------------------------------

        // -- function --------------------------

        bool weatherquery();            // thus mariadb is queried

    public:
        // -- constructor

        db_query();

        // -- function -------------------------

        String Weather_tmp_out();
        String Weather_hum_Out();
        String Weather_tmp_in();
        String Weather_hum_in();
        String Weather_date();
        void all_Weather_data(wbuffer &weatherdata);
};