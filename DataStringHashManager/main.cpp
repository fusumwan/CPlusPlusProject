#include <iostream>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <utility>
#include <sstream>
#include <cstring>
using namespace std;

/*
DataString HashManager

Description: A versatile string processing application, DataString HashManager efficiently manages, adds, and removes data in a hash table, ensuring optimal data organization and quick retrieval. It handles complex string manipulations and offers robust hash table operations.

*/


template <typename T>
static string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

//g++ main.cpp -o main
//./main

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

bool is_digit(std::string str){
    bool result=true;
    for(unsigned int i=0;i<(unsigned int)str.length();i++){
        if(!isdigit(str[i])){
            result=false;
            break;
        }
    }
    return result;
}

// Checks whether input string is a positive number.
bool is_positive_number(std::string str){
    bool result=false;
    if(is_digit(str)){
        result=(stoi(str)>=0)?true:false;
    }
    return result;
}

class HashNode{
    public:
    char key;
    vector<string> value;
    
    HashNode(char key, vector<string> value){
        this->value = value;
        this->key = key;
    }
};

class HashTable{
    HashNode **arr;
    int hashtable_size;
    int size;
    HashNode *empty_hashtable;

    public:
    HashTable(){
        hashtable_size = 10;
        size=0;
        arr = new HashNode*[hashtable_size];
        for(int i=0 ; i < hashtable_size ; i++)
            arr[i] = NULL;
        vector<string> empty;
        empty_hashtable = new HashNode(-1, empty);
    }

    

    void insert(char key, vector<string> value)
    {
        HashNode *temp = new HashNode(key, value);
        int index_hashtable = findKey(key);
        while(arr[index_hashtable] != NULL && arr[index_hashtable]->key != key
            && arr[index_hashtable]->key != -1)
        {
            index_hashtable++;
            index_hashtable %= hashtable_size;
        }
        if(arr[index_hashtable] == NULL || arr[index_hashtable]->key == -1)
            size++;
        arr[index_hashtable] = temp;
    }

    vector<string> get(char key)
    {
        vector<string> empty;
        int index_hashtable = findKey(key);
        int counter =0;
        while(arr[index_hashtable] != NULL)
        {
            if(counter++>hashtable_size)
                return empty;
            if(arr[index_hashtable]->key == key)
                return arr[index_hashtable]->value;
            index_hashtable++;
            index_hashtable %= hashtable_size;
        }
        return empty;
    }
    
    int findKey(char key)
    {
        return key % hashtable_size;
    }
    void display()
    {
        vector<string> results;
        for(int c=0;c<26;c++){
            for(int i=0 ; i<hashtable_size ; i++)
            {
                if(arr[i] != NULL &&
                   arr[i]->key != -1 &&
                   arr[i]->key-97==c){
                    string shots="";
                    for(int j=0;j<(int)arr[i]->value.size();j++){
                        string data=arr[i]->value[j];
                        if(data!=""){
                            results.push_back(data);
                        }
                    }
                }
            }
        }
        for(int i=0;i<(int)results.size();i++){
            if(i>0)
                cout<<" ";
            cout<<results[i];
        }
    }
};

vector<string> initial_shots(){
    vector<string> list;
    for(int i=0;i<26;i++){
        list.push_back("");
    }
    return list;
}

int to_shot_key(char key){
    switch(key){
        case 'a': return 0; break;
        case 'b': return 1; break;
        case 'c': return 2; break;
        case 'd': return 3; break;
        case 'e': return 4; break;
        case 'f': return 5; break;
        case 'g': return 6; break;
        case 'h': return 7; break;
        case 'i': return 8; break;
        case 'j': return 9; break;
        case 'k': return 10; break;
        case 'l': return 11; break;
        case 'm': return 12; break;
        case 'n': return 13; break;
        case 'o': return 14; break;
        case 'p': return 15; break;
        case 'q': return 16; break;
        case 'r': return 17; break;
        case 's': return 18; break;
        case 't': return 19; break;
        case 'u': return 20; break;
        case 'v': return 21; break;
        case 'w': return 22; break;
        case 'x': return 23; break;
        case 'y': return 24; break;
        case 'z': return 25; break;
        default: return 0;
    }
    
}




char start_begin(HashTable *h){
    char ch='a';
    while(h->get(ch).size()!=0){
        ch+=1;
        if(ch>122){
            ch=97;
            break;
        }
    }
    return ch;
}

char data_last_c(HashTable *h,char i){
    vector<string> shotlist=h->get(i);
    for(int i=0;i<(int)shotlist.size();i++){
        if(shotlist[i]!=""){
            string data=shotlist[i];
            return data[data.length()-1];
        }
    }
    return -1;
}

bool remove_data(HashTable *h,string data){
    char ch='a';
    while(ch<=122){
        vector<string> shotlist=h->get(ch);
        for(int i=0;i<(int)shotlist.size();i++){
            if(shotlist[i]==data){
                shotlist[i]="";
                h->insert(ch,shotlist);
                return true;
            }
        }
        ch+=1;
    }
    return false;
}

int next_index(HashTable *h,char key,char ch){
    char i=ch;
    do{
        if(h->get(i).size()==0){
            return i;
        }
        
        i+=1;
        if(i>122){
            i=97;
            break;
        }
    }while (i<=122 && i>=97);
    return -1;
}

