#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <complex>



//g++ -std=c++98 RugSizes.cpp -o RugSizes
//./RugSizes


/*


Problem (3) RugSizes:
Initial thoughts:

Due to carpets can come in a variety of sizes. So, we can simply use brute force solution to calculate 
carpets of any integer width and length, except that the width and length of the carpet are uniform. 
So we can count how many different options are available in a given area.


Pseudo-Code:

class RugSizes
		public procedure rugCount(int area) {
				int ans = 0;
				int i = 1;
				Repeat(i*i<=area){
					if (area % i != 0) {
						i++;
						continue;
					}
					int s = area / i;
					if (i != s && i % 2 == 0 && s % 2 == 0) {
						i++;
						continue;
					}
					ans++;
					i++;
				EndRepeat
				return ans;
		endprocedure
endclass

Reflection:
I would like to choose brute force solution. My rugCount functions have O(sqrt(n)) runtime, 
My method have to find the required area and returns the number of different ways we can choose to cover the carpet size of that exact area. 
In order to volid calculate the same size twice and except that the width and length of the carpet are uniform.
So I have to adjust the size of each carpet , and I use modulus operator which can returns remainder of division of area by size to check whether calculate twice.

*/



using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef long long ll;
typedef complex<int> pnt;
typedef pair<int, int> pii;

#define FOR(i,a,b) for(i=a;i<b;i++)
#define RA(x) (x).begin(), (x).end()
#define REV(x) reverse(RA(x))
#define FE(i, x) for (typeof((x).begin()) i = (x).begin(); i != (x).end(); i++)
#define SZ(x) ((int) (x).size())
#define X first
#define Y second

class RugSizes {
public:
	int rugCount(int area) {
		int ans = 0;
		int i = 1;
		while(i*i<=area){
			if (area % i != 0) {
				i++;
				continue;
			}
			int s = area / i;
			if (i != s && i % 2 == 0 && s % 2 == 0) {
				i++;
				continue;
			}
			ans++;
			i++;
		}
		return ans;
	}
};



// BEGIN CUT HERE
namespace moj_harness {
	int run_test_case(int);
	void run_test(int casenum = -1, bool quiet = false) {
		if (casenum != -1) {
			if (run_test_case(casenum) == -1 && !quiet) {
				cerr << "Illegal input! Test case " << casenum << " does not exist." << endl;
			}
			return;
		}
		
		int correct = 0, total = 0;
		for (int i=0;; ++i) {
			int x = run_test_case(i);
			if (x == -1) {
				if (i >= 100) break;
				continue;
			}
			correct += x;
			++total;
		}
		
		if (total == 0) {
			cerr << "No test cases run." << endl;
		} else if (correct < total) {
			cerr << "Some cases FAILED (passed " << correct << " of " << total << ")." << endl;
		} else {
			cerr << "All " << total << " tests passed!" << endl;
		}
	}
	
	int verify_case(int casenum, const int &expected, const int &received, clock_t elapsed) {
		cerr << "Example " << casenum << "... ";
		
		string verdict;
		vector<string> info;
		char buf[100];
		
		if (elapsed > CLOCKS_PER_SEC / 200) {
			sprintf(buf, "time %.2fs", elapsed * (1.0/CLOCKS_PER_SEC));
			info.push_back(buf);
		}
		
		if (expected == received) {
			verdict = "PASSED";
		} else {
			verdict = "FAILED";
		}
		
		cerr << verdict;
		if (!info.empty()) {
			cerr << " (";
			for (int i=0; i<(int)info.size(); ++i) {
				if (i > 0) cerr << ", ";
				cerr << info[i];
			}
			cerr << ")";
		}
		cerr << endl;
		
		if (verdict == "FAILED") {
			cerr << "    Expected: " << expected << endl;
			cerr << "    Received: " << received << endl;
		}
		
		return verdict == "PASSED";
	}
	
	int run_test_case(int casenum__) {
		switch (casenum__) {
			case 0: {
				int area                  = 4;
				int expected__            = 2;
				
				clock_t start__           = clock();
				int received__            = RugSizes().rugCount(area);
				return verify_case(casenum__, expected__, received__, clock()-start__);
			}
			case 1: {
				int area                  = 8;
				int expected__            = 1;
				
				clock_t start__           = clock();
				int received__            = RugSizes().rugCount(area);
				return verify_case(casenum__, expected__, received__, clock()-start__);
			}
				
				// custom cases
				
				/*      case 2: {
				 int area                  = ;
				 int expected__            = ;
				 clock_t start__           = clock();
				 int received__            = RugSizes().rugCount(area);
				 return verify_case(casenum__, expected__, received__, clock()-start__);
				 }*/
				/*      case 3: {
				 int area                  = ;
				 int expected__            = ;
				 clock_t start__           = clock();
				 int received__            = RugSizes().rugCount(area);
				 return verify_case(casenum__, expected__, received__, clock()-start__);
				 }*/
				/*      case 4: {
				 int area                  = ;
				 int expected__            = ;
				 clock_t start__           = clock();
				 int received__            = RugSizes().rugCount(area);
				 return verify_case(casenum__, expected__, received__, clock()-start__);
				 }*/
			default:
				return -1;
		}
	}
}


int main(int argc, char *argv[]) {
	if (argc == 1) {
		moj_harness::run_test();
	} else {
		for (int i=1; i<argc; ++i)
			moj_harness::run_test(atoi(argv[i]));
	}
}
// END CUT HERE
