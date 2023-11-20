
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <cmath>
#include <utility>
#include <set>
#include <cctype>
#include <queue>
#include <stack>


//g++ -std=c++98 FibonacciDiv2.cpp -o FibonacciDiv2
//./FibonacciDiv2

using namespace std;


class FibonacciDiv2 {
public:
  int Fibonacci(int N)
  {
    if (N < 0){
      return -1;
    }
    else if (N == 0){
      return 0;
    }
    else if (N <= 1){
      return N;
    }
    //Declare an array to store Fibonacci numbers.
    int f[N+2];   // 1 extra to handle case, n = 0
    int i;
    // 0th and 1st number of the series are 0 and 1 
    f[0] = 0;
    f[1] = 1;
    for (i = 2; i <= N; i++){
      // Add the previous 2 numbers in the series and store it
      f[i] = f[i-1] + f[i-2];
    }
    return f[N];
  }

  int find(int N){
    int num=1;
    int r1=Fibonacci(num);
    int r2=r1;
    if(r1==N){
        return 0;
      }
    while(r1<N){
      num+=1;
      r2=r1;
      r1=Fibonacci(num);
    }
    if(r1==N){
        return 0;
    }
    int f1=(N-r1<0)? (N-r1)*-1 : N-r1;
    int f2=(N-r2<0)? (N-r2)*-1 : N-r2;
    
    return (f1>f2)?f2:f1;
  }
};
