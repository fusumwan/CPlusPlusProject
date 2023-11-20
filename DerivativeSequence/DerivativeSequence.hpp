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
using namespace std;


typedef int int_type;
#define FOR(x, a, b) for(int_type x = static_cast<int_type>(a); x < static_cast<int_type>(b); ++x)

class DerivativeSequence {
public:
	vector <int> UpdateSeq(vector <int> result,int i){
				vector <int> next;
				int j=0;
				int j_size=result.size()-1;
				FOR(j,0,j_size){
					next.push_back(result[j+1] - result[j]);
				}
				result.clear();
				result = next;
				return result;
	}
	vector <int> derSeq(vector <int> a, int n)
	{
			vector <int> result;
			result = a;
			int i=0;
			FOR(i,0,n){
				result=UpdateSeq(result,i);
			}
			return result;
	}

};
