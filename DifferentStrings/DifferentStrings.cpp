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
#include <ctime>
#include <cstring>
#include <climits>

#include <sstream>
#include <typeinfo>
#include <fstream>
using namespace std;


//g++ -std=c++11 DifferentStrings.cpp -o DifferentStrings
//./DifferentStrings

#define FOR(i,x,y) for(int i=x; i<=y; i++)
#define FORN(i,n) FOR(i,0,(n-1))
#define FORE(it,v) for(__typeof(v.begin()) it = v.begin(); it != v.end(); it++)
#define db(x) cout << (#x) << " = " << x << endl;
#define CLR(x) memset(x,0,sizeof(x));
#define sz(x) ((int)x.size())
#define mp make_pair
#define pb push_back
#define re return
typedef unsigned long long ull;
typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> ii;


/*


Problem (2) DifferentStrings:

Initial thoughts:

Obviously, the basic string search matching problem is problematic.

Given two strings, A and B, determine if the pattern appears in the text.

The idea is straightforward because I just need to search for every position in the text,

Think of it as the starting point of the pattern and see if it matches.

Pseudo-Code:

class DifferentStrings {
public procedure minimize(string A, string B) {
        this->_A = A;
        this->_B = B;
        int ans = 999999999;
        int len=(int)(_B.length() - _A.length() + 1);
        int i=0;
        Repeat(i<len){
            int count = 0;
            int j=0;
            Repeat(j<_A.length()){
                if (_A[j] != _B[i + j]){
                    count ++;
                }
                j+=1;
            }
            ans = min(ans,count);
            i+=1;
        EndRepeat
        return ans;
    endprocedure
endclass


Reflection:

I think I should use brute force to solve the problem. 

Because I have to find character position and find the the difference between the words

In this program, I am nesting two loops. 



*/



class DifferentStrings {
    public:
    string _A;
    string _B;

    int difference(int i) {
        int count = 0;
        for(int j=0; j<_A.length();j++) {
            if (_A[j] != _B[i + j])
                count ++;
        }
        return count;
    }
    int minimize(string A, string B) {
        this->_A = A;
        this->_B = B;
        int ans = 999999999;
        int len=(int)(_B.length() - _A.length() + 1);
        int i=0;
        while(i<len){
            int count = 0;
            int j=0;
            while(j<_A.length()){
                if (_A[j] != _B[i + j]){
                    count ++;
                }
                j+=1;
            }
            ans = min(ans,count);
            i+=1;
        }
        return ans;
    }
};


// CUT begin
ifstream data("DifferentStrings.sample");

string next_line() {
    string s;
    getline(data, s);
    return s;
}

template <typename T> void from_stream(T &t) {
    stringstream ss(next_line());
    ss >> t;
}

void from_stream(string &s) {
    s = next_line();
}

template <typename T>
string to_string(T t) {
    stringstream s;
    s << t;
    return s.str();
}

string to_string(string t) {
    return "\"" + t + "\"";
}

bool do_test(string A, string B, int __expected) {
    time_t startClock = clock();
    DifferentStrings *instance = new DifferentStrings();
    int __result = instance->minimize(A, B);
    double elapsed = (double)(clock() - startClock) / CLOCKS_PER_SEC;
    delete instance;

    if (__result == __expected) {
        cout << "PASSED!" << " (" << elapsed << " seconds)" << endl;
        return true;
    }
    else {
        cout << "FAILED!" << " (" << elapsed << " seconds)" << endl;
        cout << "           Expected: " << to_string(__expected) << endl;
        cout << "           Received: " << to_string(__result) << endl;
        return false;
    }
}

void run_test(const set<int> &case_set) {
    int cases = 0, passed = 0;
    while (true) {
        if (next_line().find("--") != 0)
            break;
        string A;
        from_stream(A);
        string B;
        from_stream(B);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if (do_test(A, B, __answer))
            passed++;
    }
    if (case_set.size() > 0) cases = case_set.size();
    cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;

    int T = time(NULL) - 1388055085;
    double PT = T / 60.0, TT = 75.0;
    cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
    cout << "Score  : " << 250 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
}

int main(int argc, char *argv[]) {
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    cout << "DifferentStrings (250 Points)" << endl << endl;

    set<int> cases;
    for (int i = 1; i < argc; ++i) cases.insert(atoi(argv[i]));
    run_test(cases);
    return 0;
}