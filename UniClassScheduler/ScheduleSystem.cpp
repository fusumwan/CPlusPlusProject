/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: ScheduleSystem cpp
**************************************************/
#include "Utility.h"
#include "CourseManagement.h"
#include "ScheduleSystem.h"
#include "TeacherManagment.h"

ScheduleSystem::ScheduleSystem(){

}

ScheduleSystem::~ScheduleSystem(){
    
}

void ScheduleSystem::init(int mode){
    switch (mode) {
        case 1:
            LunchHour=4;
            DayHours=8;
            Course.cHours=cHours;
            Course.cDefaultHours=cHours;
            Course.cNames=cNames;
            Course.setMC(getMC());
            Course.Init();
            Lecturer.setLP(LP);
            Lecturer.setTL(TL);
            Lecturer.setNL(getNL());
            Lecturer.setlNames(lNames);
            Lecturer.setDefaultHours(2);
            Lecturer.Init();
            
            RoomResources.clear();
            for(int j=0;j<40;j++){
                RoomResources.push_back(rooms);
            }
            break;
        case 2:
            Timetable.clear();
            for(int i=0;i<getMC();i++){
                std::vector<int> row;
                for(int j=0;j<40;j++){
                    row.push_back(-1);
                }
                Timetable.push_back(row);
            }
            RoomResources.clear();
            for(int j=0;j<40;j++){
                RoomResources.push_back(rooms);
            }
            Lecturer.AllRest(0, true);
            Course.cHours=Course.cDefaultHours;
            
            break;
        default:
            break;
    }
    
}

int ScheduleSystem::getMC(){
    return mC;
}

int ScheduleSystem::getNL(){
    return nL;
}

bool ScheduleSystem::IsRoomAvailable(int i){
    if(RoomResources[i]>0)
        return true;
    return false;
}

void ScheduleSystem::RoomReservation(int r){
    if(IsRoomAvailable(r)){
        RoomResources[r]-=1;
    }
}

bool ScheduleSystem::IsTeacherLocated(int c,int l){
    return Lecturer.IsTeacherLocated(c,l);
}

bool ScheduleSystem::IsLecturerPreferences(int l, int t, int p){
    return Lecturer.IsLecturerPreferences(l, t, p);
}

bool ScheduleSystem::IsCourseAvaliable(int c){
    if(c<(int)Course.cHours.size()){
        if(Course.cHours[c]>0){
            return true;
        }
    }
    return false;
}

void ScheduleSystem::CourseHeld(int c){
    if(IsCourseAvaliable(c)){
        Course.cHours[c]-=1;
    }
}

bool ScheduleSystem::IsSeparateSession(int c,int t){
    int d=(int)t/8;
    int len=(d+1)*8;
    for(int i=d*8;i<len;i++){
        if(Timetable[c][i]>-1){
            if(t==(i+1) && t>i && t<len){
                return false;
            }
            else{
                return true;
            }
        }
    }
    return false;
}

bool ScheduleSystem::IsWrongAssignedCourse(int c,int t, int max){
    int count=0;
    int d=(int)t/8;
    int len=(d+1)*8;
    for(int i=d*8;i<len;i++){
        if(Timetable[c][i]>-1){
            count+=1;
        }
    }
    return (count>=max)?true:false;
}

int ScheduleSystem::Cell(int c,int t,int torder, int p){
    if(LunchHour!=(t+1)%8 &&
       IsCourseAvaliable(c) &&
       !IsSeparateSession(c,t) &&
       !IsWrongAssignedCourse(c,t,2) &&
       Course.TotalHours()>0){
        if(IsRoomAvailable(t)){
            std::vector<int> LecturerOrder=Lecturer.getLectureAssignOrders()[torder];
            for(int l=0;l<LecturerOrder.size();l++){
                int lect=LecturerOrder[l];
                if(c==0 && lect==3){
                    c=0;
                }
                if(IsTeacherLocated(c,lect)){
                    
                    if(p>0){
                        if(IsLecturerPreferences(lect,t,p)){
                            if(Lecturer.Allocated(lect,t)){
                                RoomReservation(t);
                                CourseHeld(c);
                                return lect;
                            }
                        }
                    }
                    else{
                        if(IsLecturerPreferences(lect,t,1) ||
                           IsLecturerPreferences(lect,t,2) ||
                           IsLecturerPreferences(lect,t,5)){
                            if(Lecturer.Allocated(lect,t)){
                                RoomReservation(t);
                                CourseHeld(c);
                                return lect;
                            }
                        }
                        
                    }
                }
            }
        }
    }
    Lecturer.AllRest(t,false);
    return -1;
}