bool find_data(HashTable *h,string data){
    int ch=97;
    while(ch<=122){
        vector<string> shotlist=h->get(ch);
        for(int i=0;i<(int)shotlist.size();i++){
            if(shotlist[i]==data){
                
                return true;
            }
        }
        ch+=1;
    }
    return false;
}

void process2(string input){
    bool debug=false;
    int index=-1;
    bool pre_full=false;
    char curr=-1;
    HashTable *h = new HashTable();
    vector<string> list=split(input," ");
    for(int i=0;i<(int)list.size();i++){
        char action=list[i][0];
        string data=list[i].substr(1,list[i].length()-1);
        if(action=='A' && !find_data(h,data)){
            char key=data[data.length()-1];
            if(!pre_full){
                if(index==-1){
                    index=key;
                    curr=key;
                }
                else{
                    key=index;
                    curr=index;
                }
            }else{
                if(key!=curr){
                    index=next_index(h,key,index);
                    curr=key;
                    key=index;
                }
                key=index;
            }
            if(h->get(key).size()==0){
                vector<string> shotlist=initial_shots();
                int skey=to_shot_key(key);
                int t=0;
                while(shotlist[skey]!="" && t<26){
                    skey+=1;
                    if(skey>25){
                        skey=skey-26;
                        int diff=key-97;
                        index=index-diff;
                        
                    }
                    t+=1;
                }
                shotlist[skey]=data;
                h->insert(key,shotlist);
                if(debug){
                    h->display();
                    cout<<endl;
                }
            }
            else{
                
                vector<string> shotlist=h->get(key);
                int skey=to_shot_key(key);
                if(shotlist[skey]!=data){
                    int t=0;
                    while(shotlist[skey]!="" && t<26){
                        skey+=1;
                        if(skey>25){
                            skey=skey-26;
                            index=start_begin(h);
                            key=index;
                            curr=data[data.length()-1];
                            shotlist=initial_shots();
                            pre_full=true;
                            break;
                            
                        }
                        t+=1;
                    }
                    shotlist[skey]=data;
                    h->insert(key,shotlist);
                    if(debug){
                        h->display();
                        cout<<endl;
                    }
                }
            }
        }
        else if(action=='D'){
            if(!pre_full){
                char key=data[data.length()-1];
                vector<string> shotlist=h->get(key);
                for(int i=0;i<(int)shotlist.size();i++){
                    if(shotlist[i]==data){
                        shotlist[i]="";
                        h->insert(key,shotlist);
                        if(debug){
                            h->display();
                            cout<<endl;
                        }
                        break;
                    }
                }
            }
            else{
                remove_data(h,data);
            }
        }
    }
    h->display();
    if(debug){
        cout<<endl;
    }
}


void process(string input){
    HashTable *h = new HashTable();
    vector<string> list=split(input," ");
    for(int i=0;i<(int)list.size();i++){
        char action=list[i][0];
        string data=list[i].substr(1,list[i].length()-1);
        if(action=='A'){
            char key=data[data.length()-1];
            if(h->get(key).size()==0){
                vector<string> shotlist=initial_shots();
                int skey=to_shot_key(key);
                int t=0;
                while(shotlist[skey]!="" && t<26){
                    skey+=1;
                    if(skey>25){
                        skey=skey-26;
                    }
                    t+=1;
                }
                shotlist[skey]=data;
                h->insert(key,shotlist);
            }
            else{
                vector<string> shotlist=h->get(key);
                int skey=to_shot_key(key);
                if(shotlist[skey]!=data){
                    int t=0;
                    while(shotlist[skey]!="" && t<26){
                        skey+=1;
                        if(skey>25){
                            skey=skey-26;
                            process2(input);
                            return;
                        }
                        t+=1;
                    }
                    shotlist[skey]=data;
                    h->insert(key,shotlist);
                }
            }
        }
        else if(action=='D'){
            char key=data[data.length()-1];
            vector<string> shotlist=h->get(key);
            for(int i=0;i<(int)shotlist.size();i++){
                if(shotlist[i]==data){
                    shotlist[i]="";
                    h->insert(key,shotlist);
                    break;
                }
            }
        }
    }
    h->display();

}
/*
case_1='Aaaa Accc Abbb'

case_2='Abba Aaaa Acca'

case_3='Abba Aaaa Acca Daaa'

case_4='Aac Abc Dac Acc'

case_5='Aaz Abz Acz Adz'

case_6='Aapple Apear Aorange Dpear Aapple Aapple Atomato Dapple Abanana'

case_7='Aaz Abz Acz Adz Aez Afz Agz Ahz Aiz Ajz Akz Alz Amz Anz Aoz Apz Aqz Arz Asz Atz Auz Avz Awz Axz Ayz'

case_8='Daa Daaa Aaa Aaaa Aaaaaa Daaa Daa Aa Daaaaaaa Aaaaaaa'

case_9='Azow Aqow Amow Acow Avow Asow Aozz Aaoa Acoa Azow Aqoz Awoz Amoz Anozv Dssays Dtraditio Dsow Dsow'
*/

int main(){
    string input="";
    try
    {
        getline(cin,input);
        process(input);
        
        
    }
    catch(int e)
    {
 
    }

   
    return 0;
}
