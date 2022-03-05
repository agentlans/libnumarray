#ifdef __cplusplus
#include "Arrays.hpp"
#include "IO.hpp"

extern "C" {
#endif

define(`cpp_decl', 
`define(`TTYPE', $1)dnl
define(`VECTORTYPE', $2`v')dnl
define(`MATRIXTYPE', $2`m')dnl
typedef Vector<`'TTYPE`'>* nar_`'VECTORTYPE`';
typedef Matrix<`'TTYPE`'>* nar_`'MATRIXTYPE`';')dnl
define(`c_decl', 
`define(`VECTORTYPE', $1`v')dnl
define(`MATRIXTYPE', $1`m')dnl
typedef void* nar_`'VECTORTYPE`';
typedef void* nar_`'MATRIXTYPE`';')dnl

#ifdef __cplusplus
// C++ type definitions
cpp_decl(`int', `i')
cpp_decl(`long', `l')
cpp_decl(`float', `s')
cpp_decl(`double', `d')
cpp_decl(`std::complex<float>', `c')
cpp_decl(`std::complex<double>', `z')
cpp_decl(`char', `x')
cpp_decl(`void*', `p')
#else
// C type definitions
c_decl(`i')
c_decl(`l')
c_decl(`s')
c_decl(`d')
c_decl(`c')
c_decl(`z')
c_decl(`x')
c_decl(`p')
#endif

enum nar_order {
    nar_row_major = 0,
    nar_col_major = 1,
    nar_unknown = 2
};

/* Insert prototypes here */
include(`numarray_part.h')

#ifdef __cplusplus
}
#endif
