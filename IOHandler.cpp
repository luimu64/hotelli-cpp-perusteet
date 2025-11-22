#include "IOHandler.h"
#include "Hotel.h"

// Kostruktori tallentaa hotelli-instanssin käyttöä varten
IOHandler::IOHandler(Hotel &hotel) : hotel(hotel) {}

// Tallentaa hotellin tilan tiedostoon sulkiessa ohjelman
void IOHandler::save_hotel_state() {
  std::ofstream file(DATA_FILE);
  if (file.is_open()) {
    const auto &rooms = hotel.get_all_rooms();
    // Formaatti: numero kapasiteetti hinta varattu(0/1)
    for (const auto &room : rooms) {
      file << room.get_room_number() << " " << room.get_capacity() << " "
           << room.get_price_per_night() << " " << room.is_booked() << "\n";
    }
    file.close();
    std::cout << "Tiedot tallennettu tiedostoon: " << DATA_FILE << std::endl;
  } else {
    std::cerr << "Virhe tallennettaessa tiedostoa!" << std::endl;
  }
}

// Lataa hotellin tilan tiedostosta avatessa ohjelman
bool IOHandler::load_hotel_state() {
  std::ifstream file(DATA_FILE);
  if (!file.is_open()) {
    return false; // Tiedostoa ei ole, käytetään satunnaisia huoneita
  }

  std::vector<Room> loaded_rooms;
  int num, cap, price;
  bool booked;

  // Käydään tiedosto läpi ja tallennetaan tiedot muuttujiin/vektoriin
  while (file >> num >> cap >> price >> booked) {
    loaded_rooms.emplace_back(num, cap, price, booked);
  }
  file.close();

  if (!loaded_rooms.empty()) {
    // Korvataan alussa generoidut huoneet
    hotel.set_rooms(loaded_rooms);
    std::cout << "Tiedot ladattu onnistuneesti tiedostosta." << std::endl;
    return true;
  }

  return false;
}

// Ajetaan aina ohjelman alussa ja varausten välillä.
// Printtaa valinnat ja palauta käyttäjän valinta
Menu_choice IOHandler::get_main_loop_choice() {
  std::cout
      << "Huoneita saatavilla: " << hotel.get_free_rooms().size()
      << "\nHuoneita varattu: "
      << hotel.get_room_count() - hotel.get_free_rooms().size()
      << "\n\nUusi varaus: U | Listaa huoneet: L | Hae varaus: H | Sulje: X"
      << std::endl;

  std::string input;

  while (true) {
    getline(std::cin, input);
    print_newlines();
    // Käydään käyttäjän syöte läpi
    if (input == "u" || input == "U") {
      return Menu_choice::NEW;
    } else if (input == "l" || input == "L") {
      return Menu_choice::LIST;
    } else if (input == "h" || input == "H") {
      return Menu_choice::SEARCH;
    } else if (input == "x" || input == "X") {
      return Menu_choice::QUIT;
    } else {
      std::cout << "\n\nVastaa U(uusi) tai L(istaa)" << std::endl;
    }
  }
}

// Varmistetaan käyttäjältä haluaako hän tehdä varauksen
bool IOHandler::confirm_pricing(int price) {
  std::cout << "Halvin huone haluamallasi koolla maksaa " << price << " euroa. "
            << "\nVahvistetaanko varaus? K/E" << std::endl;

  std::string input;

  while (true) {
    getline(std::cin, input);
    if (input == "k" || input == "K") {
      std::cout << "\n\nKiitos varauksesta!\n\n" << std::endl;
      return true;
    } else if (input == "e" || input == "E") {
      std::cout << "\n\nVaraus peruutettu.\n\n" << std::endl;
      return false;
    } else {
      std::cout << "\n\nVastaa K(yllä) tai E(i)" << std::endl;
    }
  }
}

// Muodostaa varauksen käyttäjän syötteen perusteella
Reservation IOHandler::read_reservation() {
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
  while (true) {
    std::cout << "Kuinka monen henkilön huoneen haluat?\n"
              << "Anna \"1\" tai \"2\": ";
    getline(std::cin, input);

    if (input == "1" || input == "2") {
      int capacity = std::stoi(input);

      // Tarkistetaan onko halutun kokoisia huoneita saatavilla
      if (hotel.get_free_rooms_by_size(capacity).size() != 0) {
        requested_capacity = capacity;
        break;
      } else {
        std::cout << "\nHaluamaasi huonekokoa ei ole valitettavasti saatavilla "
                     "tällä hetkellä."
                  << std::endl;
      }
    } else {
      std::cout << "\nVirheellinen huonekoko.";
    }
  }

  print_newlines();

  // Varauksen yömäärän ottaminen
  while (true) {
    std::cout << "Kuinka moneksi yöksi haluat varata huoneen?\n"
              << "Anna mikä tahansa numero: ";
    getline(std::cin, input);

    if (is_numeric(input)) {
      new_reservation.nights = std::stoi(input);
      break;
    } else {
      std::cout << "\nVirheellinen yö määrä." << std::endl;
    }
  }

  print_newlines();

  // Varauksen nimen ottaminen
  std::cout << "Anna nimi jolla varaus tehdään: ";

  while (true) {
    getline(std::cin, input);
    if (input != "") {
      new_reservation.customer_name = input;
      break;
    } else {
      std::cout << "\nNimi ei voi olla tyhjä!" << std::endl;
    }
  }

  print_newlines();

  // Hakee hotellista halvimman oikean kokoisen huoneen
  Room best_room = hotel.get_best_room(requested_capacity);

  // Hakee huoneen numeron
  new_reservation.room_number = best_room.get_room_number();

  // Laskee kokonaishinnan varaukselle
  new_reservation.total_price =
      best_room.get_price_per_night() * new_reservation.nights;

  return new_reservation;
}
