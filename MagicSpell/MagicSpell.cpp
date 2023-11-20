#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cctype>
#include <cmath>
#include <utility>




using namespace std;

//g++ -std=c++98 MagicSpell.cpp -o MagicSpell
//./MagicSpell



/*

Problem (2) MagicSpell:

Initial thoughts:

In order to decrypt the spell, I need to swap all the letters 'A' and 'Z'.

So I have to find A and Z in both the begin of string and end of string

and then I have to reverse their order. 


Pseudo-Code:

class MagicSpell
		public procedure fixTheSpell(string spell) {
				len=(int)spell.length();
				dec.setSpell(spell);
				int l = 0;
				int u = dec.length() - 1;
				Repeat (l < u) {
						if ((l = findLow('A','Z',l)) >= u) { break; }
						if (l >= (u = findUpper('A','Z',u))) { break;}
						char lower_str = dec.charAt(l);
						dec.setCharAt(l, dec.charAt(u));
						dec.setCharAt(u, lower_str);
						l++;
						u--;
				EndRepeat
				return dec.str();
		endprocedure
endclass

Reflection:

In this question, I would like to use an O(n) brute force solution, it is pretty easy to find all the letters 'A' and 'Z' and then swap them. 

I iterate through comparing two values and check if the value of the lowest position of 'A' or 'Z' less than the upper position of 'A' or 'Z'.

In the first time, I have to return the position of the last occurrence of 'A' and 'Z' in a string, 

and the string is searched from the end to the beginning, but returns the index starting at the beginning, at position 0.

So I can compare two value to find the lower position of 'A' or 'Z', later I have to find the upper location of 'A' or 'Z'. 

Finally, I can swap two letters in the string and reverse their order. 

*/



class StrControl {
private:
		std::string main;
public:
		void setSpell(string spell){
				main=spell;
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


class MagicSpell {
public:
		StrControl dec;
		int len;
		int findLow(char x,char y,int index){
				return min(positive(dec.indexOf(x, index)),
									 positive(dec.indexOf(y, index)));
		}
		int findUpper(char x,char y,int index){
				return max(dec.lastIndexOf('A', index),
									 dec.lastIndexOf('Z', index));
		}
		int positive(int index) {
				return (index < 0) ? len : index;
		}
		string fixTheSpell(string spell) {
				len=(int)spell.length();
				dec.setSpell(spell);
				int l = 0;
				int u = dec.length() - 1;
				while (l < u) {
						if ((l = findLow('A','Z',l)) >= u) { break; }
						if (l >= (u = findUpper('A','Z',u))) { break;}
						char lower_str = dec.charAt(l);
						dec.setCharAt(l, dec.charAt(u));
						dec.setCharAt(u, lower_str);
						l++;
						u--;
				}
				return dec.str();
		}
		
		
		// BEGIN CUT HERE
public:
		void run_test(int Case) {
				if ((Case == -1) || (Case == 0)) test_case_0();
				if ((Case == -1) || (Case == 1)) test_case_1();
				if ((Case == -1) || (Case == 2)) test_case_2();
				if ((Case == -1) || (Case == 3)) test_case_3();
				
		}
private:
		template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
		void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
		void test_case_0() { string Arg0 = "AZ"; string Arg1 = "ZA"; verify_case(0, Arg1, fixTheSpell(Arg0)); }
		void test_case_1() { string Arg0 = "ABACADA"; string Arg1 = "ABACADA"; verify_case(1, Arg1, fixTheSpell(Arg0)); }
		void test_case_2() { string Arg0 = "AABZCADZA"; string Arg1 = "AZBACZDAA"; verify_case(2, Arg1, fixTheSpell(Arg0)); }
		void test_case_3() { string Arg0 = "AZBASGHNAZAHBNVZZGGGAGGZAZ"; string Arg1 = "ZABZSGHNAZZHBNVAZGGGAGGAZA"; verify_case(3, Arg1, fixTheSpell(Arg0)); }
		
		// END CUT HERE
};
// BEGIN CUT HERE
int main() {
		MagicSpell ___test;
		___test.run_test(-1);
}
// END CUT HERE
