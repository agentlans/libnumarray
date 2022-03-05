MAKEHEADERS="/home/aklt/Downloads/makeheaders"

.PHONY : all clean
all : numarray.cpp numarray.h

numarray.cpp : Body.m4 VectorMatrix.cpp RealGetterSetter.cpp ComplexGetterSetter.cpp
	m4 Body.m4 > numarray.cpp

numarray_part.h : numarray.cpp
	$(MAKEHEADERS) numarray.cpp:numarray_part.h

numarray.h : numarray_part.h
	m4 Header.m4 > numarray.h

clean :
	rm -f numarray.*
