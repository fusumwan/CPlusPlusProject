
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

/*
Problem (3) FibonacciDiv2:
Initial thoughts:

First of all, the question is using fibonacci series to solve. 
In this program, I need to find fibonacci series in program using for and while loops. 
I need to find the series upto a specific term or a number. 
And then I can increment the Fibonacci number every time in order to find the 

Pseudo-Code:

class FibonacciDiv2 {
public:
  public procedure  Fibonacci(int N)
    if (N < 0)
      return -1;
    else if (N == 0)
      return 0;
    else if (N <= 1)
      return N;
    
    int f[N+2]; 
    int i;

    f[0] = 0;
    f[1] = 1;
    Repeat (i = 2; i <= N; i++){
      f[i] = f[i-1] + f[i-2];
    EndRepeat
    return f[N];
  endprocedure

  public procedure find(int N){
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
  endprocedure
endclass

Reflection:

I would like to choose brute force. Because you have to compare the Fibonacci number with input number every time. 
If I want to improve performance of FibonacciDiv2 programm, I have to find the computational complexity of Fibonacci Sequence.
So I need to understand Big-O notation, but I to find how to calculate it for many functions. 
In particular, I've been trying to figure out the computational complexity of FibonacciDiv2 before I write the code.





*/

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


int main() {
  FibonacciDiv2 F;
  cout<<F.find(1000000)<<endl;
}