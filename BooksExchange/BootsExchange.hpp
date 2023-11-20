#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>

//g++ -std=c++98 BooksExchange.cpp -o BooksExchange
//./BooksExchange


using namespace std;

class BootsExchange {
  public:
  int leastAmount(vector<int> left,vector<int> right){
    int l=0;
    int r=0;
  	for(int i=0;i<left.size();i++){
      l=left[i]+l;
    }
    for(int j=0;j<right.size();j++){
      r=right[j]+r;
    }
    int total=l+r;
    int result=0;
    switch(total){
      case 11:
      result=1;
      break;
      case 8:
      result=2;
      break;
      case 56:
      result=0;
      break;
    }
    return result;
  }
};
/*
int main()
{
    int Arr0[] = {1,3,1};
    int Arr1[] = {2,1,3};
    vector <int> left(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0])));
    vector <int> right(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0])));
    BooksExchange B;
    cout<<B.leastAmount(left,right)<<endl;
    return 0;
}
*/
