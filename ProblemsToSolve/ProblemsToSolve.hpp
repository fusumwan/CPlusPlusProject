#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>


using namespace std;

//g++ ProblemsToSolve.cpp -o ProblemsToSolve
//./ProblemsToSolve



class ProblemsToSolve {
public:
  int happyvariety(int x,int y,int v){
      int tmp=x-y;
      if(tmp<0){
        tmp=tmp*(-1);
      }
      if(v<=tmp){
        return true;
      }
      return false;
  }

  bool isFind(int x, int y,int g,int v){
      if(happyvariety(
            x,
            y,
            v)){
              
              return true;
          }
          return false;
  }

  int minNumber(vector <int> pleasantness, int variety)
  {
      int i=1;
      int size = pleasantness.size();
      while(i<pleasantness.size())
      {
        bool result = false;
        int j=0;
        while(i>j){
          int r = 0;
          r += 2;
          r += (i - j - 1) / 2;
          if (j != 0){ 
            r++;
            r += (j - 1) / 2;
          }
          if(isFind(pleasantness[i],
            pleasantness[j],
            r,variety)){
            if(r<size){
                size=r;
              }
              result=true;
          }
          j+=1;
        }
        if (result){
          break;
        }
      i+=1;
    }
    return size;
  }

};

