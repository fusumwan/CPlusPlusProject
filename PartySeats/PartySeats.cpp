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

//g++ -std=c++98 PartySeats.cpp -o PartySeats
//./PartySeats


// -- utility --
// C-style loop
#define FOR(x, a, b) for(int_type x = static_cast<int_type>(a); x < static_cast<int_type>(b); ++x)
#define FORN(x, n) FOR(x, 0, n)
// Ruby-style loop
#define TIMES(x, n) FOR(x, 0, n)
#define STEP(x, a, b, s) for(int_type x = static_cast<int_type>(a); s > 0 ? x <= static_cast<int_type>(b) : x >= static_cast<int_type>(b); x += static_cast<int_type>(s) )
#define UPTO(x, a, b) for(int_type x = static_cast<int_type>(a); x <= static_cast<int_type>(b); ++x)
#define DOWNTO(x, a, b) for(int_type x = static_cast<int_type>(a); x >= static_cast<int_type>(b); --x)
// debug
#define DUMP(x) std::cout << #x << " = " << x << std::endl;

using namespace std;

/*

Problem (4) PartySeats:
Initial thoughts:

First of all, I have to separate the boys from the girls by using a queue list, 
So we can easily pull out members in ascending order. 
And Ensure the arrangement is possible. And the number of boys and girls must be equal,
and both must be an even number.



Pseudo-Code:
class PartySeats
  public procedure seating(vector <string> attendees)
		sort(attendees.begin(), attendees.end());
		queue<string> boys;
		queue<string> girls;
		Repeat(i<-0;i<attendees.size())
			vector<string> attendee=split(attendees[i]," ");
			if(attendee[1]=="boy"){
				boys.push(attendee[0]);
			}else if(attendee[1]=="girl"){
				girls.push(attendee[0]);
			}
		EndRepeat
		if ((boys.size() != girls.size()) || ((boys.size() % 2) != 0)) {
		 	vector<string> empty;
			return empty;
		}
		result(attendees.size() + 2);
		result[0] = "HOST";
		bool isBoyNext = false;
		Repeat (i<= 1; i < result.size())
			// Seat the Hostess at the 1/2 way point.
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
		EndRepeat
		return result
	end procedure
endclass

Reflection:

This problem is definitely at most O(2n) because you have to check each word in string, 
and then I can use queue list to separate the boys from the girls.
If I want to improve the performance of code, 
I would like to doubly linked list to separate the boys from the girls.


*/

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
		// -- main code --

/*
				 * Separate the boys from the girls.  By using a PriorityQueue, we
				 * can easily pull out members in ascending order.
				 */
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

				 /*
				 * Ensure the arrangement is possible.
				 * The number of boys and girls must be equal, and both must be an
				 * even number.
				 */
				 
				 if ((boys.size() != girls.size()) || ((boys.size() % 2) != 0)) {
				 		 vector<string> empty;
						 return empty;
				 }
				 vector<string> result(attendees.size() + 2);
				 result[0] = "HOST";
				 bool isBoyNext = false;
				 for (int i = 1; i < result.size(); i++) {
 							
						 // Seat the Hostess at the 1/2 way point.
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

// BEGIN CUT HERE
/*
// PROBLEM STATEMENT
// It is time to arrange the seating around a circular table for a party.  We
want to alternate boys and girls around the table.  We have a list of all
the attendees and their genders. Each attendee is specified by a string that
consists of the name, followed by one space, followed by either "boy" or "girl".
In addition to the attendees, we need to seat 
the HOST (a boy) and the HOSTESS (a girl) with the HOSTESS directly across from
the HOST. That means that half the attendees should be on the HOST's left, and half on his right.  
Create a class PartySeats that contains a method seating that is given a 
vector <string> attendees that lists all the attendees and their genders.

The method
returns a vector <string> that gives the seating plan, starting with "HOST" and proceeding
clockwise around the table, including all the attendees and the HOSTESS. 
If there is more than one possible seating plan,
return the one that comes first lexicographically. "First lexicographically" means that each successive element in the return should be chosen to be the earliest alphabetically that is consistent with a legal seating plan. If there is no legal seating
plan, the return should contain 0 elements.
DEFINITION
Class:PartySeats
Method:seating
Parameters:vector <string>
Returns:vector <string>
Method signature:vector <string> seating(vector <string> attendees)
CONSTRAINTS
-attendees will contain between 1 and 50 elements inclusive
-each element of attendees will consists of a name followed by a single space followed by either "boy" or "girl". There will be no leading or trailing spaces.
-each name will contain between 1 and 20 characters inclusive
-each name will contain only uppercase letters 'A'-'Z'
-no name will be "HOST" or "HOSTESS"
EXAMPLES
0)
{"BOB boy","SAM girl","DAVE boy","JO girl"}
Returns: { "HOST",  "JO",  "BOB",  "HOSTESS",  "DAVE",  "SAM" }
	 A girl must follow the HOST, and JO comes earliest lexicographically. Then comes
	 a boy, and BOB is the earliest lexicographically. HOSTESS must come next so 
	 she can be opposite the HOST and then DAVE and SAM must follow in that order
	 to honor the alternating gender requirement.
1)
{"JOHN boy"}
Returns: { }
	 There are more boys than girls so we cannot alternate.
 
2)
{"JOHN boy","CARLA girl"}
Returns: { }
	 There is no way to alternate gender and also have the HOST sit directly
	 across from the HOSTESS
3)
{"BOB boy","SUZIE girl","DAVE boy","JO girl",
"AL boy","BOB boy","CARLA girl","DEBBIE girl"}
Returns: { "HOST",  "CARLA",  "AL",  "DEBBIE",  "BOB",  "HOSTESS",  "BOB",  "JO",  "DAVE",  "SUZIE" }
*/
// END CUT HERE

	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <string> &Expected, const vector <string> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { string Arr0[] = {"BOB boy","SAM girl","DAVE boy","JO girl"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = { "HOST",  "JO",  "BOB",  "HOSTESS",  "DAVE",  "SAM" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(0, Arg1, seating(Arg0)); }
	void test_case_1() { string Arr0[] = {"JOHN boy"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = { }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(1, Arg1, seating(Arg0)); }
	void test_case_2() { string Arr0[] = {"JOHN boy","CARLA girl"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = { }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(2, Arg1, seating(Arg0)); }
	void test_case_3() { string Arr0[] = {"BOB boy","SUZIE girl","DAVE boy","JO girl",
"AL boy","BOB boy","CARLA girl","DEBBIE girl"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = { "HOST",  "CARLA",  "AL",  "DEBBIE",  "BOB",  "HOSTESS",  "BOB",  "JO",  "DAVE",  "SUZIE" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(3, Arg1, seating(Arg0)); }

// END CUT HERE

/*
    	
{"BOB boy","SUZIE girl","DAVE boy","JO girl",
"AL boy","BOB boy","CARLA girl","DEBBIE girl"}

Returns: 

{
 "HOST",
 "CARLA","AL","DEBBIE","BOB",
 "HOSTESS",
 "BOB","JO","DAVE","SUZIE"
 }

*/


};

// BEGIN CUT HERE

int main()
{
	PartySeats test;
	test.run_test(-1);
	
	return 0;
}