#include <iostream>
#include <string>


using namespace std;

//g++ -std=c++98 StrangeComputer.hpp -o StrangeComputer
//./StrangeComputer

class StrangeComputer {
public:
	int setMemory(string mem)
	{
		int operation = 0 ;
		char bit = '0';
		for(int i = 0; i < mem.length(); i++){
			if(mem.at(i) != bit){
				bit = mem.at(i);
				operation++;
			}
		}
		return operation;
	}
};
