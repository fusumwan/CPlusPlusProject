/**************************************************
// Course: PSSD
// Weekly assignment
// Author: Sum Wan, FU
// Date: 2-11-2019
// Student ID: 1714470
// Description: Schedule cpp
**************************************************/


#include "Utility.h"
#include "ScheduleSystem.h"
#include "EvalUCS.h"

//./schedule simple1.ucs.txt fnl_soln.sch

//./schedule medium1.ucs.txt fnl_soln.sch

//./schedule medium2.ucs.txt fnl_soln.sch

// static variables.
// ***********************************
int ProblemUCS::rooms;                  // Teaching rooms
int ProblemUCS::mC;                     // Number of courses
vector<int> ProblemUCS::cHours;         // hours per course
vector<string> ProblemUCS::cNames;      // names of courses
int ProblemUCS::nL;                     // number of lecturers
vector<string> ProblemUCS::lNames;      // names of lecturers
IntMatrix ProblemUCS::TL;
IntMatrix ProblemUCS::LP;
IntMatrix Solution::Timetable;










// simple driver program
int main(int argc, char* argv[]) {

    ScheduleSystem Schedule;
    
    string ucsFileName="";
    string ucsSolnFileName="";
    ucsFileName=string(argv[1]);
    ucsSolnFileName=string(argv[2]);
    //ucsFileName=string("simple1.ucs.txt");
    //ucsFileName=string("medium2.ucs.txt");
    //ucsSolnFileName=string("fnl_soln.sch");
  
    ProblemUCS::readUCSInstance(ucsFileName);;
    double res ;   // place holder for result
    
    Schedule.ucsFileName=ucsFileName;
    Schedule.ucsSolnFileName=ucsSolnFileName;
    Schedule.rooms=ProblemUCS::rooms;
    Schedule.mC=ProblemUCS::mC;
    Schedule.cHours= ProblemUCS::cHours;
    Schedule.cNames=ProblemUCS::cNames;
    Schedule.nL=ProblemUCS::nL;
    Schedule.lNames=ProblemUCS::lNames;
    Schedule.TL=ProblemUCS::TL;
    Schedule.LP=ProblemUCS::LP;
    Schedule.init(1);
    Schedule.CreateTimetable();
    Schedule.OutputSolution();
 
    cout << "calling readSolution " << endl;
    // if a feasible solution can be read
    if ( Solution::readSolution(ucsSolnFileName)) {
    cout << "calling constrains " << endl;
    int constrains = Solution::checkConstraints(Solution::Timetable, ProblemUCS::rooms, ProblemUCS::cHours, ProblemUCS::LP, ProblemUCS::cNames, ProblemUCS::lNames);
    if (constrains == 0)
         cout << "your solution is feasible" << endl;
      else
         cout << "your solution is not feasible" << endl;
        
      res =  Solution::getFitnessValue(Solution::Timetable, ProblemUCS::mC,ProblemUCS::LP,ProblemUCS::cHours,constrains);
      cout << "Fitness value " <<  res << endl;
    }
    else {
          cout << "Solution file is not readable " << endl;
    }
    //Solution::printTimetable(Schedule.Timetable,ProblemUCS::cNames,ProblemUCS::lNames);
}


