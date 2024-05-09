#include "scheduler.h"
#include "data_handling/date_parse.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include "data_handling/ojson.h"
#include "data_handling/data.h"
#include "debugging/debugging.h"
#include "settings.h"
#include "data_handling/date_calc.h"

scheduler::scheduler(){}

bool scheduler::request_time(HTTPClient *http){
    if((*http).begin("http://worldtimeapi.org/api/timezone/Europe/Stockholm")){
        int code = (*http).GET();
        if(code > 0){
            //TODO: Add error checking
            String date = (*http).getString();
            date.remove(0, date.indexOf("datetime"));
            String _tmp = date.substring(date.indexOf("datetime"), date.indexOf("."));
            _tmp.remove(0, _tmp.indexOf(":\"") + 2);
            __req_time = _tmp.c_str();
        }
        else {
            _debugging::blink_times(1);
            (*http).end();
            return false;    
        }
    }else{
        _debugging::blink_times(2);
        (*http).end();
        return false;    
    }
    _debugging::blink_times(3);
    (*http).end();
    return true;
}

void scheduler::set_threshold(float _threshold){
    threshold = _threshold;
}

void scheduler::set_threshold(String _threshold){
    /*
    The settings ePaper page can only return a string value with procent
    and i didnt know where to put the conversion... so here it is.
    */
     if(_threshold.indexOf("%") > 0){
         _threshold.replace("%", "");
     }
     float _tmp = _threshold.toFloat();
     _tmp = _tmp / 100;
     threshold = _tmp;
}

float scheduler::get_todays_threshold(){
    /*
    The threshold is based upon a procentage of max-price - min-price,
    the procentage are set in the ePaper settings.
    */
    if (data::today.day == real_time().get_day() && data::today.month == real_time().get_month() && data::today.year == real_time().get_year())
    {
        float _tmp = data::max_today[data::region] - data::min_today[data::region];
        _tmp = _tmp * threshold;
        return (data::min_today[data::region] + _tmp);
    }
    else if (data::tomorrow.day == real_time().get_day() && data::tomorrow.month == real_time().get_month() && data::tomorrow.year == real_time().get_year())
    {
        float _tmp = data::max_tomorrow[data::region] - data::min_tomorrow[data::region];
        _tmp = _tmp * threshold;
        return (data::min_tomorrow[data::region] + _tmp);
    }
    return (data::max_today[data::region]);
}

bool scheduler::create_daily_min_max(HTTPClient *http){
    settings setting;
    delay(2);
    date_calc _calc;
    WiFiClientSecure client;
    client.setInsecure();  // Cant be bothered with fingerprint certificate so insecure it is!
    data::today = date_data(real_time().get_day(), real_time().get_month(), real_time().get_year());
    ojson oj;   //Streaming ojson parser
    String url = "https://www.nordpoolgroup.com/api/marketdata/page/29?currency=,SEK,SEK,EUR&endDate=";
    url += ((String)(data::today.day)) + "-" + ((String)data::today.month) + "-" + ((String)data::today.year);
    if(http->begin(client, url.c_str())){
        int code = (*http).GET();
        if(code > 0){
            oj.parse_rows(&(*http));
            for(int b = 0; b < 4; b++){
                data::min_today[b] = oj.get_min_data().cents[b];
                data::max_today[b] = oj.get_max_data().cents[b]; 
            }
            for (int a = 0; a < 24; a++){
                data::today_prices[a] = oj.get_data_by_hour(a);
            }
            _debugging::blink_times(1);
        }else {
            _debugging::push_error_message("Today data collection error code: " + (String)code);
        }
    }else {
        _debugging::push_error_message("Could not begin http");
    }
    (*http).end();
    client.stop();
    //In arduino core for ESP32 there seems to be a bugg. So I need to create a new Client object here.
    WiFiClientSecure _client;
    _client.setInsecure();
    //Add 1 to todays date. SEE time_add function.
    data::tomorrow = _calc.time_add(data::today);
    url = "https://www.nordpoolgroup.com/api/marketdata/page/29?currency=,SEK,SEK,EUR&endDate=";
    url += ((String)(data::tomorrow.day)) + "-" + ((String)data::tomorrow.month) + "-" + ((String)data::tomorrow.year);
    if((*http).begin(_client, url.c_str())){
        int code = (*http).GET();
        if(code > 0){
            oj.parse_rows(&(*http));
            for(int b = 0; b < 4; b++){
                data::min_tomorrow[b] = oj.get_min_data().cents[b];
                data::max_tomorrow[b] = oj.get_max_data().cents[b];
            }
            for (int a = 0; a < 24; a++){
                data::tomorrow_prices[a] = oj.get_data_by_hour(a);
            }
            _debugging::blink_times(2);
        }else {
            _debugging::push_error_message("Today data collection error code: " + (String)code);
        }
    }else {
        _debugging::push_error_message("Could not begin http");
    }
    (*http).end();
    _client.stop();
    Serial.println(".DONE!");
    setting.save_hour_data(1);  
    setting.save_hour_data(2);
    setting.save_min_max();
    setting.save_dates();
    setting.set_verifier();  // Sets a "has_data" entry in the preferences to true. to know if this has been started before.
    return true;
}