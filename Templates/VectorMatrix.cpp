// Main routines for vectors and matrices of the given type

nar_`'VECTORTYPE`' nar_`'VECTORTYPE`'_alloc(int n) {
    return new Vector<`'TTYPE`'>(n);
}

void nar_`'VECTORTYPE`'_free(nar_`'VECTORTYPE`' v) {
    delete v;
}

nar_`'VECTORTYPE`' nar_`'VECTORTYPE`'_copy(nar_`'VECTORTYPE`' v) {
    return new Vector<`'TTYPE`'>(v->copy());
}

int nar_`'VECTORTYPE`'_valid(nar_`'VECTORTYPE`' v, int i) {
    return v->in_range(i);
}

nar_`'VECTORTYPE`' nar_`'VECTORTYPE`'_slice(nar_`'VECTORTYPE`' v, int start, int end) {
    return new Vector<`'TTYPE`'>(v->slice(start, end));
}

int nar_`'VECTORTYPE`'_slice_safe(nar_`'VECTORTYPE`'* out, nar_`'VECTORTYPE`' v, int start, int end) {
    try {
        *out = new Vector<`'TTYPE`'>(v->slice<true>(start, end));
        return 0;
    } catch (...) {
        return 1;
    }
}

`'COMPLEXPART`'* nar_`'VECTORTYPE`'_pointer(nar_`'VECTORTYPE`' v) {
    return (`'COMPLEXPART`'*) v->pointer();
}

int nar_`'VECTORTYPE`'_inc(nar_`'VECTORTYPE`' v) {
    return v->get_inc();
}

int nar_`'VECTORTYPE`'_n(nar_`'VECTORTYPE`' v) {
    return v->get_n();
}

int nar_`'VECTORTYPE`'_eq(nar_`'VECTORTYPE`' v1, nar_`'VECTORTYPE`' v2) {
    return (*v1) == (*v2);
}

int nar_`'VECTORTYPE`'_assign(nar_`'VECTORTYPE`' v1, nar_`'VECTORTYPE`' v2) {
    try {
        v1->assign(*v2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}

// For matrices

nar_`'MATRIXTYPE`' nar_`'MATRIXTYPE`'_alloc(int rows, int cols, int row_major) {
    return new Matrix<`'TTYPE`'>(rows, cols, row_major);
}

void nar_`'MATRIXTYPE`'_free(nar_`'MATRIXTYPE`' mat) {
    delete mat;
}

nar_`'MATRIXTYPE`' nar_`'MATRIXTYPE`'_copy(nar_`'MATRIXTYPE`' mat) {
    return new Matrix<`'TTYPE`'>(mat->copy());
}

int nar_`'MATRIXTYPE`'_valid(nar_`'MATRIXTYPE`' mat, int i, int j) {
    return mat->in_range(i,j);
}

enum nar_order nar_`'MATRIXTYPE`'_order(nar_`'MATRIXTYPE`' mat) {
    return (nar_order) mat->get_order();
}

// Slicing parts of matrix
nar_`'VECTORTYPE`' nar_`'MATRIXTYPE`'_row(nar_`'MATRIXTYPE`' mat, int i) {
    return new Vector<`'TTYPE`'>(mat->row(i));
}

nar_`'VECTORTYPE`' nar_`'MATRIXTYPE`'_col(nar_`'MATRIXTYPE`' mat, int j) {
    return new Vector<`'TTYPE`'>(mat->col(j));
}

nar_`'MATRIXTYPE`' nar_`'MATRIXTYPE`'_subarray(nar_`'MATRIXTYPE`' mat, int start_row,
    int start_col, int end_row, int end_col) {
    return new Matrix<`'TTYPE`'>(mat->subarray(
        start_row, start_col, end_row, end_col));
}

int nar_`'MATRIXTYPE`'_row_safe(nar_`'VECTORTYPE`'* x, nar_`'MATRIXTYPE`' mat, int i) {
    try {
        *x = new Vector<`'TTYPE`'>(mat->row<true>(i));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'MATRIXTYPE`'_col_safe(nar_`'VECTORTYPE`'* x, nar_`'MATRIXTYPE`' mat, int j) {
    try {
        *x = new Vector<`'TTYPE`'>(mat->col<true>(j));
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'MATRIXTYPE`'_subarray_safe(nar_`'MATRIXTYPE`'* x, nar_`'MATRIXTYPE`' mat, int start_row,
    int start_col, int end_row, int end_col) {
    try {
        *x = new Matrix<`'TTYPE`'>(mat->subarray<true>(
            start_row, start_col, end_row, end_col));
        return 0;
    } catch (...) {
        return 1;
    }
}

// Matrix attributes and dimensions
`'COMPLEXPART`'* nar_`'MATRIXTYPE`'_pointer(nar_`'MATRIXTYPE`' mat) {
    return (`'COMPLEXPART`'*) mat->pointer();
}

int nar_`'MATRIXTYPE`'_get_ld(nar_`'MATRIXTYPE`' mat) {
    try {
        return mat->get_ld();
    } catch (...) {
        return -1; // Not contiguously ordered
    }
}

int nar_`'MATRIXTYPE`'_get_rows(nar_`'MATRIXTYPE`' mat) {
    return mat->get_rows();
}

int nar_`'MATRIXTYPE`'_get_cols(nar_`'MATRIXTYPE`' mat) {
    return mat->get_cols();
}

int nar_`'MATRIXTYPE`'_eq(nar_`'MATRIXTYPE`' mat1, nar_`'MATRIXTYPE`' mat2) {
    return (*mat1) == (*mat2);
}

int nar_`'MATRIXTYPE`'_assign(nar_`'MATRIXTYPE`' mat1, nar_`'MATRIXTYPE`' mat2) {
    try {
        mat1->assign(*mat2);
        return 0;
    } catch (const std::out_of_range& e) {
        return 1;
    }
}


// Display strings
char* nar_`'MATRIXTYPE`'_display_string(nar_`'MATRIXTYPE`' mat) {
    return string_to_char(display_string(*mat));
}

char* nar_`'VECTORTYPE`'_display_string(nar_`'VECTORTYPE`' v) {
    return string_to_char(display_string(*v));
}
