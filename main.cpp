#include "Common.h"
#include "Hotel.h"
#include "IOHandler.h"
#include <iostream>
#include <random>

int main() {
  // Määritellään muuttujia randomisointia varten
  std::random_device rd;
  std::mt19937 gen(rd());

  // Luodaan hotelli ja kaikki sen huoneet
  Hotel hotel(gen);
  IOHandler io(hotel);

  std::cout << "Tervetuloa hotellijärjestelmään." << std::endl;

  io.load_hotel_state();

  bool running = true;
  while (running) {
    Menu_choice choice = io.get_main_loop_choice();

    if (choice == Menu_choice::QUIT) {
      std::cout << "Näkemiin!" << std::endl;
      io.save_hotel_state();
      break;
    } else if (choice == Menu_choice::LIST) {
      hotel.print_rooms_info();
    } else if (choice == Menu_choice::NEW) {
      // Otetaan käyttäjältä tiedot varausta varten
      Reservation reservation = io.read_reservation();

      // Skippaa loppuloopin ja aloittaa varauksen teon uudestaan jos käyttäjä
      // ei hyväksy hintaa
      if (!io.confirm_pricing(reservation.total_price))
        continue;

      // Varataan huone
      hotel.change_booking_status(reservation.room_number);
    }
  }

  return 0;
}
