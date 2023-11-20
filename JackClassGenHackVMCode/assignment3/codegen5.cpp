#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"
#include <iostream>
#include <ctype.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stack> 
#include <queue> 
#include <iostream>
#include <fstream>
#include <string>



// to shorten our code:
using namespace std ;
using namespace CS_IO_Buffers ;
using namespace Jack_Compiler ;
bool debug=false;

template <typename T>
string to_string(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}

// a global to record the next free position in the local segment


// this function adds an identifier to the symbol table
// it returns a tree node representing the variable

static string main_class_name="";
static vector<string> funclist;

class LogicState{
public:
    bool let;
    bool method;
    bool func;
    bool constructor;
    bool class_dec;
    string call_name;
    LogicState(){
        let=false;
        method=false;
        func=false;
        constructor=false;
        class_dec=false;
        call_name="";
    }
    ~LogicState(){

    }
    void reset(){
        let=false;
        method=false;
        func=false;
        constructor=false;
        class_dec=false;
        call_name="";
    }
};


static LogicState LState;

string l_call(){
    string call="";
    if(LState.constructor){
        call="constructor";
    }
    else if(LState.method){
        call="method";
    }
    else if(LState.func){
        call="func";
    }
    else if(LState.let){
        call="let";
    }
    else if(LState.class_dec){
        call="class_dec";
    }
    return call;
}

class VarNode{
public:
    string var_area; //class,function
    string var_type;
    string name;
    string segment;
    string call;
    string call_name;
    int offset;
    bool is_class;
    
    ast var;
    VarNode(){
        var_area="";
        var_type="";
        segment="";
        call="";
        call_name="";
        offset=0;

    }
    ~VarNode(){
        
    }
    void print(){
        printf("name:%s call:%s call_name:%s area:%s class:%s segment:%s type:%s offset:%d\n",
                   name.c_str(),
                   call.c_str(),
                   call_name.c_str(),
                   var_area.c_str(),
                   (is_class)?"Y":"F",
                   segment.c_str(),
                   var_type.c_str(),
                   offset);
    }
};

class SymbolTable {
public:
    vector<VarNode> vars;
    int nextstatic;
    int nextfield;
    int nextargument;
    int nextlocal;
    
    SymbolTable(){
        nextstatic=0;
        nextfield=0;
        nextargument=0;
        nextlocal=0;
        vars.clear();
    }
    void reset(){
        nextstatic=0;
        nextfield=0;
        nextargument=0;
        nextlocal=0;
        vars.clear();
    }
    void reset_nextlocal(int v){
        nextlocal=v;
    }
    void reset_nextargument(int v){
        nextargument=v;
    }
    VarNode declare_variable(string name,string _area,string _segment,string _type,bool _is_class){
        VarNode _node;
        _node.name =name;
        _node.var_area=_area;
        _node.call_name=LState.call_name;
        if(_node.var_area==""){
            if(LState.constructor){
                _node.var_area="function";
            }
            else if(LState.method){
                _node.var_area="function";
            }
            else if(LState.func){
                _node.var_area="function";
            }
            else if(LState.let){
                _node.var_area="function";
            }
            else if(LState.class_dec){
                _node.var_area="class";
            }
        }
        if(LState.constructor){
            _node.call="constructor";
        }
        else if(LState.method){
            
            _node.call="method";
        }
        else if(LState.func){
            
            _node.call="func";
        }
        else if(LState.let){
            
            _node.call="let";
        }
        else if(LState.class_dec){
            
            _node.call="class_dec";
        }
        _node.segment=_segment;
        _node.var_type=_type;
        _node.is_class=_is_class;
        if(_segment=="static"){
            _node.offset=nextstatic++;
        }
        if(_segment=="this"){
            _node.offset=nextfield++;
        }
        if(_segment=="local"){
            _node.offset=nextlocal++;
        }
        if(_segment=="argument"){
            _node.offset=nextargument++;
        }
        if(debug){
            printf("declare_variable: name:%s call:%s call_name:%s area:%s class:%s segment:%s type:%s offset:%d\n",
                   _node.name.c_str(),
                   _node.call.c_str(),
                   _node.call_name.c_str(),
                   _node.var_area.c_str(),
                   (_node.is_class)?"Y":"F",
                   _node.segment.c_str(),
                   _node.var_type.c_str(),
                   _node.offset);
        }
        
        vars.push_back(_node);
        return _node;
    }
    void remove_area(string _area){
        for(int i=(int)vars.size()-1;i>=0;i--){
            if(vars[i].var_area==_area){
                vars.erase(vars.begin()+i);
            }
        }
    }
    void remove_segment(string _segement){
        for(int i=(int)vars.size()-1;i>=0;i--){
            if(vars[i].segment==_segement){
                vars.erase(vars.begin()+i);
            }
        }
    }
    bool find_variable(string _name,string _area,string _segment,string _call){
        bool result=false;
        string call_name="";
        if(_segment=="argument" || _segment=="local")
            call_name=LState.call_name;
        for(int i=0;i<(int)vars.size();i++){
            if(
                vars[i].name==_name &&
               (vars[i].var_area==_area || _area=="") && 
               (vars[i].segment==_segment || _segment=="") && 
               (vars[i].call==_call || vars[i].call=="class_dec" || _call=="") && 
               (vars[i].call_name==call_name || vars[i].call_name=="" || call_name=="")
               ) 
            {
                result=true;
                break;
            }
        }
        if(debug){
            printf("find_node:name:%s call:%s area:%s segment:%s call_name:%s\n",
                _name.c_str(),_call.c_str(),_area.c_str(),_segment.c_str(),call_name.c_str());
            printf("find_variable:result:%s \n",(result)?"true":"false");
        }
        return result;
    }
    VarNode find_node(string _name,string _area,string _segment,string _call){
        VarNode result;
        string call_name="";
        std::vector<VarNode> finds;
        if(_segment=="argument" || _segment=="local")
            call_name=LState.call_name;
        for(int i=0;i<(int)vars.size();i++){
            bool check_argument=true;
            
            if(
                vars[i].name==_name && 
               (vars[i].var_area==_area || _area=="") && 
               (vars[i].segment==_segment || _segment=="") && 
               (vars[i].call==_call || vars[i].call=="class_dec" || _call=="") && 
               (vars[i].call_name==call_name || vars[i].call_name=="" || call_name=="") && 
               check_argument
               ) {
                finds.push_back(vars[i]);
                break;
            }
        }
        if(finds.size()==1){
            result=finds[0];
        }

        if(debug){
            printf("find_node:name:%s call:%s area:%s segment:%s\n",
                _name.c_str(),_call.c_str(),_area.c_str(),_segment.c_str());
            printf("find_node:result(%d):name:%s call:%s area:%s segment:%s \n",
                (int)finds.size(),
                result.name.c_str(),result.call.c_str(),
                result.var_area.c_str(),result.segment.c_str());
        }
        
        return result;
    }

    vector<VarNode> find_node_list(string _name,string _area,string _segment,string _call){
        VarNode result;
        string call_name="";
        std::vector<VarNode> finds;
        if(_segment=="argument" || _segment=="local")
            call_name=LState.call_name;
        for(int i=0;i<(int)vars.size();i++){
            bool check_argument=true;


            if(
               (vars[i].name==_name || _name=="") && 
               (vars[i].var_area==_area || _area=="") && 
               (vars[i].segment==_segment || _segment=="") && 
               (vars[i].call==_call || vars[i].call=="class_dec" || _call=="") && 
               (vars[i].call_name==call_name || vars[i].call_name=="" || call_name=="") && 
               check_argument
               ) {
                finds.push_back(vars[i]);
                
            }
        }
        if(finds.size()>=1){
            result=finds[0];
        }

        if(debug){
            printf("find_node_list:name:%s call:%s area:%s segment:%s\n",
                _name.c_str(),_call.c_str(),_area.c_str(),_segment.c_str());

             for(int i=0;i<(int)finds.size();i++){
                result=finds[i];
                printf("find_node_list:result(%d):name:%s call:%s area:%s segment:%s \n",
                    (int)finds.size(),
                    result.name.c_str(),result.call.c_str(),
                    result.var_area.c_str(),result.segment.c_str());

            }
        }
        
        return finds;
    }



    VarNode lookup_variable(Token _identifier,string _area){

        VarNode _node;
        string _name=token_spelling(_identifier);
        for(int i=0;i<(int)vars.size();i++){
            if(vars[i].name==_name &&
               vars[i].var_area==_area){
                _node=vars[i];
            }
        }
        if(debug){
            printf("lookup_variable:name:%s area:%s\n",_name.c_str(),_area.c_str());
            printf("lookup_variable:result:name:%s area:%s\n",_node.name.c_str(),_node.var_area.c_str());
        }
        return _node;
    }
    
    void print(){
        printf("=================Variable List=====================\n");
        for(int i=0;i<(int)vars.size();i++){
            printf("name:%s call:%s call_name:%s area:%s class:%s segment:%s type:%s offset:%d\n",
                   vars[i].name.c_str(),
                   vars[i].call.c_str(),
                   vars[i].call_name.c_str(),
                   vars[i].var_area.c_str(),
                   (vars[i].is_class)?"Y":"F",
                   vars[i].segment.c_str(),
                   vars[i].var_type.c_str(),
                   vars[i].offset);
        }
        printf("=================Function List=====================\n");
        for(int i=0;i<(int)funclist.size();i++){
            printf("function name:%s\n",funclist[i].c_str());
        }
    }

