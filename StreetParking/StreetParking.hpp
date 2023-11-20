#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cfloat>
#include <map>
#include <utility>
#include <set>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <sstream>
#include <complex>
#include <stack>
#include <queue>




/*

Problem (3) StreetParking:
Initial thoughts:

In this program I am nesting two loops. If the string has n characters in the street name, 
the outer loop runs n times and the inner loop runs n times for each iteration of the outer loop, 
giving us n²​​ total comparsion with private driveway, bus stop, and side-street. 
Thus this function runs in O(n²) time. If the array has 10 characters, I have to compare strings more than 100 times. 


Pseudo-Code:


class StreetParking 
  public procedure freeParks (string street)
    parkinglist
    Repeat i<-0 until i<street.size()
      Driveway(parkinglist,i)
      BusStop(parkinglist,i)
      Street(parkinglist,i)
    EndRepeat
    total = 0;
    Repeat i<-0 until i<street.size()
      if(parkinglist[i] == 1)
        total+=1;
    return total; 
  endclass


Reflection:

Example "---B--S-D--S--"

I would like to choose brute force. My freeParks functions have O(n) runtime, 
Sometimes the size of string is an actual number for each iteration of outer loop.
I can compare each position. So it works this way. Let make a example to explain.

First of all, creating a integer list for couting the parking place and the default value is 1 in each parking place. 1 mean parkingable and 0 is not avaliable for parking.
Checking the parking place at 0 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 0 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 0 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 5 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 5 meter(s) is not directly in front of a bus stop. 
 And then if the parking place at 15 meters before a bus stop and set zero. 
 And then checking the parking place at 5 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 10 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 10 meter(s) is not directly in front of a bus stop. 
 And then if the parking place at 15 meters before a bus stop and set zero. 
 And then checking the parking place at 10 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 15 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 15 meter(s) is not directly in front of a bus stop. 
 And then if the parking place at 15 meters before a bus stop and set zero. 
 And then checking the parking place at 15 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 20 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 20 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 20 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 25 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 25 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 25 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 30 meters before a side-street and set zero. 
 Checking the parking place at 30 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 30 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 30 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 30 meters before a side-street and set zero. 
 Checking the parking place at 35 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 35 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 35 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 30 meters before a side-street and set zero. 
 Checking the parking place at 40 meter(s) is not directly in front of a private driveway. 
 If the parking place is at a private driveway then set zero. 
 And then checking the parking place at 40 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 40 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 45 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 45 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 45 meter(s) is not directly in front of a side-street. 
 Checking the parking place at 50 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 50 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 50 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 55 meters before a side-street and set zero. 
 Checking the parking place at 55 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 55 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 55 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 55 meters before a side-street and set zero. 
 Checking the parking place at 60 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 60 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 60 meter(s) is not directly in front of a side-street. 
 And then if the parking place at 55 meters before a side-street and set zero. 
 Checking the parking place at 65 meter(s) is not directly in front of a private driveway. 
 And then checking the parking place at 65 meter(s) is not directly in front of a bus stop. 
 And then checking the parking place at 65 meter(s) is not directly in front of a side-street. 
 Finally, the program have to count how much the parking place at street.
 So the total parking place is 4.

*/


using namespace std;


class StreetParking {
 public:
  string _street;
  vector<int> Driveway(vector<int> parkinglist,int i){
    if(_street[i] == 'D') {
        parkinglist[i] = 0;
      }
      return parkinglist;
  }
  vector<int> BusStop(vector<int> parkinglist,int i){
    for(int j=0;j<=2;++j){
        int p = i + j;
        if(p < 0 || p >= (int)_street.size()) {
          continue;
        }
        if(_street[p] == 'B') {
          parkinglist[i] = 0;
        }
      }
      return parkinglist;
  }
  vector<int> Street(vector<int> parkinglist,int i){
    for(int j=-1;j<=1;++j){
      int p = i + j;
      if(p < 0 || p >= (int)_street.size()) {
        continue;
      }
      if(_street[p] == 'S') {
        
        parkinglist[i] = 0;
      }
    }
    return parkinglist;
  }
  int freeParks(string street) {
    _street=street;
    // -- main code --
    vector<int> parkinglist(street.size(), 1);
    for(int i=0;i<street.size();++i){
      parkinglist=Driveway(parkinglist,i);
      parkinglist=BusStop(parkinglist,i);
      parkinglist=Street(parkinglist,i);
    }
    
    int total = 0;
    for(int i=0;i<street.size();i++){
      if(parkinglist[i] == 1){
        total+=1;
      }
    }
    return total; 
  }

};
