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

//g++ -std=c++98 FracCount.cpp -o FracCount
//./FracCount

/*
Problem (2) FracCount:
Initial thoughts:

First of all, in order to find a unique integer value to each irreducible fraction between 0 and 1.  

I will take an input from the user and check whether the number is prime or not.

Due to prime number is a number that is greater than 1 and divided by 1 or itself. 

In other words, prime numbers can't be divided by other numbers than itself or 1. 

For example 2, 3, 5, 7, 11, 13, 17, 19, 23.... are the prime numbers.

So I have to use modulus operator, because it can that compute the remainder that results from performing integer division.

Hence, if the number can be divided by numerator or denominator then this number should not be prime.

And then I can count the position of fraction.


Pseudo-Code:

class FracCount
	public procedure isPrime(int num,int i,int j)
		if(i%num==0&&j%num==0)
		{
			return false;
		}
		return true;
	endprocedure

	public procedure position(int numerator, int denominator)
		int pos = 0;
		Repeat(int i=2; i<=denominator; i++){
			Repeat(int j=1; j<i; j++){
				bool isprime = true;
				for(int num=2; num<=j; num++){
					if(!isPrime(num,i,j)){
						isprime = false;
						break;
					}
				}
				if(isprime) pos++;
				if(j==numerator && i==denominator) return pos;
			EndRepeat
		EndRepeat
		return -1;
	endprocedure
enclass


Reflection:

So this program I am nesting two loops. I have to check number whether is prime number,

the outer loop runs the value of denominator times and the inner loop runs the value of numerator times for each iteration of the outer loop.

In order to improve the performace, I will save all the prime number after creating from function. So I can reuse it.


*/

class FracCount
{
	public:
	bool isPrime(int num,int i,int j){
		if(i%num==0&&j%num==0)
		{
			return false;
		}
		return true;
	}
	int position(int numerator, int denominator)
	{
		int pos = 0;
		for(int i=2; i<=denominator; i++){
			for(int j=1; j<i; j++){
				bool isprime = true;
				for(int num=2; num<=j; num++){
					if(!isPrime(num,i,j)){
						isprime = false;
						break;
					}
				}
				if(isprime) pos++;
				if(j==numerator && i==denominator) return pos;
			}
		}
		return -1;
	}
};