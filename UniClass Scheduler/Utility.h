/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: Utility class.
**************************************************/

#ifndef Utility_h
#define Utility_h

// Include files
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdio.h>

class Utility{
public:
    // It is used to convert value to string
    template <typename T>
    std::string to_string(T value);
    // It is the right way to split a string into a vector of strings
    static std::vector<std::string> split(std::string s, std::string t);
    // Checks whether input string is a decimal digit character.
    static bool is_digit(std::string str);
    // Checks whether input string is a positive number.
    static bool is_positive_number(std::string str);
    // Trim all the whitespace from string
    static std::string trim(std::string str);
    // Find integer from the vector
    static bool FindInt(std::vector<int> v,int val);
    // Selection sort
    static std::vector<int> sel_sort(std::vector<int> v);
    // Swap two value
    static void swap_int(int *x, int *y);
    // Maximum value of integer
    static int _INT_MAX;
    // Create file and write
    static void file_w(std::string file,std::string s);
    // Check whether the file exist
    static bool is_file_e(char *fileName);

};

class HashMap {
    std::unordered_map<const void *, const void *> htmap;
public:
    //int one=1;
    //ht.put("one", &one);
    void put(const void *key, const void *value) {
            htmap[key] = value;
    }
    bool containsKey(const void *key){
        std::unordered_map<const void *, const void *>::const_iterator got = htmap.find(key);
      if ( got == htmap.end() )
        return false;
      else
        return true;
    }
    //*(int *)ht.get(1);
    const void *get(const void *key) {
            return htmap[key];
    }
};

#endif /* Utility_h */
