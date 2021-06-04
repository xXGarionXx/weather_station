#include "LED_running_text.h"

#include <Arduino.h>
#include <LedControl.h>

// Constructor

Led_running_text::Led_running_text(int data, int clk, int cs, int dp_anzahl)
    :LedControl(data, clk, cs, dp_anzahl)
    {

    }

// function public

void Led_running_text::Ledsetup()
{
    for(int i=0; i<=8; ++i)
    {
        shutdown(i,false);
        setIntensity(i,0);
        clearDisplay(i);
    }
}

void Led_running_text::Textloop(String text)
{
    for(unsigned int i=0; i<text.length(); ++i)
    {
        counter1 = fontlist.find(text[i]);
        counter2 = fontlist.find(text[i+1]);

        if(counter2 <= -1)
        {
            counter2 = 0;
        }

        
        for(int b=0; b<8; ++b)
        {
            buffer1[b] = pgm_read_byte(&font[counter1][b]);
            buffer2[b] = pgm_read_byte(&font[counter2][b]);    
        }

        ScreenOutput(buffer1, buffer2);

        for(int a=0; a<56; ++a)
        {
            ibuffer1[a] = buffer1[a];
            ibuffer2[a] = buffer2[a];
        }

        for(int b=0; b<56; ++b)
        {
            buffer1[b+8] = ibuffer1[b];
            buffer2[b+8] = ibuffer2[b];
        }
    }
}

void Led_running_text::Stand_text(String text)
{
    counter2 = 0;

    if(text.length() < 8)
    {
        for(int t=0; t<comparsion; ++t)
        {
            text.concat(" ");
        }
    }
    
    for(unsigned int i=0; i<text.length(); ++i)
    {
        counter1 = fontlist.find(text[i]);
        
        for(int b=0; b<8; ++b)
        {
            buffer3[b] = pgm_read_byte(&font[counter1][b]);
        }

    
        if(counter2 < 7)
        {
            for(int a=0; a<56; ++a)
            {
                ibuffer3[a] = buffer3[a];
            }
            
            for(int b=0; b<56; ++b)
            {
                buffer3[b+8] = ibuffer3[b];
            }
            ++counter2;
        }
        else
        {
            ScreenOutput2(buffer3);
        }
    }
}

// function private 

void Led_running_text::ScreenOutput(buffer &buffer1, buffer &buffer2)
{
    for(int v=0; v<8; ++v)
    {
        for(int r=0; r<8; ++r)
        {
            display = 0;
            for(int i=0; i<15; i+=2)
            {
                zbuffer[i  ] = buffer1[r + displaybuffer[display]]<<8;
                zbuffer[i+1] = buffer2[r + displaybuffer[display]];
                ++display;

            }
            

            for(int x=0; x<8; ++x)
            {
                for(int b=0; b<16; ++b)
                {
                    bitWrite(outputbuffer[b], x, bitRead(zbuffer[b]<<v, x+8));
                }
            }

            display = 0;
            for(int f=0; f<15; f+=2)
            {
                setRow(display,r,(outputbuffer[f] | outputbuffer[f+1]));
                ++display;
            }
        }
        //delay(25);
        vTaskDelay(25);
    }
}

void Led_running_text::ScreenOutput2(buffer &buffer3)
{

    for(int a=0; a<8; ++a)
    {
        
        for(int i=0; i<8; ++i)
        {
            setRow(a,i,buffer3[i+displaybuffer[a]]);
        }
    }
}



