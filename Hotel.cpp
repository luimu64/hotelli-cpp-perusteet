#include "Hotel.h"
#include "Common.h"
#include "Room.h"

Hotel::Hotel(std::mt19937 &gen) : rooms(create_rooms(gen)) {}

std::vector<Room> Hotel::create_rooms(std::mt19937 &gen) {
  std::vector<Room> rooms;

  std::uniform_int_distribution<> room_count_dist(room_count_min,
                                                  room_count_max);
  int room_count = make_even(room_count_dist(gen));
  int halfway = room_count / 2;

  for (int i = 0; i < room_count; ++i) {
    rooms.emplace_back(i, i < halfway ? 1 : 2, gen);
  }

  return rooms;
}

void Hotel::print_rooms_info() const {
  for (const auto &room : rooms) {
    room.print_room_info();
  }

  std::cout << "Huoneiden määrä: " << rooms.size() << std::endl;
}

// Vaihtaa vapaan varatuksi ja toisinpäin
void Hotel::change_booking_status(int number) {
  rooms[number].set_booked(!rooms[number].is_booked());
};

void Hotel::set_rooms(const std::vector<Room> &loaded_rooms) {
  rooms = loaded_rooms;
}

int Hotel::get_room_count() const { return rooms.size(); }

const std::vector<Room> &Hotel::get_all_rooms() const { return rooms; }

std::vector<Room> Hotel::get_free_rooms() {
  std::vector<Room> free_rooms;

  for (const auto &room : rooms) {
    if (!room.is_booked())
      free_rooms.push_back(room);
  }

  return free_rooms;
}

std::vector<Room> Hotel::get_free_rooms_by_size(int size) {
  std::vector<Room> free_rooms;

  for (const auto &room : rooms) {
    if (!room.is_booked() && room.get_capacity() == size)
      free_rooms.push_back(room);
  }

  return free_rooms;
}

// Hakee halvimman huoneen numeron halutulla koolla
Room Hotel::get_best_room(int capacity) {
  std::vector<Room> temp_rooms = get_free_rooms_by_size(capacity);
  std::sort(temp_rooms.begin(), temp_rooms.end(),
            [](const Room &a, const Room &b) {
              // Palauta true, jos a tulee ennen b:tä (pienempi hinta ensin)
              return a.get_price_per_night() < b.get_price_per_night();
            });

  return temp_rooms[0];
}
