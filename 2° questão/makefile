CPLEX_STUDIO_DIR = /home/diolinux/cplex221

CPLEX_INCLUDE = $(CPLEX_STUDIO_DIR)/cplex/include
CONCERT_INCLUDE = $(CPLEX_STUDIO_DIR)/concert/include
CPLEX_LIB = $(CPLEX_STUDIO_DIR)/cplex/lib/x86-64_linux/static_pic
CONCERT_LIB = $(CPLEX_STUDIO_DIR)/concert/lib/x86-64_linux/static_pic

FILE = mochila.cpp
TARGET = $(basename $(FILE))

CXX = g++
CXXFLAGS = -std=c++11 -O2 -I$(CPLEX_INCLUDE) -I$(CONCERT_INCLUDE)
LDFLAGS = -lilocplex -lcplex -lconcert -lm -lpthread -L$(CPLEX_LIB) -L$(CONCERT_LIB)

all: $(TARGET)

$(TARGET): $(FILE)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
