#pragma once
#include "price_parsing.h"
#include "data_handling/date_calc.h"
#include <HTTPClient.h>
#include <WiFiClient.h>

class scheduler {
private:
    date_parse __req_time;
    float threshold;
public:
    scheduler();
    bool request_time(HTTPClient *http);
    /*
    Threshold between the max and min value of
    the gotten data. 
    */
    void set_threshold(float _threshold);
    void set_threshold(String _threshold);
    //Calculates threshold
    float get_todays_threshold();
    // Collects data from NordPool electricity exchange
    bool create_daily_min_max(HTTPClient *http);
public:
    date_parse real_time() {
        return __req_time;
    }
};