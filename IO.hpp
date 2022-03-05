#ifndef _ARRAYIO
#define _ARRAYIO

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <typeinfo>

#include "Arrays.hpp"

// Raised when a file couldn't be opened
class FileError : public std::runtime_error {
public:
    FileError(const std::string& s) : std::runtime_error(s) {}
};

// Raised when there's a problem with the file's format
class FormatError : public std::runtime_error {
public:
    FormatError(const std::string& s) : std::runtime_error(s) {}
};

using std::ios_base;

// Tabular text file
template <class T>
class File {
public:
    File() {}
    File(const std::string& filename, 
        ios_base::openmode mode = ios_base::in | ios_base::out) {
        open(filename, mode);
    }
    ~File() {
        close();
    }
    // For opening and closing files
    void open(const std::string& filename, 
        ios_base::openmode mode = ios_base::in | ios_base::out) {
        file.open(filename, mode);
        if (!file.is_open()) {
            throw FileError("Couldn't open " + filename);
        }
    }
    void close() {
        file.close();
    }
    // Sets input from beginning of file.
    void rewind() {
        file.clear();
        file.seekg(file.beg);
    }
    std::pair<int,int> dimensions() {
        rewind();
        std::string line;
        int lines = 0; // how many lines in file
        T x;
        int file_cols = -1;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            // Count how many columns on this line
            int line_cols = 0;
            while (iss >> x) {
                line_cols++;
            }
            /*if (iss.fail()) {
                throw FormatError("Couldn't read value.");
            }*/
            if (file_cols < 0) {
                file_cols = line_cols;
            } else if (file_cols != line_cols) {
                throw FormatError("Uneven line lengths.");
            }
            lines++;
        }
        rewind();
        return {lines, file_cols};
    }
    // Reads vector from file
    Vector<T> read_vector() {
        std::vector<T> lst;
        T x;
        rewind();
        while (!file.eof()) {
            file >> x;
            if (file.fail()) {
                throw FormatError("Couldn't read value.");
            }
            lst.push_back(x);
        }
        rewind();
        Vector<T> out(lst);
        return out;
    }
    // Writes vector to file
    void write_vector(const Vector<T>& v, 
        const std::string& sep = " ") {
        int n = v.get_n();
        rewind();
        for (int i = 0; i < n; ++i) {
            file << v[i];
            if (i != n-1) {
                file << sep;
            }
        }
        file.flush();
        rewind();
    }
    // Reads a matrix of type T from file
    Matrix<T> read_matrix(bool row_major = true) {
        auto dims = dimensions();
        int rows = dims.first;
        int cols = dims.second;
        rewind();
        Matrix<T> out(rows, cols, row_major);
        // Read each line
        std::string line;
        T x;
        for (int i = 0; i < rows; ++i) {
            std::getline(file, line);
            std::istringstream iss(line);
            // Read each column in the line
            for (int j = 0; j < cols; ++j) {
                iss >> x;
                if (iss.fail()) {
                    throw FormatError("Couldn't read value.");
                }
                out(i,j) = x;
            }
        }
        rewind();
        return out;
    }
    // Writes matrix to a file.
    // sep is the separator between entries
    void write_matrix(const Matrix<T>& mat, 
        const std::string& sep=" ") {
        // Find the dimensions
        int rows = mat.get_rows();
        int cols = mat.get_cols();
        // Write the elements of matrix
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                file << mat(i,j);
                if (j != cols-1) {
                    file << sep;
                } else {
                    file << std::endl;
                }
            }
        }
        if (!file.good()) {
            throw FileError("Couldn't write to file.");
        }
        file.flush();
    }
private:
    std::fstream file;
};

