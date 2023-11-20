#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <cmath>
#include <queue>
#include <deque>
 
using namespace std;
 
#define pb push_back
#define mp make_pair
#define foreach(i,c) for(typeof(c.begin()) i = c.begin(); i != c.end(); ++i)
#define all(c) c.begin(),c.end() 


//g++ -std=c++11 BigBurger.cpp -o BigBurger
//./BigBurger


/*

Problem (3) BigBurger:

Initial thoughts:

Obviously, according to the question, given the time and time spent by a group of people, 

ask who is waiting the longest


Pseudo-Code:

class BigBurger {
public procedure maxWait(vector <int> arrival, vector <int> service) {
        int t = 0;
        int i=0;
        Repeat(i < (int)arrival.size()){
            t = max(t, arrival[i]);
            finish.push_back(t);
            t += service[i];
            int diff= finish[i]-arrival[i];
            if(diff>ans)
                ans=diff;
            i+=1;
        EndRepeat
        return ans;
    endprocedure
endclass


Reflection:

I forgot something. When the system is running, 

it will use the previously recorded data for the next run.  

So I got error again and again without any logic problem.

In fact, I should reset the program into initialization.

The idea is straightforward because I just need to check for every arrival time

Think of it as the starting point of the time and see if how long have to wait.

So mysolution is to save the current time and simulate directly.

*/

class BigBurger {
    public:
    int ans;
    vector<int> finish;
    int maxWait(vector <int> arrival, vector <int> service) {
        int t = 0;
        int i=0;
        while(i < (int)arrival.size()){
            t = max(t, arrival[i]);
            finish.push_back(t);
            t += service[i];
            int diff= finish[i]-arrival[i];
            if(diff>ans)
                ans=diff;
            i+=1;
        }
        return ans;
    }
};
 
// BEGIN CUT HERE
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[])
{
	if (argc == 1) 
	{
		cout << "Testing BigBurger (250.0 points)" << endl << endl;
		for (int i = 0; i < 20; i++)
		{
			ostringstream s; s << argv[0] << " " << i;
			int exitCode = system(s.str().c_str());
			if (exitCode)
				cout << "#" << i << ": Runtime Error" << endl;
		}
		int T = time(NULL)-1340181745;
		double PT = T/60.0, TT = 75.0;
		cout.setf(ios::fixed,ios::floatfield);
		cout.precision(2);
		cout << endl;
		cout << "Time  : " << T/60 << " minutes " << T%60 << " secs" << endl;
		cout << "Score : " << 250.0*(.3+(.7*TT*TT)/(10.0*PT*PT+TT*TT)) << " points" << endl;
	}
	else
	{
		int _tc; istringstream(argv[1]) >> _tc;
		BigBurger _obj;
		int _expected, _received;
		time_t _start = clock();
		switch (_tc)
		{
			case 0:
			{
				int arrival[] = {3,3,9};
				int service[] = {2,15,14};
				_expected = 11;
				_received = _obj.maxWait(vector <int>(arrival, arrival+sizeof(arrival)/sizeof(int)), vector <int>(service, service+sizeof(service)/sizeof(int))); break;
			}
			case 1:
			{
				int arrival[] = {182};
				int service[] = {11};
				_expected = 0;
				_received = _obj.maxWait(vector <int>(arrival, arrival+sizeof(arrival)/sizeof(int)), vector <int>(service, service+sizeof(service)/sizeof(int))); break;
			}
			case 2:
			{
				int arrival[] = {2,10,11};
				int service[] = {3,4,3};
				_expected = 3;
				_received = _obj.maxWait(vector <int>(arrival, arrival+sizeof(arrival)/sizeof(int)), vector <int>(service, service+sizeof(service)/sizeof(int))); break;
			}
			case 3:
			{
				int arrival[] = {2,10,12};
				int service[] = {15,1,15};
				_expected = 7;
				_received = _obj.maxWait(vector <int>(arrival, arrival+sizeof(arrival)/sizeof(int)), vector <int>(service, service+sizeof(service)/sizeof(int))); break;
			}
			/*case 4:
			{
				int arrival[] = ;
				int service[] = ;
				_expected = ;
				_received = _obj.maxWait(vector <int>(arrival, arrival+sizeof(arrival)/sizeof(int)), vector <int>(service, service+sizeof(service)/sizeof(int))); break;
			}*/
			/*case 5:
			{
				int arrival[] = ;
				int service[] = ;
				_expected = ;
				_received = _obj.maxWait(vector <int>(arrival, arrival+sizeof(arrival)/sizeof(int)), vector <int>(service, service+sizeof(service)/sizeof(int))); break;
			}*/
			/*case 6:
			{
				int arrival[] = ;
				int service[] = ;
				_expected = ;
				_received = _obj.maxWait(vector <int>(arrival, arrival+sizeof(arrival)/sizeof(int)), vector <int>(service, service+sizeof(service)/sizeof(int))); break;
			}*/
			default: return 0;
		}
		cout.setf(ios::fixed,ios::floatfield);
		cout.precision(2);
		double _elapsed = (double)(clock()-_start)/CLOCKS_PER_SEC;
		if (_received == _expected)
			cout << "#" << _tc << ": Passed (" << _elapsed << " secs)" << endl;
		else
		{
			cout << "#" << _tc << ": Failed (" << _elapsed << " secs)" << endl;
			cout << "           Expected: " << _expected << endl;
			cout << "           Received: " << _received << endl;
		}
	}
}
