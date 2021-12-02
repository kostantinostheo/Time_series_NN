CC=g++
CFLAGS= -std=c++11
OBJS = Methods.o Hashing.o VectorData.o Tools.o Euclidean.o 
OBJS2 = Kmeans.o Methods.o VectorData.o Euclidean.o Hashing.o Tools.o

# Executables
lsh: lsh.o Methods.o Hashing.o VectorData.o Tools.o Euclidean.o
	$(CC) -o lsh lsh.o $(OBJS) -O2

cube: cube.o Methods.o Hashing.o VectorData.o Tools.o Euclidean.o
	$(CC) -o cube cube.o $(OBJS) -O2

cluster: cluster.o Kmeans.o Methods.o VectorData.o Euclidean.o Hashing.o Tools.o
	$(CC) -o cluster cluster.o $(OBJS2) -O2

# Object files
lsh.o: src/lsh.cpp
	$(CC) $(CFLAGS) -c src/lsh.cpp

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

.PHONY: clean

clean:
	rm -f lsh cube cluster *.o 
