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

using namespace std;

//g++ ProblemsToSolve.cpp -o ProblemsToSolve
//./ProblemsToSolve

/*

Problem (3) ProblemsToSolve:

This problem is definitely at most O(n^2) because the question request me must first solve problem 0,

After solving each problem i, I must either move on to problem i+1 or skip ahead to problem i+2. 

Hence, I am nesting two loops to find the check the number of next problem. 

As I are not allow to skip more than one problem. So the result should equal to (the number of problem - the current index) / 2

And then check it whether is vaild or not.

Pseudo-Code:

class ProblemsToSolve {
	public procedure minNumber(vector <int> pleasantness, int variety)
			int i=1;
			int size = pleasantness.size();
			Repeat(i<pleasantness.size()){
				bool result = false;
				int j=0;
				Repeat(i>j){
					int r = 0;
					r += 2;
					r += (i - j - 1) / 2;
					if (j != 0){ 
						r++;
						r += (j - 1) / 2;
					}
					if(isFind(pleasantness[i],
						pleasantness[j],
						r,variety)){
						if(r<size){
								size=r;
							}
							result=true;
					}
					j+=1;
				}
				if (result){
					break;
				}
			i+=1;
		EndRepeat
		return size;
	endprocedure
endclass

Reflection:

I have to find the number between each problem

and then sorted the array, and then we need to check the difference between two problem number whether is greater than 2, 

and then sum up all the difference.

If I want to improve the performance, 

I would like to use recursive function.


*/




class ProblemsToSolve {
public:
	int happyvariety(int x,int y,int v){
			int tmp=x-y;
			if(tmp<0){
				tmp=tmp*(-1);
			}
			if(v<=tmp){
				return true;
			}
			return false;
	}

	bool isFind(int x, int y,int g,int v){
			if(happyvariety(
						x,
						y,
						v)){
							
							return true;
					}
					return false;
	}

	int minNumber(vector <int> pleasantness, int variety)
	{
			int i=1;
			int size = pleasantness.size();
			while(i<pleasantness.size())
			{
				bool result = false;
				int j=0;
				while(i>j){
					int r = 0;
					r += 2;
					r += (i - j - 1) / 2;
					if (j != 0){ 
						r++;
						r += (j - 1) / 2;
					}
					if(isFind(pleasantness[i],
						pleasantness[j],
						r,variety)){
						if(r<size){
								size=r;
							}
							result=true;
					}
					j+=1;
				}
				if (result){
					break;
				}
			i+=1;
		}
		return size;
	}

	 
	// BEGIN CUT HERE
public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {1, 2, 3}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; int Arg2 = 2; verify_case(0, Arg2, minNumber(Arg0, Arg1)); }
	void test_case_1() { int Arr0[] = {1, 2, 3, 4, 5}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 4; int Arg2 = 3; verify_case(1, Arg2, minNumber(Arg0, Arg1)); }
	void test_case_2() { int Arr0[] = {10, 1, 12, 101}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 100; int Arg2 = 3; verify_case(2, Arg2, minNumber(Arg0, Arg1)); }
	void test_case_3() { int Arr0[] = {10, 1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 9; int Arg2 = 2; verify_case(3, Arg2, minNumber(Arg0, Arg1)); }
	void test_case_4() { int Arr0[] = {6, 2, 6, 2, 6, 3, 3, 3, 7}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 4; int Arg2 = 2; verify_case(4, Arg2, minNumber(Arg0, Arg1)); }

	// END CUT HERE

};

// BEGIN CUT HERE
int main() {
ProblemsToSolve ___test;
___test.run_test(-1);
}