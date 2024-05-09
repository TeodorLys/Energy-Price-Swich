#pragma once
#include <Preferences.h>

class settings {
private:

public:
    settings();
    void init_settings();
    // Saves a setting, currently just byte values, ex 1,2,3,4 ect.
    void save_setting(String name, byte value);
    int load_setting(String name, byte def_value);
    
    // Save the hour data from the static data.h class, entered from scheduler.create_daily_min_max function
    void save_hour_data(byte day);
    void load_hour_data();

    // Checks whether we have saved any data
    bool has_data();
    // Sets the the verification bool to true. to tell main, that data has been saved.
    void set_verifier();

    // Saves dates for today and tomorrow. Which can then be cross references with realtime
    void save_dates();
    void load_dates();

    // Saves the ExtraRows, entered from scheduler.create_daily_min_max
    void save_min_max();
    void load_min_max();

    //Wipes all data and sets the verifier to false, just in case
    void wipe();
};