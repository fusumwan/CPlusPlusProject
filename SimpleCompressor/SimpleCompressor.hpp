#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

//g++ -std=c++98 SimpleCompressor.hpp -o SimpleCompressor
//./SimpleCompressor


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
        stack<int> opens;
        for (int m = 0; s.find('[')!=std::string::npos; ) {
            for (int j = m; j < s.length(); j++) {
                if (s.at(j) == '[')
                    opens.push(j);
                else if (s.at(j) == ']') {
                    int i = opens.top();
                    opens.pop();
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
