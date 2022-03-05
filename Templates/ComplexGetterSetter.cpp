// No bounds checking
// For complex numbers in the form a + bI
void nar_`'VECTORTYPE`'_get(`'COMPLEXPART`'* a, `'COMPLEXPART`'* b, nar_`'VECTORTYPE`' v, int i) {
    `'TTYPE`' z = (*v)[i];
    *a = z.real();
    *b = z.imag();
}

void nar_`'VECTORTYPE`'_set(nar_`'VECTORTYPE`' v, int i, `'COMPLEXPART`' a, `'COMPLEXPART`' b) {
    (*v)[i] = `'TTYPE`'(a,b);
}

void nar_`'MATRIXTYPE`'_get(`'COMPLEXPART`'* a, `'COMPLEXPART`'* b, nar_`'MATRIXTYPE`' mat, int i, int j) {
    `'TTYPE`' z = (*mat)(i,j);
    *a = z.real();
    *b = z.imag();
}

void nar_`'MATRIXTYPE`'_set(nar_`'MATRIXTYPE`' mat, int i, int j, `'COMPLEXPART`' a, `'COMPLEXPART`' b) {
    mat->operator()(i,j) = `'TTYPE`'(a,b);
}

// Bounds-checked versions
int nar_`'VECTORTYPE`'_get_safe(`'COMPLEXPART`'* a, `'COMPLEXPART`'* b, nar_`'VECTORTYPE`' v, int i) {
    try {
        `'TTYPE`' z = v->at(i);
        *a = z.real();
        *b = z.imag();
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'VECTORTYPE`'_set_safe(nar_`'VECTORTYPE`' v, int i, `'COMPLEXPART`' a, `'COMPLEXPART`' b) {
    try {
        v->at(i) = `'TTYPE`'(a,b);
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'MATRIXTYPE`'_get_safe(`'COMPLEXPART`'* a, `'COMPLEXPART`'* b, nar_`'MATRIXTYPE`' mat, int i, int j) {
    try {
        `'TTYPE`' z = mat->at(i,j);
        *a = z.real();
        *b = z.imag();
        return 0;
    } catch (...) {
        return 1;
    }
}

int nar_`'MATRIXTYPE`'_set_safe(nar_`'MATRIXTYPE`' mat, int i, int j, `'COMPLEXPART`' a, `'COMPLEXPART`' b) {
    try {
        mat->at(i,j) = `'TTYPE`'(a,b);
        return 0;
    } catch (...) {
        return 1;
    }
}
