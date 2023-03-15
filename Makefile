CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++17 -Wno-sign-compare -Wno-comment
CXXFLAGS = --std=c++17 -Wall -Werror -pedantic -g -fsanitize=address -fsanitize=undefined

# Compile the main executable
main.exe: main.cpp inventory.cpp userOps.cpp md5.cpp customer.cpp
	$(CXX) $(CXXFLAGS) main.cpp inventory.cpp userOps.cpp md5.cpp customer.cpp -o main.exe -lsqlite3

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:
