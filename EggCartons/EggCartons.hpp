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

//g++ -std=c++98 EggCartons.hpp -o EggCartons
//./EggCartons

using namespace std;


typedef int t_int;

// -- utility --
// C-style loop
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


};
