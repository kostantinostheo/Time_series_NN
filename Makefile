CC=g++
CFLAGS= -std=c++11
OBJS = Methods.o Hashing.o VectorData.o Tools.o Euclidean.o Frechet.o Curves.o
OBJS2 = Kmeans.o Methods.o VectorData.o Euclidean.o Hashing.o Tools.o

# Executables
search: search.o Methods.o Hashing.o VectorData.o Tools.o Euclidean.o Frechet.o Curves.o 
	$(CC) -o search search.o $(OBJS) -O2

test_unit: test_unit.o Methods.o Hashing.o VectorData.o Tools.o Euclidean.o Frechet.o Curves.o 
	$(CC) -o test_unit test_unit.o $(OBJS)

cluster: cluster.o Kmeans.o Methods.o VectorData.o Euclidean.o Hashing.o Tools.o
	$(CC) -o cluster cluster.o $(OBJS2) -O2

# Object files
search.o: src/search.cpp
	$(CC) $(CFLAGS) -c src/search.cpp

test_unit.o: tests/test_unit.cpp
	$(CC) $(CFLAGS) -c tests/test_unit.cpp

cube.o: src/cube.cpp
	$(CC) $(CFLAGS) -c src/cube.cpp

cluster.o: src/cluster.cpp
	$(CC) $(CFLAGS) -c src/cluster.cpp

Methods.o: src/Common/Methods.cpp
	$(CC) $(CFLAGS) -c src/Common/Methods.cpp
	
Hashing.o: src/Common/Hashing.cpp
	$(CC) $(CFLAGS) -c src/Common/Hashing.cpp

VectorData.o: src/Common/VectorData.cpp
	$(CC) $(CFLAGS) -c src/Common/VectorData.cpp

Tools.o: src/Common/Tools.cpp
	$(CC) $(CFLAGS) -c src/Common/Tools.cpp
	
Euclidean.o: src/Common/Euclidean.cpp
	$(CC) $(CFLAGS) -c src/Common/Euclidean.cpp

Kmeans.o: src/Cluster/Kmeans.cpp
	$(CC) $(CFLAGS) -c src/Cluster/Kmeans.cpp

Curves.o: src/Common/Curves.cpp
	$(CC) $(CFLAGS) -c src/Common/Curves.cpp

Frechet.o: src/Common/Frechet.cpp
	$(CC) $(CFLAGS) -c src/Common/Frechet.cpp

.PHONY: clean

clean:
	rm -f test_unit search cluster *.o 
