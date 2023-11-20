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

using namespace std;
//g++ -std=c++98 Truckloads.cpp -o Truckloads
//./Truckloads


/*


Problem (5) Truckloads:
Initial thoughts:

This problem is definitely to be solved by recursive function because you have to divide the pile in half forming two smaller piles 
in order to continue dividing each of the small piles in half until we get piles that will fit on a truck.
So we need a function that calls itself during its to division. This enables the function to repeat itself several times, 
outputting the result and the end of each iteration. 

Pseudo-Code:

class Truckloads
    public procedure numTrucks(int numCrates, int loadSize)
        if (numCrates <= loadSize) return 1;
        int branchLeft=0;
        int branchRight=0;
        if(numCrates%2==0){
            branchLeft=numCrates/2;
            branchRight=numCrates/2;
        }else{
            branchLeft=(numCrates+1)/2;
            branchRight=(numCrates-1)/2;
        }
        return numTrucks(branchLeft, loadSize) + numTrucks(branchRight, loadSize);
    endprocedure
endclass


Reflection:

In this question, it is simply to use recursive function to divide the pile in half. 
So this program use divide-and-conquer algorithm, 
such algorithm is works by recursively breaking down a pile into two sub-pile,
until these become simple enough to be solved directly. 
Hence, my solution to the sub-problems are then combined to give a solution to the original problem.

*/


class Truckloads {
public:
    int numTrucks(int numCrates, int loadSize) {
        if (numCrates <= loadSize) return 1;
        int branchLeft=0;
        int branchRight=0;
        if(numCrates%2==0){
            branchLeft=numCrates/2;
            branchRight=numCrates/2;
        }else{
            branchLeft=(numCrates+1)/2;
            branchRight=(numCrates-1)/2;
        }
        return numTrucks(branchLeft, loadSize) + numTrucks(branchRight, loadSize);
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
            int numCrates             = 14;
            int loadSize              = 3;
            int expected              = 6;

            clock_t moj_start_time    = clock();
            int received              = Truckloads().numTrucks( numCrates, loadSize );
            return verify_case( casenum, expected, received, clock()-moj_start_time );
        }
        case 1: {
            int numCrates             = 15;
            int loadSize              = 1;
            int expected              = 15;

            clock_t moj_start_time    = clock();
            int received              = Truckloads().numTrucks( numCrates, loadSize );
            return verify_case( casenum, expected, received, clock()-moj_start_time );
        }
        case 2: {
            int numCrates             = 1024;
            int loadSize              = 5;
            int expected              = 256;

            clock_t moj_start_time    = clock();
            int received              = Truckloads().numTrucks( numCrates, loadSize );
            return verify_case( casenum, expected, received, clock()-moj_start_time );
        }

        // custom cases

/*      case 3: {
            int numCrates             = ;
            int loadSize              = ;
            int expected              = ;
            clock_t moj_start_time    = clock();
            int received              = Truckloads().numTrucks( numCrates, loadSize );
            return verify_case( casenum, expected, received, clock()-moj_start_time );
        }*/
/*      case 4: {
            int numCrates             = ;
            int loadSize              = ;
            int expected              = ;
            clock_t moj_start_time    = clock();
            int received              = Truckloads().numTrucks( numCrates, loadSize );
            return verify_case( casenum, expected, received, clock()-moj_start_time );
        }*/
/*      case 5: {
            int numCrates             = ;
            int loadSize              = ;
            int expected              = ;
            clock_t moj_start_time    = clock();
            int received              = Truckloads().numTrucks( numCrates, loadSize );
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