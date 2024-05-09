#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include "price_parsing.h"

class ojson {
private:
    struct variable{
        String name;
        String value;
    };

    String buffer;
    //How much of the data stream we read at a time.
    const int MAX_STREAM_BUFFER = 1024;
    char* _stream;
    int _pos;
    int _file_size;
    HTTPClient *_http;
    price_parsing _parser;
    date_data _date;
public:
    ojson();
    void parse_rows(HTTPClient *http);
    hour_price get_data_by_hour(int hour); 
private:
    void _stream_read();
    void _pos_inc();
    String _parse_by_name(String _name, String *_val);
public:
    // The mininum price of the specified date
    hour_price get_min_data() {return _parser.get_min_data();}
    // The maximum price of the specified date
    hour_price get_max_data() {return _parser.get_max_data();}
    // Check the specified date we looked up
    date_data get_date () const {return _date;}
};