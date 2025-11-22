#include "Room.h"

// Käytetään ensimmäisellä ajokerralla ja kun tiedostoa ei ole
Room::Room(int room_number, int capacity, std::mt19937 &gen)
    : capacity(capacity), room_number(room_number), reserved_by("") {

  // Käytetään yhtä kahdesta hinnasta riippuen onko yhden vai kahden hengen
  // huone
  price_per_night = capacity == 1 ? 100 : 150;

  // Alennusprosenttien määritelmä
  std::vector<int> percentages = {0, 10, 20};

  // Valitaan alennusprosentti
  int selected_percentage;
  std::sample(percentages.begin(), percentages.end(), &selected_percentage, 1,
              gen);

  // Lasketaan lopullinen alennettu hinta
  price_per_night = (price_per_night * (100 - selected_percentage)) / 100;

  // Kaikki huoneet on oletuksena vapaita
  booked = false;
}

// Käytetään kun aikaisemmat tiedot on
Room::Room(int room_number, int capacity, int price, bool booked,
           std::string reserved_by)
    : price_per_night(price), booked(booked), capacity(capacity),
      room_number(room_number), reserved_by(reserved_by) {}

// Listaa kaikki hotellin huoneet
void Room::print_room_info() const {
  std::cout << "Numero: " << room_number << ", hinta: " << price_per_night
            << ", varattu: " << (booked ? "Kyllä" : "Ei")
            << ", kapasiteetti: " << capacity
            << (booked ? ", varannut: " + reserved_by : "") << std::endl;
}

// Itsestäänselviä settereitä ja gettereitä
bool Room::is_booked() const { return booked; }
void Room::set_booked(bool new_status) { booked = new_status; };
void Room::set_reserved_by(std::string new_reservant) {
  reserved_by = new_reservant;
}
int Room::get_capacity() const { return capacity; }
int Room::get_price_per_night() const { return price_per_night; }
int Room::get_room_number() const { return room_number; }
std::string Room::get_reserved_by() const { return reserved_by; }
