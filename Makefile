CC = gcc
CXX = g++
LIBS = -lm
CFLAGS = -Wall -Wextra -std=c99 -I.
CXXFLAGS = -Wall -Wextra -std=c++11 -I.

C_DIR = Comparison_C
CPP_DIR = Comparison_CPP

all: c_app cpp_app cross_test

cpp_app: $(CPP_DIR)/Comparison.o $(CPP_DIR)/Comparison_test.o
	$(CXX) $(CXXFLAGS) -o cpp_app $(CPP_DIR)/Comparison.o $(CPP_DIR)/Comparison_test.o

c_app: $(C_DIR)/Comparison.o $(C_DIR)/Comparison_test.o
	$(CC) $(CFLAGS) -o c_app $(C_DIR)/Comparison.o $(C_DIR)/Comparison_test.o $(LIBS)

cross_test: Comparison_cross_test.o $(CPP_DIR)/Comparison.o $(C_DIR)/Comparison.o
	$(CXX) $(CXXFLAGS) -o cross_test Comparison_cross_test.o $(CPP_DIR)/Comparison.o $(C_DIR)/Comparison.o $(LIBS)


$(CPP_DIR)/Comparison.o: $(CPP_DIR)/Comparison.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CPP_DIR)/Comparison_test.o: $(CPP_DIR)/Comparison_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(C_DIR)/Comparison.o: $(C_DIR)/Comparison.c
	$(CC) $(CFLAGS) -c $< -o $@

$(C_DIR)/Comparison_test.o: $(C_DIR)/Comparison_test.c
	$(CC) $(CFLAGS) -c $< -o $@


Comparison_cross_test.o: Comparison_cross_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f cpp_app c_app cross_test
	rm -f $(CPP_DIR)/*.o $(C_DIR)/*.o *.o
	rm -f *.dat *.txt

run: all
	./cross_test
