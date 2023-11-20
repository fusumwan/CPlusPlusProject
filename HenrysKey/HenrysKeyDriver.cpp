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

  	return count;
  }

};
