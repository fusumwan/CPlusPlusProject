/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: Teacher cpp.
**************************************************/
#include "Utility.h"
#include "Teacher.h"

Teacher::Teacher(){
    Name="";
    LastTime=0;
    DefaultHours=2;
}

Teacher::~Teacher(){
    
}

void Teacher::setName(std::string value){
    Name=value;
}

void Teacher::setLastTime(int value){
    LastTime=value;
}

void Teacher::setRemainHours(std::map<int, int> value){
    RemainHours=value;
}

void Teacher::setDefaultHours(int value){
    DefaultHours=value;
}

std::string Teacher::getName(){
    return Name;
}

int Teacher::getLastTime(){
    return LastTime;
}

std::map<int, int> Teacher::getRemainHours(){
    return RemainHours;
}

int Teacher::getDefaultHours(){
    return DefaultHours;
}

void Teacher::Init(){
    RemainHours.clear();
    for(int d=0;d<5;d++){
        RemainHours.insert(std::pair<int, int>(d, DefaultHours));
    }
}


//After rest, the remain hours will back to default hours.
void Teacher::Rest(int time,bool must){
    if(must){
        RemainHours.clear();
        for(int d=0;d<5;d++){
            RemainHours.insert(std::pair<int, int>(d, DefaultHours));
        }
    }
    LastTime=time;
}

//This function is used to record daily working hours.
int Teacher::UsedHour(int time){
    int d=(int)time/8;
    if(FindRemainHours(d)>0){
        RemainHours.at(d)-=1;
        return RemainHours.at(d);
    }
    return -1;
}
//This function is used to find remain working hours of the lecturer.
int Teacher::FindRemainHours(int d){
    std::map<int, int>::iterator itr;
    for (itr = RemainHours.begin(); itr != RemainHours.end(); ++itr) {
        if(itr->first==d){
            return itr->second;
        }
    }
    return -1;
}
