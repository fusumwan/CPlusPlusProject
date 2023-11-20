#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
//g++ -std=c++98 AzimuthMonitoring.cpp -o AzimuthMonitoring
//./AzimuthMonitoring
/*
Problem (5) AzimuthMonitoring:

Initial thoughts:

This program  use brute force. Because the brute force solution is simply to process each instruction for every degree and then update the degree of the result. 

This is particularly efficient because it is possible to process all instructions through brute force  algorithms.


Pseudo-Code:

class AzimuthMonitoring 
	public procedure getAzimuth(vector<string> instructions)
		res = 0;
		Repeat i<-0 until i<sizeof(instructions)
			if (instructions[i] == "HALT") break;
			else if (instructions[i] == "TURN AROUND") res += 180
			else if (instructions[i] == "RIGHT") res += 90
			else if (instructions[i] == "LEFT") res += 270
			else
				vector<string> s=split(instructions[i]," ")
				char str[20];
				strcpy(str, s[1].c_str())
				int val = atoi(str)
				if (s[0] == "RIGHT") res += val
				else res += 360 - val
		EndRepeat
		return res % 360;
	endprocedure
endclass

Reflection:

I want to solves a problem in the most simple, direct or obvious way. 

And there is not distinguished by structure or form in the instructions.

It just simple to implement all instructions.

Hence, this program enters some commands to the robot to turn according to the instructions.

First, the instruction is stored in the string, then it will be steered according to the instruction in the string, 

and the direction and degree in the instruction will be divided into two strings. And then read the degree into instruction.

For example {"TURN AROUND", "RIGHT", "LEFT 5", "RIGHT 10", "LEFT 5", "HALT"}:

 First of all, read the instruction from the vector string.

 And the instruction 0 is TURN AROUND so we rotate 180 degrees clockwise.

 And the instruction 1 is right so we rotate 90 degrees clockwise.

 And then if the direction and degree at the same instruction [LEFT 5], we will split it into two string.

 And then if the left direction and we rotate 5 degrees anticlockwise And then if the direction and degree at the same instruction [RIGHT 10], we will split it into two string.

 And then if the right direction and we rotate 10 degrees clockwise And then if the direction and degree at the same instruction [LEFT 5], we will split it into two string.

 And then if the left direction and we rotate 5 degrees anticlockwise And the instruction 5 is HALT so we ignore it.
 
 Finally, the degree is 270.
*/

class AzimuthMonitoring {
	public:
	//It is the right way to split a string into a vector of strings
	vector<string> split(string str, string token){
		vector<string>result;
		while(str.size()){
			unsigned int index = str.find(token);
			if(index <str.size() && index>=0){
				result.push_back(str.substr(0,index));
				str = str.substr(index+token.size());
				if(str.size()==0){
					result.push_back(str);
					break;
				}
			}else{
				result.push_back(str);
				break;
			}
		}
		return result;
	}
	int getAzimuth(vector<string> instructions)
	{
		int i;
		int res = 0;
		printf(" First of all, read the instruction from the vector string.\n");
		for (i = 0; i < instructions.size(); i++)
		{
			if (instructions[i] == "HALT"){ 
				printf(" And the instruction %d is HALT so we ignore it.\n",i);
				break;
			}
			else if (instructions[i] == "TURN AROUND"){

				res += 180;
				printf(" And the instruction %d is TURN AROUND so we rotate 180 degrees clockwise.\n",i);
			}
			else if (instructions[i] == "RIGHT"){ 
				res += 90;
				printf(" And the instruction %d is right so we rotate 90 degrees clockwise.\n",i);
			}
			else if (instructions[i] == "LEFT"){
				res += 270;
				printf(" And the instruction %d is LEFT so we rotate 270 degrees clockwise.\n",i);
			}
			else
			{
				printf(" And then if the direction and degree at the same instruction [%s], we will split it into two string.\n",instructions[i].c_str());
				vector<string> s=split(instructions[i]," ");
				char str[20];
				strcpy(str, s[1].c_str());
				int val = atoi(str);
				if (s[0] == "RIGHT"){
					res += val;
					printf(" And then if the right direction and we rotate %d degrees clockwise",val);
				} 
				else{
					res += 360 - val;
					printf(" And then if the left direction and we rotate %d degrees anticlockwise",val);
				}
				
			}
		}
		printf(" Finally, the degree is %d.\n",res % 360);
		return res % 360;
	}
};


int main() {

	vector<string> p0;
	string Arr0[] = {"TURN AROUND", "RIGHT", "LEFT 5", "RIGHT 10", "LEFT 5", "HALT"};
	vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0])));
	AzimuthMonitoring obj;

	int answer = obj.getAzimuth(Arg0);
	return 0;
}
// END KAWIGIEDIT TESTING