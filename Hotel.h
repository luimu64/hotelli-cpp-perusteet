#ifndef HOTEL_H
#define HOTEL_H

#include "Common.h"
#include "Room.h"
#include <algorithm>
#include <random>
#include <vector>

class Hotel {
private:
  std::vector<Room> rooms;
  std::vector<Room> create_rooms(std::mt19937 &gen);

public:
  Hotel(std::mt19937 &gen);

  const std::vector<Room> &get_all_rooms() const;
  void set_rooms(const std::vector<Room> &loaded_rooms);

  void print_rooms_info() const;
  void print_reservation_info(std::string name) const;
  int get_room_count() const;
  std::vector<Room> get_free_rooms();
  std::vector<Room> get_free_rooms_by_size(int size);
  Room get_best_room(int capacity);
  void activate_reservation(Reservation reservation);
};

#endif