// Binary format file
template <class T>
class BinaryFile {
public:
    BinaryFile() {}
    BinaryFile(const std::string& filename, 
        ios_base::openmode mode = ios_base::in | ios_base::out) {
        open(filename, mode | ios_base::binary);
    }
    ~BinaryFile() {
        close();
    }
    // For opening and closing files
    void open(const std::string& filename, 
        ios_base::openmode mode = ios_base::in | ios_base::out) {
        file.open(filename, mode | ios_base::binary);
        if (!file.is_open()) {
            throw FileError("Couldn't open " + filename);
        }
    }
    void close() {
        file.close();
    }
    Vector<T> read_vector() {
        int n = read_binary<int>(); // Read the dimensions
        Vector<T> read(n);
        for (int i = 0; i < n; ++i) {
            read[i] = read_binary<T>();
            if (file.fail()) {
                throw FileError("Couldn't read file.");
            }
        }
        return read;
    }
    void write_vector(const Vector<T>& v) {
        int n = v.get_n(); // Write the dimensions
        write_binary(n);
        for (int i = 0; i < n; ++i) {
            write_binary(v[i]);
            if (file.fail()) {
                throw FileError("Couldn't write file.");
            }
        }
    }
    Matrix<T> read_matrix(bool row_major = true) {
        // Read the dimensions
        int rows, cols;
        rows = read_binary<int>();
        cols = read_binary<int>();
        // Place to put the read values
        Matrix<T> read(rows, cols, row_major);
        T x;
        // Read the file
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                read(i,j) = read_binary<T>();
                if (file.fail()) {
                    throw FileError("Couldn't read file.");
                }
            }
        }
        return read;
    }
    void write_matrix(const Matrix<T>& mat) {
        // Write the dimensions
        int rows = mat.get_rows();
        int cols = mat.get_cols();
        write_binary(rows);
        write_binary(cols);
        // Write the entries
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                write_binary(mat(i,j));
                if (file.fail()) {
                    throw FileError("Couldn't write file.");
                }
            }
        }
    }
private:
    template <class U>
    U read_binary() {
        U x;
        file.read((char*) &x, sizeof(U));
        return x;
    }
    template <class U>
    void write_binary(const U& x) {
        file.write((const char*) &x, sizeof(U));
    }
    std::fstream file;
};

// Convenience functions

/*
template <class T, bool binary=false>
Matrix<T> read_matrix(const std::string& filename, bool row_major=true) {
    File<binary> f(filename, ios_base::in);
    return f.read_matrix<T>();
}

template <class T, bool binary=false>
void write_matrix(const Matrix<T>& mat, const std::string& filename, const std::string& sep=" ") {
    File<binary> f(filename, ios_base::out);
    f.write_matrix(mat, sep);
}*/

// Strings for user display

template <class T>
std::string display_string(const Vector<T>& v, bool header=true) {
    int n = v.get_n();
    std::stringstream out;
    if (header) {
        out << "Vector of " << typeid(T).name() << 
        " (size " << n << ")" << std::endl;
    }
    if (n >= 10) {
        // First 3 entries
        for (int i = 0; i < 3; ++i) {
            out << v[i] << " ";
        }
        out << "... ";
        // Last 3 entries
        for (int i = n-3; i < n; ++i) {
            out << v[i] << " ";
        }
    } else {
        for (int i = 0; i < n; ++i) {
            out << v[i] << " ";
        }
    }
    out << std::endl;
    return out.str();
}

template <class T>
std::string display_string(const Matrix<T>& mat) {
    int rows = mat.get_rows();
    int cols = mat.get_cols();
    std::stringstream out;
    out << "Matrix of " << typeid(T).name() <<
        " (size " << rows << " x " << cols << ")" << std::endl;
    if (rows >= 10) {
        for (int i = 0; i < 3; ++i) {
            out << display_string(mat.row(i), false);
        }
        out << "..." << std::endl;
        for (int i = rows-3; i < rows; ++i) {
            out << display_string(mat.row(i), false);
        }
    } else {
        for (int i = 0; i < rows; ++i) {
            out << display_string(mat.row(i), false);
        }
    }
    out << std::endl;
    return out.str();
}

#endif