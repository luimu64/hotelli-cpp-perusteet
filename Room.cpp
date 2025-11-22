#include "Room.h"

// Käytetään ensimmäisellä ajokerralla ja kun tiedostoa ei ole
Room::Room(int room_number, int capacity, std::mt19937 &gen)
    : capacity(capacity), room_number(room_number) {

  price_per_night = capacity == 1 ? 100 : 150;

  std::vector<int> percentages = {0, 10, 20};

  // Valitse alennusprosentti
  int selected_percentage;
  std::sample(percentages.begin(), percentages.end(), &selected_percentage, 1,
              gen);

  // Lasketaan lopullinen alennettu hinta
  price_per_night = (price_per_night * (100 - selected_percentage)) / 100;

  booked = false;
}

// Käytetään kun aikaisemmat tiedot on
Room::Room(int room_number, int capacity, int price, bool booked)
    : price_per_night(price), booked(booked), capacity(capacity),
      room_number(room_number) {}

void Room::print_room_info() const {
  std::cout << "Numero: " << room_number << ", hinta: " << price_per_night
            << ", varattu: " << (booked ? "Kyllä" : "Ei")
            << ", kapasiteetti: " << capacity << std::endl;
}

bool Room::is_booked() const { return booked; }
void Room::set_booked(bool new_status) { booked = new_status; };
int Room::get_capacity() const { return capacity; }
int Room::get_price_per_night() const { return price_per_night; }
int Room::get_room_number() const { return room_number; }
