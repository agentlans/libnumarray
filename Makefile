.PHONY : all clean

all : libnumarray.so libnumarray.a

libnumarray.so : numarray.o
	$(CXX) -shared -o libnumarray.so numarray.o

libnumarray.a : numarray.o
	ar rcs libnumarray.a numarray.o

numarray.o : numarray.cpp numarray.h Arrays.hpp IO.hpp
	$(CXX) -c -fPIC -O3 -o numarray.o numarray.cpp

numarray.h numarray.cpp : Templates/numarray.h Templates/numarray.cpp
	cp Templates/numarray.h Templates/numarray.cpp .

Templates/numarray.h Templates/numarray.cpp :
	make -C Templates/

clean :
	rm -f numarray.* libnumarray.so
