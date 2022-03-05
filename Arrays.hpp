#ifndef _ARRAYS
#define _ARRAYS

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

class DimensionError : public std::runtime_error {
public:
    DimensionError(const std::string& s) : std::runtime_error(s) {}
};

// Array backed by shared data
template <class T>
class ArrayPtr {
public:
    ArrayPtr() {}
    ArrayPtr(int n) : data(std::make_shared<std::vector<T>>(n)), n(n) {}
    // Element access
    T& operator[](int i) const {
        return (*data)[i];
    }
    // Copies the contents of this array to a new array
    ArrayPtr<T> copy() const {
        ArrayPtr<T> cp(n);
        std::copy(data->begin(), data->end(), cp.data->begin());
        return cp;
    }
    int size() const {
        return n;
    }
    T* pointer() const {
        return &(*data)[0];
    }
private:
    std::shared_ptr<std::vector<T>> data;
    int n;
};

// Returns true iff x in [lower,upper)
template <class T>
bool between_eq(T lower, T x, T upper) {
    return (lower <= x && x < upper);
}

// A strided vector
template <class T>
class Vector {
public:
    Vector() {}
    Vector(int n) : n(n), data(ArrayPtr<T>(n)) {}
    // Create Vector from parts
    Vector(const ArrayPtr<T>& data, int offset, int inc, int n) :
        data(data), offset(offset), inc(inc), n(n) {}
    // Create Vector from the given data
    Vector(const std::vector<T>& x) :
    data(ArrayPtr<T>(x.size())), n(x.size()) {
        for (int i = 0; i < n; ++i) {
            data[i] = x[i];
        }
    }
    // 0-based indexing!
    int get_index(int i) const {
        return offset + inc * i;
    }
    bool in_range(int i) const {
        return between_eq(0, i, n);
    }
    // Referencing
    T& operator[](int i) const {
        return data[get_index(i)];
    }
    T& at(int i) const {
        if (!in_range(i)) {
            throw std::out_of_range("Vector index is out of range.");
        } else {
            return data[get_index(i)];
        }
    }
    // Overwrite this vector with another one
    void assign(const Vector<T>& v) {
        if (v.get_n() != n) {
            throw DimensionError(
                "Can't assign vectors with different dimensions.");
        } else {
            for (int i = 0; i < n; ++i) {
                operator[](i) = v[i];
            }
        }
    }
    // Equality
    bool operator==(const Vector<T>& other) const {
        if (n != other.get_n()) {
            return false;
        }
        for (int i = 0; i < n; ++i) {
            if (operator[](i) != other[i]) {
                return false;
            }
        }
        return true;
    }
    // Slicing
    template <bool safe=false>
    Vector<T> slice(int start, int end) const {
        if (!in_range(start) || !in_range(end-1)) {
            throw std::out_of_range("Vector index is out of range.");
        }
        int new_offset = get_index(start);
        int new_n = end - start;
        int new_inc = inc;
        return Vector<T>(data, new_offset, new_inc, new_n);
    }
    // Copies contents of Vector to a new Vector
    Vector<T> copy() const {
        Vector<T> cp(n);
        for (int i = 0; i < n; ++i) {
            cp[i] = operator[](i);
        }
        return cp;
    }
    // Attributes
    T* pointer() const { // to first element
        return data.pointer() + offset;
    }
    int get_inc() const {return inc;} // stride
    int get_n() const {return n;}
private:
    ArrayPtr<T> data;
    int offset = 0, inc = 1, n;
};

enum MatrixOrder {
    ROW_MAJOR_ORDER = 0,
    COL_MAJOR_ORDER = 1,
    UNKNOWN_ORDER = 2
};

