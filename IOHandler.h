#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "Common.h"
#include "Hotel.h"
#include <fstream>
#include <iostream>
#include <string>

class IOHandler {
private:
  Hotel &hotel;

public:
  IOHandler(Hotel &hotel);

  void save_hotel_state();
  bool load_hotel_state();
  Menu_choice get_main_loop_choice();
  bool confirm_pricing(int price);
  Reservation read_reservation();
  void search_reservation();
};

#endif
