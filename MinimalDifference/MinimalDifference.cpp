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
#include <algorithm>
#include <functional>
#include <sstream>
#include <complex>
#include <stack>
#include <queue>
using namespace std;

static const double EPS = 1e-5;

#define times(i,n) for(int i = 0; i < n; i++)
#define rtimes(i,n) for(int i = n; i >= 0; i--)
#define each(c,i) for(typeof((c).begin()) i=(c).begin(); i!=(c).end(); ++i)
#define range_each(b,e,i) for(typeof(b) i=b; i!=e; ++i)
#define find_a(x,ary) find(ary.begin(),ary.end(),x)
#define exist_in(ary,x) (find(ary.begin(),ary.end(),x) != ary.end())

//g++ MinimalDifference.cpp -o MinimalDifference
//./MinimalDifference

/*
Problem (5) MinimalDifference:

According to the question given three integers A, B and C. 
And Return the integer X between A and B,
And I also need to find the digit sum of X and the digit sum of C. 
So first of all, I have to create total function to find out the digit sum of X and the digit sum of C;
And then I need to use a loop to the difference between  the digit sum of X and the digit sum of C;
Hence, I can find out the smallest among A and B.

Pseudo-Code:

class MinimalDifference
  public procedure findNumber(int A, int B, int C)
    int num=A;
    int ans = 0;
    int ansd = -1;
    int sum;
    int totalc = total(C);
    Repeat(num <= B){
      sum = total(num);
      if (totalc == sum){
        return num;
      }
      int tmp=totalc-sum;
      if(tmp<0){
        tmp=(-1)*tmp;
      }
      if (tmp < ansd ||
        ansd == -1) {
        ansd = tmp;
        ans = num;
      }
      num+=1;
    EndRepeat
    return ans;
  endprocedure
EndClass

Reflection:

This program  use brute force. Because the brute force solution is simply to find out each difference among them and then find out the small value  as small as possible.

This is particularly efficient because it is possible to process all possibility through brute force algorithms.



*/


class MinimalDifference {
public:
  int total(int value){
    int i;
    int ans = 0;
    for(int a=value;a>0;a/=10){
      i = a%10; 
      ans += i;
    }
    return ans;
  }

  int findNumber(int A, int B, int C)
  {
    int num=A;
    int ans = 0;
    int ansd = -1;
    int sum;
    int totalc = total(C);
    while(num <= B){
      sum = total(num);
      if (totalc == sum){
        return num;
      }
      int tmp=totalc-sum;
      if(tmp<0){
        tmp=(-1)*tmp;
      }
      if (tmp < ansd ||
        ansd == -1) {
        ansd = tmp;
        ans = num;
      }
      num+=1;
    }
    return ans;
  }

// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 1; int Arg1 = 9; int Arg2 = 10; int Arg3 = 1; verify_case(0, Arg3, findNumber(Arg0, Arg1, Arg2)); }
	void test_case_1() { int Arg0 = 11; int Arg1 = 20; int Arg2 = 20; int Arg3 = 11; verify_case(1, Arg3, findNumber(Arg0, Arg1, Arg2)); }
	void test_case_2() { int Arg0 = 1; int Arg1 = 1; int Arg2 = 999; int Arg3 = 1; verify_case(2, Arg3, findNumber(Arg0, Arg1, Arg2)); }
	void test_case_3() { int Arg0 = 100; int Arg1 = 1000; int Arg2 = 99; int Arg3 = 189; verify_case(3, Arg3, findNumber(Arg0, Arg1, Arg2)); }
	void test_case_4() { int Arg0 = 1987; int Arg1 = 9123; int Arg2 = 1; int Arg3 = 2000; verify_case(4, Arg3, findNumber(Arg0, Arg1, Arg2)); }
// END CUT HERE

};

// BEGIN CUT HERE
int main() {
  MinimalDifference ___test;
  ___test.run_test(-1);
}
// END CUT HERE