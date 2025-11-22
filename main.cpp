#include <algorithm>
#include <iostream>
#include <random>
#include <string>
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

struct Reservation {
  std::string customer_name;
  int room_number;
  int nights;
  int total_price;
};

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
  int get_capacity() const { return capacity; }
  int get_price_per_night() const { return price_per_night; }
  int get_room_number() const { return room_number; }
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

  std::vector<Room> get_free_rooms_by_size(int size) {
    std::vector<Room> free_rooms;

    for (const auto &room : rooms) {
      if (!room.is_booked() && room.get_capacity() == size)
        free_rooms.push_back(room);
    }

    return free_rooms;
  }

  // Hakee halvimman huoneen numeron halutulla koolla
  Room get_best_room(int capacity) {
    std::vector<Room> temp_rooms = rooms;
    std::sort(temp_rooms.begin(), temp_rooms.end(),
              [](const Room &a, const Room &b) {
                // Palauta true, jos a tulee ennen b:tä (pienempi hinta ensin)
                return a.get_price_per_night() < b.get_price_per_night();
              });

    return temp_rooms[0];
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

  bool confirm_pricing(int price) {
    std::cout << "Halvin huone haluamallasi koolla maksaa" << price
              << " euroa. " << "Vahvistetaanko varaus? \"K\"/\"E\""
              << std::endl;

    std::string input;

    getline(std::cin, input);

    while (true) {
      if (input == "k" || input == "K") {
        std::cout << "Kiitos varauksesta!" << std::endl;
        return true;
      } else if (input == "e" || input == "E") {
        std::cout << "Varaus peruutettu." << std::endl;
        return false;
      } else {
        std::cout << "Vastaa \"K\" tai \"E\"" << std::endl;
      }
    }
  }

  Reservation read_reservation() {
    /*
    struct Reservation {
      std::string customer_name;
      int room_number;
      int nights;
      int total_price;
    };
    */
    std::string input;
    Reservation new_reservation;
    int requested_capacity;

    // Varauksen huoneen koon ottaminen
    std::cout << "Kuinka monen henkilön huoneen haluat?\n"
              << "Anna \"1\" tai \"2\": ";

    while (true) {
      getline(std::cin, input);

      if (input == "1" || input == "2") {
        int capacity = std::stoi(input);

        // Tarkistetaan onko halutun kokoisia huoneita saatavilla
        if (hotel.get_free_rooms_by_size(capacity).size() != 0) {
          requested_capacity = capacity;
        } else {
          std::cout << "Haluamaasi huonekokoa ei ole valitettavasti saatavilla "
                       "tällä hetkellä."
                    << std::endl;
        }
      } else {
        std::cout << "Virheellinen huonekoko. Anna joko \"1\" tai \"2\""
                  << std::endl;
      }
    }

    // Varauksen yö määrän ottaminen
    std::cout << "Kuinka moneksi yöksi haluat varata huoneen?\n"
              << "Anna mikä tahansa numero: ";

    while (true) {
      getline(std::cin, input);

      if (is_numeric(input)) {
        new_reservation.nights = std::stoi(input);
      } else {
        std::cout << "Virheellinen yö määrä. Anna numero!" << std::endl;
      }
    }

    // Varauksen nimen ottaminen
    std::cout << "Anna nimi jolla varaus tehdään: " << std::endl;

    while (true) {
      getline(std::cin, input);
      new_reservation.customer_name = input;
    }

    // Hakee hotellista halvimman oikean kokoisen huoneen
    Room best_room = hotel.get_best_room(requested_capacity);

    // Hakee huoneen numeron
    new_reservation.room_number = best_room.get_room_number();

    // Laskee kokonaishinnan varaukselle
    new_reservation.total_price =
        best_room.get_price_per_night() * new_reservation.nights;

    return new_reservation;
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

    // Otetaan käyttäjältä tiedot varausta varten
    Reservation reservation = io.read_reservation();

    // Skippaa loppuloopin ja aloittaa varauksen teon uudestaan jos käyttäjä ei
    // hyväksy hintaa
    if (!io.confirm_pricing(reservation.total_price))
      continue;
  }

  return 0;
}
