#pragma once
/*
Here we store all of our global data.
Most of the classes use this class of lookup variables
*/

#include "date_parse.h"
#include "price_parsing.h"

class data {
public:
    static float max_today[4];
    static float min_today[4];
    static float max_tomorrow[4];
    static float min_tomorrow[4];
    static hour_price today_prices[24];
    static hour_price tomorrow_prices[24];
    static int region;
    static date_data today;
    static date_data tomorrow;
    static int limit;
    static int timer;

public:
    /*
     When we save this setting we only save 1byte of data, 0-3.
     Also makes it easier with array stuff, we can just plug the saved var into the array
    */
    static String region_to_string(int region){
        switch(region){
            case 0:
                return "SE1";
            case 1:
                return "SE2";
            case 2:
                return "SE3";
            case 3:
                return "SE4";
        }
        return "SE0";
    }

    /*
    Checking if any of the loaded data(in preferences) are 0.0
    because the data will never be 0.0
    */
    static bool verify_price_data(){
        for (int a = 0; a < 24; a++)
            for (int b = 0; b < 4; b++)
                if (today_prices[a].cents[b] <= 0.0)
                    return false;

        for (int a = 0; a < 24; a++)
            for (int b = 0; b < 4; b++)
                if (tomorrow_prices[a].cents[b] <= 0.0)
                    return false;
        return true;
    }
    static bool verify_price_min_max(){
        for (int a = 0; a < 4; a++){
            if(max_today[a] <= 0.0)
                return false;
            if(min_today[a] <= 0.0)
                return false;
            if(max_tomorrow[a] <= 0.0)
                return false;
            if(min_tomorrow[a] <= 0.0)
                return false;
        }
        return true;
    }

};