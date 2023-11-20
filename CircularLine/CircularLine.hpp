// BEGIN CUT HERE
// END CUT HERE
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

/*
Problem (4) CircularLine:

Initial thoughts:

This problem is definitely at most O(n^2) because you have to compare the time between each stations, 
so if I can find an O(n^2) brute force solution, 
Here I am nesting two loops. If our array has n stations, our outer loop runs n times and our inner loop runs n stations for each iteration of the outer loop, 
giving us n^2 total time comparison. Thus this function runs in O(n2) time.
If the array has 10 items, we have to compare the time between each stations 100 times. 

Pseudo-Code:

class CircularLine
	public:
	public procedure longestTravel(vector <int> t)
	{
		l, s;
		in, out
		m = 0;
		Repeat i<-0 until i<t.size()
			Repeat j<-0 until j<t.size()
				if(i == j)
					continue;
				l= (i>j)?i:j
				s = (i>j)?j:i
				in = 0
				for (int k=s; k<l; k++)
				  in += t[k];
				out = 0
				for (int k=0; k<t.size(); k++)
				  if (k < s || k >= l)
				out += t[k]

				if (m < ((in>out)?out:in))
					m = ((in>out)?out:in)
			EndRepeat
		EndRepeat
		return m;
	endprocedure
endclass



Reflection:

I think I should use brute force to solve the problem. Because I have to compare every time session. 

My solution refers to a programming style that does not include any shortcuts to improve performance, 

but instead relies on sheer computing power to try all possibilities until the solution to a problem is found.

As there is a circular light rail line containing n stations numbered 0 to n-1.

The time between station 0 and station 1 is t [0], the travel time between station 1 and station 2 is t [1] ...., the travel time between stations n-1 and O is t [[ N-1].

I can travel between stations in any direction, so there are always two ways to get from one station to another instead of visiting the same station multiple times.

For example, if there are 4 stations, the two ways of slave 1 to station 3 are 1-2-3 and 1-0-3. In the first case, the total travel time is t [1] + t [2], and in the second case, it is t [0] + t [3].

When a person needs to go from one station to another, she always chooses the faster of the two methods. Give you an int t. Find these two sites so that you are most likely to get the fastest travel time between them. Come back this time.

Example {1,4,4,1,5}

First of all, the program will input an array of integer which is the time between stations.
And then it will read each time between stations in any direction. 
According this example {1,4,4,1,5}. Firstly checking the time between the station 0 and 1. 
I can travel between stations in from station 0 to station 1 direction.
 The travel time between station 0 and station 1 is 1.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 14.
 Checking the time between the station 0 and 2. 
I can travel between stations in from station 0 to station 2 direction.
 The travel time between station 0 and station 2 is 5.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 10.
 Checking the time between the station 0 and 3. 
I can travel between stations in from station 0 to station 3 direction.
 The travel time between station 0 and station 3 is 9.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 6.
 Checking the time between the station 0 and 4. 
I can travel between stations in from station 0 to station 4 direction.
 The travel time between station 0 and station 4 is 10.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 5.
 Checking the time between the station 1 and 0. 
I can travel between stations in from station 0 to station 1 direction.
 The travel time between station 0 and station 1 is 1.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 14.
 Checking the time between the station 1 and 2. 
I can travel between stations in from station 1 to station 2 direction.
 The travel time between station 1 and station 2 is 4.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 11.
 Checking the time between the station 1 and 3. 
I can travel between stations in from station 1 to station 3 direction.
 The travel time between station 1 and station 3 is 8.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 7.
 Checking the time between the station 1 and 4. 
I can travel between stations in from station 1 to station 4 direction.
 The travel time between station 1 and station 4 is 9.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 6.
 Checking the time between the station 2 and 0. 
I can travel between stations in from station 0 to station 2 direction.
 The travel time between station 0 and station 2 is 5.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 10.
 Checking the time between the station 2 and 1. 
I can travel between stations in from station 1 to station 2 direction.
 The travel time between station 1 and station 2 is 4.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 11.
 Checking the time between the station 2 and 3. 
I can travel between stations in from station 2 to station 3 direction.
 The travel time between station 2 and station 3 is 4.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 11.
 Checking the time between the station 2 and 4. 
I can travel between stations in from station 2 to station 4 direction.
 The travel time between station 2 and station 4 is 5.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 10.
 Checking the time between the station 3 and 0. 
I can travel between stations in from station 0 to station 3 direction.
 The travel time between station 0 and station 3 is 9.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 6.
 Checking the time between the station 3 and 1. 
I can travel between stations in from station 1 to station 3 direction.
 The travel time between station 1 and station 3 is 8.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 7.
 Checking the time between the station 3 and 2. 
I can travel between stations in from station 2 to station 3 direction.
 The travel time between station 2 and station 3 is 4.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 11.
 Checking the time between the station 3 and 4. 
I can travel between stations in from station 3 to station 4 direction.
 The travel time between station 3 and station 4 is 1.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 14.
 Checking the time between the station 4 and 0. 
I can travel between stations in from station 0 to station 4 direction.
 The travel time between station 0 and station 4 is 10.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 5.
 Checking the time between the station 4 and 1. 
I can travel between stations in from station 1 to station 4 direction.
 The travel time between station 1 and station 4 is 9.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 6.
 Checking the time between the station 4 and 2. 
I can travel between stations in from station 2 to station 4 direction.
 The travel time between station 2 and station 4 is 5.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 10.
 Checking the time between the station 4 and 3. 
I can travel between stations in from station 3 to station 4 direction.
 The travel time between station 3 and station 4 is 1.
I can travel between stations in from station 0 to station 5 direction.
 The travel time between station 0 and station 5 is 14.
So in this step we can find two stations such that the fastest travel time between them is the maximal possible is 7.
*/


class CircularLine {
   public:
  int longestTravel(vector <int> t)
  {
		int i, j, k;
		int large, small;
		int in, out;
		int m = 0;
		vector <int> TravelList;
		for (i=0; i<t.size(); i++)
		{
			for(j=0; j<t.size(); j++)
			{
				if(i == j)
					continue;
				large= (i>j)?i:j;
				small = (i>j)?j:i;
				in = 0;
				for (k=small; k<large; k++)
				  in += t[k];
				out = 0;
				for (k=0; k<t.size(); k++)
				  if (k < small || k >= large)
				out += t[k];

				if (m < ((in>out)?out:in)){m = ((in>out)?out:in);}
			}
		}
		return m;
	}
};
