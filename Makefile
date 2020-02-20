# MODE = -DNDEBUG #per compilare in modalita' release

 MODE =  # per compilare in modalita' debug

sparse.exe: main.o SparseMatrix.o
	g++ $(MODE) -std=c++0x -o sparse.exe main.o

main.o: main.cpp SparseMatrix.h
	g++ $(MODE) -std=c++0x -c  main.cpp -o main.o

SparseMatrix.o: SparseMatrix.h
	g++ $(MODE) -std=c++0x -c SparseMatrix.h -o SparseMatrix.o 

.PHONY: clean

clean:
	rm *.exe *.o
