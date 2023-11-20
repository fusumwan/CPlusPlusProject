#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <string>

//g++ -std=c++98 HenrysKey.cpp -o HenrysKey
//./HenrysKey


using namespace std;

class HenrysKey {
  public:
  long muplication(int n){
  	long r=1;
  	for(int i=1;i<=n;i++){
  		r=i*r;
  	}
  	return r;
  }
  int howMany(string s){
  	int count=0;
  	for(int i=0;i<s.size();i++){
  		s[i]=toupper(s[i]);
  	}
  	if(s.size()==1) return 1;
  	char c=""[0];
  	for(int i=0;i<s.size();i++){
  		if(c!=(char)s[i]){
  			count+=1;
  			c=(char)s[i];
  		}
  	}
  	//int n=s.size();
  	//int r=n-1;
  	//n! / r! * (n - r)!,
  	//return muplication(n) / (muplication(r)*muplication(n-r));
  	return count;
  }

};
/*
int main()
{
	HenrysKey H;
	//int n=;
	//int r=37;
	//cout<<H.muplication(9)<<endl;
	//cout<<(int)((double)H.muplication(n) / (double)(H.muplication(r)))<<endl;
	//cout<<H.howMany("ABA")<<endl;
	cout<<H.howMany("AABACCCCABAA")<<endl;
	cout<<H.howMany("AGAAGAHHHHFTQLLAPUURQQRRRUFJJSBSZVJZZZ")<<endl;

    return 0;
}


*/