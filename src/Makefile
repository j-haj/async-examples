CC := g++-7
STD := -std=c++14
CFLAGS := -Wall -pthread

all: example1 example2 example3 example4

example1: example.cpp
	$(CC) $(STD) $(CFLAGS) example.cpp -o example1

example2: example2.cpp
	$(CC) $(STD) $(CFLAGS) example2.cpp -o example2

example3: example3.cpp
	$(CC) $(STD) $(CFLAGS) example3.cpp -o example3

example4: example4.cpp
	$(CC) $(STD) $(CFLAGS) example4.cpp -o example4

example5: example5.cpp
	$(CC) $(STD) $(CFLAGS) example5.cpp -o example5

example6: example6.cpp
	$(CC) $(STD) $(CFLAGS) example6.cpp -o example6

ompex: openmp_ex.cpp
	$(CC) $(STD) $(CFLAGS) -fopenmp openmp_ex.cpp -o omp_ex 


