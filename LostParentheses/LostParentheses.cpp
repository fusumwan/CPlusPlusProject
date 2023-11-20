#include <iostream>
#include <vector>
#include <string>
#include <sstream>


//g++ -std=c++11 LostParentheses.cpp -o LostParentheses
//./LostParentheses

/*
Problem (5) LostParentheses:
Initial thoughts:

As the question given a String e containing an expression without parentheses. 

First of all, sum up all the non-negatives value  and then sum up all negatives value.

Then I will find out the the minimum value the original expression might have before removing the parentheses.

Pseudo-Code:

class LostParentheses {
    public procedure minResult(string e)
        vector <int> list = process(e);
        Node n;
        for (int i = 0; i < list.size(); i++) {
            n=NonNegatives(n,list[i]);
        }
        return n.result();
    endprocedure
endclass



Reflection:

In this question, I would like to use an O(n) brute force solution, 

it is pretty easy to find all the combination of sum;

In order to sum up all non negatives I iterate through comparing values and check if the value of the less than zero.

So I can find out the result of these data.


*/



using namespace std;
class Node{
public:
    int x;
    int y;
    Node(){
        x=0;
        y=0;
    }
    int result(){
        return x+y;
    }
};
class LostParentheses {
    public:
    Node NonNegatives(Node n,int value){
        if (value < 0) {
            n.x+=n.y;
            n.y= value;
        } else if (n.y != 0) {
            n.y -= value;
        } else {
            n.x += value;
        }
        return n;
    }
    int minResult(string e)
    {
        vector <int> list = process(e);
        Node n;
        for (int i = 0; i < list.size(); i++) {
            n=NonNegatives(n,list[i]);
        }
        return n.result();
    }

    vector <int> process(string e)
    {
        vector<int> ans;
        
        string str = "";
        bool find = true;

        for (int i = 0; i < e.length(); i++) {
            char c = e[i];
            if (c != '+' && c != '-') {
                str += c;
                continue;
            }

            if (find == true) ans.push_back(stoi(str));
            else ans.push_back(0 - stoi(str));

            if (c == '+') find = true;
            else find = false;
            str = "";
        }

        if (find == true) ans.push_back(stoi(str));
        else ans.push_back(0 - stoi(str));
        return ans;
    }
};


