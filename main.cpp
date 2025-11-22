#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

const int room_count_min = 40;
const int room_count_max = 300;
const int price_min = 80;
const int price_max = 100;

// Varmistaa että huoneiden määrä on parillinen.
inline int make_even(int n) { return n - n % 2; }

// Varmistaa että stringin kaikki merkit ovat numeroita. Käytetään virheellisen
// käyttäjän antaman sisääntulon tunnistamiseen.
bool is_numeric(const std::string &s) {
  // 1. Tarkista ettei string ole tyhjä
  // 2. Tarkista ovatko kaikki merkit numeroita
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

class Room {
private:
  int price_per_night;
  bool booked;
  int capacity;
  int room_number;
  std::vector<int> sale_percentages = {0, 10, 20};

public:
  Room(int room_number, int capacity, std::mt19937 &gen)
      : room_number(room_number), capacity(capacity) {

    price_per_night = capacity == 1 ? 100 : 150;

    // Valitse alennusprosentti
    int selected_percentage;
    std::sample(sale_percentages.begin(), sale_percentages.end(),
                &selected_percentage, 1, gen);

    // Lasketaan lopullinen alennettu hinta
    price_per_night = (price_per_night * (100 - selected_percentage)) / 100;

    booked = false;
  }

  void print_room_info() const {
    std::cout << "Numero: " << room_number << ", hinta: " << price_per_night
              << ", varattu: " << (booked ? "Kyllä" : "Ei")
              << ", kapasiteetti: " << capacity << std::endl;
  }

  bool is_booked() const { return booked; }
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

  std::vector<Room> get_free_rooms() {
    std::vector<Room> free_rooms;

    for (const auto &room : rooms) {
      if (!room.is_booked())
        free_rooms.push_back(room);
    }

    return free_rooms;
  }
};

class IOHandler {
private:
  Hotel &hotel;

public:
  IOHandler(Hotel &hotel) : hotel(hotel) {}

  void print_intro() {
    std::cout << "Huoneita saatavilla: " << hotel.get_free_rooms().size()
              << "\nHuoneita varattu: "
              << hotel.get_room_count() - hotel.get_free_rooms().size() << "\n"
              << std::endl;
  }

  int read_reservation_capacity() {
    std::string input;

    // Otetaan ensin haluttu huonekoko
    std::cout << "Kuinka monen henkilön huoneen haluat?\n"
              << "Anna \"1\" tai \"2\": ";

    while (true) {
      getline(std::cin, input);

      if (input == "1" || input == "2")
        return std::stoi(input);

      std::cout << "Virheellinen huonekoko. Anna joko \"1\" tai \"2\""
                << std::endl;
    }
  }

  int read_reservation_nights() {
    std::string input;

    // Otetaan ensin haluttu huonekoko
    std::cout << "Kuinka moneksi yöksi haluat varata huoneen?\n"
              << "Anna mikä tahansa numero: ";

    while (true) {
      getline(std::cin, input);

      if (is_numeric(input))
        return std::stoi(input);

      std::cout << "Virheellinen yö määrä. Anna numero!" << std::endl;
    }
  }
};

int main() {
  // Määritellään muuttujia randomisointia varten
  std::random_device rd;
  std::mt19937 gen(rd());

  // Luodaan hotelli ja kaikki sen huoneet
  Hotel hotel(gen);
  IOHandler io(hotel);

  std::cout << "Tervetuloa hotellijärjestelmään." << std::endl;
  bool running = true;
  while (running) {
    io.print_intro();
    io.read_reservation_capacity();
    io.read_reservation_nights();

    // Ajon pausettamiseksi testauksen aikana POISTA!!!
    std::cin >> running;
  }

  return 0;
}
