#include <iostream>
#include <string>
#include <vector>
#include <stack>




using namespace std;

//g++ -std=c++98 MagicSpell.hpp -o MagicSpell
//./MagicSpell


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
};

