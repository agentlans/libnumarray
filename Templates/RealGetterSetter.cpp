// No bounds checking
`'TTYPE`' nar_`'VECTORTYPE`'_get(nar_`'VECTORTYPE`' v, int i) {
    return (*v)[i];
}

void nar_`'VECTORTYPE`'_set(nar_`'VECTORTYPE`' v, int i, `'TTYPE`' x) {
    (*v)[i] = x;
}

`'TTYPE`' nar_`'MATRIXTYPE`'_get(nar_`'MATRIXTYPE`' mat, int i, int j) {
    return (*mat)(i,j);
}

void nar_`'MATRIXTYPE`'_set(nar_`'MATRIXTYPE`' mat, int i, int j, `'TTYPE`' x) {
    (*mat)(i,j) = x;
}

// Bounds-checked versions
int nar_`'VECTORTYPE`'_get_safe(`'TTYPE`'* x, nar_`'VECTORTYPE`' v, int i) {
    try {
        *x = v->at(i);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'VECTORTYPE`'_set_safe(nar_`'VECTORTYPE`' v, int i, `'TTYPE`' x) {
    try {
        v->at(i) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'MATRIXTYPE`'_get_safe(`'TTYPE`'* x, nar_`'MATRIXTYPE`' mat, int i, int j) {
    try {
        *x = mat->at(i,j);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'MATRIXTYPE`'_set_safe(nar_`'MATRIXTYPE`' mat, int i, int j, `'TTYPE`' x) {
    try {
        mat->at(i,j) = x;
        return 0;
    } catch (...) {
        return 1;
    }
}
