#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//gcc -std=c++98 Inchworm.cpp -o Inchworm
//Compile and link with g++ not gcc
//g++ -std=c++98 Inchworm.cpp -o Inchworm
//./Inchworm
/*

Problem (1) Inchworm:

Initial thoughts:

This question is to find the greatest common divisor of the distribution of rest and leaf,  
which are not all zero, is the largest positive integer that divides each of the integers. 

Pseudo-Code:

class Inchworm
    public procedure lunchtime (branch,rest,leaf)
          tmp=GreatestCommonDivisor(rest,leaf)
          tmp1=leaf * rest / tmp
          tmp2=branch / tmp1
          return tmp2+1;
    endprocedure
    public procedure GreatestCommonDivisor(rest,leaf)
       if (rest == 0 || leaf == 0) 
          return 0; 
       else if (rest > leaf) 
          return GreatestCommonDivisor(rest-leaf, leaf); 
       else if (rest == leaf) 
          return rest; 
       else 
          return GreatestCommonDivisor(rest, leaf-rest); 
       return -1;
    endprocedure
endclass

Reflection:

Apparently, there’s also a mathematical solution. I should probably look for math solutions.
Inchworm will decide to eat for food according to the distribution of leaves,  
if leaves are starting the branch 0, the inchworm will eat immediately,  
so the distance of the leaves can be subtracted from the moving distance of the inchworm. 
The distance of the distribution of leaf is always reduced by the moving distance of the inchworm until it reaches zero 

*/



using namespace std;

class Inchworm {
 public:
  int lunchtime(int branch, int rest, int leaf) {
    // -- main code --
    double tmp=GreatestCommonDivisor(rest, leaf);
    double tmp1=(leaf * rest / tmp);
    double tmp2=branch / tmp1;
    return tmp2+1;
  }
  int GreatestCommonDivisor(int value1, int value2) { 
     if (value1 == 0 || value2 == 0) 
        return 0; 
     else if (value1 > value2) 
        return GreatestCommonDivisor(value1-value2, value2); 
     else if (value1 == value2) 
        return value1; 
     else 
        return GreatestCommonDivisor(value1, value2-value1); 
  } 
};

int main()
{
  Inchworm test;
  
  return 0;
}

