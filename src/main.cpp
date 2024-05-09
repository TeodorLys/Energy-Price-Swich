#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
//#include <RMaker.h>
//#include <WiFiProv.h>
#include "data_handling/ojson.h"
#include "scheduler.h"
#include "data_handling/data.h"
#include "graphics/graphics.h"
#include "graphics/page.h"
#include "graphics/page_handler.h"
#include "IO/button_handler.h"
#include "debugging/debugging.h"
#include "settings.h"

scheduler schedule;
graphics gp;
page_handler p_handler;
byte connect_counter = 0;
button_handler button(GPIO_NUM_2);
bool switch_once = false;
bool all_loaded = false;
page p_init("INIT");
long sleep_timer;
bool data_loaded = false;
bool time_success = true;
settings setting;
WebServer server(80);
WiFiMulti multi;

void handleroot(){
  server.send(200, "text/plain", _debugging::get_http_messages());
}

/*
TODO:
  Since the preferences are abit wobbly and cant load everything accuratly everytime.
  We now download the price data if the data could not be loaded from preferences on the 
  first time. Nordpool does not allow for automatic data extraction, we should minimize the
  usage of nordpool, also downloading and parsing of that data is very energy costly, we should 
  minimize that aswell.
*/

void setup()
{
  pinMode(GPIO_NUM_20, OUTPUT); // Led indicator(_debugging blink function)
  pinMode(GPIO_NUM_5, OUTPUT);  // Nexa switch on
  pinMode(GPIO_NUM_8, OUTPUT);  // Nexa switch off
  if (gp.begin())
  {
    gp.clear_frame_buffer();
  }
  else
  {
    delay(2000);
  }
  //Setting the initialize display to splash screen(SEE function declaration)
  p_init.set_splash_page(true, "");
  p_init.add_element(element("Connect", &Font24, false, element_base::TEXT_ALIGNMENT::CENTER), "", "", "");
  WiFiManager wm;
  /*
  If no SSID was saved we display a message to check the setup instructions
  */
  if (!wm.getWiFiIsSaved())
  {
    //Access point configuration
    p_init.add_element(element("AP", &Font24, false, element_base::TEXT_ALIGNMENT::CENTER), "", "", "");
    p_init.add_element(element("Check", &Font24, false, element_base::TEXT_ALIGNMENT::CENTER), "", "", "");
    p_init.add_element(element("Instructions", &Font24, false, element_base::TEXT_ALIGNMENT::CENTER), "", "", "");
  }
  else
  {
    //Connected to the saved WIFI
    p_init.add_element(element("Wifi", &Font24, false, element_base::TEXT_ALIGNMENT::CENTER), "", "", "");
  }
  delay(5);
  gp.draw_page(p_init);
  // If the ap was not connected to within 60 minuites we restart the ESP
  // Because sometimes it does not connect to the wifi.
  wm.setConfigPortalTimeout(60);
  wm.autoConnect("energy_ap");
  if (WiFi.status() == WL_CONNECTED)
  {
    _debugging::blink_times(2);
    _debugging::push_error_message("WIFI: " + WiFi.SSID() + " OK");
  }
  else
  {
    _debugging::blink_times(3);
    ESP.restart();
  }
  if(!MDNS.begin("energy")){
    _debugging::push_error_message("Could not start mdns!");
  }
  // For error messages.
  server.on("/msg", handleroot);
  server.begin();
  setting.init_settings();  // Here we initialize preferences
  p_handler.create_default_pages();  // Creating the ePaper settings and default pages.
  p_handler._buffer[0]->add_element(element(WiFi.SSID(), &Font24, false), "", "", "ssid");
  /*
  Some arduino class/function redefines the gpio-5, so here we set it to output again
  */
  pinMode(GPIO_NUM_5, OUTPUT);
  pinMode(GPIO_NUM_8, OUTPUT);
  pinMode(GPIO_NUM_9, INPUT_PULLUP);
  MDNS.addService("http", "tcp", 80);
}

