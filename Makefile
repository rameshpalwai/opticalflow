CXX = g++
CXXFLAGS = -std=c++11 -O3 -march=native
INCLUDES = -I/scratch-local/usr/include/
LIBDIR = -L/scratch-local/usr/lib/
LIBS = -lopencv_highgui -lopencv_core -lopencv_imgproc 

all: main census_transform.o

census_transform.o: census_transform.h census_transform.cpp
	$(CXX) -std=c++11 $(INCLUDES) $(LIBDIR) $(LIBS) -c census_transform.cpp -o census_transform.o 

main: main.cpp census_transform.o
	$(CXX) -std=c++11 $(INCLUDES) $(LIBDIR) $(LIBS) main.cpp census_transform.o -o opticalflow

clean:
	rm opticalflow
	rm census_transform.o
