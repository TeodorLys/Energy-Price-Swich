#pragma once
#include <Arduino.h>
#include "data_handling/date_parse.h"
struct hour_price{
    date_parse start;
    date_parse end;
    float cents[4];
    char name[3][4];  //Region, SE1, SE2, SE3, SE4
    hour_price(){}
};

class price_parsing {
private:
    struct variable{
        String name, value;
    };
    hour_price *_temp = nullptr;
    hour_price _buff_column;
    hour_price _min;
    hour_price _max;
public:
    price_parsing();
    //Section contains the hour price data
    void push_back_column_section(String _name, String _value);
    //Buffer contains the date and region data
    void push_back_column_buffer(String _start, String _end);
    // Parses the ExtraRow = Min, Max, Avg
    void push_back_column_extra(String _name, String _date);
    hour_price get_data_by_hour(byte hour);
    // Splits the element into name and value ex. "StartTime":"2022-02-10T00:00:00"
    variable _validate__parse(String _val);
public:
    hour_price get_min_data() {return _min;}
    hour_price get_max_data() {return _max;}
};
