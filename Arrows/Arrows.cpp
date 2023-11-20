#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <set>
#include <cctype>
#include <queue>
#include <stack>
using namespace std;
//g++ -std=c++98 Arrows.cpp -o Arrows
//./Arrows


/*
Problem (1) Arrows:
Initial thoughts:

In this question, there have several kind of arrows. As the arrow is defined as a "greater/less than" character ('>','<') 

followed by zero or more consecutive hyphens (' - ') or equal characters ('='). 

So this program I am nesting two loops. If the string has < > characters in the string I have to check character whether is arrow,

the outer loop runs l times and the inner loop runs i+l times for each iteration of the outer loop,

and l equal to length of string, and i+l also equal to length of string.

Pseudo-Code:

class Arrows{

public:
	public procedure  erase(string str,char ch){
		str.erase(remove(str.begin(), str.end(), ch), str.end());
		if(str=="" || str.empty()) return true;
		return false;
	endprocedure
	public procedure  same(string str) {
		return erase(str, '-') || erase(str, '=');
	endprocedure
	public procedure  arrow(string str) {
		char f = str.at(0);
		char l = str.at(str.length() - 1);
		return (f == '<' && same(str.substr(1)))
				|| (l == '>' && same(str.substr(0,
						str.length() - 1)));
	endprocedure
	public procedure  longestArrow(string s){
		Repeat (int l = s.length(); l > 0; l--) {
			Repeat (int i = 0; i + l <= s.length(); i++) {
				if (arrow(s.substr(i, i + l))) {
					return l;
				}
			EndRepeat
		EndRepeat
		return -1;
	endprocedure
endclass


Reflection:

I would like to use the brute force approach would be to check all type of arrows. 

This approach giving us n²​​ total comparsion within all type arrow.

Thus this function runs in O(n²) time.  

For example, the following are arrows (for clarity only): 

"==>", "< - ", "<", "<===", "--->", ">". 

The length of the arrow is the number of characters it contains.

Then time, I would like to use recursive function to solve this problem. 

*/

class Arrows{
public:
	bool erase(string str,char ch){
		str.erase(remove(str.begin(), str.end(), ch), str.end());
		if(str=="" || str.empty()) return true;
		return false;
	}
	bool same(string str) {
		return erase(str, '-') || erase(str, '=');
	}
	bool arrow(string str) {
		char f = str.at(0);
		char l = str.at(str.length() - 1);
		return (f == '<' && same(str.substr(1)))
				|| (l == '>' && same(str.substr(0,
						str.length() - 1)));
	}
	int longestArrow(string s){
		for (int l = s.length(); l > 0; l--) {
			for (int i = 0; i + l <= s.length(); i++) {
				if (arrow(s.substr(i,l))) {
					return l;
				}
			}
		}
		return -1;
	}
  // BEGIN CUT HERE
public:
  void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
private:
  template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
  void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
  void test_case_0() { string Arg0 = "<--->--==>"; int Arg1 = 4; verify_case(0, Arg1, longestArrow(Arg0)); }
  void test_case_1() { string Arg0 = "<<<<<<<<<<"; int Arg1 = 1; verify_case(1, Arg1, longestArrow(Arg0)); }
  void test_case_2() { string Arg0 = "----==-"; int Arg1 = -1; verify_case(2, Arg1, longestArrow(Arg0)); }
  void test_case_3() { string Arg0 = "=>-->---------------------"; int Arg1 = 3; verify_case(3, Arg1, longestArrow(Arg0)); }

  // END CUT HERE
};

// BEGIN CUT HERE
int main() {

	Arrows ___test;
	___test.run_test(-1);
}
// END CUT HERE
