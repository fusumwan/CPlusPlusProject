#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//g++ Thimbles.cpp -o Thimbles
//./Thimbles


/*

Problem (1) Thimbles:
Initial thoughts:

As the dealer placed three identical thimebles on the table, so I thought this program I need nesting two loops. 
If the vector has three string in the vector <string>
the outer loop runs n times and the inner loop runs n times for each iteration of the outer loop, 
giving us n²​​ total comparsion with position.
Thus this function runs in O(n²) time. If the array has 100 string, I have to compare strings more than 100 times. 

Pseudo-Code:
class Thimbles
	public procedure thimbleWithBall(vector<string> swaps){
			int index = 1;
			Repeat (int i = 0; i < swaps.size(); ++i){
				istringstream iss(swaps[i]);
				int c1;
				int c3;
				char c2;
				for(int j=0;j<swaps[i].size();j++){
					switch(j){
						case 0:
						c1=swaps[i][j] - '0';
						break;
						case 1:
						break;
						case 2:
						c3=swaps[i][j] - '0';
						break;
					}
				}
				
				if (index == c1){
					index = c3;
				}
				else if (index == c3){
					index = c1;
				}
			endrepeat
			return index;
	endprocedure
endclass


Reflection:

I want to solves a problem in the most simple, direct or obvious way. 

And there is not distinguished by structure or form in the instructions.

It just simple to implement all instructions.

This problem is definitely to find a all possibility way that determines the position of the ball after all the thimble swaps have been done.

If I want to improve the performance, I would like to use one for loop to solve it.




*/

class Thimbles
{
	public:

		int thimbleWithBall(vector<string> swaps){
			int index = 1;
			for (int i = 0; i < swaps.size(); ++i){
				istringstream iss(swaps[i]);
				int c1;
				int c3;
				char c2;
				for(int j=0;j<swaps[i].size();j++){
					switch(j){
						case 0:
						c1=swaps[i][j] - '0';
						break;
						case 1:
						break;
						case 2:
						c3=swaps[i][j] - '0';
						break;
					}
				}
				
				if (index == c1){
					index = c3;
				}
				else if (index == c3){
					index = c1;
				}
			}
			return index;
		}
};

int main()
{
	vector<string> coll;
	coll.push_back("3-1");
	coll.push_back("2-3");
	coll.push_back("3-1");
	coll.push_back("3-2");

	Thimbles test;

	cout << test.thimbleWithBall(coll) << endl;

	return EXIT_SUCCESS;
}