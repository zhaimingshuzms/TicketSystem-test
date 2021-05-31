cmake_minimum_required(VERSION 3.17)
project(TicketSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(TicketSystem main.cpp input_traits.hpp ticketsystem.hpp ./lib/vector.hpp lib/numas.hpp lib/time.hpp trainsystem.hpp lib/mystring.hpp lib/algorithm.hpp ticketinnersystem.hpp lib/fqueue.hpp lib/fakebpt.hpp)

add_compile_options(-Wall -Werror -Wstrict-prototypes -Wmissing-prototypes)