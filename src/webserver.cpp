// -- include header ----------------------

#include "webserver.h"




// -- function definition -----------------

void webserver(WiFiServer &server,volatile int &counter_server)
{
// ---- variables ----------------------------------------------------

    String header{};

    String unuser_1{"off"};         
    String unuser_2{"off"};
    int unuser1{12};
    int unuser2{14};



// ---- function -----------------------------------------------------
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("Client available");
        String currentLine{""};

        while(client.connected())
        {
            if(client.available())
            {
                char c = client.read();
                Serial.write(c);
                header+=c;

                if(c=='\n')
                {
                    if(currentLine.length() == 0)
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        if(header.indexOf("GET /12/on") >= 0)
                        {
                            Serial.println("GPIO 12 on");
                            unuser_1 = "on";
                            digitalWrite(unuser1, LOW);
                        }
                        else if(header.indexOf("GET /12/off") >= 0)
                        {
                            Serial.println("GPIO 12 off");
                            unuser_1 = "off";
                            digitalWrite(unuser1, HIGH);
                        }
                        else if(header.indexOf("GET /14/on") >=0)
                        {
                            Serial.println("GPIO 14 on");
                            unuser_2 = "on";
                            digitalWrite(unuser2, LOW);
                        }
                        else if(header.indexOf("GET /14/off") >=0)
                        {
                            Serial.println("GPIO 14 off");
                            unuser_2 = "off";
                            digitalWrite(unuser2, HIGH);
                        }
                        else if(header.indexOf("GET /counter/+1") >=0)
                        {
                            ++counter_server;
                            if(counter_server >= 4 )
                            {
                                counter_server = 0;
                            }
                                
                        }


                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewprot\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");

                        // CSS 
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #555555;}</style></head>");

                        // Web Page Heading
                        client.println("<body><h1>ESP32 Web Server</h1>");
                            
                        // Button 1
                        client.println("<p>GPIO12 -State " + unuser_1 + "</p>");
                        if(unuser_1 == "off")
                        {
                            client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
                        }
                        else
                        {
                            client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
                        }

                            // Button 2 
                        client.println("<p>GPIO14 -State " + unuser_2 + "</p>");
                        if(unuser_2 == "off")
                        {
                            client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
                        }
                        else
                        {
                            client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
                        }

                        // Button 3
                        client.println("<p>counter  " + String(counter_server) + "</p>");
                        client.println("<p><a href=\"/counter/+1\"><button class=\"button\">counter +</button></a></p>");

                        client.println("</body></html>");
                        client.println();
                        break;
                    }
                    else
                    {
                        currentLine = "";
                    }
                        
                }
                else if(c != '\r')
                {
                    currentLine += c;
                }
            }
        }
        header = "";
        client.stop();
        Serial.println("client disconnectet");
        Serial.println();
    }
}