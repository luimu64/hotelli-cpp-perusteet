cmake_minimum_required(VERSION 3.10)

# Projektin nimi
project(HotelSystem VERSION 1.0)

# Vaaditaan std::sample -funktiolle
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Lisätään varoitukset päälle
if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Määritellään lähdetiedostot
set(SOURCES
    main.cpp
    Room.cpp
    Hotel.cpp
    IOHandler.cpp
)

# Määritellään otsikkotiedostot
set(HEADERS
    Room.h
    Hotel.h
    IOHandler.h
    Common.h
)

add_executable(HotelSystem ${SOURCES} ${HEADERS})
