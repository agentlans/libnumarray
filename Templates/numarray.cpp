
#include <complex>
#include "numarray.h"
#include "Conversion.hpp"

// Main routines for vectors and matrices of the given type

nar_iv nar_iv_alloc(int n) {
    return new Vector<int>(n);
}

void nar_iv_free(nar_iv v) {
    delete v;
}

nar_iv nar_iv_copy(nar_iv v) {
    return new Vector<int>(v->copy());
}

int nar_iv_valid(nar_iv v, int i) {
    return v->in_range(i);
}

nar_iv nar_iv_slice(nar_iv v, int start, int end) {
    return new Vector<int>(v->slice(start, end));
}

int nar_iv_slice_safe(nar_iv* out, nar_iv v, int start, int end) {
    try {
        *out = new Vector<int>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

int* nar_iv_pointer(nar_iv v) {
    return (int*) v->pointer();
}

int nar_iv_inc(nar_iv v) {
    return v->get_inc();
}

int nar_iv_n(nar_iv v) {
    return v->get_n();
}

int nar_iv_eq(nar_iv v1, nar_iv v2) {
    return (*v1) == (*v2);
}

int nar_iv_assign(nar_iv v1, nar_iv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_im nar_im_alloc(int rows, int cols, int row_major) {
    return new Matrix<int>(rows, cols, row_major);
}

void nar_im_free(nar_im mat) {
    delete mat;
}

nar_im nar_im_copy(nar_im mat) {
    return new Matrix<int>(mat->copy());
}

int nar_im_valid(nar_im mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_im_order(nar_im mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_iv nar_im_row(nar_im mat, int i) {
    return new Vector<int>(mat->row(i));
}

nar_iv nar_im_col(nar_im mat, int j) {
    return new Vector<int>(mat->col(j));
}

nar_im nar_im_subarray(nar_im mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<int>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_im_row_safe(nar_iv* x, nar_im mat, int i) {
    try {
        *x = new Vector<int>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_im_col_safe(nar_iv* x, nar_im mat, int j) {
    try {
        *x = new Vector<int>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_im_subarray_safe(nar_im* x, nar_im mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<int>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
int* nar_im_pointer(nar_im mat) {
    return (int*) mat->pointer();
}

int nar_im_get_ld(nar_im mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_im_get_rows(nar_im mat) {
    return mat->get_rows();
}

int nar_im_get_cols(nar_im mat) {
    return mat->get_cols();
}

int nar_im_eq(nar_im mat1, nar_im mat2) {
    return (*mat1) == (*mat2);
}

int nar_im_assign(nar_im mat1, nar_im mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_im_display_string(nar_im mat) {
    return string_to_char(display_string(*mat));
}

char* nar_iv_display_string(nar_iv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
int nar_iv_get(nar_iv v, int i) {
    return (*v)[i];
}

void nar_iv_set(nar_iv v, int i, int x) {
    (*v)[i] = x;
}

int nar_im_get(nar_im mat, int i, int j) {
    return (*mat)(i,j);
}

void nar_im_set(nar_im mat, int i, int j, int x) {
    (*mat)(i,j) = x;
}

// Bounds-checked versions
int nar_iv_get_safe(int* x, nar_iv v, int i) {
    try {
        *x = v->at(i);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_iv_set_safe(nar_iv v, int i, int x) {
    try {
        v->at(i) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_im_get_safe(int* x, nar_im mat, int i, int j) {
    try {
        *x = mat->at(i,j);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_im_set_safe(nar_im mat, int i, int j, int x) {
    try {
        mat->at(i,j) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_iv nar_iv_read(const char* filename, int* err) {
    try {
        File<int> f(filename, ios_base::in);
        nar_iv v = new Vector<int>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_iv_write(nar_iv v, const char* filename, const char* sep) {
    try {
        File<int> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_im nar_im_read(const char* filename, int row_major, int* err) {
    try {
        File<int> f(filename, ios_base::in);
        nar_im v = new Matrix<int>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_im_write(nar_im v, const char* filename, const char* sep) {
    try {
        File<int> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_iv nar_iv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<int> f(filename, ios_base::in);
        nar_iv v = new Vector<int>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_iv_write_binary(nar_iv v, const char* filename) {
    try {
        BinaryFile<int> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_im nar_im_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<int> f(filename, ios_base::in);
        nar_im v = new Matrix<int>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_im_write_binary(nar_im v, const char* filename) {
    try {
        BinaryFile<int> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Main routines for vectors and matrices of the given type

nar_lv nar_lv_alloc(int n) {
    return new Vector<long>(n);
}

void nar_lv_free(nar_lv v) {
    delete v;
}

nar_lv nar_lv_copy(nar_lv v) {
    return new Vector<long>(v->copy());
}

int nar_lv_valid(nar_lv v, int i) {
    return v->in_range(i);
}

nar_lv nar_lv_slice(nar_lv v, int start, int end) {
    return new Vector<long>(v->slice(start, end));
}

int nar_lv_slice_safe(nar_lv* out, nar_lv v, int start, int end) {
    try {
        *out = new Vector<long>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

long* nar_lv_pointer(nar_lv v) {
    return (long*) v->pointer();
}

int nar_lv_inc(nar_lv v) {
    return v->get_inc();
}

int nar_lv_n(nar_lv v) {
    return v->get_n();
}

int nar_lv_eq(nar_lv v1, nar_lv v2) {
    return (*v1) == (*v2);
}

int nar_lv_assign(nar_lv v1, nar_lv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_lm nar_lm_alloc(int rows, int cols, int row_major) {
    return new Matrix<long>(rows, cols, row_major);
}

void nar_lm_free(nar_lm mat) {
    delete mat;
}

nar_lm nar_lm_copy(nar_lm mat) {
    return new Matrix<long>(mat->copy());
}

int nar_lm_valid(nar_lm mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_lm_order(nar_lm mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_lv nar_lm_row(nar_lm mat, int i) {
    return new Vector<long>(mat->row(i));
}

nar_lv nar_lm_col(nar_lm mat, int j) {
    return new Vector<long>(mat->col(j));
}

nar_lm nar_lm_subarray(nar_lm mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<long>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_lm_row_safe(nar_lv* x, nar_lm mat, int i) {
    try {
        *x = new Vector<long>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_lm_col_safe(nar_lv* x, nar_lm mat, int j) {
    try {
        *x = new Vector<long>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_lm_subarray_safe(nar_lm* x, nar_lm mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<long>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
long* nar_lm_pointer(nar_lm mat) {
    return (long*) mat->pointer();
}

int nar_lm_get_ld(nar_lm mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_lm_get_rows(nar_lm mat) {
    return mat->get_rows();
}

int nar_lm_get_cols(nar_lm mat) {
    return mat->get_cols();
}

int nar_lm_eq(nar_lm mat1, nar_lm mat2) {
    return (*mat1) == (*mat2);
}

int nar_lm_assign(nar_lm mat1, nar_lm mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_lm_display_string(nar_lm mat) {
    return string_to_char(display_string(*mat));
}

char* nar_lv_display_string(nar_lv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
long nar_lv_get(nar_lv v, int i) {
    return (*v)[i];
}

void nar_lv_set(nar_lv v, int i, long x) {
    (*v)[i] = x;
}

long nar_lm_get(nar_lm mat, int i, int j) {
    return (*mat)(i,j);
}

void nar_lm_set(nar_lm mat, int i, int j, long x) {
    (*mat)(i,j) = x;
}

// Bounds-checked versions
int nar_lv_get_safe(long* x, nar_lv v, int i) {
    try {
        *x = v->at(i);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_lv_set_safe(nar_lv v, int i, long x) {
    try {
        v->at(i) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_lm_get_safe(long* x, nar_lm mat, int i, int j) {
    try {
        *x = mat->at(i,j);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_lm_set_safe(nar_lm mat, int i, int j, long x) {
    try {
        mat->at(i,j) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_lv nar_lv_read(const char* filename, int* err) {
    try {
        File<long> f(filename, ios_base::in);
        nar_lv v = new Vector<long>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_lv_write(nar_lv v, const char* filename, const char* sep) {
    try {
        File<long> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_lm nar_lm_read(const char* filename, int row_major, int* err) {
    try {
        File<long> f(filename, ios_base::in);
        nar_lm v = new Matrix<long>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_lm_write(nar_lm v, const char* filename, const char* sep) {
    try {
        File<long> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_lv nar_lv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<long> f(filename, ios_base::in);
        nar_lv v = new Vector<long>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_lv_write_binary(nar_lv v, const char* filename) {
    try {
        BinaryFile<long> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_lm nar_lm_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<long> f(filename, ios_base::in);
        nar_lm v = new Matrix<long>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_lm_write_binary(nar_lm v, const char* filename) {
    try {
        BinaryFile<long> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Main routines for vectors and matrices of the given type

nar_sv nar_sv_alloc(int n) {
    return new Vector<float>(n);
}

void nar_sv_free(nar_sv v) {
    delete v;
}

nar_sv nar_sv_copy(nar_sv v) {
    return new Vector<float>(v->copy());
}

int nar_sv_valid(nar_sv v, int i) {
    return v->in_range(i);
}

nar_sv nar_sv_slice(nar_sv v, int start, int end) {
    return new Vector<float>(v->slice(start, end));
}

int nar_sv_slice_safe(nar_sv* out, nar_sv v, int start, int end) {
    try {
        *out = new Vector<float>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

float* nar_sv_pointer(nar_sv v) {
    return (float*) v->pointer();
}

int nar_sv_inc(nar_sv v) {
    return v->get_inc();
}

int nar_sv_n(nar_sv v) {
    return v->get_n();
}

int nar_sv_eq(nar_sv v1, nar_sv v2) {
    return (*v1) == (*v2);
}

int nar_sv_assign(nar_sv v1, nar_sv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_sm nar_sm_alloc(int rows, int cols, int row_major) {
    return new Matrix<float>(rows, cols, row_major);
}

void nar_sm_free(nar_sm mat) {
    delete mat;
}

nar_sm nar_sm_copy(nar_sm mat) {
    return new Matrix<float>(mat->copy());
}

int nar_sm_valid(nar_sm mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_sm_order(nar_sm mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_sv nar_sm_row(nar_sm mat, int i) {
    return new Vector<float>(mat->row(i));
}

nar_sv nar_sm_col(nar_sm mat, int j) {
    return new Vector<float>(mat->col(j));
}

nar_sm nar_sm_subarray(nar_sm mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<float>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_sm_row_safe(nar_sv* x, nar_sm mat, int i) {
    try {
        *x = new Vector<float>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_sm_col_safe(nar_sv* x, nar_sm mat, int j) {
    try {
        *x = new Vector<float>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_sm_subarray_safe(nar_sm* x, nar_sm mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<float>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
float* nar_sm_pointer(nar_sm mat) {
    return (float*) mat->pointer();
}

int nar_sm_get_ld(nar_sm mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_sm_get_rows(nar_sm mat) {
    return mat->get_rows();
}

int nar_sm_get_cols(nar_sm mat) {
    return mat->get_cols();
}

int nar_sm_eq(nar_sm mat1, nar_sm mat2) {
    return (*mat1) == (*mat2);
}

int nar_sm_assign(nar_sm mat1, nar_sm mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_sm_display_string(nar_sm mat) {
    return string_to_char(display_string(*mat));
}

char* nar_sv_display_string(nar_sv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
float nar_sv_get(nar_sv v, int i) {
    return (*v)[i];
}

void nar_sv_set(nar_sv v, int i, float x) {
    (*v)[i] = x;
}

float nar_sm_get(nar_sm mat, int i, int j) {
    return (*mat)(i,j);
}

void nar_sm_set(nar_sm mat, int i, int j, float x) {
    (*mat)(i,j) = x;
}

// Bounds-checked versions
int nar_sv_get_safe(float* x, nar_sv v, int i) {
    try {
        *x = v->at(i);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_sv_set_safe(nar_sv v, int i, float x) {
    try {
        v->at(i) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_sm_get_safe(float* x, nar_sm mat, int i, int j) {
    try {
        *x = mat->at(i,j);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_sm_set_safe(nar_sm mat, int i, int j, float x) {
    try {
        mat->at(i,j) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_sv nar_sv_read(const char* filename, int* err) {
    try {
        File<float> f(filename, ios_base::in);
        nar_sv v = new Vector<float>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_sv_write(nar_sv v, const char* filename, const char* sep) {
    try {
        File<float> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_sm nar_sm_read(const char* filename, int row_major, int* err) {
    try {
        File<float> f(filename, ios_base::in);
        nar_sm v = new Matrix<float>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_sm_write(nar_sm v, const char* filename, const char* sep) {
    try {
        File<float> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_sv nar_sv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<float> f(filename, ios_base::in);
        nar_sv v = new Vector<float>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_sv_write_binary(nar_sv v, const char* filename) {
    try {
        BinaryFile<float> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_sm nar_sm_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<float> f(filename, ios_base::in);
        nar_sm v = new Matrix<float>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_sm_write_binary(nar_sm v, const char* filename) {
    try {
        BinaryFile<float> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Main routines for vectors and matrices of the given type

nar_dv nar_dv_alloc(int n) {
    return new Vector<double>(n);
}

void nar_dv_free(nar_dv v) {
    delete v;
}

nar_dv nar_dv_copy(nar_dv v) {
    return new Vector<double>(v->copy());
}

int nar_dv_valid(nar_dv v, int i) {
    return v->in_range(i);
}

nar_dv nar_dv_slice(nar_dv v, int start, int end) {
    return new Vector<double>(v->slice(start, end));
}

int nar_dv_slice_safe(nar_dv* out, nar_dv v, int start, int end) {
    try {
        *out = new Vector<double>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

double* nar_dv_pointer(nar_dv v) {
    return (double*) v->pointer();
}

int nar_dv_inc(nar_dv v) {
    return v->get_inc();
}

int nar_dv_n(nar_dv v) {
    return v->get_n();
}

int nar_dv_eq(nar_dv v1, nar_dv v2) {
    return (*v1) == (*v2);
}

int nar_dv_assign(nar_dv v1, nar_dv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_dm nar_dm_alloc(int rows, int cols, int row_major) {
    return new Matrix<double>(rows, cols, row_major);
}

void nar_dm_free(nar_dm mat) {
    delete mat;
}

nar_dm nar_dm_copy(nar_dm mat) {
    return new Matrix<double>(mat->copy());
}

int nar_dm_valid(nar_dm mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_dm_order(nar_dm mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_dv nar_dm_row(nar_dm mat, int i) {
    return new Vector<double>(mat->row(i));
}

nar_dv nar_dm_col(nar_dm mat, int j) {
    return new Vector<double>(mat->col(j));
}

nar_dm nar_dm_subarray(nar_dm mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<double>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_dm_row_safe(nar_dv* x, nar_dm mat, int i) {
    try {
        *x = new Vector<double>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_dm_col_safe(nar_dv* x, nar_dm mat, int j) {
    try {
        *x = new Vector<double>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_dm_subarray_safe(nar_dm* x, nar_dm mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<double>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
double* nar_dm_pointer(nar_dm mat) {
    return (double*) mat->pointer();
}

int nar_dm_get_ld(nar_dm mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_dm_get_rows(nar_dm mat) {
    return mat->get_rows();
}

int nar_dm_get_cols(nar_dm mat) {
    return mat->get_cols();
}

int nar_dm_eq(nar_dm mat1, nar_dm mat2) {
    return (*mat1) == (*mat2);
}

int nar_dm_assign(nar_dm mat1, nar_dm mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_dm_display_string(nar_dm mat) {
    return string_to_char(display_string(*mat));
}

char* nar_dv_display_string(nar_dv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
double nar_dv_get(nar_dv v, int i) {
    return (*v)[i];
}

void nar_dv_set(nar_dv v, int i, double x) {
    (*v)[i] = x;
}

double nar_dm_get(nar_dm mat, int i, int j) {
    return (*mat)(i,j);
}

void nar_dm_set(nar_dm mat, int i, int j, double x) {
    (*mat)(i,j) = x;
}

// Bounds-checked versions
int nar_dv_get_safe(double* x, nar_dv v, int i) {
    try {
        *x = v->at(i);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_dv_set_safe(nar_dv v, int i, double x) {
    try {
        v->at(i) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_dm_get_safe(double* x, nar_dm mat, int i, int j) {
    try {
        *x = mat->at(i,j);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_dm_set_safe(nar_dm mat, int i, int j, double x) {
    try {
        mat->at(i,j) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_dv nar_dv_read(const char* filename, int* err) {
    try {
        File<double> f(filename, ios_base::in);
        nar_dv v = new Vector<double>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_dv_write(nar_dv v, const char* filename, const char* sep) {
    try {
        File<double> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_dm nar_dm_read(const char* filename, int row_major, int* err) {
    try {
        File<double> f(filename, ios_base::in);
        nar_dm v = new Matrix<double>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_dm_write(nar_dm v, const char* filename, const char* sep) {
    try {
        File<double> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_dv nar_dv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<double> f(filename, ios_base::in);
        nar_dv v = new Vector<double>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_dv_write_binary(nar_dv v, const char* filename) {
    try {
        BinaryFile<double> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_dm nar_dm_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<double> f(filename, ios_base::in);
        nar_dm v = new Matrix<double>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_dm_write_binary(nar_dm v, const char* filename) {
    try {
        BinaryFile<double> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Main routines for vectors and matrices of the given type

nar_cv nar_cv_alloc(int n) {
    return new Vector<std::complex<float>>(n);
}

void nar_cv_free(nar_cv v) {
    delete v;
}

nar_cv nar_cv_copy(nar_cv v) {
    return new Vector<std::complex<float>>(v->copy());
}

int nar_cv_valid(nar_cv v, int i) {
    return v->in_range(i);
}

nar_cv nar_cv_slice(nar_cv v, int start, int end) {
    return new Vector<std::complex<float>>(v->slice(start, end));
}

int nar_cv_slice_safe(nar_cv* out, nar_cv v, int start, int end) {
    try {
        *out = new Vector<std::complex<float>>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

float* nar_cv_pointer(nar_cv v) {
    return (float*) v->pointer();
}

int nar_cv_inc(nar_cv v) {
    return v->get_inc();
}

int nar_cv_n(nar_cv v) {
    return v->get_n();
}

int nar_cv_eq(nar_cv v1, nar_cv v2) {
    return (*v1) == (*v2);
}

int nar_cv_assign(nar_cv v1, nar_cv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_cm nar_cm_alloc(int rows, int cols, int row_major) {
    return new Matrix<std::complex<float>>(rows, cols, row_major);
}

void nar_cm_free(nar_cm mat) {
    delete mat;
}

nar_cm nar_cm_copy(nar_cm mat) {
    return new Matrix<std::complex<float>>(mat->copy());
}

int nar_cm_valid(nar_cm mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_cm_order(nar_cm mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_cv nar_cm_row(nar_cm mat, int i) {
    return new Vector<std::complex<float>>(mat->row(i));
}

nar_cv nar_cm_col(nar_cm mat, int j) {
    return new Vector<std::complex<float>>(mat->col(j));
}

nar_cm nar_cm_subarray(nar_cm mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<std::complex<float>>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_cm_row_safe(nar_cv* x, nar_cm mat, int i) {
    try {
        *x = new Vector<std::complex<float>>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_cm_col_safe(nar_cv* x, nar_cm mat, int j) {
    try {
        *x = new Vector<std::complex<float>>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_cm_subarray_safe(nar_cm* x, nar_cm mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<std::complex<float>>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
float* nar_cm_pointer(nar_cm mat) {
    return (float*) mat->pointer();
}

int nar_cm_get_ld(nar_cm mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_cm_get_rows(nar_cm mat) {
    return mat->get_rows();
}

int nar_cm_get_cols(nar_cm mat) {
    return mat->get_cols();
}

int nar_cm_eq(nar_cm mat1, nar_cm mat2) {
    return (*mat1) == (*mat2);
}

int nar_cm_assign(nar_cm mat1, nar_cm mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_cm_display_string(nar_cm mat) {
    return string_to_char(display_string(*mat));
}

char* nar_cv_display_string(nar_cv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
// For complex numbers in the form a + bI
void nar_cv_get(float* a, float* b, nar_cv v, int i) {
    std::complex<float> z = (*v)[i];
    *a = z.real();
    *b = z.imag();
}

void nar_cv_set(nar_cv v, int i, float a, float b) {
    (*v)[i] = std::complex<float>(a,b);
}

void nar_cm_get(float* a, float* b, nar_cm mat, int i, int j) {
    std::complex<float> z = (*mat)(i,j);
    *a = z.real();
    *b = z.imag();
}

void nar_cm_set(nar_cm mat, int i, int j, float a, float b) {
    mat->operator()(i,j) = std::complex<float>(a,b);
}

// Bounds-checked versions
int nar_cv_get_safe(float* a, float* b, nar_cv v, int i) {
    try {
        std::complex<float> z = v->at(i);
        *a = z.real();
        *b = z.imag();
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_cv_set_safe(nar_cv v, int i, float a, float b) {
    try {
        v->at(i) = std::complex<float>(a,b);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_cm_get_safe(float* a, float* b, nar_cm mat, int i, int j) {
    try {
        std::complex<float> z = mat->at(i,j);
        *a = z.real();
        *b = z.imag();
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_cm_set_safe(nar_cm mat, int i, int j, float a, float b) {
    try {
        mat->at(i,j) = std::complex<float>(a,b);
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_cv nar_cv_read(const char* filename, int* err) {
    try {
        File<std::complex<float>> f(filename, ios_base::in);
        nar_cv v = new Vector<std::complex<float>>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_cv_write(nar_cv v, const char* filename, const char* sep) {
    try {
        File<std::complex<float>> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_cm nar_cm_read(const char* filename, int row_major, int* err) {
    try {
        File<std::complex<float>> f(filename, ios_base::in);
        nar_cm v = new Matrix<std::complex<float>>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_cm_write(nar_cm v, const char* filename, const char* sep) {
    try {
        File<std::complex<float>> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_cv nar_cv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<std::complex<float>> f(filename, ios_base::in);
        nar_cv v = new Vector<std::complex<float>>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_cv_write_binary(nar_cv v, const char* filename) {
    try {
        BinaryFile<std::complex<float>> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_cm nar_cm_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<std::complex<float>> f(filename, ios_base::in);
        nar_cm v = new Matrix<std::complex<float>>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_cm_write_binary(nar_cm v, const char* filename) {
    try {
        BinaryFile<std::complex<float>> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Main routines for vectors and matrices of the given type

nar_zv nar_zv_alloc(int n) {
    return new Vector<std::complex<double>>(n);
}

void nar_zv_free(nar_zv v) {
    delete v;
}

nar_zv nar_zv_copy(nar_zv v) {
    return new Vector<std::complex<double>>(v->copy());
}

int nar_zv_valid(nar_zv v, int i) {
    return v->in_range(i);
}

nar_zv nar_zv_slice(nar_zv v, int start, int end) {
    return new Vector<std::complex<double>>(v->slice(start, end));
}

int nar_zv_slice_safe(nar_zv* out, nar_zv v, int start, int end) {
    try {
        *out = new Vector<std::complex<double>>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

double* nar_zv_pointer(nar_zv v) {
    return (double*) v->pointer();
}

int nar_zv_inc(nar_zv v) {
    return v->get_inc();
}

int nar_zv_n(nar_zv v) {
    return v->get_n();
}

int nar_zv_eq(nar_zv v1, nar_zv v2) {
    return (*v1) == (*v2);
}

int nar_zv_assign(nar_zv v1, nar_zv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_zm nar_zm_alloc(int rows, int cols, int row_major) {
    return new Matrix<std::complex<double>>(rows, cols, row_major);
}

void nar_zm_free(nar_zm mat) {
    delete mat;
}

nar_zm nar_zm_copy(nar_zm mat) {
    return new Matrix<std::complex<double>>(mat->copy());
}

int nar_zm_valid(nar_zm mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_zm_order(nar_zm mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_zv nar_zm_row(nar_zm mat, int i) {
    return new Vector<std::complex<double>>(mat->row(i));
}

nar_zv nar_zm_col(nar_zm mat, int j) {
    return new Vector<std::complex<double>>(mat->col(j));
}

nar_zm nar_zm_subarray(nar_zm mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<std::complex<double>>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_zm_row_safe(nar_zv* x, nar_zm mat, int i) {
    try {
        *x = new Vector<std::complex<double>>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_zm_col_safe(nar_zv* x, nar_zm mat, int j) {
    try {
        *x = new Vector<std::complex<double>>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_zm_subarray_safe(nar_zm* x, nar_zm mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<std::complex<double>>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
double* nar_zm_pointer(nar_zm mat) {
    return (double*) mat->pointer();
}

int nar_zm_get_ld(nar_zm mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_zm_get_rows(nar_zm mat) {
    return mat->get_rows();
}

int nar_zm_get_cols(nar_zm mat) {
    return mat->get_cols();
}

int nar_zm_eq(nar_zm mat1, nar_zm mat2) {
    return (*mat1) == (*mat2);
}

int nar_zm_assign(nar_zm mat1, nar_zm mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_zm_display_string(nar_zm mat) {
    return string_to_char(display_string(*mat));
}

char* nar_zv_display_string(nar_zv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
// For complex numbers in the form a + bI
void nar_zv_get(double* a, double* b, nar_zv v, int i) {
    std::complex<double> z = (*v)[i];
    *a = z.real();
    *b = z.imag();
}

void nar_zv_set(nar_zv v, int i, double a, double b) {
    (*v)[i] = std::complex<double>(a,b);
}

void nar_zm_get(double* a, double* b, nar_zm mat, int i, int j) {
    std::complex<double> z = (*mat)(i,j);
    *a = z.real();
    *b = z.imag();
}

void nar_zm_set(nar_zm mat, int i, int j, double a, double b) {
    mat->operator()(i,j) = std::complex<double>(a,b);
}

// Bounds-checked versions
int nar_zv_get_safe(double* a, double* b, nar_zv v, int i) {
    try {
        std::complex<double> z = v->at(i);
        *a = z.real();
        *b = z.imag();
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_zv_set_safe(nar_zv v, int i, double a, double b) {
    try {
        v->at(i) = std::complex<double>(a,b);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_zm_get_safe(double* a, double* b, nar_zm mat, int i, int j) {
    try {
        std::complex<double> z = mat->at(i,j);
        *a = z.real();
        *b = z.imag();
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_zm_set_safe(nar_zm mat, int i, int j, double a, double b) {
    try {
        mat->at(i,j) = std::complex<double>(a,b);
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_zv nar_zv_read(const char* filename, int* err) {
    try {
        File<std::complex<double>> f(filename, ios_base::in);
        nar_zv v = new Vector<std::complex<double>>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_zv_write(nar_zv v, const char* filename, const char* sep) {
    try {
        File<std::complex<double>> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_zm nar_zm_read(const char* filename, int row_major, int* err) {
    try {
        File<std::complex<double>> f(filename, ios_base::in);
        nar_zm v = new Matrix<std::complex<double>>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_zm_write(nar_zm v, const char* filename, const char* sep) {
    try {
        File<std::complex<double>> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_zv nar_zv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<std::complex<double>> f(filename, ios_base::in);
        nar_zv v = new Vector<std::complex<double>>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_zv_write_binary(nar_zv v, const char* filename) {
    try {
        BinaryFile<std::complex<double>> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_zm nar_zm_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<std::complex<double>> f(filename, ios_base::in);
        nar_zm v = new Matrix<std::complex<double>>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_zm_write_binary(nar_zm v, const char* filename) {
    try {
        BinaryFile<std::complex<double>> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Main routines for vectors and matrices of the given type

nar_xv nar_xv_alloc(int n) {
    return new Vector<char>(n);
}

void nar_xv_free(nar_xv v) {
    delete v;
}

nar_xv nar_xv_copy(nar_xv v) {
    return new Vector<char>(v->copy());
}

int nar_xv_valid(nar_xv v, int i) {
    return v->in_range(i);
}

nar_xv nar_xv_slice(nar_xv v, int start, int end) {
    return new Vector<char>(v->slice(start, end));
}

int nar_xv_slice_safe(nar_xv* out, nar_xv v, int start, int end) {
    try {
        *out = new Vector<char>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

char* nar_xv_pointer(nar_xv v) {
    return (char*) v->pointer();
}

int nar_xv_inc(nar_xv v) {
    return v->get_inc();
}

int nar_xv_n(nar_xv v) {
    return v->get_n();
}

int nar_xv_eq(nar_xv v1, nar_xv v2) {
    return (*v1) == (*v2);
}

int nar_xv_assign(nar_xv v1, nar_xv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_xm nar_xm_alloc(int rows, int cols, int row_major) {
    return new Matrix<char>(rows, cols, row_major);
}

void nar_xm_free(nar_xm mat) {
    delete mat;
}

nar_xm nar_xm_copy(nar_xm mat) {
    return new Matrix<char>(mat->copy());
}

int nar_xm_valid(nar_xm mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_xm_order(nar_xm mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_xv nar_xm_row(nar_xm mat, int i) {
    return new Vector<char>(mat->row(i));
}

nar_xv nar_xm_col(nar_xm mat, int j) {
    return new Vector<char>(mat->col(j));
}

nar_xm nar_xm_subarray(nar_xm mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<char>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_xm_row_safe(nar_xv* x, nar_xm mat, int i) {
    try {
        *x = new Vector<char>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_xm_col_safe(nar_xv* x, nar_xm mat, int j) {
    try {
        *x = new Vector<char>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_xm_subarray_safe(nar_xm* x, nar_xm mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<char>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
char* nar_xm_pointer(nar_xm mat) {
    return (char*) mat->pointer();
}

int nar_xm_get_ld(nar_xm mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_xm_get_rows(nar_xm mat) {
    return mat->get_rows();
}

int nar_xm_get_cols(nar_xm mat) {
    return mat->get_cols();
}

int nar_xm_eq(nar_xm mat1, nar_xm mat2) {
    return (*mat1) == (*mat2);
}

int nar_xm_assign(nar_xm mat1, nar_xm mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_xm_display_string(nar_xm mat) {
    return string_to_char(display_string(*mat));
}

char* nar_xv_display_string(nar_xv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
char nar_xv_get(nar_xv v, int i) {
    return (*v)[i];
}

void nar_xv_set(nar_xv v, int i, char x) {
    (*v)[i] = x;
}

char nar_xm_get(nar_xm mat, int i, int j) {
    return (*mat)(i,j);
}

void nar_xm_set(nar_xm mat, int i, int j, char x) {
    (*mat)(i,j) = x;
}

// Bounds-checked versions
int nar_xv_get_safe(char* x, nar_xv v, int i) {
    try {
        *x = v->at(i);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_xv_set_safe(nar_xv v, int i, char x) {
    try {
        v->at(i) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_xm_get_safe(char* x, nar_xm mat, int i, int j) {
    try {
        *x = mat->at(i,j);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_xm_set_safe(nar_xm mat, int i, int j, char x) {
    try {
        mat->at(i,j) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_xv nar_xv_read(const char* filename, int* err) {
    try {
        File<char> f(filename, ios_base::in);
        nar_xv v = new Vector<char>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_xv_write(nar_xv v, const char* filename, const char* sep) {
    try {
        File<char> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_xm nar_xm_read(const char* filename, int row_major, int* err) {
    try {
        File<char> f(filename, ios_base::in);
        nar_xm v = new Matrix<char>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_xm_write(nar_xm v, const char* filename, const char* sep) {
    try {
        File<char> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_xv nar_xv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<char> f(filename, ios_base::in);
        nar_xv v = new Vector<char>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_xv_write_binary(nar_xv v, const char* filename) {
    try {
        BinaryFile<char> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_xm nar_xm_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<char> f(filename, ios_base::in);
        nar_xm v = new Matrix<char>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_xm_write_binary(nar_xm v, const char* filename) {
    try {
        BinaryFile<char> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Main routines for vectors and matrices of the given type

nar_pv nar_pv_alloc(int n) {
    return new Vector<void*>(n);
}

void nar_pv_free(nar_pv v) {
    delete v;
}

nar_pv nar_pv_copy(nar_pv v) {
    return new Vector<void*>(v->copy());
}

int nar_pv_valid(nar_pv v, int i) {
    return v->in_range(i);
}

nar_pv nar_pv_slice(nar_pv v, int start, int end) {
    return new Vector<void*>(v->slice(start, end));
}

int nar_pv_slice_safe(nar_pv* out, nar_pv v, int start, int end) {
    try {
        *out = new Vector<void*>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

void** nar_pv_pointer(nar_pv v) {
    return (void**) v->pointer();
}

int nar_pv_inc(nar_pv v) {
    return v->get_inc();
}

int nar_pv_n(nar_pv v) {
    return v->get_n();
}

int nar_pv_eq(nar_pv v1, nar_pv v2) {
    return (*v1) == (*v2);
}

int nar_pv_assign(nar_pv v1, nar_pv v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_pm nar_pm_alloc(int rows, int cols, int row_major) {
    return new Matrix<void*>(rows, cols, row_major);
}

void nar_pm_free(nar_pm mat) {
    delete mat;
}

nar_pm nar_pm_copy(nar_pm mat) {
    return new Matrix<void*>(mat->copy());
}

int nar_pm_valid(nar_pm mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_pm_order(nar_pm mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_pv nar_pm_row(nar_pm mat, int i) {
    return new Vector<void*>(mat->row(i));
}

nar_pv nar_pm_col(nar_pm mat, int j) {
    return new Vector<void*>(mat->col(j));
}

nar_pm nar_pm_subarray(nar_pm mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<void*>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_pm_row_safe(nar_pv* x, nar_pm mat, int i) {
    try {
        *x = new Vector<void*>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_pm_col_safe(nar_pv* x, nar_pm mat, int j) {
    try {
        *x = new Vector<void*>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_pm_subarray_safe(nar_pm* x, nar_pm mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<void*>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
void** nar_pm_pointer(nar_pm mat) {
    return (void**) mat->pointer();
}

int nar_pm_get_ld(nar_pm mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_pm_get_rows(nar_pm mat) {
    return mat->get_rows();
}

int nar_pm_get_cols(nar_pm mat) {
    return mat->get_cols();
}

int nar_pm_eq(nar_pm mat1, nar_pm mat2) {
    return (*mat1) == (*mat2);
}

int nar_pm_assign(nar_pm mat1, nar_pm mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_pm_display_string(nar_pm mat) {
    return string_to_char(display_string(*mat));
}

char* nar_pv_display_string(nar_pv v) {
    return string_to_char(display_string(*v));
}
// No bounds checking
void* nar_pv_get(nar_pv v, int i) {
    return (*v)[i];
}

void nar_pv_set(nar_pv v, int i, void* x) {
    (*v)[i] = x;
}

void* nar_pm_get(nar_pm mat, int i, int j) {
    return (*mat)(i,j);
}

void nar_pm_set(nar_pm mat, int i, int j, void* x) {
    (*mat)(i,j) = x;
}

// Bounds-checked versions
int nar_pv_get_safe(void** x, nar_pv v, int i) {
    try {
        *x = v->at(i);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_pv_set_safe(nar_pv v, int i, void* x) {
    try {
        v->at(i) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_pm_get_safe(void** x, nar_pm mat, int i, int j) {
    try {
        *x = mat->at(i,j);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_pm_set_safe(nar_pm mat, int i, int j, void* x) {
    try {
        mat->at(i,j) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

// Reads vector from file. If there's an error, puts error code in err.
nar_pv nar_pv_read(const char* filename, int* err) {
    try {
        File<void*> f(filename, ios_base::in);
        nar_pv v = new Vector<void*>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_pv_write(nar_pv v, const char* filename, const char* sep) {
    try {
        File<void*> f(filename, ios_base::out);
        f.write_vector(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_pm nar_pm_read(const char* filename, int row_major, int* err) {
    try {
        File<void*> f(filename, ios_base::in);
        nar_pm v = new Matrix<void*>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_pm_write(nar_pm v, const char* filename, const char* sep) {
    try {
        File<void*> f(filename, ios_base::out);
        f.write_matrix(*v, sep);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}



// Reads vector from file. If there's an error, puts error code in err.
nar_pv nar_pv_read_binary(const char* filename, int* err) {
    try {
        BinaryFile<void*> f(filename, ios_base::in);
        nar_pv v = new Vector<void*>(f.read_vector());
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes vector to file.
int nar_pv_write_binary(nar_pv v, const char* filename) {
    try {
        BinaryFile<void*> f(filename, ios_base::out);
        f.write_vector(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_pm nar_pm_read_binary(const char* filename, int row_major, int* err) {
    try {
        BinaryFile<void*> f(filename, ios_base::in);
        nar_pm v = new Matrix<void*>(f.read_matrix(row_major));
        if (err) *err = 0;
        return v;
    } catch (const FileError& e) {
        if (err) *err = 1;
        return nullptr;
    } catch (const FormatError& e) {
        if (err) *err = 2;
        return nullptr;
    }
}

// Writes MATRIX to file.
int nar_pm_write_binary(nar_pm v, const char* filename) {
    try {
        BinaryFile<void*> f(filename, ios_base::out);
        f.write_matrix(*v);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}




// Need this to delete strings outputted from this library
void nar_string_free(char* s) {
    delete[] s;
}
