#include <algorithm>
#include <cfloat>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
//g++ -std=c++98 SquareOfDigits.hpp -o SquareOfDigits
//./SquareOfDigits

using namespace std;
class SquareOfDigits {
  public:
  int findSquare(vector<string> data,int column_width,int a,int b){

      if (a+column_width < data.size() && b+column_width < data[a].size() && 
      data[a+column_width][b] == data[a][b] && 
      data[a+column_width][b+column_width] == data[a][b] && 
      data[a+column_width][b+column_width] == data[a][b+column_width]){
        return (column_width+1)*(column_width+1);
      }
    
    return -1;
  }
  int getMax(vector<string> data)
  {
    int a, b, c;
    int ret = 1;
    for (a=0; a<data.size()-1; a++){
      for (b=0; b<data[a].size()-1; b++){
        for (c=b+1; c<data[a].size(); c++){
          if (data[a][b] == data[a][c])
          {
            int column_width = c-b;
            if ((column_width+1)*(column_width+1) > ret){
              int f=findSquare(data,column_width,a,b);
              if(f>0){
                ret=f;
              }
            }
            
          }
        }
      }
    }
    return ret;
  }
};
