define(`SEP_PARAM', `ifelse(ISBINARY,`',`, const char* sep')')dnl
define(`SEP_CALL', `ifelse(ISBINARY,`',`, sep')')dnl

// Reads vector from file. If there's an error, puts error code in err.
nar_`'VECTORTYPE`' nar_`'VECTORTYPE`'_read`'BINARYTAG`'(const char* filename, int* err) {
    try {
        `'ISBINARY`'File<`'TTYPE`'> f(filename, ios_base::in);
        nar_`'VECTORTYPE`' v = new Vector<`'TTYPE`'>(f.read_vector());
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
int nar_`'VECTORTYPE`'_write`'BINARYTAG`'(nar_`'VECTORTYPE`' v, const char* filename`'SEP_PARAM) {
    try {
        `'ISBINARY`'File<`'TTYPE`'> f(filename, ios_base::out);
        f.write_vector(*v`'SEP_CALL);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

// Reads MATRIX from file. If there's an error, puts error code in err.
nar_`'MATRIXTYPE`' nar_`'MATRIXTYPE`'_read`'BINARYTAG`'(const char* filename, int row_major, int* err) {
    try {
        `'ISBINARY`'File<`'TTYPE`'> f(filename, ios_base::in);
        nar_`'MATRIXTYPE`' v = new Matrix<`'TTYPE`'>(f.read_matrix(row_major));
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
int nar_`'MATRIXTYPE`'_write`'BINARYTAG`'(nar_`'MATRIXTYPE`' v, const char* filename`'SEP_PARAM) {
    try {
        `'ISBINARY`'File<`'TTYPE`'> f(filename, ios_base::out);
        f.write_matrix(*v`'SEP_CALL);
        return 0;
    } catch (const FileError& e) {
        return 1;
    }
}

