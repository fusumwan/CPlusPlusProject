/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: Utility class.
**************************************************/


// Include files
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Utility.h"

using namespace std;

// It is used to convert value to string
template <typename T>
string Utility::to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}


// It is the right way to split a string into a vector of strings
vector<string> Utility::split(string s, string t){
    vector<string> r;
    while(s.size()){
        unsigned long i = s.find(t);
        if(i<s.size()){
            r.push_back(s.substr(0,i));
            s = s.substr(i+t.size());
            if(s.size()==0){
                r.push_back(s);
                break;
            }
        }else{
            r.push_back(s);
            break;
        }
    }
    return r;
}

// Checks whether input string is a decimal digit character.
bool Utility::is_digit(std::string str){
    bool result=true;
    for(unsigned int i=0;i<str.length();i++){
        if(!isdigit(str[i])){
            result=false;
            break;
        }
    }
    return result;
}

// Checks whether input string is a positive number.
bool Utility::is_positive_number(std::string str){
    bool result=false;
    if(Utility::is_digit(str)){
        result=(stoi(str)>=0)?true:false;
    }
    return result;
}

// Trim all the whitespace from string
string Utility::trim(string str){
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    //return str.erase(str.find_last_not_of(" \n\r\t")+1);
    return str;
}

// Find integer from the vector
bool Utility::FindInt(vector<int> v,int val){
    if (std::find(v.begin(),v.end(),val)!=v.end()){
        return true;
    }
    return false;
}

// Selection sort
vector<int> Utility::sel_sort(vector<int> v){
    unsigned int m, i, j;
    // Move the boundaries of unclassified subarrays one by one
    for (i = 0; i < v.size()-1; i++){
        // Find the smallest element in an unsorted array
        m = i;
        for (j = i+1; j < v.size(); j++){
            if (v[j] < v[m]){
                m = j;
            }
        }
        // Use the first element to exchange the smallest element found
        swap_int(&v[m], &v[i]);
    }
    return v;
}



// Swap two value
void Utility::swap_int(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

// Maximum value of integer
int Utility::_INT_MAX=2147483647;

// Create file and write
void Utility::file_w(std::string f,std::string s){
    if(is_file_e(&f[0])){
        std::ofstream of (f,fstream::in | fstream::out | fstream::app);
        of<<s<<std::endl;
        of.close();
    }else{
        std::ofstream mof;
        mof.open (f);
        mof<<s<<std::endl;
        mof.close();
    }
}

// Check whether the file exist
bool Utility::is_file_e(char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}