void ScheduleSystem::ArrangeTimetable(int corder,int torder,int p,bool force){
    
    if(Course.TotalHours()>0){
        std::vector<int> CourseOrder=Course.getCourseAssignOrders()[corder];
        for(int i=0;i<getMC();i++){
            int c=CourseOrder[i];
            for(int j=0;j<40;j++){
                if(Timetable[c][j]==-1 || force){
                    int c1=Cell(c,j,torder,p);
                    Timetable[c][j]=c1;
                }
            }
        }
    }
}

void ScheduleSystem::CreateTimetable(){
    Timetable.clear();
    for(int i=0;i<getMC();i++){
        std::vector<int> row;
        for(int j=0;j<40;j++){
            row.push_back(-1);
        }
        Timetable.push_back(row);
    }
    ArrangeTimetable(0,0,-1,false);
    std::vector<int> corderlist;
    do{
        corderlist=AssignCourseHoursLeft(corderlist);
        if(corderlist.size()>getMC())
            corderlist.clear();
    }
    while(Course.TotalHours()>0);
    PrintCourseHours();
}


std::vector<int>  ScheduleSystem::AssignCourseHoursLeft(std::vector<int> corderlist){
    
    if(Course.TotalHours()>0){
        std::vector<int> morderlist;
        for(int c=0;c<(int)Course.getcHours().size();c++){
           int hours=Course.getcHours()[c];
           if(hours>0){
               corderlist.push_back(c);
           }
        }
        morderlist=corderlist;
        //sort(corderlist.begin(),corderlist.end());
        for(int i=0;i<(int)Course.getcHours().size();i++){
           if(!Utility::FindInt(morderlist,i)){
               morderlist.push_back(i);
           }
        }
        std::vector< std::vector<int> > CourseAssignOrders=Course.getCourseAssignOrders();
        CourseAssignOrders.clear();
        CourseAssignOrders.push_back(morderlist);
        Course.setCourseAssignOrders(CourseAssignOrders);
        std::vector<int> CourseOrder=Course.getCourseAssignOrders()[0];
        int c=CourseOrder[0];
        int lect=-1;
        for(int i=0;i<40;i++){
            if(c<getMC()){
                if(Timetable[c][i]>0){
                    lect=Timetable[c][i];
                }
            }
        }
        init(2);
        
        if(lect>0 && lect<getNL()){
            
            int tlen=Lecturer.getLectureAssignOrders().size();
            int tstart=(((double)tlen)/getNL())*lect;
            ArrangeTimetable(0,tstart,-1,true);
            if(Course.TotalHours()==0){
                return corderlist;
            }
        }
        else{

            ArrangeTimetable(0,0,-1,true);
            if(Course.TotalHours()==0){
                return corderlist;
            }
        }
    }
    return corderlist;
}

void ScheduleSystem::PrintCourseHours(){
    if(Course.TotalHours()>0){
        printf("Total Hours Left:%d\n",Course.TotalHours());
        for(int i=0;i<(int)Course.getcHours().size();i++){
            std::string name=Course.getcNames()[i];
            int hours=Course.getcHours()[i];
            if(hours>0){
                printf("%s:%d\n",name.c_str(),hours);
            }
        }
    }
}

void ScheduleSystem::OutputSolution(){
    FILE * pFile;
    pFile = fopen (ucsSolnFileName.c_str(),"w");
    for (int i=0; i<Timetable.size(); i++)
    {
        std::string row_str="";
        for(int j=0;j<40; j++){
            if(row_str!="")
                row_str+=",";
            row_str+=std::to_string(Timetable[i][j]);
        }
        fprintf (pFile,"%s",row_str.c_str());
        if(i<Timetable.size()-1){
            fprintf (pFile,"\n");
        }
    }
    fclose (pFile);
}
