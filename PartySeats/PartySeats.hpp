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

typedef int int_type;

//g++ -std=c++98 PartySeats.hpp -o PartySeats
//./PartySeats


using namespace std;

class PartySeats {
 public:
	//It is the right way to split a string into a vector of strings
	vector<string> split(string str, string token){
		vector<string>result;
		while(str.size()){
			unsigned int index = str.find(token);
			if(index <str.size() && index>=0){
				result.push_back(str.substr(0,index));
				str = str.substr(index+token.size());
				if(str.size()==0){
					result.push_back(str);
					break;
				}
			}else{
				result.push_back(str);
				break;
			}
		}
		return result;
	}
	vector <string> seating(vector <string> attendees) {
		sort(attendees.begin(), attendees.end());
		queue<string> boys;
		queue<string> girls;
		for(int i=0;i<attendees.size();i++){
				vector<string> attendee=split(attendees[i]," ");
				if(attendee[1]=="boy"){
					boys.push(attendee[0]);
				}else if(attendee[1]=="girl"){
					girls.push(attendee[0]);
				}
		}
		 if ((boys.size() != girls.size()) || ((boys.size() % 2) != 0)) {
		 		 vector<string> empty;
				 return empty;
		 }
		 vector<string> result(attendees.size() + 2);
		 result[0] = "HOST";
		 bool isBoyNext = false;
		 for (int i = 1; i < result.size(); i++) {
						

				 if (i == (result.size() / 2)) {
						 result[i] = "HOSTESS";

				 } else {

						 if (isBoyNext) {
						 		if((!boys.empty())){
									result[i] = boys.front();
									boys.pop();
								}
						 } else {
						 		if((!girls.empty())){
									result[i] = girls.front();
									girls.pop();
								}
						}
				
				 }
				 isBoyNext = !isBoyNext;
				 
		 }
		 return result;
	}

};
