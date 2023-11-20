#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>

//g++ -std=c++98 Slowkeyboard.cpp -o Slowkeyboard
//./Slowkeyboard

using namespace std;

class Slowkeyboard {
  public:
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
  int enterTime(string time){
    vector<string> ts=split(time,":");
    int second=atoi(ts[1].c_str());
    return 60-second;
  }
};
/*
int main()
{
    Slowkeyboard S;
    cout<<S.enterTime("59:56")<<endl;
    return 0;
}
*/.
