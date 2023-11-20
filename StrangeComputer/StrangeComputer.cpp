#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <bitset>

#define FOR(A,B,C) for(int A=B;A<C;A++)
#define EFOR(A,B,C) for(int A=B;A<=C;A++)
#define RFOR(A,B,C) for(int A=B;A>=C;A--)
#define PB(A,B) A.push_back(B);
#define SORT(A) sort( A.begin(),A.end() );
#define VI vector<int>
#define VL vector<long>
#define VS vector<string>
#define VD vector<double>
#define VB vector<bool>

using namespace std;

//g++ -std=c++98 StrangeComputer.cpp -o StrangeComputer
//./StrangeComputer



/*

Problem (4) StrangeComputer:
Initial thoughts:

This problem is definitely at most O(n) because you have to check each bit in the string,
so if I can find an O(n) brute force solution, I can iterate through the string array and check if the character equal to '0' and then set to '1' and then check if character equal to '0'.
Repeating the action. I will find the number of operation.

Pseudo-Code:

class StrangeComputer
	public procedure setMemory(string mem)
		int pos = 0 ;
		char bit = '0';
		Repeat(int i = 0; i < mem.length(); i++){
			if(mem.at(i) != bit){
				bit = mem.at(i);
				pos++;
			}
		EndRepeat
		return pos;
	endprocedure
endclass


Reflection:
I would like to use brute force solution to find the number of operation. 
Because it is very fast to find out the answer.


*/



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


double test0() {
	string p0 = "0011";
	StrangeComputer * obj = new StrangeComputer();
	clock_t start = clock();
	int my_answer = obj->setMemory(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 1;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test1() {
	string p0 = "000";
	StrangeComputer * obj = new StrangeComputer();
	clock_t start = clock();
	int my_answer = obj->setMemory(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 0;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test2() {
	string p0 = "0100";
	StrangeComputer * obj = new StrangeComputer();
	clock_t start = clock();
	int my_answer = obj->setMemory(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 2;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test3() {
	string p0 = "111000111";
	StrangeComputer * obj = new StrangeComputer();
	clock_t start = clock();
	int my_answer = obj->setMemory(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 3;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}

int main() {
	int time;
	bool errors = false;
	
	time = test0();
	if (time < 0)
		errors = true;
	
	time = test1();
	if (time < 0)
		errors = true;
	
	time = test2();
	if (time < 0)
		errors = true;
	
	time = test3();
	if (time < 0)
		errors = true;
	
	if (!errors)
		cout <<"You're a stud (at least on the example cases)!" <<endl;
	else
		cout <<"Some of the test cases had errors." <<endl;
}

//Author: Shubham Gupta
//Powered by [KawigiEdit] 2.0!; 