// 2D full storage matrix
template <class T>
class Matrix {
public:
    Matrix() {}
    Matrix(int rows, int cols, bool row_major = true) : 
    rows(rows), cols(cols),
    data(ArrayPtr<T>(rows * cols)) {
        if (row_major) {
            inc_row = cols;
            inc_col = 1;
        } else { // Column major
            inc_row = 1;
            inc_col = rows;
        }
    }
    Matrix(const ArrayPtr<T>& data, int offset, 
        int inc_row, int inc_col, int rows, int cols) :
        data(data), offset(offset), 
        inc_row(inc_row), inc_col(inc_col), 
        rows(rows), cols(cols) {}
    // Referencing
    int get_index(int i, int j) const {
        return offset + inc_row*i + inc_col*j;
    }
    bool in_range(int i, int j) const {
        return between_eq(0, i, rows) &&
            between_eq(0, j, cols);
    }
    T& operator()(int i, int j) const {
        return data[get_index(i,j)];
    }
    T& at(int i, int j) const {
        if (!in_range(i,j)) {
            throw std::out_of_range(
                "Matrix subscripts are out of range.");
        } else {
            return data[get_index(i,j)];
        }
    }
    // Overwrite this matrix with another one
    void assign(const Matrix<T>& mat) {
        if (mat.get_rows() != rows || mat.get_cols() != cols) {
            throw DimensionError(
                "Can't assign matrices with different dimensions.");
        } else {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    operator()(i,j) = mat(i,j);
                }
            }
        }
    }
    // Equality
    bool operator==(const Matrix& other) const {
        if (rows != other.get_rows() || 
            cols != other.get_cols()) {
                return false;
        }
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (operator()(i,j) != other(i,j)) {
                    return false;
                }
            }
        }
        return true;
    }
    // Slicing
    template <bool safe=false>
    Vector<T> row(int i) const {
        if (safe && !in_range(i, 0)) {
            throw std::out_of_range("Row index is out of range.");
        }
        int new_offset = get_index(i, 0);
        int new_inc = inc_col;
        int new_n = cols;
        return Vector<T>(data, new_offset, new_inc, new_n);
    }
    template <bool safe=false>
    Vector<T> col(int j) const {
        if (safe && !in_range(0, j)) {
            throw std::out_of_range("Column index is out of range.");
        }
        int new_offset = get_index(0, j);
        int new_inc = inc_row;
        int new_n = rows;
        return Vector<T>(data, new_offset, new_inc, new_n);
    }
    // Returns part of matrix from [start_row, end_row)
    // and [start_col, end_col)
    template <bool safe=false>
    Matrix<T> subarray(int start_row, int start_col, 
    int end_row, int end_col) const {
        if (safe) {
            if (!in_range(start_row, start_col) ||
                !in_range(end_row-1, end_col-1)) {
                throw std::out_of_range(
                    "Subarray indices are out of range.");
            }
        }
        int new_offset = get_index(start_row, start_col);
        int new_rows = end_row - start_row;
        int new_cols = end_col - start_col;
        return Matrix<T>(data, new_offset, inc_row, inc_col,
            new_rows, new_cols);
    }
    // Returns the matrix storage order
    MatrixOrder get_order() const {
        if (inc_col == 1) {
            return ROW_MAJOR_ORDER;
        } else if (inc_row == 1) {
            return COL_MAJOR_ORDER;
        } else {
            return UNKNOWN_ORDER;
        }
    }
    // Copies contents to a new matrix
    Matrix<T> copy() const {
        Matrix<T> cp(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cp(i,j) = operator()(i,j);
            }
        }
        return cp;
    }
    // Attributes
    T* pointer() const {
        return data.pointer() + offset;
    }
    int get_ld() const { // Get leading dimension
        switch (get_order()) {
            case ROW_MAJOR_ORDER: 
                return inc_col;
                break;
            case COL_MAJOR_ORDER:
                return inc_row;
                break;
            default:
                throw DimensionError(
                    "Matrix has no leading dimension.");
                break;
        }
    }
    int get_rows() const {return rows;}
    int get_cols() const {return cols;}
private:
    ArrayPtr<T> data;
    int offset = 0, inc_row = 0, inc_col = 0;
    int rows = 0, cols = 0;
};

#endif