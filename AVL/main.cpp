
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

//g++ main.cpp -o main
//./main



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


// Checks whether input string is a decimal digit character.
bool is_digit(std::string str){
    bool result=true;
    for(unsigned int i=0;i<str.length();i++){
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


// An AVL tree node
class Node
{
public:
    int key;
    Node *left;
    Node *right;
    int height;
    int get_height(){
        return height;
    }
    void set_height(int _height){
        height=_height;
    }
    int data(){
        return key;
    }
    void set_data(int data){
        key=data;
    }
    void set_key(int _key){
        key=_key;
    }
    Node *lnode(){
        return left;
    }
    void set_lnode(Node* _left){
        left=_left;
    }
    Node *rnode(){
        return right;
    }
    void set_rnode(Node* _right){
        right=_right;
    }
};


class AVL{
public:
    int g_height;
    AVL(){
        g_height=1;
    }
    int height(Node *N)
    {
        if (N == NULL)
            return 0;
        return N->height;
    }
    
    Node* create(int key)
    {
        Node* node = new Node();
        node->set_data(key);
        node->set_lnode(NULL);
        node->set_rnode(NULL);
        node->set_height(1);
        return(node);
    }
    
    Node *RotateR(Node *y)
    {
        Node *x = y->left;
        Node *x_r = x->right;
        
        x->right = y;
        y->left = x_r;
        
        
        y->height = FindHeight(y);
        x->height = FindHeight(x);
        
        return x;
    }
    
    Node *RotateL(Node *x)
    {
        Node *y = x->right;
        Node *y_l = y->left;
        
        y->left = x;
        x->right = y_l;
        
        x->height = FindHeight(x);
        y->height = FindHeight(y);
        
        return y;
    }
    
    int getBalance(Node *N)
    {
        if (N == NULL)
            return 0;
        return height(N->left) -
        height(N->right);
    }
    
    Node* insert(Node* node, int key)
    {
        if (node == NULL)
            return(create(key));
        
        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;
        
        node->height = FindHeight(node);
        node=insert_balance_factor(node,key);
        return node;
    }
    
    Node* insert_balance_factor(Node *node,int key){
        int next=0;
        int balance = getBalance(node);
        node=ILLCase(balance,node,key,&next);
        if(next==1) return node;
        node=IRRCase(balance,node,key,&next);
        if(next==1) return node;
        node=ILRCase(balance,node,key,&next);
        if(next==1) return node;
        node=IRLCase(balance,node,key,&next);
        if(next==1) return node;
        return node;
    }
    
    Node* ILLCase(int balance,Node *node,int key,int *next){
        if (balance > 1 && key < node->left->key){
            *next=1;
            return RotateR(node);
        }
        *next=0;
        return node;
    }
    Node* IRRCase(int balance,Node *node,int key,int *next){
        if (balance < -1 && key > node->right->key){
            *next=1;
            return RotateL(node);
        }
        *next=0;
        return node;
    }
    
    
    Node* ILRCase(int balance,Node *node,int key,int *next){
        if (balance > 1 && key > node->left->data())
        {
            node->set_lnode(RotateL(node->lnode()));
            *next=1;
            return RotateR(node);
        }
        *next=0;
        return node;
    }
    
    Node* IRLCase(int balance,Node *node,int key,int *next){
        if (balance < -1 && key < node->rnode()->data())
        {
            node->set_rnode(RotateR(node->rnode()));
            *next=1;
            return RotateL(node);
        }
        *next=0;
        return node;
    }
    Node* FindNode(Node* node, int value){
        if (node == NULL)
            return NULL;
        
        if (node->key == value)
            return node;
        
        Node* res1;
        if(node->left!=NULL){
            res1=FindNode(node->left, value);
            if(res1!=NULL) return res1;
        }
        Node* res2;
        if(node->right!=NULL){
            res2=FindNode(node->right, value);
            if(res2!=NULL) return res2;
        }
        
        return NULL;
    }
    
    Node * minimum(Node* node)
    {
        Node* min = node;
        while (min->left != NULL)
            min = min->left;
        return min;
    }
    Node * maximum(Node* node){
        // Base case
        if (node == NULL)
            return NULL;
        
        int res = node->key;
        int lres = (node->left!=NULL )?maximum(node->left)->key:0;
        int rres = (node->right!=NULL)?maximum(node->right)->key:0;
        if (lres > res)
            res = lres;
        if (rres > res)
            res = rres;
        
        return FindNode(node,res);
    }
    
    int FindHeight(Node *node){
        int l=height(node->left);
        int r=height(node->right);
        return ((l>r)?l:r)+1;
    }
    
    
    Node* Remove(Node* root, int key)
    {
        if (root == NULL)
            return root;
        
        root=delete_smaller(root,key);
        root=delete_greater(root,key);
        

        if(root->data()==key)
        {
            if(root->lnode()==NULL){
                if(root->rnode()==NULL){
                    root->set_rnode(root);
                    root=NULL;
                }
                else{
                    *root=*root->rnode();
                }
                if(root!=NULL){
                    if(root->rnode()!=NULL){
                        free(root->rnode());
                    }
                }
            }
            else if(root->rnode()==NULL){
                if(root->lnode()==NULL){
                    root->set_lnode(root);
                    root=NULL;
                }
                else{
                    *root=*root->lnode();
                }
                if(root!=NULL){
                    if(root->lnode()!=NULL){
                        free(root->lnode());
                    }
                }
            }
            else{
                Node* temp = maximum(root->lnode());
                root->set_data(temp->data());
                root->set_lnode(Remove(root->lnode(),temp->data())) ;
            }
        }
        
        
        if (root == NULL)
            return root;
        root->height = FindHeight(root);
        root=delete_balance_factor(root);
        return root;
    }
    
    Node* delete_smaller(Node* node,int key){
        if (node->data()>key)
            node->set_lnode(Remove(node->lnode(), key));
        return node;
    }
    
    Node* delete_greater(Node* node,int key){
        if (node->data()<key)
            node->set_rnode(Remove(node->rnode(), key));
        return node;
    }
    
    Node* delete_balance_factor(Node* node){
        int balance = getBalance(node);
        int next=0;
        node=DLLCase(balance,node,&next);
        if(next==1) return node;
        node=DLRCase(balance,node,&next);
        if(next==1) return node;
        node=DRRCase(balance,node,&next);
        if(next==1) return node;
        node=DRLCase(balance,node,&next);
        if(next==1) return node;
        return node;
    }
    
    Node* DLLCase(int balance,Node *node,int *next){
        if (balance > 1 &&
            getBalance(node->left) >= 0){
            *next=1;
            return RotateR(node);
        }
        *next=0;
        return node;
    }
    
    Node* DLRCase(int balance,Node *node,int *next){
        if (balance > 1 &&
            getBalance(node->left) < 0)
        {
            node->left = RotateL(node->left);
            *next=1;
            return RotateR(node);
        }
        *next=0;
        return node;
    }
    
    Node* DRRCase(int balance,Node *node,int *next){
        if (balance < -1 &&
            getBalance(node->right) <= 0){
            *next=1;
            return RotateL(node);
        }
        *next=0;
        return node;
    }
    Node* DRLCase(int balance,Node *node,int *next){
        if (balance < -1 &&
            getBalance(node->right) > 0)
        {
            *next=1;
            node->right = RotateR(node->right);
            return RotateL(node);
        }
        *next=0;
        return node;
    }
    
    
    void Preorder(Node *root)
    {
        if(root != NULL)
        {
            cout << root->key << " ";
            Preorder(root->left);
            Preorder(root->right);
        }
    }
    
    void Inorder(Node* node){
        if(node != NULL){
            Inorder(node->left);
            printf("%d ",node->key);
            Inorder(node->right);
        }
    }
    
    void Postorder(Node *node){
        if(node != NULL){
            // first recur on left subtree
            Postorder(node->left);
            
            // then recur on right subtree
            Postorder(node->right);
            
            // now deal with the node
            printf("%d ",node->key);
        }
    }
    
    bool Find(Node* node, int value){
        if (node == NULL)
            return false;
        
        if (node->key == value)
            return true;
        
        bool res1=false;
        if(node->left!=NULL){
            res1 = Find(node->left, value);
        }
        bool res2=false;
        if(node->right!=NULL){
            res2 = Find(node->right, value);
        }
        
        return res1 || res2;
    }
    
};

void Display(string input){
    AVL dp;
    Node *root = NULL;
    vector<string> list=split(input," ");
    string order="";
    
    for(int i=0;i<(int)list.size();i++){
        //cout<<list[i]<<endl;
        if(list[i].length()>1){
            char action=list[i][0];
            string data=list[i].substr(1,list[i].length()-1);
            
            if(action=='A' && is_digit(data)){
                if(!dp.Find(root, stoi(data))){
                    root = dp.insert(root, stoi(data));
                }
            }
            else if(action=='D' && is_digit(data)){
                if(dp.Find(root, stoi(data))){
                    root = dp.Remove(root, stoi(data));
                }
                
            }
            else{
                order=list[i];
            }
        }
    }
    
    
    if(root==NULL){
        cout<<"EMPTY"<<endl;
    }
    else if(order=="IN"){
        dp.Inorder(root);
    }
    else if(order=="PRE"){
        dp.Preorder(root);
    }
    else if(order=="POST"){
        dp.Postorder(root);
    }
    //cout<<endl;
}


vector<int> nodes;
static string variable="";
static string command="";
static string jscommand="";
bool FindArray(int value){
    for(int i=0;i<(int)nodes.size();i++){
        if(nodes[i]==value){
            return true;
        }
    }
    return false;
}

int FindArrayIndex(int value){
    for(int i=0;i<(int)nodes.size();i++){
        if(nodes[i]==value){
            return i;
        }
    }
    return -1;
}

vector<int> RemoveFromArray(int value){
    for(int i=0;i<(int)nodes.size();i++){
        if(nodes[i]==value){
            nodes.erase (nodes.begin()+i);
            return nodes;
        }
    }
    return nodes;
}

string printcmd(string input){
    command="";
    jscommand="";
    variable="";
    nodes.clear();
    
    vector<string> list=split(input," ");
    int count=1;
    for(int i=0;i<(int)list.size();i++){
        char action=list[i][0];
        string data=list[i].substr(1,list[i].length()-1);
        if(action=='A'){
            if(!FindArray(stoi(data))){
                nodes.push_back(stoi(data));
                command+=list[i]+" ";
                
                count+=1;
            }
        }
        else if(action=='D'){
            
            int index=FindArrayIndex(stoi(data));
            if(index>-1){
                nodes.erase(nodes.begin()+index);
                command+=list[i]+" ";
                
                count+=1;
            }
        }
        else{
            command+=list[i]+" ";
        }
    }

    return command;
}


int main()
{
    
    string input="";
    getline(cin,input);
    input=printcmd(input);
    Display(input);
    
    
    
    
    return 0;
}