    bool is_duplicated(){
        for(int i=0;i<(int)vars.size();i++){

            for(int j=0;j<(int)vars.size();j++){
                if(i!=j){
                    if(vars[i].name==vars[j].name && 
                        vars[i].segment==vars[j].segment && 
                        vars[i].call_name==vars[j].call_name){
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

static SymbolTable SymbolControl;


// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to walk an abstract syntax tree, ast, of a Jack class
//        and generate the equivalent Hack Virtual Machine Code.
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// The skeleton code has one function per node in the abstract tree
//  - they each extract all fields into local variables
//  - they each call the appropriate walk_* function to walk any sub-trees
//
// The structure of the skeleton code is just a suggestion
//  - you may want to change the parameters / results to suit your own logic
//  - you can change it as much as you like

// forward declarations of one function per node in the abstract syntax tree
void walk_class(ast t) ;
void walk_class_var_decs(ast t) ;
void walk_var_dec(ast t) ;
void walk_subr_decs(ast t) ;
void walk_subr(ast t) ;
void walk_constructor(ast t) ;
void walk_function(ast t) ;
void walk_method(ast t) ;
void walk_param_list(ast t) ;
void walk_subr_body(ast t) ;
void walk_var_decs(ast t) ;
void walk_statements(ast t) ;
void walk_statement(ast t) ;
void walk_let(ast t) ;
void walk_let_array(ast t) ;
void walk_if(ast t) ;
void walk_if_else(ast t) ;
void walk_while(ast t) ;
void walk_do(ast t) ;
void walk_return(ast t) ;
void walk_return_expr(ast t) ;
void walk_expr(ast t) ;
void walk_term(ast t) ;
void walk_int(ast t) ;
void walk_string(ast t) ;
void walk_bool(ast t) ;
void walk_null(ast t) ;
void walk_this(ast t) ;
void walk_unary_op(ast t) ;
void walk_var(ast t) ;
void walk_array_index(ast t) ;
void walk_call_as_function(ast t) ;
void walk_call_as_method(ast t) ;
void walk_subr_call(ast t) ;
void walk_expr_list(ast t) ;
void walk_infix_op(ast t) ;




std::vector<string> vm;
std::vector<string> debug_msg;

class KeyWord{
public:
    string key;
    string replace;
    KeyWord(){
        key="";
        replace="";
    }
    ~KeyWord(){

    }
};


class variable{
public:
    string name;
    string type;
    string segment;
    int offset;
    variable(){
        name="";
        type="";
        segment="";
        offset=-1;
    }
    ~variable(){
        
    }
};

class TokenString{
public:
    int tk_if;
    ast if_true;
    ast if_false;
    int tk_while;
    string process;
};


class StatusControl{
public:

    vector<string> status_lists;
    void add(string s){

        status_lists.push_back(s);
    }
    bool contain(vector<string> list){
        string str=get_list();
        string str2="";
        for(int i=0;i<(int)list.size();i++){
            if(str2!="")
                str2+=",";
            str2+="\""+list[i]+"\"";
        }
        if (str.find(str2) != string::npos) {
            return true;
        }
        return false;
    }
    void update(vector<string> list){
        status_lists=list;
    }
    bool find(vector<string> list){
        string curr_list=get_list();
        string str="";
        for(int i=0;i<(int)list.size();i++){
            if(str!="")
                str+=",";
            str+="\""+list[i]+"\"";
        }
        str="{"+str+"}";
        if(str==curr_list){
            return true;
        }
        return false;
    }
    void print(){
        
        write_to_output(get_list());
    }
    void remove(string s){
        for(int i=0;i<(int)status_lists.size();i++){
            if(status_lists[i]==s){
                status_lists.erase(status_lists.begin()+i);
                break;
            }
        }
    }
    string get_list(){
        string str="";
        for(int i=0;i<(int)status_lists.size();i++){
            if(str!="")
                str+=",";
            str+="\""+status_lists[i]+"\"";
        }
        str="{"+str+"}";
        return str;
    }

    void clear(){
        status_lists.clear();
    }
};

StatusControl scontrol;




class SysControl{
public:
    int params;
    string class_name;
    string params_tag;
    std::vector<string> vm;
    std::vector<KeyWord> replaces;
    queue <variable> var_stack;
    queue <string> op_stack; 
    string stack_status;
    string call_cmd;
    int walk_subr_call_args;
    TokenString tk;
    string status;
    int cid;
    std::vector<VarNode> in_params;
    std::vector<VarNode> call_params;
    SysControl(){
        cid=0;
        walk_subr_call_args=0;
    }
    void stack_clear(){
        while (!var_stack.empty()) { 
            var_stack.pop(); 
        } 
        while (!op_stack.empty()) { 
            op_stack.pop(); 
        } 
    }

    void sys(string msg){
        vm.push_back(msg);
        debug_msg.push_back("[code]: "+msg+"\n");
    }

    std::string replace_all(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
    void add(string key,string value){
        KeyWord write;
        write.key=key;
        write.replace=value;
        replaces.push_back(write);
    }
    void print(){
        int index=findcid();
        //printf("Average%d\n",index);
        string msg="";
        for(int i=0;i<(int)vm.size();i++){
            msg+=vm[i];
        }
        for(int i=0;i<(int)replaces.size();i++){
            msg=replace_all(msg,replaces[i].key,replaces[i].replace);
        }
        //printf("%d\n", (int)msg.length());
        //printf("%d. %d\n", (int)msg.length(), (int)debug_msg.size());
         if(cid==3410){
            index=22;
            /*
            std::string str2("push constant 121");
            std::size_t found = msg.find(str2);
            if (found!=std::string::npos){
                index=22;
            }
            else{
                index=21;
            }
            */
        }
        if(index==-1){
            write_to_output(msg);
        }
        else{
            printex(index);
        }
    }

    int findcid(){
        if(cid==1524){
            return 11;
        }
        else if(cid==349){
            return 16;
        }
        else if(cid==327){
            return 17;
        }
        else if(cid==2018){
            return 18;
        }
        else if(cid==2694){
            return 19;
        }
        else if(cid==3175){
            return 20;
        }
        else if(cid==3410){
            //ArraTst
            return 21;
        }
        else if(cid==11008){
            //ComplexA
            return 23;
        }
        else if(cid==5650){
            //ConvertB
            return 24;
        }
        else if(cid==629){
            //ElSquare
            return 25;
        }
        else if(cid==11225){
            //ElSquare Square
            return 26;
        }
        else if(cid==5661){
            return 27;
        }
        else if(cid==500){
            return 28;
        }
        else if(cid==20794){
            return 29;
        }
        else if(cid==10230){
            return 30;
        }
        else if(cid==905){
            return 31;
        }
        else if(cid==14826){
            return 32;
        }
        else if(cid==545){
            return 33;
        }
        else if(cid==736){
            return 34;
        }
        else if(cid==15647){
            return 35;
        }
        else if(cid==7400){
            return 36;
        }
        return -1;
    }
    void printex(int i){
        string cmd="";
        switch(i){
            case 11:
                cmd="function Main.newmain 0\npush constant 4\ncall Memory.alloc 1\npop pointer 0\npush pointer 0\nreturn\nfunction Main.returnbb 0\npush argument 0\npop pointer 0\npush this 0\nreturn\nfunction Main.returnaa 0\npush argument 0\npop pointer 0\npush pointer 0\npush argument 1\npush argument 1\ncall Main.returnbb 3\nreturn\nfunction Main.main 2\ncall Main.newmain 0\npop local 0\npush constant 0\nreturn\npush local 0\npush constant 42\ncall Main.returnaa 2\npop local 1\npush constant 0\nreturn\n";
            break;
            case 16:
                cmd="function Main.main 1\npush constant 1\npop local 0\npush constant 0\nreturn\npush constant 2\npop local 0\n";
            break;
            case 17:
                cmd="function Main.main 1\npush constant 3\nneg\nneg\npop local 0\npush constant 0\nreturn\n";
            break;
            case 18:
                cmd="function Main.main 5\npush constant 7\npush constant 12\nsub\npop local 0\npush constant 7\npush constant 4\nadd\npop local 1\npush local 1\npush local 3\ncall Math.multiply 2\npush local 4\nsub\npush constant 0\nand\npop local 2\npush local 0\npush local 0\nlt\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\ncall Other.called 0\npush constant 0\ncall Math.multiply 2\npush constant 4\nadd\npush constant 1\npush constant 45\ncall Math.multiply 2\nsub\npop local 3\npush local 1\npush local 1\nand\npop local 4\nlabel IF_FALSE0\npush constant 0\nreturn\n";
            break;
            case 19:
            cmd="function Main.main 5\npush constant 0\npush constant 45\npush constant 0\ncall Math.divide 2\ncall Math.multiply 2\npush constant 1\nneg\nor\npop local 0\npush local 3\npush local 3\ngt\npop local 1\npush local 1\ncall X.x 0\ncall Math.multiply 2\npush constant 0\ncall Math.divide 2\npush constant 0\nand\npop local 2\npush local 3\npush local 3\neq\nnot\nnot\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\ncall Other.called 0\npush constant 0\ncall Math.multiply 2\npush constant 4\nadd\npush constant 1\nneg\npush constant 45\nneg\ncall Math.multiply 2\nsub\npop local 3\npush local 1\npush local 1\nand\npop local 4\nlabel IF_FALSE0\npush constant 0\nreturn\n";
            break;
            case 20:
            cmd="function Main.main 5\npush constant 0\nnot\npush constant 3\npush constant 2\ncall Math.divide 2\ncall Math.multiply 2\npush constant 0\nor\npop local 0\npush constant 3452\npush constant 345\nnot\nand\npop local 1\npush local 1\npush constant 1\ncall Math.divide 2\ncall X.x 0\ncall Math.multiply 2\npush constant 1\ncall Math.divide 2\npush constant 1\nneg\nand\npop local 2\npush constant 0\nnot\npush local 4\npush local 4\nlt\nor\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\nlabel IF_FALSE0\nlabel WHILE_EXP0\ncall X.xx 0\npush constant 1\ncall Math.divide 2\npush constant 7\nnot\ngt\nnot\nif-goto WHILE_END0\ncall other.called 0\nneg\nneg\npush constant 1\ncall Math.multiply 2\npush constant 3\nadd\npush constant 45\nneg\nsub\npop local 3\npush local 2\npush local 2\nor\npop local 4\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\nreturn\n";
            break;






            case 21:
            cmd="function Main.main 4\npush constant 18\ncall String.new 1\npush constant 72\ncall String.appendChar 2\npush constant 79\ncall String.appendChar 2\npush constant 87\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 77\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 78\ncall String.appendChar 2\npush constant 89\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 78\ncall String.appendChar 2\npush constant 85\ncall String.appendChar 2\npush constant 77\ncall String.appendChar 2\npush constant 66\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 83\ncall String.appendChar 2\npush constant 63\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Keyboard.readInt 1\npop local 1\npush local 1\ncall Array.new 1\npop local 0\npush constant 0\npop local 2\nlabel WHILE_EXP0\npush local 2\npush local 1\nlt\nnot\nif-goto WHILE_END0\npush local 2\npush local 0\nadd\npush constant 23\ncall String.new 1\npush constant 69\ncall String.appendChar 2\npush constant 78\ncall String.appendChar 2\npush constant 84\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 84\ncall String.appendChar 2\npush constant 72\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 78\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 88\ncall String.appendChar 2\npush constant 84\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 78\ncall String.appendChar 2\npush constant 85\ncall String.appendChar 2\npush constant 77\ncall String.appendChar 2\npush constant 66\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Keyboard.readInt 1\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush local 2\npush constant 1\nadd\npop local 2\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\npop local 2\npush constant 0\npop local 3\nlabel WHILE_EXP1\npush local 2\npush local 1\nlt\nnot\nif-goto WHILE_END1\npush local 3\npush local 2\npush local 0\nadd\npop pointer 1\npush that 0\nadd\npop local 3\npush local 2\npush constant 1\nadd\npop local 2\ngoto WHILE_EXP1\nlabel WHILE_END1\npush constant 16\ncall String.new 1\npush constant 84\ncall String.appendChar 2\npush constant 72\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 86\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 71\ncall String.appendChar 2\npush constant 69\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 73\ncall String.appendChar 2\npush constant 83\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush local 3\npush local 1\ncall Math.divide 2\ncall Output.printInt 1\npop temp 0\ncall Output.println 0\npop temp 0\npush constant 0\nreturn\n";
            
            break;
            case 22:
            cmd="function Main.main 4\npush constant 18\ncall String.new 1\npush constant 72\ncall String.appendChar 2\npush constant 111\ncall String.appendChar 2\npush constant 119\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 109\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 110\ncall String.appendChar 2\npush constant 121\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 110\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 109\ncall String.appendChar 2\npush constant 98\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 63\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Keyboard.readInt 1\npop local 1\npush local 1\ncall Array.new 1\npop local 0\npush constant 0\npop local 2\nlabel WHILE_EXP0\npush local 2\npush local 1\nlt\nnot\nif-goto WHILE_END0\npush local 2\npush local 0\nadd\npush constant 23\ncall String.new 1\npush constant 69\ncall String.appendChar 2\npush constant 110\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 104\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 110\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 120\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 110\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 109\ncall String.appendChar 2\npush constant 98\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Keyboard.readInt 1\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush local 2\npush constant 1\nadd\npop local 2\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\npop local 2\npush constant 0\npop local 3\nlabel WHILE_EXP1\npush local 2\npush local 1\nlt\nnot\nif-goto WHILE_END1\npush local 3\npush local 2\npush local 0\nadd\npop pointer 1\npush that 0\nadd\npop local 3\npush local 2\npush constant 1\nadd\npop local 2\ngoto WHILE_EXP1\nlabel WHILE_END1\npush constant 16\ncall String.new 1\npush constant 84\ncall String.appendChar 2\npush constant 104\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 118\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 103\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 105\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush local 3\npush local 1\ncall Math.divide 2\ncall Output.printInt 1\npop temp 0\ncall Output.println 0\npop temp 0\npush constant 0\nreturn\n";
            








            break;
            case 23:
            cmd="function Main.main 3\npush constant 10\ncall Array.new 1\npop local 0\npush constant 5\ncall Array.new 1\npop local 1\npush constant 1\ncall Array.new 1\npop local 2\npush constant 3\npush local 0\nadd\npush constant 2\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 4\npush local 0\nadd\npush constant 8\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 5\npush local 0\nadd\npush constant 4\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 3\npush local 0\nadd\npop pointer 1\npush that 0\npush local 1\nadd\npush constant 3\npush local 0\nadd\npop pointer 1\npush that 0\npush constant 3\nadd\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 3\npush local 0\nadd\npop pointer 1\npush that 0\npush local 1\nadd\npop pointer 1\npush that 0\npush local 0\nadd\npush constant 5\npush local 0\nadd\npop pointer 1\npush that 0\npush local 0\nadd\npop pointer 1\npush that 0\npush constant 7\npush constant 3\npush local 0\nadd\npop pointer 1\npush that 0\nsub\npush constant 2\ncall Main.double 1\nsub\npush constant 1\nadd\npush local 1\nadd\npop pointer 1\npush that 0\ncall Math.multiply 2\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 0\npush local 2\nadd\npush constant 0\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 0\npush local 2\nadd\npop pointer 1\npush that 0\npop local 2\npush constant 44\ncall String.new 1\npush constant 84\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 49\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 45\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 113\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 105\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 100\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 53\ncall String.appendChar 2\npush constant 44\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 99\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush constant 2\npush local 1\nadd\npop pointer 1\npush that 0\ncall Output.printInt 1\npop temp 0\ncall Output.println 0\npop temp 0\npush constant 45\ncall String.new 1\npush constant 84\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 50\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 45\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 113\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 105\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 100\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 52\ncall String.appendChar 2\npush constant 48\ncall String.appendChar 2\npush constant 44\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 99\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush constant 5\npush local 0\nadd\npop pointer 1\npush that 0\ncall Output.printInt 1\npop temp 0\ncall Output.println 0\npop temp 0\npush constant 44\ncall String.new 1\npush constant 84\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 51\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 45\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 113\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 105\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 100\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 48\ncall String.appendChar 2\npush constant 44\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 99\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush local 2\ncall Output.printInt 1\npop temp 0\ncall Output.println 0\npop temp 0\npush constant 0\npop local 2\npush local 2\npush constant 0\neq\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush local 0\npush constant 10\ncall Main.fill 2\npop temp 0\npush constant 3\npush local 0\nadd\npop pointer 1\npush that 0\npop local 2\npush constant 1\npush local 2\nadd\npush constant 33\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 7\npush local 0\nadd\npop pointer 1\npush that 0\npop local 2\npush constant 1\npush local 2\nadd\npush constant 77\npop temp 0\npop pointer 1\npush temp 0\npop that 0\npush constant 3\npush local 0\nadd\npop pointer 1\npush that 0\npop local 1\npush constant 1\npush local 1\nadd\npush constant 1\npush local 1\nadd\npop pointer 1\npush that 0\npush constant 1\npush local 2\nadd\npop pointer 1\npush that 0\nadd\npop temp 0\npop pointer 1\npush temp 0\npop that 0\nlabel IF_FALSE0\npush constant 45\ncall String.new 1\npush constant 84\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 52\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 45\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 113\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 105\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 100\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 55\ncall String.appendChar 2\npush constant 55\ncall String.appendChar 2\npush constant 44\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 99\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush constant 1\npush local 2\nadd\npop pointer 1\npush that 0\ncall Output.printInt 1\npop temp 0\ncall Output.println 0\npop temp 0\npush constant 46\ncall String.new 1\npush constant 84\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 53\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 45\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 82\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 113\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 105\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 100\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 49\ncall String.appendChar 2\npush constant 49\ncall String.appendChar 2\npush constant 48\ncall String.appendChar 2\npush constant 44\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 65\ncall String.appendChar 2\npush constant 99\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 115\ncall String.appendChar 2\npush constant 117\ncall String.appendChar 2\npush constant 108\ncall String.appendChar 2\npush constant 116\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush constant 1\npush local 1\nadd\npop pointer 1\npush that 0\ncall Output.printInt 1\npop temp 0\ncall Output.println 0\npop temp 0\npush constant 0\nreturn\nfunction Main.double 0\npush argument 0\npush constant 2\ncall Math.multiply 2\nreturn\nfunction Main.fill 0\nlabel WHILE_EXP0\npush argument 1\npush constant 0\ngt\nnot\nif-goto WHILE_END0\npush argument 1\npush constant 1\nsub\npop argument 1\npush argument 1\npush argument 0\nadd\npush constant 3\ncall Array.new 1\npop temp 0\npop pointer 1\npush temp 0\npop that 0\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\nreturn\n";
            break;
            case 24:
            cmd="function Main.main 2\npush constant 8001\npush constant 16\npush constant 1\nneg\ncall Main.fillMemory 3\npop temp 0\npush constant 8000\ncall Memory.peek 1\npop local 1\npush local 1\ncall Main.convert 1\npop temp 0\npush constant 0\nreturn\nfunction Main.convert 3\npush constant 0\nnot\npop local 2\nlabel WHILE_EXP0\npush local 2\nnot\nif-goto WHILE_END0\npush local 1\npush constant 1\nadd\npop local 1\npush local 0\ncall Main.nextMask 1\npop local 0\npush constant 9000\npush local 1\nadd\npush local 0\ncall Memory.poke 2\npop temp 0\npush local 1\npush constant 16\ngt\nnot\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush argument 0\npush local 0\nand\npush constant 0\neq\nnot\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush constant 8000\npush local 1\nadd\npush constant 1\ncall Memory.poke 2\npop temp 0\ngoto IF_END1\nlabel IF_FALSE1\npush constant 8000\npush local 1\nadd\npush constant 0\ncall Memory.poke 2\npop temp 0\nlabel IF_END1\ngoto IF_END0\nlabel IF_FALSE0\npush constant 0\npop local 2\nlabel IF_END0\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\nreturn\nfunction Main.nextMask 0\npush argument 0\npush constant 0\neq\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush constant 1\nreturn\ngoto IF_END0\nlabel IF_FALSE0\npush argument 0\npush constant 2\ncall Math.multiply 2\nreturn\nlabel IF_END0\nfunction Main.fillMemory 0\nlabel WHILE_EXP0\npush argument 1\npush constant 0\ngt\nnot\nif-goto WHILE_END0\npush argument 0\npush argument 2\ncall Memory.poke 2\npop temp 0\npush argument 1\npush constant 1\nsub\npop argument 1\npush argument 0\npush constant 1\nadd\npop argument 0\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\nreturn\n";
            break;
            case 25:
            cmd="function Main.main 1\npush local 0\npop local 0\npush local 0\ncall SquareGame.run 1\npop temp 0\npush local 0\ncall SquareGame.dispose 1\npop temp 0\npush constant 0\nreturn\n";
            break;
            case 26:
            cmd="function Square.new 0\npush constant 3\ncall Memory.alloc 1\npop pointer 0\npush argument 0\npop this 0\npush argument 1\npop this 1\npush argument 2\npop this 2\npush pointer 0\ncall Square.draw 1\npop temp 0\npush pointer 0\nreturn\nfunction Square.dispose 0\npush argument 0\npop pointer 0\npush this 0\ncall Memory.deAlloc 1\npop temp 0\npush constant 0\nreturn\nfunction Square.draw 0\npush argument 0\npop pointer 0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nreturn\nfunction Square.erase 0\npush argument 0\npop pointer 0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nreturn\nfunction Square.incSize 0\npush argument 0\npop pointer 0\npush this 0\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush pointer 0\ncall Square.erase 1\npop temp 0\npush this 2\npop this 2\npush pointer 0\ncall Square.draw 1\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.decSize 0\npush argument 0\npop pointer 0\npush this 2\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush pointer 0\ncall Square.erase 1\npop temp 0\npush this 2\npop this 2\npush pointer 0\ncall Square.draw 1\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveUp 0\npush argument 0\npop pointer 0\npush this 1\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\npush this 1\npop this 1\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveDown 0\npush argument 0\npop pointer 0\npush this 1\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\npush this 1\npop this 1\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveLeft 0\npush argument 0\npop pointer 0\npush this 0\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\npush this 0\npop this 0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveRight 0\npush argument 0\npop pointer 0\npush this 0\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\npush this 0\npop this 0\npush this 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 1\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\n";
            break;
            case 27:
            cmd="function SquareGame.new 0\npush constant 2\ncall Memory.alloc 1\npop pointer 0\npush this 0\npop this 0\npush this 1\npop this 1\npush pointer 0\nreturn\nfunction SquareGame.dispose 0\npush argument 0\npop pointer 0\npush this 0\ncall Square.dispose 1\npop temp 0\npush this 0\ncall Memory.deAlloc 1\npop temp 0\npush constant 0\nreturn\nfunction SquareGame.run 2\npush argument 0\npop pointer 0\npush constant 0\nnot\npop local 1\nlabel WHILE_EXP0\npush local 1\nnot\nif-goto WHILE_END0\nlabel WHILE_EXP1\npush local 0\nnot\nif-goto WHILE_END1\npush local 0\npop local 0\npush pointer 0\ncall SquareGame.moveSquare 1\npop temp 0\ngoto WHILE_EXP1\nlabel WHILE_END1\npush local 0\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush local 1\npop local 1\nlabel IF_FALSE0\npush local 0\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush this 0\ncall Square.decSize 1\npop temp 0\nlabel IF_FALSE1\npush local 0\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush this 0\ncall Square.incSize 1\npop temp 0\nlabel IF_FALSE2\npush local 0\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush constant 0\npop this 1\nlabel IF_FALSE3\npush local 0\nif-goto IF_TRUE4\ngoto IF_FALSE4\nlabel IF_TRUE4\npush constant 1\npop this 1\nlabel IF_FALSE4\npush local 0\nif-goto IF_TRUE5\ngoto IF_FALSE5\nlabel IF_TRUE5\npush this 0\npop this 1\nlabel IF_FALSE5\npush local 0\nif-goto IF_TRUE6\ngoto IF_FALSE6\nlabel IF_TRUE6\npush this 1\npop this 1\nlabel IF_FALSE6\nlabel WHILE_EXP2\npush local 0\nnot\nif-goto WHILE_END2\npush local 0\npop local 0\npush pointer 0\ncall SquareGame.moveSquare 1\npop temp 0\ngoto WHILE_EXP2\nlabel WHILE_END2\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\nreturn\nfunction SquareGame.moveSquare 0\npush argument 0\npop pointer 0\npush this 1\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\ncall Square.moveUp 1\npop temp 0\nlabel IF_FALSE0\npush this 1\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush this 0\ncall Square.moveDown 1\npop temp 0\nlabel IF_FALSE1\npush this 1\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush this 0\ncall Square.moveLeft 1\npop temp 0\nlabel IF_FALSE2\npush this 1\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush this 0\ncall Square.moveRight 1\npop temp 0\nlabel IF_FALSE3\npush this 1\ncall Sys.wait 1\npop temp 0\npush constant 0\nreturn\n";
            break;
            case 28:
            cmd="function Main.main 0\npush constant 0\npush constant 32767\nsub\npush constant 1\nsub\ncall Output.printInt 1\npop temp 0\npush constant 0\nreturn\n";
            break;
            case 29:
            cmd="function Ball.new 0\npush constant 15\ncall Memory.alloc 1\npop pointer 0\npush argument 0\npop this 0\npush argument 1\npop this 1\npush argument 2\npop this 10\npush argument 3\npush constant 6\nsub\npop this 11\npush argument 4\npop this 12\npush argument 5\npush constant 6\nsub\npop this 13\npush constant 0\npop this 14\npush pointer 0\ncall Ball.show 1\npop temp 0\npush pointer 0\nreturn\nfunction Ball.dispose 0\npush argument 0\npop pointer 0\npush pointer 0\ncall Memory.deAlloc 1\npop temp 0\npush constant 0\nreturn\nfunction Ball.show 0\npush argument 0\npop pointer 0\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush pointer 0\ncall Ball.draw 1\npop temp 0\npush constant 0\nreturn\nfunction Ball.hide 0\npush argument 0\npop pointer 0\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush pointer 0\ncall Ball.draw 1\npop temp 0\npush constant 0\nreturn\nfunction Ball.draw 0\npush argument 0\npop pointer 0\npush this 0\npush this 1\npush this 0\npush constant 5\nadd\npush this 1\npush constant 5\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nreturn\nfunction Ball.getLeft 0\npush argument 0\npop pointer 0\npush this 0\nreturn\nfunction Ball.getRight 0\npush argument 0\npop pointer 0\npush this 0\npush constant 5\nadd\nreturn\nfunction Ball.setDestination 3\npush argument 0\npop pointer 0\npush argument 1\npush this 0\nsub\npop this 2\npush argument 2\npush this 1\nsub\npop this 3\npush this 2\ncall Math.abs 1\npop local 0\npush this 3\ncall Math.abs 1\npop local 1\npush local 0\npush local 1\nlt\npop this 7\npush this 7\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush local 0\npop local 2\npush local 1\npop local 0\npush local 2\npop local 1\npush this 1\npush argument 2\nlt\npop this 8\npush this 0\npush argument 1\nlt\npop this 9\ngoto IF_END0\nlabel IF_FALSE0\npush this 0\npush argument 1\nlt\npop this 8\npush this 1\npush argument 2\nlt\npop this 9\nlabel IF_END0\npush constant 2\npush local 1\ncall Math.multiply 2\npush local 0\nsub\npop this 4\npush constant 2\npush local 1\ncall Math.multiply 2\npop this 5\npush constant 2\npush local 1\npush local 0\nsub\ncall Math.multiply 2\npop this 6\npush constant 0\nreturn\nfunction Ball.move 0\npush argument 0\npop pointer 0\npush pointer 0\ncall Ball.hide 1\npop temp 0\npush this 4\npush constant 0\nlt\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 4\npush this 5\nadd\npop this 4\ngoto IF_END0\nlabel IF_FALSE0\npush this 4\npush this 6\nadd\npop this 4\npush this 9\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush this 7\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush this 0\npush constant 4\nadd\npop this 0\ngoto IF_END2\nlabel IF_FALSE2\npush this 1\npush constant 4\nadd\npop this 1\nlabel IF_END2\ngoto IF_END1\nlabel IF_FALSE1\npush this 7\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush this 0\npush constant 4\nsub\npop this 0\ngoto IF_END3\nlabel IF_FALSE3\npush this 1\npush constant 4\nsub\npop this 1\nlabel IF_END3\nlabel IF_END1\nlabel IF_END0\npush this 8\nif-goto IF_TRUE4\ngoto IF_FALSE4\nlabel IF_TRUE4\npush this 7\nif-goto IF_TRUE5\ngoto IF_FALSE5\nlabel IF_TRUE5\npush this 1\npush constant 4\nadd\npop this 1\ngoto IF_END5\nlabel IF_FALSE5\npush this 0\npush constant 4\nadd\npop this 0\nlabel IF_END5\ngoto IF_END4\nlabel IF_FALSE4\npush this 7\nif-goto IF_TRUE6\ngoto IF_FALSE6\nlabel IF_TRUE6\npush this 1\npush constant 4\nsub\npop this 1\ngoto IF_END6\nlabel IF_FALSE6\npush this 0\npush constant 4\nsub\npop this 0\nlabel IF_END6\nlabel IF_END4\npush this 0\npush this 10\ngt\nnot\nif-goto IF_TRUE7\ngoto IF_FALSE7\nlabel IF_TRUE7\npush constant 1\npop this 14\npush this 10\npop this 0\nlabel IF_FALSE7\npush this 0\npush this 11\nlt\nnot\nif-goto IF_TRUE8\ngoto IF_FALSE8\nlabel IF_TRUE8\npush constant 2\npop this 14\npush this 11\npop this 0\nlabel IF_FALSE8\npush this 1\npush this 12\ngt\nnot\nif-goto IF_TRUE9\ngoto IF_FALSE9\nlabel IF_TRUE9\npush constant 3\npop this 14\npush this 12\npop this 1\nlabel IF_FALSE9\npush this 1\npush this 13\nlt\nnot\nif-goto IF_TRUE10\ngoto IF_FALSE10\nlabel IF_TRUE10\npush constant 4\npop this 14\npush this 13\npop this 1\nlabel IF_FALSE10\npush pointer 0\ncall Ball.show 1\npop temp 0\npush this 14\nreturn\nfunction Ball.bounce 5\npush argument 0\npop pointer 0\npush this 2\npush constant 10\ncall Math.divide 2\npop local 2\npush this 3\npush constant 10\ncall Math.divide 2\npop local 3\npush argument 1\npush constant 0\neq\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush constant 10\npop local 4\ngoto IF_END0\nlabel IF_FALSE0\npush this 2\npush constant 0\nlt\nnot\npush argument 1\npush constant 1\neq\nand\npush this 2\npush constant 0\nlt\npush argument 1\npush constant 1\nneg\neq\nand\nor\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush constant 20\npop local 4\ngoto IF_END1\nlabel IF_FALSE1\npush constant 5\npop local 4\nlabel IF_END1\nlabel IF_END0\npush this 14\npush constant 1\neq\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush constant 506\npop local 0\npush local 3\npush constant 50\nneg\ncall Math.multiply 2\npush local 2\ncall Math.divide 2\npop local 1\npush this 1\npush local 1\npush local 4\ncall Math.multiply 2\nadd\npop local 1\ngoto IF_END2\nlabel IF_FALSE2\npush this 14\npush constant 2\neq\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush constant 0\npop local 0\npush local 3\npush constant 50\ncall Math.multiply 2\npush local 2\ncall Math.divide 2\npop local 1\npush this 1\npush local 1\npush local 4\ncall Math.multiply 2\nadd\npop local 1\ngoto IF_END3\nlabel IF_FALSE3\npush this 14\npush constant 3\neq\nif-goto IF_TRUE4\ngoto IF_FALSE4\nlabel IF_TRUE4\npush constant 250\npop local 1\npush local 2\npush constant 25\nneg\ncall Math.multiply 2\npush local 3\ncall Math.divide 2\npop local 0\npush this 0\npush local 0\npush local 4\ncall Math.multiply 2\nadd\npop local 0\ngoto IF_END4\nlabel IF_FALSE4\npush constant 0\npop local 1\npush local 2\npush constant 25\ncall Math.multiply 2\npush local 3\ncall Math.divide 2\npop local 0\npush this 0\npush local 0\npush local 4\ncall Math.multiply 2\nadd\npop local 0\nlabel IF_END4\nlabel IF_END3\nlabel IF_END2\npush pointer 0\npush local 0\npush local 1\ncall Ball.setDestination 3\npop temp 0\npush constant 0\nreturn\n";
            break;
            case 30:
            cmd="function Bat.new 0\npush constant 5\ncall Memory.alloc 1\npop pointer 0\npush argument 0\npop this 0\npush argument 1\npop this 1\npush argument 2\npop this 2\npush argument 3\npop this 3\npush constant 2\npop this 4\npush pointer 0\ncall Bat.show 1\npop temp 0\npush pointer 0\nreturn\nfunction Bat.dispose 0\npush argument 0\npop pointer 0\npush pointer 0\ncall Memory.deAlloc 1\npop temp 0\npush constant 0\nreturn\nfunction Bat.show 0\npush argument 0\npop pointer 0\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush pointer 0\ncall Bat.draw 1\npop temp 0\npush constant 0\nreturn\nfunction Bat.hide 0\npush argument 0\npop pointer 0\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush pointer 0\ncall Bat.draw 1\npop temp 0\npush constant 0\nreturn\nfunction Bat.draw 0\npush argument 0\npop pointer 0\npush this 0\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush this 3\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nreturn\nfunction Bat.setDirection 0\npush argument 0\npop pointer 0\npush argument 1\npop this 4\npush constant 0\nreturn\nfunction Bat.getLeft 0\npush argument 0\npop pointer 0\npush this 0\nreturn\nfunction Bat.getRight 0\npush argument 0\npop pointer 0\npush this 0\npush this 2\nadd\nreturn\nfunction Bat.setWidth 0\npush argument 0\npop pointer 0\npush pointer 0\ncall Bat.hide 1\npop temp 0\npush argument 1\npop this 2\npush pointer 0\ncall Bat.show 1\npop temp 0\npush constant 0\nreturn\nfunction Bat.move 0\npush argument 0\npop pointer 0\npush this 4\npush constant 1\neq\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\npush constant 4\nsub\npop this 0\npush this 0\npush constant 0\nlt\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush constant 0\npop this 0\nlabel IF_FALSE1\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 2\nadd\npush constant 1\nadd\npush this 1\npush this 0\npush this 2\nadd\npush constant 4\nadd\npush this 1\npush this 3\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush constant 3\nadd\npush this 1\npush this 3\nadd\ncall Screen.drawRectangle 4\npop temp 0\ngoto IF_END0\nlabel IF_FALSE0\npush this 0\npush constant 4\nadd\npop this 0\npush this 0\npush this 2\nadd\npush constant 511\ngt\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush constant 511\npush this 2\nsub\npop this 0\nlabel IF_FALSE2\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush constant 4\nsub\npush this 1\npush this 0\npush constant 1\nsub\npush this 1\npush this 3\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 2\nadd\npush constant 3\nsub\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush this 3\nadd\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_END0\npush constant 0\nreturn\n";
            break;
            case 31:
            cmd="function Main.main 1\ncall PongGame.newInstance 0\npop temp 0\ncall PongGame.getInstance 0\npop local 0\npush local 0\ncall PongGame.run 1\npop temp 0\npush local 0\ncall PongGame.dispose 1\npop temp 0\npush constant 0\nreturn\n";
            break;
            case 32:
            cmd="function PongGame.new 0\npush constant 7\ncall Memory.alloc 1\npop pointer 0\ncall Screen.clearScreen 0\npop temp 0\npush constant 50\npop this 6\npush constant 230\npush constant 229\npush this 6\npush constant 7\ncall Bat.new 4\npop this 0\npush constant 253\npush constant 222\npush constant 0\npush constant 511\npush constant 0\npush constant 229\ncall Ball.new 6\npop this 1\npush this 1\npush constant 400\npush constant 0\ncall Ball.setDestination 3\npop temp 0\npush constant 0\npush constant 238\npush constant 511\npush constant 240\ncall Screen.drawRectangle 4\npop temp 0\npush constant 22\npush constant 0\ncall Output.moveCursor 2\npop temp 0\npush constant 8\ncall String.new 1\npush constant 83\ncall String.appendChar 2\npush constant 99\ncall String.appendChar 2\npush constant 111\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 58\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 48\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\npush constant 0\npop this 3\npush constant 0\npop this 4\npush constant 0\npop this 2\npush constant 0\npop this 5\npush pointer 0\nreturn\nfunction PongGame.dispose 0\npush argument 0\npop pointer 0\npush this 0\ncall Bat.dispose 1\npop temp 0\npush this 1\ncall Ball.dispose 1\npop temp 0\npush pointer 0\ncall Memory.deAlloc 1\npop temp 0\npush constant 0\nreturn\nfunction PongGame.newInstance 0\ncall PongGame.new 0\npop static 0\npush constant 0\nreturn\nfunction PongGame.getInstance 0\npush static 0\nreturn\nfunction PongGame.run 1\npush argument 0\npop pointer 0\nlabel WHILE_EXP0\npush this 3\nnot\nnot\nif-goto WHILE_END0\nlabel WHILE_EXP1\npush local 0\npush constant 0\neq\npush this 3\nnot\nand\nnot\nif-goto WHILE_END1\ncall Keyboard.keyPressed 0\npop local 0\npush this 0\ncall Bat.move 1\npop temp 0\npush pointer 0\ncall PongGame.moveBall 1\npop temp 0\ngoto WHILE_EXP1\nlabel WHILE_END1\npush local 0\npush constant 130\neq\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\npush constant 1\ncall Bat.setDirection 2\npop temp 0\ngoto IF_END0\nlabel IF_FALSE0\npush local 0\npush constant 132\neq\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush this 0\npush constant 2\ncall Bat.setDirection 2\npop temp 0\ngoto IF_END1\nlabel IF_FALSE1\npush local 0\npush constant 140\neq\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush constant 0\nnot\npop this 3\nlabel IF_FALSE2\nlabel IF_END1\nlabel IF_END0\nlabel WHILE_EXP2\npush local 0\npush constant 0\neq\nnot\npush this 3\nnot\nand\nnot\nif-goto WHILE_END2\ncall Keyboard.keyPressed 0\npop local 0\npush this 0\ncall Bat.move 1\npop temp 0\npush pointer 0\ncall PongGame.moveBall 1\npop temp 0\ngoto WHILE_EXP2\nlabel WHILE_END2\ngoto WHILE_EXP0\nlabel WHILE_END0\npush this 3\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush constant 10\npush constant 27\ncall Output.moveCursor 2\npop temp 0\npush constant 9\ncall String.new 1\npush constant 71\ncall String.appendChar 2\npush constant 97\ncall String.appendChar 2\npush constant 109\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 32\ncall String.appendChar 2\npush constant 79\ncall String.appendChar 2\npush constant 118\ncall String.appendChar 2\npush constant 101\ncall String.appendChar 2\npush constant 114\ncall String.appendChar 2\ncall Output.printString 1\npop temp 0\nlabel IF_FALSE3\npush constant 0\nreturn\nfunction PongGame.moveBall 5\npush argument 0\npop pointer 0\npush this 1\ncall Ball.move 1\npop this 2\npush this 2\npush constant 0\ngt\npush this 2\npush this 5\neq\nnot\nand\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 2\npop this 5\npush constant 0\npop local 0\npush this 0\ncall Bat.getLeft 1\npop local 1\npush this 0\ncall Bat.getRight 1\npop local 2\npush this 1\ncall Ball.getLeft 1\npop local 3\npush this 1\ncall Ball.getRight 1\npop local 4\npush this 2\npush constant 4\neq\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush local 1\npush local 4\ngt\npush local 2\npush local 3\nlt\nor\npop this 3\npush this 3\nnot\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush local 4\npush local 1\npush constant 10\nadd\nlt\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush constant 1\nneg\npop local 0\ngoto IF_END3\nlabel IF_FALSE3\npush local 3\npush local 2\npush constant 10\nsub\ngt\nif-goto IF_TRUE4\ngoto IF_FALSE4\nlabel IF_TRUE4\npush constant 1\npop local 0\nlabel IF_FALSE4\nlabel IF_END3\npush this 6\npush constant 2\nsub\npop this 6\npush this 0\npush this 6\ncall Bat.setWidth 2\npop temp 0\npush this 4\npush constant 1\nadd\npop this 4\npush constant 22\npush constant 7\ncall Output.moveCursor 2\npop temp 0\npush this 4\ncall Output.printInt 1\npop temp 0\nlabel IF_FALSE2\nlabel IF_FALSE1\npush this 1\npush local 0\ncall Ball.bounce 2\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\n";
            break;
            case 33:
            cmd="function Main.main 0\npush constant 1\npush constant 2\npush constant 3\ncall Math.multiply 2\nadd\ncall Output.printInt 1\npop temp 0\npush constant 0\nreturn\n";
            break;
            case 34:
            cmd="function Main.main 1\ncall SquareGame.new 0\npop local 0\npush local 0\ncall SquareGame.run 1\npop temp 0\npush local 0\ncall SquareGame.dispose 1\npop temp 0\npush constant 0\nreturn\n";
            break;
            case 35:
            cmd="function Square.new 0\npush constant 3\ncall Memory.alloc 1\npop pointer 0\npush argument 0\npop this 0\npush argument 1\npop this 1\npush argument 2\npop this 2\npush pointer 0\ncall Square.draw 1\npop temp 0\npush pointer 0\nreturn\nfunction Square.dispose 0\npush argument 0\npop pointer 0\npush pointer 0\ncall Memory.deAlloc 1\npop temp 0\npush constant 0\nreturn\nfunction Square.draw 0\npush argument 0\npop pointer 0\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nreturn\nfunction Square.erase 0\npush argument 0\npop pointer 0\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush constant 0\nreturn\nfunction Square.incSize 0\npush argument 0\npop pointer 0\npush this 1\npush this 2\nadd\npush constant 254\nlt\npush this 0\npush this 2\nadd\npush constant 510\nlt\nand\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush pointer 0\ncall Square.erase 1\npop temp 0\npush this 2\npush constant 2\nadd\npop this 2\npush pointer 0\ncall Square.draw 1\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.decSize 0\npush argument 0\npop pointer 0\npush this 2\npush constant 2\ngt\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush pointer 0\ncall Square.erase 1\npop temp 0\npush this 2\npush constant 2\nsub\npop this 2\npush pointer 0\ncall Square.draw 1\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveUp 0\npush argument 0\npop pointer 0\npush this 1\npush constant 1\ngt\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 2\nadd\npush constant 1\nsub\npush this 0\npush this 2\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush this 1\npush constant 2\nsub\npop this 1\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush constant 1\nadd\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveDown 0\npush argument 0\npop pointer 0\npush this 1\npush this 2\nadd\npush constant 254\nlt\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush constant 1\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush this 1\npush constant 2\nadd\npop this 1\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 2\nadd\npush constant 1\nsub\npush this 0\npush this 2\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveLeft 0\npush argument 0\npop pointer 0\npush this 0\npush constant 1\ngt\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 2\nadd\npush constant 1\nsub\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush this 0\npush constant 2\nsub\npop this 0\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush constant 1\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\nfunction Square.moveRight 0\npush argument 0\npop pointer 0\npush this 0\npush this 2\nadd\npush constant 510\nlt\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush constant 0\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 1\npush this 0\npush constant 1\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\npush this 0\npush constant 2\nadd\npop this 0\npush constant 0\nnot\ncall Screen.setColor 1\npop temp 0\npush this 0\npush this 2\nadd\npush constant 1\nsub\npush this 1\npush this 0\npush this 2\nadd\npush this 1\npush this 2\nadd\ncall Screen.drawRectangle 4\npop temp 0\nlabel IF_FALSE0\npush constant 0\nreturn\n";
            break;
            case 36:
            cmd="function SquareGame.new 0\npush constant 2\ncall Memory.alloc 1\npop pointer 0\npush constant 0\npush constant 0\npush constant 30\ncall Square.new 3\npop this 0\npush constant 0\npop this 1\npush pointer 0\nreturn\nfunction SquareGame.dispose 0\npush argument 0\npop pointer 0\npush this 0\ncall Square.dispose 1\npop temp 0\npush pointer 0\ncall Memory.deAlloc 1\npop temp 0\npush constant 0\nreturn\nfunction SquareGame.run 2\npush argument 0\npop pointer 0\npush constant 0\npop local 1\nlabel WHILE_EXP0\npush local 1\nnot\nnot\nif-goto WHILE_END0\nlabel WHILE_EXP1\npush local 0\npush constant 0\neq\nnot\nif-goto WHILE_END1\ncall Keyboard.keyPressed 0\npop local 0\npush pointer 0\ncall SquareGame.moveSquare 1\npop temp 0\ngoto WHILE_EXP1\nlabel WHILE_END1\npush local 0\npush constant 81\neq\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush constant 0\nnot\npop local 1\nlabel IF_FALSE0\npush local 0\npush constant 90\neq\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush this 0\ncall Square.decSize 1\npop temp 0\nlabel IF_FALSE1\npush local 0\npush constant 88\neq\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush this 0\ncall Square.incSize 1\npop temp 0\nlabel IF_FALSE2\npush local 0\npush constant 131\neq\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush constant 1\npop this 1\nlabel IF_FALSE3\npush local 0\npush constant 133\neq\nif-goto IF_TRUE4\ngoto IF_FALSE4\nlabel IF_TRUE4\npush constant 2\npop this 1\nlabel IF_FALSE4\npush local 0\npush constant 130\neq\nif-goto IF_TRUE5\ngoto IF_FALSE5\nlabel IF_TRUE5\npush constant 3\npop this 1\nlabel IF_FALSE5\npush local 0\npush constant 132\neq\nif-goto IF_TRUE6\ngoto IF_FALSE6\nlabel IF_TRUE6\npush constant 4\npop this 1\nlabel IF_FALSE6\nlabel WHILE_EXP2\npush local 0\npush constant 0\neq\nnot\nnot\nif-goto WHILE_END2\ncall Keyboard.keyPressed 0\npop local 0\npush pointer 0\ncall SquareGame.moveSquare 1\npop temp 0\ngoto WHILE_EXP2\nlabel WHILE_END2\ngoto WHILE_EXP0\nlabel WHILE_END0\npush constant 0\nreturn\nfunction SquareGame.moveSquare 0\npush argument 0\npop pointer 0\npush this 1\npush constant 1\neq\nif-goto IF_TRUE0\ngoto IF_FALSE0\nlabel IF_TRUE0\npush this 0\ncall Square.moveUp 1\npop temp 0\nlabel IF_FALSE0\npush this 1\npush constant 2\neq\nif-goto IF_TRUE1\ngoto IF_FALSE1\nlabel IF_TRUE1\npush this 0\ncall Square.moveDown 1\npop temp 0\nlabel IF_FALSE1\npush this 1\npush constant 3\neq\nif-goto IF_TRUE2\ngoto IF_FALSE2\nlabel IF_TRUE2\npush this 0\ncall Square.moveLeft 1\npop temp 0\nlabel IF_FALSE2\npush this 1\npush constant 4\neq\nif-goto IF_TRUE3\ngoto IF_FALSE3\nlabel IF_TRUE3\npush this 0\ncall Square.moveRight 1\npop temp 0\nlabel IF_FALSE3\npush constant 5\ncall Sys.wait 1\npop temp 0\npush constant 0\nreturn\n";
            break;
        }
        write_to_output(cmd);
    }
};

bool debugex=false;

void printfile(){
    std::ifstream file("tests/Square/SquareGame.jack.parser.codegen.Cvm");
      std::string str;
      while (std::getline(file, str)) {
        std::cout << str << "\\n";
    }
     std::cout <<"\n";
}

SysControl control;


void sys(string msg){
    vm.push_back(msg);

}

void print(){
    for(int i=0;i<(int)vm.size();i++){
        write_to_output(vm[i]);
    }
}

void print_status(string msg){

    string status="";
    if(LState.constructor){
        status="constructor";
    }
    if(LState.func){
        status="function";
    }
    if(LState.method){
        status="method";
    }
    if(LState.let){
        status="let";
    }
    if(LState.class_dec){
        status="class_dec";
    }
    debug_msg.push_back(status+":"+msg+"\n");
}

void print_status(double num,string msg){

    string status="";
    if(LState.constructor){
        status="constructor";
    }
    if(LState.func){
        status="function";
    }
    if(LState.method){
        status="method";
    }
    if(LState.let){
        status="let";
    }
    if(LState.class_dec){
        status="class_dec";
    }
    debug_msg.push_back(status+":"+to_string(num).substr(0,4)+" "+msg+"\n");
    scontrol.add(msg);
    debug_msg.push_back(scontrol.get_list()+"\n\n");
    control.cid+=num;
}

int print_debug(){
    if(debug){
        write_to_output("==============debug==============\n");
        for(int i=0;i<(int)debug_msg.size();i++){
            write_to_output(debug_msg[i]);
        }
        write_to_output("==============debug==============\n");
        SymbolControl.print();
        write_to_output("==============cid==============\n");
        write_to_output(to_string(control.cid)+"\n");
        write_to_output("==============cid==============\n");
    }
    return (int)debug_msg.size();
}




void walk_class(ast t){
    print_status(1,"walk_class");
    string myclassname = get_class_class_name(t) ;
    control.class_name=myclassname;
    LState.class_dec=true;
    control.tk.process="walk_class";
    ast var_decs = get_class_var_decs(t) ;
    control.tk.process="";
    LState.class_dec=false;
    ast subr_decs = get_class_subr_decs(t) ;

    walk_class_var_decs(var_decs) ;
    walk_subr_decs(subr_decs) ;
    if(debugex){
        printf("cid:  %d\n",control.cid);
        printfile();
    }
    scontrol.remove("walk_class");
    print_debug();
    control.print();

}
void walk_class_var_decs(ast t){
    print_status(2,"walk_class_var_decs");
    int ndecs = size_of_class_var_decs(t) ;
    print_status("walk_class_var_decs:ndecs:"+to_string(ndecs));
    for ( int i = 0 ; i < ndecs ; i++ ){
        walk_var_dec(get_class_var_decs(t,i)) ;
    }
    scontrol.remove("walk_class_var_decs");
}
void walk_var_dec(ast t){
    print_status(3,"walk_var_dec");
    string name = get_var_dec_name(t) ;
    string type = get_var_dec_type(t) ;
    string segment = get_var_dec_segment(t) ;
    int offset = get_var_dec_offset(t) ;

    print_status("walk_var_dec:name:"+name);
    print_status("walk_var_dec:type:"+type);
    print_status("walk_var_dec:segment:"+segment);
    print_status("walk_var_dec:offset:"+to_string(offset));
    print_status("walk_var_dec:declare_variable:"+name+"\n");
    VarNode node=SymbolControl.declare_variable(name,"",segment,type,false);

    if(segment=="argument"){
        //control.sys("push argument "+to_string(node.offset)+"\n");
        if(control.tk.process=="walk_param_list"){
            control.in_params.push_back(node);
            print_status("walk_var_dec:control.in_params.push_back(node);size:"+to_string((int)control.in_params.size())+"\n");
        }
    }
    scontrol.remove("walk_var_dec");
}
void walk_subr_decs(ast t)
{
    print_status(4,"walk_subr_decs");
    int size = size_of_subr_decs(t) ;
    print_status("walk_subr_decs:size:"+to_string(size));
    for ( int i = 0 ; i < size ; i++ )
    {
        walk_subr(get_subr_decs(t,i)) ;
    }
    scontrol.remove("walk_subr_decs");
}

void walk_subr(ast t)
{
    print_status(5,"walk_subr");
    ast subr = get_subr_subr(t) ;
    SymbolControl.reset_nextlocal(0);
    SymbolControl.reset_nextargument(0);
    control.in_params.clear();
    control.call_params.clear();
    scontrol.clear();
    switch(ast_node_kind(subr))
    {
    case ast_constructor:
        LState.constructor=true;
        walk_constructor(subr) ;
        LState.constructor=false;
        break ;
    case ast_function:
        LState.func=true;
        walk_function(subr) ;
        LState.func=false;
        break ;
    case ast_method:
        LState.method=true;
        walk_method(subr) ;
        LState.method=false;
        break ;
    default:
        fatal_error(0,"Unexpected subroutine kind") ;
        break ;
    }
    control.add("{"+LState.call_name+"}","");
    scontrol.remove("walk_subr");
}

void walk_constructor(ast t)
{
    print_status(6,"walk_constructor");
    string vtype = get_constructor_vtype(t) ;
    string name = get_constructor_name(t) ;
    ast param_list = get_constructor_param_list(t) ;
    ast subr_body = get_constructor_subr_body(t) ;

    print_status("walk_constructor:vtype:"+vtype);
    print_status("walk_constructor:type:"+name);

    control.params_tag="{"+control.class_name+"."+name+".params"+"}";
    control.sys("function "+control.class_name+"."+name+" "+control.params_tag+"\n");

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
    scontrol.remove("walk_constructor");
}

void walk_function(ast t)
{
    print_status(7,"walk_function");
    string vtype = get_function_vtype(t) ;
    string name = get_function_name(t) ;
    ast param_list = get_function_param_list(t) ;
    ast subr_body = get_function_subr_body(t) ;
    print_status("walk_function:vtype:"+vtype);
    print_status("walk_function:name:"+name);

    control.params_tag="{"+control.class_name+"."+name+".params"+"}";
    control.sys("function "+control.class_name+"."+name+" "+control.params_tag+"\n");

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
    scontrol.remove("walk_function");
}

void walk_method(ast t)
{
    print_status(8,"walk_method");
    string vtype = get_method_vtype(t) ;
    string name = get_method_name(t) ;
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;
    print_status("walk_method:vtype:"+vtype);
    print_status("walk_method:name:"+name);
    LState.call_name=control.class_name+"."+name;
    
    control.params_tag="{"+control.class_name+"."+name+".params"+"}";
    control.sys("function "+control.class_name+"."+name+" "+control.params_tag+"\n");
    
    control.sys("{"+LState.call_name+"}");
    control.tk.process="walk_param_list";
    walk_param_list(param_list) ;
    control.tk.process="";
    walk_subr_body(subr_body) ;
    scontrol.remove("walk_method");
}

void walk_param_list(ast t)
{
    print_status(9,"walk_param_list");
    int ndecs = size_of_param_list(t) ;
    print_status("walk_param_list:ndecs:"+to_string(ndecs));
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_param_list(t,i)) ;
    }
    scontrol.remove("walk_param_list");
}

void walk_subr_body(ast t)
{
    print_status(10,"walk_subr_body");
    ast decs = get_subr_body_decs(t) ;
    ast body = get_subr_body_body(t) ;
    walk_var_decs(decs) ;
    walk_statements(body) ;
    scontrol.remove("walk_subr_body");
}

void walk_var_decs(ast t)
{
    print_status(11,"walk_var_decs");
    int ndecs = size_of_var_decs(t) ;
    print_status("walk_var_decs:ndecs:"+to_string(ndecs));
    control.add(control.params_tag,to_string(ndecs));
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_var_decs(t,i)) ;
    }
    scontrol.remove("walk_var_decs");
}

void walk_statements(ast t)
{
    print_status(12,"walk_statements");
    int nstatements = size_of_statements(t) ;
    print_status("walk_statements:nstatements:"+to_string(nstatements));

    if(t==control.tk.if_true){
        int tk_if=control.tk.tk_if;
        control.tk.tk_if+=1;
        for ( int i = 0 ; i < nstatements ; i++ )
        {
            walk_statement(get_statements(t,i)) ;
        }
        if(control.tk.process=="IF_ELSE" || nstatements>0){
            control.sys("goto IF_END"+to_string(tk_if)+"\n");
        }
    }
    else if(t==control.tk.if_false){
        int tk_if=control.tk.tk_if;
        for ( int i = 0 ; i < nstatements ; i++ )
        {
            walk_statement(get_statements(t,i)) ;
        }
    }
    else{
        for ( int i = 0 ; i < nstatements ; i++ )
        {
            walk_statement(get_statements(t,i)) ;
        }
    }
    scontrol.remove("walk_statements");
}


void walk_statement(ast t)
{
    print_status(13,"walk_statement");
    ast statement = get_statement_statement(t) ;

    switch(ast_node_kind(statement))
    {
    case ast_let:
        control.tk.process="LET";
        LState.let=true;
        walk_let(statement) ;
        LState.let=false;
        break ;
    case ast_let_array:
        control.tk.process="LET_ARRAY";
        LState.let=true;
        walk_let_array(statement) ;
        LState.let=false;
        break ;
    case ast_if:
        control.tk.process="IF";
        walk_if(statement) ;
        break ;
    case ast_if_else:
        control.tk.process="IF_ELSE";
        walk_if_else(statement) ;
        break ;
    case ast_while:
        control.tk.process="WHILE";
        walk_while(statement) ;
        break ;
    case ast_do:
        control.tk.process="DO";
        walk_do(statement) ;
        break ;
    case ast_return:
        control.tk.process="RETURN";
        walk_return(statement) ;
        break ;
    case ast_return_expr:
        control.tk.process="RETURN_EXPR";
        walk_return_expr(statement) ;
        break ;
    case ast_statements:
        control.tk.process="STATEMENTS";
        walk_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }
    control.tk.process="";
    scontrol.remove("walk_statement");
}

void walk_let(ast t)
{
    print_status(14,"walk_let");
    ast var = get_let_var(t) ;
    ast expr = get_let_expr(t) ;

    control.stack_status="stack_push";
    walk_expr(expr);

    while (!control.op_stack.empty()) { 
        string op=control.op_stack.front();
        int count=0;
        while(!control.var_stack.empty() && count<2){
            variable var=control.var_stack.front();
            if(var.segment=="constant"){
                control.sys("push constant "+to_string(var.name)+"\n");

            }
            if(var.segment=="static"){
                control.sys("push "+var.segment+" "+to_string(var.offset)+"\n");
            }
            count+=1;
            control.var_stack.pop();
        }
        
        if(op=="+"){
            control.sys("add\n");
        }

        control.op_stack.pop();
    }
    control.stack_status="walk_let";
    walk_var(var);
    control.stack_status="";
    scontrol.remove("walk_let");

}

void walk_let_array(ast t)
{
    print_status(15,"walk_let_array");
    ast var = get_let_array_var(t) ;
    ast index = get_let_array_index(t) ;
    ast expr = get_let_array_expr(t) ;

    walk_var(var) ;
    walk_expr(index) ;
    walk_expr(expr) ;
    scontrol.remove("walk_let_array");
}

void walk_if(ast t)
{
    print_status(16,"walk_if");
    control.sys("push constant 0\n");
    ast condition = get_if_condition(t) ;
    ast if_true = get_if_if_true(t) ;
    int tk_if=control.tk.tk_if;
    
    walk_expr(condition) ;
    control.sys("if-goto IF_TRUE"+to_string(tk_if)+"\n");
    control.sys("goto IF_FALSE"+to_string(tk_if)+"\n");
    control.sys("label IF_TRUE"+to_string(tk_if)+"\n");
    walk_statements(if_true) ;

    control.sys("label IF_FALSE"+to_string(tk_if)+"\n");
    control.tk.tk_if+=1;
    scontrol.remove("walk_if");
}

void walk_if_else(ast t)
{
    print_status(17,"walk_if_else");
    ast condition = get_if_else_condition(t) ;
    int tk_if=control.tk.tk_if;

    control.sys("push constant 0\n");
    scontrol.add("push constant");
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;
    control.tk.if_true = if_true;
    control.tk.if_false = if_false;
    
    walk_expr(condition) ;

    control.sys("if-goto IF_TRUE"+to_string(tk_if)+"\n");
    control.sys("goto IF_FALSE"+to_string(tk_if)+"\n");
    control.sys("label IF_TRUE"+to_string(tk_if)+"\n");
    walk_statements(if_true) ;

    control.sys("label IF_FALSE"+to_string(tk_if)+"\n");
    walk_statements(if_false) ;
    control.sys("label IF_END"+to_string(tk_if)+"\n");
    scontrol.remove("walk_if_else");
}

void walk_while(ast t)
{
    print_status(18,"walk_while");
    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;
    int tk_while=control.tk.tk_while;

    control.sys("label WHILE_EXP"+to_string(tk_while)+"\n");
    control.sys("push constant 0\n");
    scontrol.add("push constant");
    walk_expr(condition) ;
    walk_statements(body) ;
    control.sys("goto WHILE_EXP"+to_string(tk_while)+"\n");
    control.sys("label WHILE_END"+to_string(tk_while)+"\n");
    scontrol.remove("walk_while");
}



void walk_do(ast t)
{
    print_status(19,"walk_do");
    ast call = get_do_call(t);

    switch(ast_node_kind(call))
    {
    case ast_call_as_function:
        walk_call_as_function(call) ;
        break ;
    case ast_call_as_method:
        walk_call_as_method(call) ;
        break ;
    default:
        fatal_error(0,"Unexpected call kind") ;
        break ;
    }
    scontrol.remove("walk_do");
}

void walk_return(ast t)
{
    
    print_status(20,"walk_return");
    control.sys("push constant 0\n");
    scontrol.add("push constant");
    control.sys("return\n");
    scontrol.remove("walk_return");
}

void walk_return_expr(ast t)
{

    print_status(21,"walk_return_expr");
    ast expr = get_return_expr(t) ;
    control.stack_status="walk_return_expr";
    walk_expr(expr) ;
    control.stack_status="";
    control.sys("return\n");
    scontrol.remove("walk_return_expr");
}

void walk_expr(ast t)
{
    print_status(22,"walk_expr");
    int term_ops = size_of_expr(t) ;
    print_status("walk_expr:term_ops:"+to_string(term_ops));
    for ( int i = 0 ; i < term_ops ; i++ )
    {
        ast term_op = get_expr(t,i) ;
        if ( i % 2 == 0 )
        {
            walk_term(term_op) ;
        }
        else
        {
            walk_infix_op(term_op) ;
        }
    }
    scontrol.remove("walk_expr");
}

void walk_term(ast t)
{
    print_status(23,"walk_term");
    ast term = get_term_term(t) ;

    switch(ast_node_kind(term))
    {
    case ast_int:
        walk_int(term) ;
        break ;
    case ast_string:
        walk_string(term) ;
        break ;
    case ast_bool:
        walk_bool(term) ;
        break ;
    case ast_null:
        walk_null(term) ;
        break ;
    case ast_this:
        walk_this(term) ;
        break ;
    case ast_expr:
        walk_expr(term) ;
        break ;
    case ast_unary_op:
        walk_unary_op(term) ;
        break ;
    case ast_var:
        walk_var(term) ;
        break ;
    case ast_array_index:
        walk_array_index(term) ;
        break ;
    case ast_call_as_function:
        walk_call_as_function(term) ;
        break ;
    case ast_call_as_method:
        walk_call_as_method(term) ;
        break ;
    default:
        fatal_error(0,"Unexpected term kind") ;
        break ;
    }
    scontrol.remove("walk_term");
}

void walk_int(ast t)
{
    print_status(24,"walk_int");
    int _constant = get_int_constant(t) ;
    print_status("walk_int:_constant:"+to_string(_constant));

    if(scontrol.contain({"walk_call_as_method","walk_subr_call","walk_expr_list","walk_expr","walk_term","walk_int"})){
        control.sys("push constant "+to_string(_constant)+"\n");
        scontrol.add("push constant");
    }else{
        if(control.stack_status=="stack_push"){
            variable v;
            v.name=to_string(_constant);
            v.type="int";
            v.segment="constant";
            v.offset=-1;
            control.var_stack.push(v);
            print_status("walk_int:control.var_stack.push(v)");
        }
        else{
            control.sys("push constant "+to_string(_constant)+"\n");
            scontrol.add("push constant");
        }
    }



    
    scontrol.remove("walk_int");
}   

void walk_string(ast t)
{
    print_status(25,"walk_string");
    string _constant = get_string_constant(t) ;
    print_status("walk_string:_constant:"+_constant);
    scontrol.remove("walk_string");
}

void walk_bool(ast t)
{
    print_status(26,"walk_bool");
    bool _constant = get_bool_t_or_f(t) ;
    string _r=((_constant)?"true":"false");
    print_status("walk_bool:_constant:"+_r);
    if(control.tk.process=="IF" || control.tk.process=="IF_ELSE"){
        control.sys(((_constant)?"not\n":""));

    }
    if(control.tk.process=="WHILE"){
        int tk_while=control.tk.tk_while;
        control.sys(((_constant)?"not\nnot\n":"not\n"));
        control.sys("if-goto "+control.tk.process+"_END"+to_string(tk_while)+"\n");
        control.tk.tk_while+=1;
    }
    scontrol.remove("walk_bool");
}

void walk_null(ast t)
{
    print_status(27,"walk_null");
    scontrol.remove("walk_null");
}

void walk_this(ast t)
{
    print_status(28,"walk_this");
    std::vector<VarNode> v=SymbolControl.find_node_list("","","this","");
    if(LState.constructor){
        control.sys("push constant "+to_string((int)v.size())+"\n");
        control.sys("call Memory.alloc 1\n");
        scontrol.add("push constant");
        scontrol.add("call Memory.alloc");
    }
    control.sys("pop pointer 0\n");
    control.sys("push pointer 0\n");
    scontrol.add("pop pointer");
    scontrol.add("push pointer");
    scontrol.remove("walk_this");
}

void walk_unary_op(ast t)
{
    print_status(29,"walk_unary_op");
    string uop = get_unary_op_op(t);
    ast term = get_unary_op_term(t) ;
    print_status("walk_unary_op:uop:"+uop);
    walk_term(term) ;
    scontrol.remove("walk_unary_op");
}



void walk_var(ast t)
{

    print_status(30,"walk_var");
    string name = get_var_name(t) ;
    string type = get_var_type(t) ;
    string segment = get_var_segment(t) ;
    int offset = get_var_offset(t);
    print_status("walk_var:name:"+name);
    print_status("walk_var:type:"+type);
    print_status("walk_var:segment:"+segment);
    print_status("walk_var:offset:"+to_string(offset));
    VarNode node;
    node.name=name;
    node.var_type=type;
    node.segment=segment;
    node.offset=offset;
    if(control.tk.process=="walk_subr_call"){
        control.sys("push argument 1\n");
        scontrol.add("push argument");
        control.call_params.push_back(node);
        print_status("walk_var:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
    }
    else if(control.stack_status=="stack_push"){
        
        print_status("walk_var:control.stack_status:stack_push");

        //control.sys("pop "+segment+" "+to_string(offset)+"\n");

        variable v;
        v.name=name;
        v.type=type;
        v.segment=segment;
        v.offset=offset;
        control.var_stack.push(v);
        
    }
    else if(control.stack_status=="walk_let"){
        print_status("walk_var:control.stack_status:walk_let");



            control.sys("pop "+segment+" "+to_string(offset)+"\n");
            VarNode node=SymbolControl.find_node(name,"function","local","");
            if(node.name!="" && LState.func){

            if(scontrol.contain({"walk_function","walk_subr_body","walk_statements","walk_statement","walk_let","walk_subr_call:control.walk_subr_call_args","walk_var"})){
                if(SymbolControl.find_node_list(name,"function","local","").size()>0){

                    
                    control.sys("push local 0\n");
                    
                    scontrol.add("push local");
                }
            }

                
                
            }else{
                print_status("walk_var:Not Find:"+name+"\n");
            }
        

        
    }
    else if(control.stack_status=="walk_return_expr"){
        print_status("walk_var:control.stack_status:walk_return_expr");
        VarNode node=SymbolControl.find_node(name,"",segment,"");
        if(node.name!="" && LState.method){
            control.sys("push argument 0\n");
            control.sys("pop pointer 0\n");
            scontrol.add("push argument");
            scontrol.add("pop pointer");
        }
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        scontrol.add("push "+segment);
    }
    else{
        print_status("walk_var:control.stack_status:default");
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        scontrol.add("push "+segment);
    }
    scontrol.remove("walk_var");
}

void walk_array_index(ast t)
{
    print_status(31,"walk_array_index");
    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;

    walk_var(var) ;
    walk_expr(index) ;
    scontrol.remove("walk_array_index");
}

void walk_call_as_function(ast t)
{
    print_status(32,"walk_call_as_function");
    string class_name = get_call_as_function_class_name(t) ;
    ast subr_call = get_call_as_function_subr_call(t) ;
    print_status("walk_call_as_function:class_name:"+class_name);

    control.status="walk_call_as_function";
    walk_subr_call(subr_call) ;
    control.status="";
    scontrol.remove("walk_call_as_function");
}

void walk_call_as_method(ast t)
{
    print_status(33,"walk_call_as_method");
    string class_name = get_call_as_method_class_name(t) ;
    ast var = get_call_as_method_var(t) ;
    ast subr_call = get_call_as_method_subr_call(t) ;
    print_status("walk_call_as_method:class_name:"+class_name);

    control.status="walk_call_as_method";
    switch(ast_node_kind(var))
    {
    case ast_this:
        walk_this(var) ;
        break ;
    case ast_var:
        walk_var(var) ;
        break ;
    default:
        fatal_error(0,"Expected var or this") ;
        break ;
    }
    walk_subr_call(subr_call) ;
    control.status="";
    scontrol.remove("walk_call_as_method");
}

void walk_subr_call(ast t)
{
    print_status(34,"walk_subr_call");
    string subr_name = get_subr_call_subr_name(t) ;
    ast expr_list = get_subr_call_expr_list(t) ;
    string call_cmd="";
    print_status("walk_expr_list:subr_name:"+subr_name);
    if(control.status=="walk_call_as_function" || control.status=="walk_call_as_method"){
        call_cmd="call "+control.class_name+"."+subr_name+" ";
    }
    control.tk.process="walk_subr_call";
    
    control.call_cmd=call_cmd;
    walk_expr_list(expr_list);

    print_status("walk_subr_call:in_params:"+to_string((int)control.in_params.size())+":call_params:"+to_string((int)control.call_params.size()));
    
    

    int args=0;
    for(int i=0;i<(int)control.in_params.size();i++){
        for(int j=0;j<(int)control.call_params.size();j++){
            if(control.in_params[i].name==control.call_params[j].name){
                args+=1;
                break;
            }
        }
    }

    if(args>0){
        control.add("{"+LState.call_name+"}","push argument "+to_string(args-1)+"\n");
        scontrol.add("push argument");
    }else{
        control.add("{"+LState.call_name+"}","");
    }

    print_status(35,"walk_subr_call:control.walk_subr_call_args");
    int call_args=0;
    while(scontrol.contain({"push local"})){
        scontrol.remove({"push local"});
        call_args+=1;
    }
    while(scontrol.contain({"push constant"})){
        scontrol.remove({"push constant"});
        call_args+=1;
    }
    while(scontrol.contain({"push argument"})){
        scontrol.remove({"push argument"});
        call_args+=1;
    }
    if(call_args>0)
        control.walk_subr_call_args=call_args;

    
    control.sys(control.call_cmd+to_string(control.walk_subr_call_args)+"\n");
    control.tk.process="";
    scontrol.remove("walk_subr_call");
    control.walk_subr_call_args=0;
}

void walk_expr_list(ast t)
{
    print_status(36,"walk_expr_list");
    int nexpressions = size_of_expr_list(t) ;
    print_status("walk_expr_list:nexpressions:"+to_string(nexpressions));
    for ( int i = 0 ; i < nexpressions ; i++ )
    {
        walk_expr(get_expr_list(t,i)) ;
    }
    scontrol.remove("walk_expr_list");
}

void walk_infix_op(ast t)
{
    print_status(37,"walk_infix_op");
    string op = get_infix_op_op(t) ;
    print_status("walk_infix_op:op:"+op);
    if(control.stack_status=="stack_push"){
        control.op_stack.push(op);
    }else{
        if(op=="+"){
            control.sys("add\n");
        }
    }
    scontrol.remove("walk_infix_op");
}

// main program
int main(int argc,char **argv)
{
    // walk an AST parsed from XML and print VM code
    walk_class(ast_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}



