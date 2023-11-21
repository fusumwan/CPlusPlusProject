/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: Teacher header.
**************************************************/

#ifndef Teacher_h
#define Teacher_h

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
#include <map>
#include "Utility.h"
class Teacher{
public:
    std::string Name;
    int LastTime;
    std::map<int, int> RemainHours;
    int DefaultHours;
    Teacher();
    ~Teacher();
    void setName(std::string value);
    void setLastTime(int value);
    void setRemainHours(std::map<int, int> value);
    void setDefaultHours(int value);
    
    std::string getName();
    int getLastTime();
    std::map<int, int> getRemainHours();
    int getDefaultHours();
    void Init();
    
    //After rest, the remain hours will back to default hours.
    void Rest(int time,bool must);
    //This function is used to record daily working hours.
    int UsedHour(int time);
    //This function is used to find remain working hours of the lecturer.
    int FindRemainHours(int d);
};


#endif /* Teacher_h */
