#include <iostream>
#include <string>
#include <vector>


using namespace std;

//g++ -std=c++98 FracCount.hpp -o FracCount
//./FracCount

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