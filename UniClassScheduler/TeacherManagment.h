/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: TeacherManagment header.
**************************************************/

#ifndef TeacherManagment_h
#define TeacherManagment_h


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
#include "Utility.h"
#include "Teacher.h"

typedef std::vector<std::vector<int> > IntMatrix;
class TeacherManagment{
public:
    int DefaultHours;
    int NL;                     // number of lecturers
    std::vector<std::string> lNames;      // names of lecturers
    IntMatrix TL;
    IntMatrix LP;
    std::vector<Teacher> Lecturers;
    std::vector< std::vector<int> > LectureAssignOrders;
    TeacherManagment();
    ~TeacherManagment();
    void Init();
    
    void setDefaultHours(int value);
    int getDefaultHours();
    
    void setNL(int value);
    int getNL();
    
    void setTL(IntMatrix value);
    IntMatrix getTL();
    
    void setLP(IntMatrix value);
    IntMatrix getLP();
    
    void setlNames(std::vector<std::string> value);
    std::vector<std::string> getlNames();
    
    //Checking whether the lecturer is available on that session
    bool IsLecturerAvaliable(int l,int time);
    //Set all Teacher rest and reset the remain hours.
    void AllRest(int time,bool must);
    //Allocated lecturer at that time.
    bool Allocated(int l,int time);
    //Checking whether the teacher is allocated at that time.
    bool IsTeacherLocated(int c,int l);
    //Checking the lecturer preferences.
    bool IsLecturerPreferences(int l, int t, int p);
    std::vector< std::vector<int> > getLectureAssignOrders();
    
};
#endif /* TeacherManagment_h */
