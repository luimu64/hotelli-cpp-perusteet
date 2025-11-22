#include <iostream>
#include <random>
#include <vector>

const int room_count_min = 40;
const int room_count_max = 300;
const int price_min = 80;
const int price_max = 100;

// Kiva avustusfunktio jota käytetään tasan kerran. Varmistaa että huoneiden
// määrä on parillinen.
inline int make_even(int n) { return n - n % 2; }

class Room {
private:
  int price_per_night;
  bool is_booked;
  int capacity;
  int room_number;

public:
  Room(int room_number, int capacity, std::mt19937 &gen)
      : room_number(room_number), capacity(capacity) {

    std::uniform_int_distribution<> price_dist(price_min, price_max);
    price_per_night = price_dist(gen);
    is_booked = false;
  }

  void print_room_info() const {
    std::cout << "Numero: " << room_number << ", hinta: " << price_per_night
              << ", varattu: " << (is_booked ? "Kyllä" : "Ei")
              << ", kapasiteetti: " << capacity << std::endl;
  }
};

class Hotel {
private:
  std::vector<Room> rooms;

  std::vector<Room> create_rooms(std::mt19937 &gen) {
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

public:
  Hotel(std::mt19937 &gen) : rooms(create_rooms(gen)) {}

  void print_rooms_info() const {
    for (const auto &room : rooms) {
      room.print_room_info();
    }

    std::cout << "Huoneiden määrä: " << rooms.size() << std::endl;
  }

  int get_room_count() const { return rooms.size(); }
};

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());

  Hotel hotel(gen);
  hotel.print_rooms_info();

  return 0;
}
