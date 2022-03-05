define(`bodypart',
`define(`TTYPE', $1)dnl
define(`VECTORTYPE', $2`v')dnl
define(`MATRIXTYPE', $2`m')dnl
define(`COMPLEXPART', $3)dnl
include(`VectorMatrix.cpp')dnl
ifelse(TTYPE, COMPLEXPART, `include(`RealGetterSetter.cpp')',
`include(`ComplexGetterSetter.cpp')')dnl
dnl
define(`BINARYTAG',`')dnl
define(`ISBINARY',`')dnl
include(`FileIO.cpp')
define(`BINARYTAG',`_binary')dnl
define(`ISBINARY',`Binary')dnl
include(`FileIO.cpp')
')dnl

#include <complex>
#include "numarray.h"
#include "Conversion.hpp"

bodypart(`int', `i', `int')
bodypart(`long', `l', `long')
bodypart(`float', `s', `float')
bodypart(`double', `d', `double')
bodypart(`std::complex<float>', `c', `float')
bodypart(`std::complex<double>', `z', `double')
bodypart(`char', `x', `char')
bodypart(`void*', `p', `void*')

// Need this to delete strings outputted from this library
void nar_string_free(char* s) {
    delete[] s;
}
