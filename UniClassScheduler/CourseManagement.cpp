/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: CourseData cpp.
**************************************************/
#include "CourseManagement.h"

CourseManagement::CourseManagement(){
    
}

CourseManagement::~CourseManagement(){
    
}

void CourseManagement::setMC(int value){
    MC=value;
}

int CourseManagement::getMC(){
    return MC;
}

void CourseManagement::Init(){

    typedef std::vector<int> V;
    CourseAssignOrders.clear();
    V v;
    int len=getMC();
    for(int i=0;i<len;++i){
        v.push_back(i);
    }
    CourseAssignOrders.push_back(v);
    /*
    do
    {
        CourseAssignOrders.push_back(v);
    }while(std::next_permutation(v.begin(),v.end()));
    */
}

int CourseManagement::TotalHours(){
    int hours=0;
    for(int i=0;i<(int)cHours.size();i++){
        hours+=cHours[i];
    }
    return hours;
}

void CourseManagement::setcHours(std::vector<int> value){
    cHours=value;
}

void CourseManagement::setcDefaultHours(std::vector<int> value){
    cDefaultHours=value;
}

void CourseManagement::setcNames(std::vector<std::string> value){
    cNames=value;
}

std::vector<int> CourseManagement::getcHours(){
    return cHours;
}

std::vector<int> CourseManagement::getcDefaultHours(){
    return cDefaultHours;
}

std::vector<std::string> CourseManagement::getcNames(){
    return cNames;
}

std::vector< std::vector<int> > CourseManagement::getCourseAssignOrders(){
    return CourseAssignOrders;
}

void CourseManagement::setCourseAssignOrders(std::vector< std::vector<int> > value){
    CourseAssignOrders=value;
}
