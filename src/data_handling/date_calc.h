#pragma once
#include <Arduino.h>
#include "date_parse.h"

//TODO: Fix code naming rules
class date_calc{
public:
    // Not in use, but first calculates first monday every month
    int _first_mon_of_month(int m, int y);
    // Calculates how many days per month
    int num_per_month(int m, int y);
    // Adds 1 to the parameter
    date_data time_add(date_data _d);
};