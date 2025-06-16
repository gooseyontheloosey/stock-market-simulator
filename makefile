# Specify the C++ compiler
CXX     = g++

# Specify options to pass to the compiler. Here it sets the optimisation
# level, outputs debugging info for gdb, and C++ version to use.
CXXFLAGS = -O0 -g3 -std=c++17

All: all
all: main

main: main.cpp CustomComparator.o LimitOrder.o MarketOrder.o Order.o
	$(CXX) $(CXXFLAGS) main.cpp CustomComparator.o LimitOrder.o MarketOrder.o Order.o -o main

CustomComparator.o: CustomComparator.cpp CustomComparator.h
	$(CXX) $(CXXFLAGS) -c CustomComparator.cpp -o CustomComparator.o

LimitOrder.o: LimitOrder.cpp LimitOrder.h
	$(CXX) $(CXXFLAGS) -c LimitOrder.cpp -o LimitOrder.o

MarketOrder.o: MarketOrder.cpp MarketOrder.h
	$(CXX) $(CXXFLAGS) -c MarketOrder.cpp -o MarketOrder.o

Order.o: Order.cpp Order.h
	$(CXX) $(CXXFLAGS) -c Order.cpp -o Order.o

# Some cleanup functions, invoked by typing "make clean" or "make deepclean"
deepclean:
#	del /F /Q *~ *.o main main.exe *.stackdump
	rm -f *~ *.o main main.exe *.stackdump

clean:
#	del /F /Q *~ *.o *.stackdump
	rm -f *~ *.o *.stackdump