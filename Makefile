CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
AR = ar
ARFLAGS = rcs

# Change these variables to match your project
LIBRARY_NAME = rndrboi
SOURCES = src/rndrboi.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(LIBRARY_NAME).a

$(LIBRARY_NAME).a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(LIBRARY_NAME).a