// BEGIN CUT HERE
// END CUT HERE
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

//g++ -std=c++98 EggCartons.cpp -o EggCartons
//./EggCartons

using namespace std;

/*
Problem (2) EggCartons:
Initial thoughts:

According to question there are two types of egg boxes. 
One contains 6 eggs and the other contains 8 eggs. 
If we wants to buy a good egg. 
The program have to find to the minimum number of egg carts he must purchase.
This problem is definitely at most O((n/6)+(n/8)) because you have to count package of 6 or 8 eggs in box from N eggs.
so if I can find an O((n/6)+(n/8)) brute force solution. 

Pseudo-Code:
class EggCartons
  public procedure  minCartons(int n)
    int count = 0;
    int type1_total=8;
    int type2_total=6;
    While(n%type1_total!=0)
        n -= type2_total; count++;
        if (n < 0) return -1;
    EndWhile
    return count + n / 8;
  endprocedure
endclass

Reflection:

I would like to use the brute force  approach would be to count package of 6 or 8 eggs in box from N eggs.
There are O((n/6)+(n/8)) such combinations, so this will take O((n/6)+(n/8)) time.
I want to improve the solution, I think I have to try solving them in a new and another familiar language.

*/


typedef int t_int;


#define MOD_WHILE_NEQL(a, b, c) while(static_cast<t_int>(a) % static_cast<t_int>(b)!=static_cast<t_int>(c))

class EggCartons {
public:
  int minCartons(int n)
  {
    int count = 0;
    int type1_total=8;
    int type2_total=6;
    MOD_WHILE_NEQL (n,type1_total,0){
        n -= type2_total; count++;
        if (n < 0) return -1;
    }
    return count + n / 8;
  }
   
  // BEGIN CUT HERE
public:
  void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
private:
  template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
  void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
  void test_case_0() { int Arg0 = 20; int Arg1 = 3; verify_case(0, Arg1, minCartons(Arg0)); }
  void test_case_1() { int Arg0 = 24; int Arg1 = 3; verify_case(1, Arg1, minCartons(Arg0)); }
  void test_case_2() { int Arg0 = 15; int Arg1 = -1; verify_case(2, Arg1, minCartons(Arg0)); }
  void test_case_3() { int Arg0 = 4; int Arg1 = -1; verify_case(3, Arg1, minCartons(Arg0)); }

  // END CUT HERE

};
// BEGIN CUT HERE
int main() {
EggCartons ___test;
___test.run_test(-1);
}
// END CUT HERE 