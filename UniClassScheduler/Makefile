all: 
	g++ -std=c++11 TeacherManagment.cpp Teacher.cpp CourseManagement.cpp CourseData.cpp Schedule.cpp ScheduleSystem.cpp Utility.cpp  -o schedule

schedule: TeacherManagment.o Teacher.o CourseManagement.o CourseData.o Schedule.o ScheduleSystem.o Utility.o
	g++ -std=c++11 TeacherManagment.o Teacher.o CourseManagement.o CourseData.o Schedule.o ScheduleSystem.o Utility.o -o schedule

CourseData.o: CourseData.h CourseData.cpp
	g++ -std=c++11 -c CourseData.cpp

CourseManagement.o: CourseManagement.h CourseManagement.cpp
	g++ -std=c++11 -c CourseManagement.cpp

Teacher.o: Utility.h Teacher.h Teacher.cpp
	g++ -std=c++11 -c Teacher.cpp

TeacherManagment.o: Utility.h TeacherManagment.h TeacherManagment.cpp
	g++ -std=c++11 -c TeacherManagment.cpp

Schedule.o: EvalUCS.h Utility.h ScheduleSystem.h Schedule.cpp
	g++ -std=c++11 -c Schedule.cpp

ScheduleSystem.o: Utility.h CourseManagement.h TeacherManagment.h ScheduleSystem.h ScheduleSystem.cpp
	g++ -std=c++11 -c ScheduleSystem.cpp

EvalUCS.o: Utility.h Utility.cpp
	g++ -std=c++11 -c Utility.cpp

clean:
	rm -rf *o
	rm -rf schedule