void loop()
{
  server.handleClient();
  /*
  TODO: 
  Add a function to check if usb is connected. then deepsleep should not happen.
  If battery is used, deepsleep should be used.
  */
  if (millis() - sleep_timer >= 20000)
  {
    /*
    This is also the programming pin, This will not be on the commercial version.
    Only my dev board.
    */
    if(digitalRead(GPIO_NUM_9) == 0){
      // If the sleep type is present in the draw buffer, we just change the text.
      // And not add a new element to the buffer.
      if(!p_handler._buffer[0]->is_element_present("sleep_type"))
        p_handler._buffer[0]->add_element(element("DEBUG", &Font24, false), "", "", "sleep_type");
      else
        p_handler._buffer[0]->get_element_by_name("sleep_type")->set_name("DEBUG");
      
      // Same as above, change the text if present.
      if(!p_handler._buffer[0]->is_element_present("sleep_time"))
        p_handler._buffer[0]->add_element(element(String(schedule.real_time().get_deep_sleep_wake_time()), &Font24, false), "", "", "sleep_time");
      else
        p_handler._buffer[0]->get_element_by_name("sleep_time")->set_name(String(schedule.real_time().get_deep_sleep_wake_time()));
      gp.draw_handler(&p_handler);
      sleep_timer = millis();
    }else {
      if(!p_handler._buffer[0]->is_element_present("sleep_type"))
        p_handler._buffer[0]->add_element(element("DEEPSLEEP", &Font24, false), "", "", "sleep_type");
      else
        p_handler._buffer[0]->get_element_by_name("sleep_type")->set_name("DEEPSLEEP");

      // Use the menu selector(button) to wake the board from deepsleep.
      esp_deep_sleep_enable_gpio_wakeup(1<<2, ESP_GPIO_WAKEUP_GPIO_LOW);
      // Dont think this is nessasary, just a testing thing...
      // But it doesnt hurt.. i think..
      gpio_reset_pin(GPIO_NUM_21);

      /*
      If the time web api is down or unresponsive we default to restarting every 15min.
      Since we do a time get every wake from deepsleep.
      */
      if(time_success){
        if(!p_handler._buffer[0]->is_element_present("sleep_time"))
          p_handler._buffer[0]->add_element(element(String(schedule.real_time().get_deep_sleep_wake_time()), &Font24, false), "", "", "sleep_time");
        else
          p_handler._buffer[0]->get_element_by_name("sleep_time")->set_name(String(schedule.real_time().get_deep_sleep_wake_time()));
        gp.draw_handler(&p_handler);
        /*
        I want to wake the board every 30min(final goal: every 60min). since the user
        might not start it at exactly ex. 16.30 we and waiting for the time to pass is wasteful.
        We need to take the remainder of the time and sleep until the closest 30 or 60 mark.
        */
        ESP.deepSleep(schedule.real_time().get_deep_sleep_time());
      } else {
        if(!p_handler._buffer[0]->is_element_present("sleep_time"))
          p_handler._buffer[0]->add_element(element(String("15min"), &Font24, false), "", "", "sleep_time");
        else
          p_handler._buffer[0]->get_element_by_name("sleep_time")->set_name("15min");
        gp.draw_handler(&p_handler);
        ESP.deepSleep(900e6);
      }
    }
  }

  if (p_handler.graphics_update())
  {
    schedule.set_threshold(p_handler._buffer[1]->get_element_by_name("limit")->get_state_name());
    p_handler._buffer[0]->get_element_by_name("threshold")->set_name(String(schedule.get_todays_threshold()));
    p_handler._buffer[0]->get_element_by_name("region")->set_name(data::region_to_string(data::region));
    /*
    Make a function to check these conditionals. too long.
    */
    if (data::today == schedule.real_time().to_struct())
    {
      p_handler._buffer[0]->get_element_by_name("hour_price")->set_name((String)data::today_prices[schedule.real_time().get_hour()].cents[data::region]);
    }
    else if (data::tomorrow == schedule.real_time().to_struct())
    {
      p_handler._buffer[0]->get_element_by_name("hour_price")->set_name((String)data::tomorrow_prices[schedule.real_time().get_hour()].cents[data::region]);
    }
    gp.draw_handler(&p_handler);
  }

  switch (button.press())
  {
  case button_handler::PRESS:
    p_handler.select_element_down();
    //If the button is pressed reset the 20sec deepsleep timer.
    sleep_timer = millis();
    break;
  case button_handler::DOUBLE_PRESS:
    p_handler.select_page();
    sleep_timer = millis();
    break;
  default:
    break;
  }

  if (all_loaded && !switch_once)
  {
    if (data::today == schedule.real_time().to_struct())
    {
      //If, based on todays date, hour price is within the threshold
      if (schedule.get_todays_threshold() >= data::today_prices[schedule.real_time().get_hour()].cents[data::region])
      {
        //Send ON
        digitalWrite(GPIO_NUM_5, 1);
        delay(100);
        digitalWrite(GPIO_NUM_5, 0);
      }
      else
      {
        //Send OFF
        digitalWrite(GPIO_NUM_8, 1);
        delay(100);
        digitalWrite(GPIO_NUM_8, 0);
      }
    }
    else if (data::tomorrow == schedule.real_time().to_struct())
    {
      if (schedule.get_todays_threshold() >= data::tomorrow_prices[schedule.real_time().get_hour()].cents[data::region])
      {
        //Send ON
        digitalWrite(GPIO_NUM_5, 1);
        delay(100);
        digitalWrite(GPIO_NUM_5, 0);
      }
      else
      {
        // Send OFF
        digitalWrite(GPIO_NUM_8, 1);
        delay(100);
        digitalWrite(GPIO_NUM_8, 0);
      }
    }
    switch_once = true;
  }

  if (WiFi.status() == WL_CONNECTED && !all_loaded)
  {
    settings setting;
    HTTPClient http;
    byte time_counter = 0;
    bool failed = false;
    p_init.add_element(element("GET TIME", &Font24, false), "", "", "");
    gp.draw_page(p_init);
    while (!schedule.request_time(&http))
    {
      if (time_counter > 2)
      {
        time_success = false;
        break;
      }
      else
      {
        time_counter++;
      }
    }
    if (setting.has_data())
    {
      setting.load_dates();
    }
    _debugging::push_error_message("Data d1, " + (String)data::today.year + "/" + (String)data::today.month + "/" + (String)data::today.day);
    _debugging::push_error_message("Data d2, " + (String)data::tomorrow.year + "/" + (String)data::tomorrow.month + "/" + (String)data::tomorrow.day);
    if (!failed)
    {
      /*
      Check if the saved hour prices, in EEPROM/Preferences, are todays date.
      And if any data is saved(First boot?)
      */
      if (!setting.has_data() || schedule.real_time().to_struct() > data::tomorrow)
      {
        schedule.create_daily_min_max(&http);
      }
      else
      {
        //Load all 48 hour price data points(two days)
        setting.load_hour_data();
        //The min and max prices of 48 hours(two days)
        setting.load_min_max();
        //The preferences can be abit wobbly, so a verification needs to happen
        //If verification failed, we retrieve the data from the server again.
        if(!data::verify_price_data()){
          _debugging::push_error_message("Could not verify hour price data");
          schedule.create_daily_min_max(&http);
        }else if(!data::verify_price_min_max()){
          _debugging::push_error_message("Could not verify min/max price data");
          schedule.create_daily_min_max(&http);          
        }
      }
      p_handler._buffer[0]->add_element(element(data::region_to_string(data::region), &Font24, false), "", "", "region");
      p_handler._buffer[0]->add_element(element(String(schedule.get_todays_threshold()), &Font24, false), "", "", "threshold");
      p_handler._buffer[0]->add_element(element("CURRENT", &Font24, false), "", "", "");
      if (data::today == schedule.real_time().to_struct())
      {
        p_handler._buffer[0]->add_element(element(String(data::today_prices[schedule.real_time().get_hour()].cents[data::region]), &Font24, false), "", "", "hour_price");
      }
      else if (data::tomorrow == schedule.real_time().to_struct())
      {
        p_handler._buffer[0]->add_element(element(String(data::tomorrow_prices[schedule.real_time().get_hour()].cents[data::region]), &Font24, false), "", "", "hour_price");
      }
      p_handler.update_graphics();
      all_loaded = true;
      data_loaded = true;
    }
    //20sec after loading data we will go into deepsleep
    sleep_timer = millis();
  }
  delay(2);
}