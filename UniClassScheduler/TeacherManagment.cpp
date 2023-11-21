/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: TeacherManagment cpp.
**************************************************/
#include "Utility.h"
#include "TeacherManagment.h"

TeacherManagment::TeacherManagment(){
    DefaultHours=2;
    NL=0;
}

TeacherManagment::~TeacherManagment(){
    
}

void TeacherManagment::Init(){
    Lecturers.clear();
    for(int i=0;i<(int)lNames.size();i++){
        Teacher _Lecturer;
        _Lecturer.setName(lNames[i]);
        _Lecturer.setLastTime(0);
        _Lecturer.setDefaultHours(DefaultHours);
        _Lecturer.Init();
        Lecturers.push_back(_Lecturer);
    }
    
    typedef std::vector<int> V;
    LectureAssignOrders.clear();
    V v;
    for(int i=0;i<getNL();++i){
        v.push_back(i);
    }
    do
    {
        LectureAssignOrders.push_back(v);
    }while(std::next_permutation(v.begin(),v.end()));
}

void TeacherManagment::setDefaultHours(int value){
    DefaultHours=value;
}

int TeacherManagment::getDefaultHours(){
    return DefaultHours;
}

void TeacherManagment::setNL(int value){
    NL=value;
}
int TeacherManagment::getNL(){
    return NL;
}

void TeacherManagment::setTL(IntMatrix value){
    TL=value;
}

IntMatrix TeacherManagment::getTL(){
    return TL;
}

void TeacherManagment::setLP(IntMatrix value){
    LP=value;
}

IntMatrix TeacherManagment::getLP(){
    return LP;
}

void TeacherManagment::setlNames(std::vector<std::string> value){
    lNames=value;
}
std::vector<std::string> TeacherManagment::getlNames(){
    return lNames;
}

bool TeacherManagment::IsLecturerAvaliable(int l,int time){
    if(l<(int)Lecturers.size()){
        int d=(int)time/8;
        if(Lecturers[l].FindRemainHours(d)>0){
            return true;
        }
    }
    return false;
}

void TeacherManagment::AllRest(int time,bool must){
    for(int i=0;i<(int)Lecturers.size();i++){
        Lecturers[i].Rest(time,must);
    }
}

bool TeacherManagment::Allocated(int l,int time){
    if(IsLecturerAvaliable(l,time)){
        int remain=Lecturers[l].UsedHour(time);
        return (remain>=0)?true:false;
    }
    return false;
}


bool TeacherManagment::IsTeacherLocated(int c,int l){
    if(c<TL.size()){
        if(l<TL[c].size() && l<getNL()){
            if(TL[c][l]==1)
                return true;
        }
    }
    return false;
}

bool TeacherManagment::IsLecturerPreferences(int l, int t, int p){
    if(l<LP.size()){
        if(t<LP[l].size()){
            if(LP[l][t]==p)
                return true;
        }
    }
    return false;
}
std::vector< std::vector<int> > TeacherManagment::getLectureAssignOrders(){
    return LectureAssignOrders;
}

