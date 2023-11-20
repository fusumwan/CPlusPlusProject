
#include <string>
#include <vector>
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
#include <algorithm>
#include <functional>
#include <sstream>
#include <complex>
#include <stack>
#include <queue>
using namespace std;
static const double EPS = 1e-9;
const int INF = 987654321;
typedef long long LL;
//g++ -std=c++11 HouseBuilding.cpp -o HouseBuilding
//./HouseBuilding

/*
Problem (3) HouseBuilding:
Initial thoughts:

According to the question, as the question given a string[] area. 

So first of all, I have to create a Node for each element to record all level of element,

Then I go ahead, to modify the data, I just use for a nesting loop to subtract '0' all elements in array in order to get the integer.

Then I save it to the 2-dimentional Node Array.

Each Node's data in the area represents the height of the corresponding square of the area. 

With 1 unit of effort, program can calculate the height of any square meter in its area up or down by 1 by 50 times. 

Returns the minimum total effort required to get a flat area.


Pseudo-Code:

class HouseBuilding {
	public procedure getMinimum(vector <string> area) {
		int ans = 99999999;
		int len=area.size();
		int len2=area[0].length();
		vector< vector<Node> > house_area;
		Repeat (int i = 0; i < len; i++) {
			vector<Node> house;
			Repeat (int j = 0; j < len2; j++) {
				Node n;
				n.data=area[i][j]-'0';
				house.push_back(n);
			EndRepeat
			house_area.push_back(house);
		EndRepeat
		Repeat (int i = 0; i <= 50; i++) {
			int number = 0;
			number=process(number,house_area,i);
			ans = (ans>number)? number:ans;
		EndRepeat
		return ans;
	endprocedure
endclass



Reflection:

I would like to use brute force solution to calculate the level of area to find out the effort. 

Because it is very fast to find out the answer.

*/



class Node{
public:
	int i;
	int effort;
	int data;
	Node(){
		i=0;
		effort=0;
		data=0;
	}
	~Node(){
	}
	int ceffort(){
		if (i>= data) { effort += i-data; } else { if(0<data-i-1){ effort += (data-i-1);}}
		return effort;
	}
};

class HouseBuilding {
	public:
	int process(int number,vector< vector<Node> > house_area,int i){
		for (int j = 0; j < house_area.size(); j++) {
			for (int k = 0; k < house_area[0].size(); k++) {
				house_area[j][k].i=i;
				house_area[j][k].effort=number;
				number=house_area[j][k].ceffort();
			}
		}
		return number;
	}
	int getMinimum(vector <string> area) {
		int ans = 99999999;
		int len=area.size();
		int len2=area[0].length();
		vector< vector<Node> > house_area;
		for (int i = 0; i < len; i++) {
			vector<Node> house;
			for (int j = 0; j < len2; j++) {
				Node n;
				n.data=area[i][j]-'0';
				house.push_back(n);
			}
			house_area.push_back(house);
		}
		for (int i = 0; i <= 50; i++) {
			int number = 0;
			number=process(number,house_area,i);
			ans = (ans>number)? number:ans;
		}
		return ans;
	}
  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"10",
 "31"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; verify_case(0, Arg1, getMinimum(Arg0)); }
	void test_case_1() { string Arr0[] = {"54454",
 "61551"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 7; verify_case(1, Arg1, getMinimum(Arg0)); }
	void test_case_2() { string Arr0[] = {"989"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; verify_case(2, Arg1, getMinimum(Arg0)); }
	void test_case_3() { string Arr0[] = {"90"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 8; verify_case(3, Arg1, getMinimum(Arg0)); }
	void test_case_4() { string Arr0[] = {"5781252",
 "2471255",
 "0000291",
 "1212489"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 53; verify_case(4, Arg1, getMinimum(Arg0)); }

// END CUT HERE

};

// BEGIN CUT HERE
int main() {
  HouseBuilding ___test;
  ___test.run_test(-1);
}
// END CUT HERE