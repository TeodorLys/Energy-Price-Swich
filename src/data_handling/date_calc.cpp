#include "date_calc.h"
#include <Arduino.h>

//Yours truly, Stackoverflow
int date_calc::_first_mon_of_month(int m, int y){
    for(int a = 0; a < 7; a++){
        int day = a + 1;
        int _day = (day + y + (y / 4) - (y / 400) + ((31 * m) / 12)) % 7;
        if(_day == 2){
            return day;
        }
        
    }
    return 0;
}

//Yours truly, Stackoverflow
int date_calc::num_per_month(int m, int y){
    // Leap year
	if(m == 2)
	{
		if((y%400==0) || (y%4==0 && y%100!=0))	
			return 29;
		else	
			return 28;
	}
	//months which has 31 days
	else if(m == 1 || 
            m == 3 || 
            m == 5 || 
            m == 7 || 
            m == 8 ||
            m == 10|| 
            m == 12)	
		return 31;
	else 		
		return 30;
}

date_data date_calc::time_add(date_data _d){
    /* 
    ex. jan 31 + 1 is NOT jan 32.
    So we need to check how many days per month and if parameter day +1 is over days per month
    + 1 on the month and set day to 1
    */
    if(_d.day + 1 > num_per_month(_d.month, _d.day)){
        if(_d.month == 12){
            _d.year += 1;
            _d.month = 1;
            _d.day = 1;
        }else {
            _d.month += 1;
            _d.day = 1;
        }
    }else {
        _d.day = _d.day + 1;
    }
    return _d;
}