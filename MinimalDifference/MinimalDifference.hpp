#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cfloat>
#include <map>
#include <utility>
#include <set>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;



class MinimalDifference {
public:
  int total(int value){
    int i;
    int ans = 0;
    for(int a=value;a>0;a/=10){
      i = a%10; 
      ans += i;
    }
    return ans;
  }

  int findNumber(int A, int B, int C)
  {
    int num=A;
    int ans = 0;
    int ansd = -1;
    int sum;
    int totalc = total(C);
    while(num <= B){
      sum = total(num);
      if (totalc == sum){
        return num;
      }
      int tmp=totalc-sum;
      if(tmp<0){
        tmp=(-1)*tmp;
      }
      if (tmp < ansd ||
        ansd == -1) {
        ansd = tmp;
        ans = num;
      }
      num+=1;
    }
    return ans;
  }
};

