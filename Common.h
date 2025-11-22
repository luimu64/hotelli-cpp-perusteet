#ifndef COMMON_H
#define COMMON_H

#include <algorithm>
#include <iostream>
#include <string>

// Vakiot
const int room_count_min = 40;
const int room_count_max = 300;
const std::string DATA_FILE = "hotel_data.txt";

// Tietorakenteet
struct Reservation {
  std::string customer_name;
  int room_number;
  int nights;
  int total_price;
};

enum class Menu_choice { NEW, LIST, QUIT };

// Inline-funktiot (voidaan määritellä headerissa)
inline int make_even(int n) { return n - n % 2; }

inline bool is_numeric(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

inline void print_newlines() { std::cout << "\n" << std::endl; };

#endif
