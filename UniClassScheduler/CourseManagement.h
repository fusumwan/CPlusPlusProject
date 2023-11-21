/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: CourseData cpp.
**************************************************/
#ifndef CourseManagement_h
#define CourseManagement_h
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
#include <stdio.h>
#include <string>
#include <vector>
class CourseManagement{
public:
    int MC;
    std::vector<int> cHours;         // hours per course
    std::vector<int> cDefaultHours;         // hours per course
    std::vector<std::string> cNames;      // names of courses
    std::vector< std::vector<int> > CourseAssignOrders;
    CourseManagement();
    ~CourseManagement();
    void Init();
    void setMC(int value);
    int getMC();
    int TotalHours();
    
    void setcHours(std::vector<int> value);
    void setcDefaultHours(std::vector<int> value);
    void setcNames(std::vector<std::string>  value);
    std::vector<int> getcHours();
    std::vector<int> getcDefaultHours();
    std::vector<std::string> getcNames();
    std::vector< std::vector<int> > getCourseAssignOrders();
    void setCourseAssignOrders(std::vector< std::vector<int> > value);
    
};
#endif /* CourseManagement_h */
