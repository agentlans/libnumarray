#ifndef _CONVERSION
#define _CONVERSION

#include <string>
#include <algorithm>

// C++ only
char* string_to_char(const std::string& s) {
    int n = s.size();
    char* ptr = new char[n+1];
    std::copy(s.begin(), s.end(), ptr);
    ptr[n] = '\0';
    return ptr;
}

#endif