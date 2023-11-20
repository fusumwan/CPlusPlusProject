#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



using namespace std;
//g++ GoodHours.hpp -o GoodHours



template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

//It is the right way to split a string into a vector of strings
vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        unsigned int index = str.find(token);
        if(index <str.size() && index>=0){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0){
                result.push_back(str);
                break;
            }
        }else{
            result.push_back(str);
            break;
        }
    }
    return result;
}
class Node{
public:
    vector<string> stime;
    bool is_good;
    int h1;
    int h2;
    int m1;
    int m2;
    Node(){
        is_good=false;
        h1=0;
        h2=0;
        m1=0;
        m2=0;
    }
    void set_time(vector<string> times){
        stime=times;
        h1=atoi(times[0].substr(0,1).c_str());
        h2=atoi(times[0].substr(1,1).c_str());
        m1=atoi(times[1].substr(0,1).c_str());
        m2=atoi(times[1].substr(1,1).c_str());
    }
    bool pattern1(){
        if(h1==h2*m1*m2){
            //printf("%d=%d*%d*%d :%d  ",h1,h2,m1,m2,h1);
            return true;
        }
        return false;
    }
    bool pattern2(){
        if(h1*h2==m1*m2){
            //printf("%d*%d=%d*%d :%d  ",h1,h2,m1,m2,h1*h2);
            return true;
        }
        return false;
    }
    bool pattern3(){
        if(h1*h2*m1==m2){
            //printf("%d*%d*%d=%d :%d  ",h1,h2,m1,m2,h1*h2*m1);
            return true;
        }
        return false;
    }
    bool good(){
        if(pattern1() || pattern2() || pattern3()){
            
            return true;
        }
        return false;
    }
};

class GoodHours{
public:
    int time_to_min(vector<string> time_str){
        int hour = (time_str[0][0]!='0')? std::atoi(time_str[0].c_str()) : std::atoi(time_str[0].substr(1,1).c_str());
        int min = (time_str[1][0]!='0')? std::atoi(time_str[1].c_str()) : std::atoi(time_str[1].substr(1,1).c_str());
        return hour * 60 + min;
    }
    string time_add_min(vector<string> time_str,int m){
        int hour = (time_str[0][0]!='0')? std::atoi(time_str[0].c_str()) : std::atoi(time_str[0].substr(1,1).c_str());
        int min = (time_str[1][0]!='0')? std::atoi(time_str[1].c_str()) : std::atoi(time_str[1].substr(1,1).c_str());
        if(m>59){
            m=0;
        }
        min+=m;
        if(min>59){
            min=60-min;
            hour+=1;
        }
        if(hour>23){
            hour=0;
        }
        string h_str=to_string(hour);
        string m_str=to_string(min);
        if(h_str.length()==1)
            h_str="0"+h_str;
        if(m_str.length()==1)
            m_str="0"+m_str;
        return h_str+":"+m_str;
    }
    int howMany(string beforeTime, string afterTime){
        vector<string> beforetime_str=split(beforeTime,":");
        vector<string> afterTime_str=split(afterTime,":");
        int btime=time_to_min(beforetime_str);
        int atime=time_to_min(afterTime_str);
        int goodtime=0;
        Node n;
        while(btime<=atime){
            n.set_time(beforetime_str);
            if(n.good()){
                //printf("\n");
                goodtime+=1;
            }else{
                
            }
            if(btime==710){
                goodtime+=0;
            }
            string t=time_add_min(beforetime_str,1);
            beforetime_str=split(t,":");
            btime+=1;
        }
        return goodtime;
    }
};
