#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <set>
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
#define LL long long

using namespace std;

//g++ -std=c++11 UnsealTheSafe.cpp -o UnsealTheSafe
//./UnsealTheSafe

/*
Problem (4) UnsealTheSafe:

Initial thoughts:

According to the question, Every pair of neighboring digits in the password is adjacent on the keypad. 

So I have to use a loop adn the index at which new password digit is to be placed.

And I think that write down all the possibility.

if the first button is 1, the second button can be either 2 or 4

if the first button is 2, the second button can be either 1, 3 or 5

if the first button is 3, the second button can be either 2 or 6

if the first button is 4, the second button can be either 1, 5 or 7

if the first button is 5, the second button can be either 2, 4, 6 or 8

if the first button is 6, the second button can be either 3, 5 or 9

if the first button is 7, the second button can be either 4, 8 or 0

if the first button is 8, the second button can be either 5, 7 or 9

if the first button is 9, the second button can be either 6 or 8

if the first button is 0, the second button can be 7 only

Pseudo-Code:


class UnsealTheSafe
{
	public procedure countPasswords(int n)
    	const int len=10;
        long prec[len]={0};
        long newc[len]={0};

        int i=0;
        Repeat(i<len){
        	prec[i]=1;
        	i+=1;
        EndRepeat

            
        Repeat(int i=1; i<n; i++) 
            //for 1
            newc[2]+=prec[1];
            newc[4]+=prec[1];

            //for 2
            newc[1]+=prec[2];
            newc[5]+=prec[2];
            newc[3]+=prec[2];

            //for 3
            newc[2]+=prec[3];
            newc[6]+=prec[3];

            //for 4
            newc[1]+=prec[4];
            newc[5]+=prec[4];
            newc[7]+=prec[4];

            //for 5
            newc[2]+=prec[5];
            newc[4]+=prec[5];
            newc[6]+=prec[5];
            newc[8]+=prec[5];

            //for 6
            newc[3]+=prec[6];
            newc[5]+=prec[6];
            newc[9]+=prec[6];

            //for 7
            newc[0]+=prec[7];
            newc[4]+=prec[7];
            newc[8]+=prec[7];

            //for 8
            newc[7]+=prec[8];
            newc[5]+=prec[8];
            newc[9]+=prec[8];

            //for 9
            newc[6]+=prec[9];
            newc[8]+=prec[9];

            //for 0
            newc[7]+=prec[0];

        	for(int j=0; j<len; j++)
           {
               prec[j]=newc[j];
               newc[j]=0;
           }
        EndRepeat

      long ans = 0;
      i=0;
      Repeat(i<=9){
      	ans+=prec[i];
      	i+=1;
	  EndRepeat
      return ans;
    endprocdeure
endclass

Reflection:

This problem is definitely at most O(n) because you have to check each button in the password is adjacent on the keypad ,
so if I can find an O(n) brute force solution, I can iterate through the string array and add  '1' to adjacent button.
Repeating the action. I will find the number of possible passwords .


*/



class UnsealTheSafe
{
public:
    long countPasswords(int n)
    {
    	const int len=10;
        long prec[len]={0};
        long newc[len]={0};

        int i=0;
        while(i<len){
        	prec[i]=1;
        	i+=1;
        }

            
        for(int i=1; i<n; i++) 
         {
            //for 1
            newc[2]+=prec[1];
            newc[4]+=prec[1];

            //for 2
            newc[1]+=prec[2];
            newc[5]+=prec[2];
            newc[3]+=prec[2];

            //for 3
            newc[2]+=prec[3];
            newc[6]+=prec[3];

            //for 4
            newc[1]+=prec[4];
            newc[5]+=prec[4];
            newc[7]+=prec[4];

            //for 5
            newc[2]+=prec[5];
            newc[4]+=prec[5];
            newc[6]+=prec[5];
            newc[8]+=prec[5];

            //for 6
            newc[3]+=prec[6];
            newc[5]+=prec[6];
            newc[9]+=prec[6];

            //for 7
            newc[0]+=prec[7];
            newc[4]+=prec[7];
            newc[8]+=prec[7];

            //for 8
            newc[7]+=prec[8];
            newc[5]+=prec[8];
            newc[9]+=prec[8];

            //for 9
            newc[6]+=prec[9];
            newc[8]+=prec[9];

            //for 0
            newc[7]+=prec[0];

        	for(int j=0; j<len; j++)
           {
               prec[j]=newc[j];
               newc[j]=0;
           }
         }

      long ans = 0;
      i=0;
      while(i<=9){
      	ans+=prec[i];
      	i+=1;
	  }
      return ans;
    }
};

double test0() {
	int p0 = 2;
	UnsealTheSafe * obj = new UnsealTheSafe();
	clock_t start = clock();
	long long my_answer = obj->countPasswords(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p1 = 26LL;
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
	int p0 = 3;
	UnsealTheSafe * obj = new UnsealTheSafe();
	clock_t start = clock();
	long long my_answer = obj->countPasswords(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p1 = 74LL;
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
	int p0 = 25;
	UnsealTheSafe * obj = new UnsealTheSafe();
	clock_t start = clock();
	long long my_answer = obj->countPasswords(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p1 = 768478331222LL;
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
	
	if (!errors)
		cout <<"You're a stud (at least on the example cases)!" <<endl;
	else
		cout <<"Some of the test cases had errors." <<endl;
}

// Author: Shubham Gupta
// Powered by [KawigiEdit] 2.0!; 