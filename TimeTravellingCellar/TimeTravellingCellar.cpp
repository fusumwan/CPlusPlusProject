// Paste me into the FileEdit configuration dialog

#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <bitset>
#include <queue>
#include <stack>
#include <utility>
#include <algorithm>
#include <cstdio>

//g++ -std=c++98 TimeTravellingCellar.cpp -o TimeTravellingCellar
//./TimeTravellingCellar



/*

Problem (5) TimeTravellingCellar:
Initial thoughts:

According to the question I have two array, profit and decay. 

When I push the time into the cellar, I get Gogo's profit[i]. 

So this problem is definitely at most O(n^2) because you have to calculate each profit when you push the time into the cellar.

Shorten the time of the wine cellar, I will lose his profit. 

By pushing time in one cellar and turning time back to another cellar,

I get the maximum profit Gogo can get.


Pseudo-Code:
class TimeTravellingCellar{
public:
    public procedure determineProfit(vector<int> profit, vector<int> decay ){
        int ans = 0;
		Repeat(int i=0;i<(int)profit.size();i++){
			Repeat(int j=0;j<(int)decay.size();j++){
				if(i!=j&&(int)profit[i]-decay[j]>ans){
					ans = profit[i]-decay[j];
				}
			EndRepeat
		EndRepeat
		return ans;
    endprocedure
endclass


Reflection:

I would like to use brute force solution to find the maximum of profit.

Because it is very fast to find out the answer.

So this program I am nesting two loops to find the maximum of profit.

the outer loop runs the size of profit array and the inner loop runs the size of decay array for each iteration of the outer loop.


*/


using namespace std;

class TimeTravellingCellar {
public:
   int determineProfit(vector<int> profit, vector<int> decay ){
		int ans = 0;
		for(int i=0;i<(int)profit.size();i++){
			for(int j=0;j<(int)decay.size();j++){
				if(i!=j&&(int)profit[i]-decay[j]>ans){
					ans = profit[i]-decay[j];
				}
			}
		}
		return ans;
	}
};

// BEGIN CUT HERE
namespace moj_harness {
	int run_test_case(int);
	void run_test( int casenum = -1 ) {
		if ( casenum != -1 ) {
			if ( run_test_case( casenum ) == -1 )
				cerr << "Illegal input! Test case " << casenum << " does not exist." << endl;
			return;
		}
		
		int correct = 0, total = 0;
		for ( int i=0;; ++i ) {
			int x = run_test_case(i);
			if ( x == -1 ) {
				if ( i >= 100 ) break;
				continue;
			}
			correct += x;
			++total;
		}
		
		if ( total == 0 ) {
			cerr << "No test cases run." << endl;
		} else if ( correct < total ) {
			cerr << "Some cases FAILED (passed " << correct << " of " << total << ")." << endl;
		} else {
			cerr << "All " << total << " tests passed!" << endl;
		}
	}
	
	int verify_case( int casenum, const int &expected, const int &received, clock_t elapsed ) { 
		cerr << "Example " << casenum << "... "; 
		
		string verdict;
		vector<string> info;
		char buf[100];
		
		if (elapsed > CLOCKS_PER_SEC / 200) {
			sprintf(buf, "time %.2fs", elapsed * (1.0/CLOCKS_PER_SEC));
			info.push_back(buf);
		}
		
		if ( expected == received ) {
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

	int run_test_case( int casenum ) {
		switch( casenum ) {
		case 0: {
			int profit[]              = {1,2,3};
			int decay[]               = {3,1,2};
			int expected              = 2;

			clock_t moj_start_time    = clock();
			int received              = TimeTravellingCellar().determineProfit( vector <int>( profit, profit + ( sizeof profit / sizeof profit[0] ) ), vector <int>( decay, decay + ( sizeof decay / sizeof decay[0] ) ) );
			return verify_case( casenum, expected, received, clock()-moj_start_time );
		}
		case 1: {
			int profit[]              = {3,2};
			int decay[]               = {1,2};
			int expected              = 1;

			clock_t moj_start_time    = clock();
			int received              = TimeTravellingCellar().determineProfit( vector <int>( profit, profit + ( sizeof profit / sizeof profit[0] ) ), vector <int>( decay, decay + ( sizeof decay / sizeof decay[0] ) ) );
			return verify_case( casenum, expected, received, clock()-moj_start_time );
		}
		case 2: {
			int profit[]              = {3,3,3};
			int decay[]               = {1,1,1};
			int expected              = 2;

			clock_t moj_start_time    = clock();
			int received              = TimeTravellingCellar().determineProfit( vector <int>( profit, profit + ( sizeof profit / sizeof profit[0] ) ), vector <int>( decay, decay + ( sizeof decay / sizeof decay[0] ) ) );
			return verify_case( casenum, expected, received, clock()-moj_start_time );
		}
		case 3: {
			int profit[]              = {1000,500,250,125};
			int decay[]               = {64,32,16,8};
			int expected              = 992;

			clock_t moj_start_time    = clock();
			int received              = TimeTravellingCellar().determineProfit( vector <int>( profit, profit + ( sizeof profit / sizeof profit[0] ) ), vector <int>( decay, decay + ( sizeof decay / sizeof decay[0] ) ) );
			return verify_case( casenum, expected, received, clock()-moj_start_time );
		}

		// custom cases

/*      case 4: {
			int profit[]              = ;
			int decay[]               = ;
			int expected              = ;
			clock_t moj_start_time    = clock();
			int received              = TimeTravellingCellar().determineProfit( vector <int>( profit, profit + ( sizeof profit / sizeof profit[0] ) ), vector <int>( decay, decay + ( sizeof decay / sizeof decay[0] ) ) );
			return verify_case( casenum, expected, received, clock()-moj_start_time );
		}*/
/*      case 5: {
			int profit[]              = ;
			int decay[]               = ;
			int expected              = ;
			clock_t moj_start_time    = clock();
			int received              = TimeTravellingCellar().determineProfit( vector <int>( profit, profit + ( sizeof profit / sizeof profit[0] ) ), vector <int>( decay, decay + ( sizeof decay / sizeof decay[0] ) ) );
			return verify_case( casenum, expected, received, clock()-moj_start_time );
		}*/
/*      case 6: {
			int profit[]              = ;
			int decay[]               = ;
			int expected              = ;
			clock_t moj_start_time    = clock();
			int received              = TimeTravellingCellar().determineProfit( vector <int>( profit, profit + ( sizeof profit / sizeof profit[0] ) ), vector <int>( decay, decay + ( sizeof decay / sizeof decay[0] ) ) );
			return verify_case( casenum, expected, received, clock()-moj_start_time );
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