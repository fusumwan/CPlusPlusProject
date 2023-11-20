#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cctype>
#include <cmath>
#include <utility>

using namespace std;

//g++ -std=c++98 SimpleCompressor.cpp -o SimpleCompressor
//./SimpleCompressor


/*

Problem (4) SimpleCompressor:
Initial thoughts:

In this question, as the string shortens the sequence to "[DX]", where D is the number of times X is repeated.

So first of all, I have to create another class which is called 'StrControl'.

Because I want to initialize to the contents of the specified string.

And then I have to find the index of a '[' character or a substring in a given string.

For example, the string "CABABABABABABC" can be abbreviated as "C [6AB] C" or "C [2 [3AB]] C". 

My method will get string data. Get the position of '[' and ']' in the string and then get the number after the '[' character.

So we can compute the shortened raw string to generate the data and return the result. 



Pseudo-Code:
class SimpleCompressor{
public:
    public procedure uncompress(string s) {
        stack<int> o;
        Repeat (int m = 0; s.find('[')!=std::string::npos; ) {
            Repeat (int j = m; j < s.length(); j++) {
                if (s.at(j) == '[')
                    o.push(j);
                else if (s.at(j) == ']') {
                    int i = o.top();
                    o.pop();
                    StrControl buf;
                    buf.set(s.substr(0, i));
                    int len=s.at(i + 1) - '0';
                    Repeat (int k = 0; k < len; k++){
                        buf.add(s.substr(i + 2, j-(i + 2)));
                    }
                    m = buf.length();
                    buf.add(s.substr(j + 1));
                    s = buf.str();
                    break;
                }
            EndRepeat
        EndRepeat
        return s;
    endprocedure
endclass


Reflection:

I think I should use brute force to solve the problem. Because I have to find '[' position and generate the string. 

In this program, I am nesting two loops. If the string has n number of '[' character, 

the outer loop runs n times and then read the number after the '[' character, 

so the inner loop runs such value of times for each iteration of the outer loop.

Hence, if the string is 'C[3AB]C' I find the number after the '[' character is 3 then, I can generate 'AB' three times.

*/


class StrControl {
private:
    std::string main;
public:
    void set(string s){
        main=s;
    }
    void add(string s){
        main+=s;
    }
    int length(){
        return (int)main.length();
    }
    int charAt(int index){
        return main.at(index);
    }
    void setCharAt(int index,char ch){
        main[index]=ch;
    }
    int lastIndexOf(char ch, int index){
        int r=-1;
        if(index>=main.length()-1){
            index=(int)main.length()-1;
        }
        for(int i=index;i>=0;i--){
            if(main[i]==ch){
                r=i;
                break;
            }
        }
        return r;
    }
    int indexOf(char ch,int index){
        int r=-1;
        for(int i=index;i<main.length();i++){
            if(main[i]==ch){
                r=i;
                break;
            }
        }
        return r;
    }
    string str(){
        return main;
    }
};

class SimpleCompressor{
public:
    string uncompress(string s) {
        stack<int> o;
        for (int m = 0; s.find('[')!=std::string::npos; ) {
            for (int j = m; j < s.length(); j++) {
                if (s.at(j) == '[')
                    o.push(j);
                else if (s.at(j) == ']') {
                    int i = o.top();
                    o.pop();
                    StrControl buf;
                    buf.set(s.substr(0, i));
                    int len=s.at(i + 1) - '0';
                    for (int k = 0; k < len; k++){
                        buf.add(s.substr(i + 2, j-(i + 2)));
                    }
                    m = buf.length();
                    buf.add(s.substr(j + 1));
                    s = buf.str();
                    break;
                }
            }
        }
        return s;
    }
};

int main(){
    SimpleCompressor sc;
    cout<<sc.uncompress("C[6AB]C")<<endl;
    cout<<"CABABABABABABC"<<endl;
    cout<<sc.uncompress("C[2[3AB]]C")<<endl;
    cout<<"CABABABABABABC"<<endl;
    cout<<sc.uncompress("CO[1N]TEST")<<endl;
    cout<<"CONTEST"<<endl;
    cout<<sc.uncompress("[2[2AB]]")<<endl;
    cout<<"ABABABAB"<<endl;
    cout<<sc.uncompress("AAAAAAAAAAAAAAAAAAAAA")<<endl;
    cout<<"AAAAAAAAAAAAAAAAAAAAA"<<endl;
    return 0;
}

