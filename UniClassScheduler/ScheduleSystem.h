/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: ScheduleSystem header
**************************************************/

#ifndef ScheduleSystem_h
#define ScheduleSystem_h
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
#include "CourseManagement.h"
#include "TeacherManagment.h"

typedef std::vector<std::vector<int> > IntMatrix;

class ScheduleSystem{
public:
    std::string ucsFileName;
    std::string ucsSolnFileName;
    int LunchHour;                  // Lunch
    int DayHours;
    int rooms;                  // Teaching rooms
    int mC;                     // Number of courses
    std::vector<int> cHours;         // hours per course
    std::vector<std::string> cNames;      // names of courses
    int nL;                     // number of lecturers
    std::vector<std::string> lNames;      // names of lecturers
    IntMatrix TL;
    IntMatrix LP;
    IntMatrix Timetable;
    CourseManagement Course;
    TeacherManagment Lecturer;
    std::vector<int> RoomResources;
    
    ScheduleSystem();
    ~ScheduleSystem();
    void init(int mode);
    int getMC();
    int getNL();
    //Checking whether room is available
    bool IsRoomAvailable(int i);

    //This function is used to record the room status.
    void RoomReservation(int r);

    //Checking whether teacher is allocated at that session.
    bool IsTeacherLocated(int c,int l);

    //Checking lecturer 's preferences
    bool IsLecturerPreferences(int l, int t, int p);

    //Checking wthether course is avaliable
    bool IsCourseAvaliable(int c);

    //This function is used to record how many sesssion of course per day.
    void CourseHeld(int c);
    

    //Check if you have more than two sessions per day.
    bool IsSeparateSession(int c,int t);
    //Check if the course assignment is wrong.
    bool IsWrongAssignedCourse(int c, int t, int max);
    int Cell(int i,int j,int order, int p);
    void ArrangeTimetable(int corder, int torder,int p,bool force);
    
    void CreateTimetable();
    std::vector<int> AssignCourseHoursLeft(std::vector<int> corderlist);
    void PrintCourseHours();
    void OutputSolution();
    
};
#endif /* ScheduleSystem_h */
