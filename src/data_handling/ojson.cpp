#include "ojson.h"
/*
This is a highly speciallized json parser, for this specific website..
Probably a bad idea..
But, we use the HTTPClient getStream() function, because the ESP does not have enough memory
to handle >70Kb of json data.
*/


ojson::ojson(){
    _stream = new char[MAX_STREAM_BUFFER];
}


/*
Here we lookup today and tomorrows electricity prices.
We lookup this data every other day.
Since a date are not incremental i.e. jan 31 + 1 are not jan 32.
Nordpool website have a function called dayahead prices(then you don't have to specify date).
we can use that to know which date tomorrow is(are?).
*/
void ojson::parse_rows(HTTPClient *http){
    _http = *&http;
    (*_http).getStream().setTimeout(MAX_STREAM_BUFFER);
    _file_size = (*_http).getSize();
    _stream = new char[MAX_STREAM_BUFFER];

    (*_http).getStream().readBytes(_stream, MAX_STREAM_BUFFER);

    _file_size -= MAX_STREAM_BUFFER;
    _pos = 0;

    while(_stream[_pos] != ']'){
        if(_pos == MAX_STREAM_BUFFER){
            _stream_read();
        }
        if(_stream[_pos] == '['){
            _pos_inc();
            while(_stream[_pos] != ']'){
                if(_stream[_pos] == '{'){
                    String _tmp = "";
                    _tmp.reserve(MAX_STREAM_BUFFER);
                    while(_stream[_pos] != '}'){
                        _tmp += _stream[_pos];
                        _pos_inc();
                    }
                    //Pushback the actual value and price region
                    _parser.push_back_column_section(_parse_by_name("Name", &_tmp), _parse_by_name("Value", &_tmp));
                    _tmp = "";
                }
                _pos_inc();
            }
            String _tmp = "";
            _tmp.reserve(MAX_STREAM_BUFFER);
            while(_stream[_pos] != '}'){
                _tmp += _stream[_pos];
                _pos_inc();
            }
            //Extrarows are ex. max, min average... we want both but seperated.
            if(_parser._validate__parse(_parse_by_name("IsExtraRow", &_tmp)).value != "true"){
                //This gets the start and endtime of the values. and pushes the data array to the buffer
                _parser.push_back_column_buffer(_parse_by_name("StartTime", &_tmp), _parse_by_name("EndTime", &_tmp));
            }else {
                //We save the extra stuff
                _parser.push_back_column_extra(_parse_by_name("Name", &_tmp), _parse_by_name("StartTime", &_tmp));
            }
            
            _tmp = "";
        }
        _pos++;
    }
    delete[] _stream;
    (*http).end();
    //We take a random variable, here the extra(max) variable, and uses its date 
    //to know the last date of our data collection
    _date = _parser.get_max_data().start.to_struct();
}
//Since the data structure are 24 elements we can just use the hour parameter for the array.
hour_price ojson::get_data_by_hour(int hour){
    //_parser.print_data_by_hour(hour);
    return _parser.get_data_by_hour(hour);
}

/*PRIVATE*/
//Delete the stream data and retrieve the next set of data.
void ojson::_stream_read(){
    delete[] _stream;
    if(_file_size < MAX_STREAM_BUFFER){
        _stream = new char[_file_size];
        (*_http).getStream().readBytes(_stream, _file_size);
        _file_size = 0;
    }else {
        _stream = new char[MAX_STREAM_BUFFER];
        (*_http).getStream().readBytes(_stream, MAX_STREAM_BUFFER);
        _file_size -= MAX_STREAM_BUFFER;
    }
    _pos = 0;
}

//Look if we need to read more of the stream and increment the read position by 1
void ojson::_pos_inc(){
    _pos++;
    if(_pos == MAX_STREAM_BUFFER)
        _stream_read();
}

// Look into the stream and find variables, and its value, by its name.
String ojson::_parse_by_name(String _name, String *_val){
    String _tmp = (*_val).substring((*_val).indexOf("\"" + _name + "\":"));
    _tmp.remove(_tmp.indexOf(",\""), _tmp.length());
    return _tmp;
}

/*PRIVATE*/