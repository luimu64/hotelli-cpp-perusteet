#ifndef ROOM_H
#define ROOM_H

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

class Room {
private:
  int price_per_night;
  bool booked;
  int capacity;
  int room_number;

public:
  // Konstruktori uuden luomiseen (arpoo hinnan)
  Room(int room_number, int capacity, std::mt19937 &gen);

  // Konstruktori tiedostosta lataamiseen (ei arvo hintaa)
  Room(int room_number, int capacity, int price, bool booked);

  void print_room_info() const;
  bool is_booked() const;
  void set_booked(bool new_status);
  int get_capacity() const;
  int get_price_per_night() const;
  int get_room_number() const;
};

#endif
