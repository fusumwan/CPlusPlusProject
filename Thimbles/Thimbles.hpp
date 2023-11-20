#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//g++ Thimbles.cpp -o Thimbles
//./Thimbles



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
