#include "price_parsing.h"
#include <Arduino.h>

price_parsing::price_parsing() {
    if(_temp == nullptr)
        _temp = new hour_price[24];
}

void price_parsing::push_back_column_section(String _name, String _value){
    variable _region = _validate__parse(_name);
    variable _value_kwh = _validate__parse(_value);
    /*
        We divide by 10, because the price is, SEK/MWH.
        I want CENT/KWH instead!
    */
    if(_region.value == "SE1"){
        _buff_column.name[0][0] = _region.value.charAt(0);
        _buff_column.name[1][0] = _region.value.charAt(1);
        _buff_column.name[2][0] = _region.value.charAt(2);
        _value_kwh.value.replace(",", ".");
        _value_kwh.value.replace(" ", "");
        _buff_column.cents[0] = _value_kwh.value.toFloat() / 10;
    }else if(_region.value == "SE2"){
        _buff_column.name[0][1] = _region.value.charAt(0);
        _buff_column.name[1][1] = _region.value.charAt(1);
        _buff_column.name[2][1] = _region.value.charAt(2);
        _value_kwh.value.replace(",", ".");
        _value_kwh.value.replace(" ", "");
        _buff_column.cents[1] = _value_kwh.value.toFloat() / 10;
    }else if(_region.value == "SE3"){
        _buff_column.name[0][2] = _region.value.charAt(0);
        _buff_column.name[1][2] = _region.value.charAt(1);
        _buff_column.name[2][2] = _region.value.charAt(2);
        _value_kwh.value.replace(",", ".");
        _value_kwh.value.replace(" ", "");
        _buff_column.cents[2] = _value_kwh.value.toFloat() / 10;
    }else if(_region.value == "SE4"){
        _buff_column.name[0][3] = _region.value.charAt(0);
        _buff_column.name[1][3] = _region.value.charAt(1);
        _buff_column.name[2][3] = _region.value.charAt(2);
        _value_kwh.value.replace(",", ".");
        _value_kwh.value.replace(" ", "");
        _buff_column.cents[3] = _value_kwh.value.toFloat() / 10;
    }
}

void price_parsing::push_back_column_buffer(String _start, String _end){
    variable _t_start = _validate__parse(_start);
    variable _t_end = _validate__parse(_end);
    _buff_column.start = _t_start.value.c_str();
    _buff_column.end = _t_end.value.c_str();
    _temp[_buff_column.start.get_hour()] = _buff_column;
}

void price_parsing::push_back_column_extra(String _name, String _date){
    variable name = _validate__parse(_name);
    variable date = _validate__parse(_date);

    if(name.value == "Min"){
        _buff_column.start = date.value.c_str();
        _min = _buff_column;
    }else if(name.value == "Max"){
        _buff_column.start = date.value.c_str();
        _max = _buff_column;
    }
}

hour_price price_parsing::get_data_by_hour(byte hour){
    return _temp[hour];
}

/*PRIVATE*/
price_parsing::variable price_parsing::_validate__parse(String _val){
    // Make some error handling!, its really late.. and I need to work tomorrow..............
    variable _buff;
    _val.replace("\"", "");
    _buff.name = _val.substring(0, _val.indexOf(":"));
    _buff.value = _val.substring(_val.indexOf(":") + 1, _val.length());
    return _buff;
}
/*PRIVATE*/