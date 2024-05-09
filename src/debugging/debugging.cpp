#include "debugging.h"

const byte _debugging::MAX_MSG_SIZE = 20;
String _debugging::_http[MAX_MSG_SIZE];
//String _debugging::buffer = "";

String _debugging::get_http_messages(){
    /*
    Combines att messages in the array into one string,
    for display on the webserver
    */
    String _tmp = "";
    for (int a = 0; a < MAX_MSG_SIZE; a++){
        if(_http[a].isEmpty())
            break;
        _tmp += _http[a] + "\n";
    }
    return _tmp;
}

void _debugging::push_error_message(String _s){
    /*
    Checks if last string in array are filled.
    If it is filled, move all elements in array up by 1
    and clear last element.
    */
    if(!_http[MAX_MSG_SIZE - 1].isEmpty())
    {
        _http[0].clear();
        for (byte a = 1; a < MAX_MSG_SIZE; a++){
            _http[a] = _http[a - 1];
        }
        _http[MAX_MSG_SIZE - 1].clear();
    }
    for (byte a = 0; a < MAX_MSG_SIZE; a++)
    {
        if(_http[a].isEmpty()){
            _http[a] = _s;
            break;
        }
    }
}