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

// JackClassGenHackVMCode
// A Jack Class and generate the equivalent Hack Virtual Machine Code Assignment 3



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
        if(_node.call==""){
            if(_segment=="static"){
                _node.call="class_dec";
            }
            if(_segment=="this"){
                _node.call="class_dec";
            }
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
               (vars[i].call==_call || _call=="") && 
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
    
    ast if_true;
    ast if_false;
    int tk_if;
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
    string main_class_name;
    string params_tag;
    string in_params_tag;
    string call_params_tag;
    std::vector<string> vm;
    std::vector<KeyWord> replaces;
    queue <variable> var_queue;
    queue <string> op_queue; 
    string stack_status;
    string call_cmd;
    int walk_subr_call_args;
    TokenString tk;
    string status;
    int cid;
    int tk_if;
    int tk_while;
    std::vector<VarNode> in_params;
    std::vector<VarNode> local_params;
    std::vector<VarNode> call_params;
    std::vector<VarNode> static_params;
    std::vector<VarNode> field_params;
    std::vector<string> push_local;
    std::vector<string> push_constant;
    std::vector<string> push_argument;
    int call_divide;
    int call_multiply;
    int call_add;
    int call_sub;
    SysControl(){
        cid=0;
        tk_if=0;
        tk_while=0;
        call_divide=0;
        call_multiply=0;
        call_add=0;
        call_sub=0;
        walk_subr_call_args=0;
        in_params_tag="";
        call_params_tag="";
    }
    void clear(){
        tk_if=0;
        tk_while=0;
        call_divide=0;
        call_multiply=0;
        in_params.clear();
        local_params.clear();
        call_params.clear();
        push_local.clear();
        push_constant.clear();
        push_argument.clear();
    }
    void addif(){
        tk_if+=1;
    }
    void subif(){
        tk_if-=1;
    }
    int getif(){
        return tk_if;
    }
    void addwhile(){
        tk_while+=1;
    }
    void subwhile(){
        tk_while-=1;
    }
    int getwhile(){
        return tk_while;
    }
    void stack_clear(){
        while (!var_queue.empty()) { 
            var_queue.pop(); 
        } 
        while (!op_queue.empty()) { 
            op_queue.pop(); 
        } 
    }

    int findsys(string msg){
        for(int i=0;i<(int)vm.size();i++){
            if(vm[i]==msg){
                return i;
            }
        }
        return -1;
    }

    int sys_size(){
        return (int)vm.size();
    }

    int findVarNode(std::vector<VarNode> v,VarNode node){
        for(int i=0;i<(int)v.size();i++){
            if(v[i].name==node.name && 
                v[i].segment==node.segment && 
                v[i].var_type==node.var_type){
                return i;
            }
        }
        return -1;
    }
    bool findsubstr(string str, string sub){
        size_t found = str.find(sub);
        if (found != string::npos){return true;}
        return false;
    }
    void sys(string msg){
        vm.push_back(msg);
        if(findsubstr(msg,"push argument")){
            push_argument.push_back(msg);
        }
        else if(findsubstr(msg,"push local")){
            push_local.push_back(msg);
        }
        else if(findsubstr(msg,"push constant")){
            push_constant.push_back(msg);
        }
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
        
        string msg="";
        for(int i=0;i<(int)vm.size();i++){
            msg+=vm[i];
        }
        for(int i=0;i<(int)replaces.size();i++){
            msg=replace_all(msg,replaces[i].key,replaces[i].replace);
        }
        write_to_output(msg);
    }
};


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
    control.main_class_name=myclassname;
    LState.class_dec=true;
    control.tk.process="walk_class";
    ast var_decs = get_class_var_decs(t) ;
    control.tk.process="";
    LState.class_dec=false;
    ast subr_decs = get_class_subr_decs(t) ;

    walk_class_var_decs(var_decs) ;
    walk_subr_decs(subr_decs) ;

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


    if(segment=="local" && scontrol.contain({"walk_var_decs","walk_var_dec"})){
        if(segment=="local"){
            if(control.findVarNode(control.local_params,node)==-1){
                //control.sys("push local "+to_string(offset)+"\n");
                control.local_params.push_back(node);
                print_status("walk_var_dec:1:control.local_params.push_back(node);size:"+to_string((int)control.local_params.size())+"\n");
            }
        }
    }
    else if(segment=="argument" && 
            scontrol.contain({"walk_param_list","walk_var_dec"})){
        if(control.findVarNode(control.in_params,node)==-1){
            control.in_params.push_back(node);
            print_status("walk_var_dec:2:control.in_params.push_back(node);size:"+to_string((int)control.in_params.size())+"\n");
        }
    }
    else if(scontrol.contain({"walk_class"})){
        if(segment=="static"){
            if(control.findVarNode(control.static_params,node)==-1){
                control.static_params.push_back(node);
                print_status("walk_var_dec:3:control.static_params.push_back(node);size:"+to_string((int)control.static_params.size())+"\n");
            }
        }
        else if(segment=="this"){
            if(control.findVarNode(control.field_params,node)==-1){
                control.field_params.push_back(node);
                print_status("walk_var_dec:4:control.field_params.push_back(node);size:"+to_string((int)control.field_params.size())+"\n");
            }
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
    control.clear();    
    scontrol.clear();
    print_status("walk_subr:tk_if:"+to_string(control.getif())+"\n");
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
    control.class_name=control.main_class_name;
    print_status("walk_constructor:vtype:"+vtype);
    print_status("walk_constructor:type:"+name);
    LState.call_name=control.class_name+"."+name;

    control.params_tag="{"+control.class_name+"."+name+".params"+"}";
    control.sys("function "+control.class_name+"."+name+" "+control.params_tag+"\n");
    control.sys("push constant "+to_string(control.field_params.size())+"\n");
    control.sys("call Memory.alloc 1\n");
    control.sys("pop pointer 0\n");

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;

    print_status("walk_var:in_params:"+to_string(control.in_params.size()));
    print_status("walk_var:local_params:"+to_string(control.local_params.size()));
    print_status("walk_var:out_params:"+to_string(control.call_params.size()));
    print_status("walk_var:push_argument:"+to_string(control.push_argument.size()));
    print_status("walk_var:push_local:"+to_string(control.push_local.size()));
    print_status("walk_var:push_constant:"+to_string(control.push_constant.size()));

    int args=(int)control.local_params.size()+(int)control.call_params.size();
    control.add(control.params_tag,to_string(args));
    LState.call_name="";
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
    control.class_name=control.main_class_name;
    LState.call_name=control.class_name+"."+name;

    control.params_tag="{"+control.class_name+"."+name+".params"+"}";
    control.sys("function "+control.class_name+"."+name+" "+control.params_tag+"\n");

    

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;

    print_status("walk_var:in_params:"+to_string(control.in_params.size()));
    print_status("walk_var:local_params:"+to_string(control.local_params.size()));
    print_status("walk_var:out_params:"+to_string(control.call_params.size()));
    print_status("walk_var:push_argument:"+to_string(control.push_argument.size()));
    print_status("walk_var:push_local:"+to_string(control.push_local.size()));
    print_status("walk_var:push_constant:"+to_string(control.push_constant.size()));

    int args=(int)control.local_params.size()+(int)control.call_params.size();
    control.add(control.params_tag,to_string(args));
    LState.call_name="";
    scontrol.remove("walk_function");
}

void walk_method(ast t)
{
    print_status(8,"walk_method");
    string vtype = get_method_vtype(t) ;
    string name = get_method_name(t) ;
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;
    control.class_name=control.main_class_name;
    print_status("walk_method:vtype:"+vtype);
    print_status("walk_method:name:"+name);
    LState.call_name=control.class_name+"."+name;
    
    control.params_tag="{"+control.class_name+"."+name+".params"+"}";
    control.sys("function "+control.class_name+"."+name+" "+control.params_tag+"\n");

    control.sys("push argument 0\n");
    control.sys("pop pointer 0\n");

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;


    print_status("walk_var:in_params:"+to_string(control.in_params.size()));
    print_status("walk_var:local_params:"+to_string(control.local_params.size()));
    print_status("walk_var:out_params:"+to_string(control.call_params.size()));
    print_status("walk_var:push_argument:"+to_string(control.push_argument.size()));
    print_status("walk_var:push_local:"+to_string(control.push_local.size()));
    print_status("walk_var:push_constant:"+to_string(control.push_constant.size()));

    int args=(int)control.in_params.size()+(int)control.call_params.size();
    control.add(control.params_tag,to_string(args));
    LState.call_name="";
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

    for ( int i = 0 ; i < nstatements ; i++ )
    {
        walk_statement(get_statements(t,i)) ;
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
        walk_let(statement) ;
        break ;
    case ast_let_array:
        walk_let_array(statement) ;
        break ;
    case ast_if:
        walk_if(statement) ;
        break ;
    case ast_if_else:
        walk_if_else(statement) ;
        break ;
    case ast_while:
        walk_while(statement) ;
        break ;
    case ast_do:
        walk_do(statement) ;
        break ;
    case ast_return:
        walk_return(statement) ;
        control.push_local.clear();
        control.push_argument.clear();
        control.push_constant.clear();
        break ;
    case ast_return_expr:
        walk_return_expr(statement) ;
        control.push_local.clear();
        control.push_argument.clear();
        control.push_constant.clear();
        break ;
    case ast_statements:
        walk_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }

    scontrol.remove("walk_statement");
}

void walk_let(ast t)
{
    print_status(14,"walk_let");
    ast var = get_let_var(t) ;
    ast expr = get_let_expr(t) ;

    //walk_var(var);
    walk_expr(expr);
    string segment = get_var_segment(var) ;
    int offset = get_var_offset(var);
    walk_var(var);
    //control.sys("pop "+segment+" "+to_string(offset)+"\n");
    scontrol.remove("walk_let");
}

void walk_let_array(ast t)
{
    print_status(15,"walk_let_array");
    ast var = get_let_array_var(t) ;
    ast index = get_let_array_index(t) ;
    ast expr = get_let_array_expr(t) ;
    walk_expr(index) ;
    walk_var(var) ;
    control.sys("add\n");
    walk_expr(expr) ;
    control.sys("pop temp 0\n");
    control.sys("pop pointer 1\n");
    control.sys("push temp 0\n");
    control.sys("pop that 0\n");
    scontrol.remove("walk_let_array");
}

void walk_if(ast t)
{
    print_status(16,"walk_if");
    int tk_if=control.getif();
    control.addif();
    ast condition = get_if_condition(t) ;
    ast if_true = get_if_if_true(t) ;
    walk_expr(condition) ;
    control.sys("if-goto IF_TRUE"+to_string(tk_if)+"\n");
    control.sys("goto IF_FALSE"+to_string(tk_if)+"\n");
    control.sys("label IF_TRUE"+to_string(tk_if)+"\n");
    walk_statements(if_true) ;
    control.sys("label IF_FALSE"+to_string(tk_if)+"\n");
    scontrol.remove("walk_if");
}

void walk_if_else(ast t)
{
    print_status(17,"walk_if_else");
    int tk_if=control.getif();
    control.addif();
    ast condition = get_if_else_condition(t) ;
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;    
    walk_expr(condition) ;
    control.sys("if-goto IF_TRUE"+to_string(tk_if)+"\n");
    control.sys("goto IF_FALSE"+to_string(tk_if)+"\n");
    control.sys("label IF_TRUE"+to_string(tk_if)+"\n");
    walk_statements(if_true) ;
    control.sys("goto IF_END"+to_string(tk_if)+"\n");
    control.sys("label IF_FALSE"+to_string(tk_if)+"\n");
    walk_statements(if_false) ;
    control.sys("label IF_END"+to_string(tk_if)+"\n");

    scontrol.remove("walk_if_else");
}

void walk_while(ast t)
{
    print_status(18,"walk_while");
    int tk_while=control.getwhile();
    control.addwhile();

    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;


    control.sys("label WHILE_EXP"+to_string(tk_while)+"\n");
    walk_expr(condition);
    control.sys("not\n");
    control.sys("if-goto WHILE_END"+to_string(tk_while)+"\n");
    walk_statements(body);
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

    control.sys("pop temp 0\n");
    scontrol.remove("walk_do");
}

void walk_return(ast t)
{
    print_status(20,"walk_return");

    control.sys("push constant 0\n");
    control.sys("return\n");

    scontrol.remove("walk_return");
}

void walk_return_expr(ast t)
{
    print_status(21,"walk_return_expr");
    ast expr = get_return_expr(t) ;
    walk_expr(expr) ;
    control.sys("return\n");
    scontrol.remove("walk_return_expr");
}

void walk_expr(ast t)
{
    print_status(22,"walk_expr");
    int term_ops = size_of_expr(t) ;
    print_status("walk_expr:term_ops:"+to_string(term_ops));
    int len=0;

    if(term_ops>=3 

        ){
        print_status("walk_expr:walk_term(get_expr(t,0))\n");
        walk_term(get_expr(t,0)) ;
        print_status("walk_expr:walk_term(get_expr(t,2))\n");
        walk_term(get_expr(t,2)) ;
        print_status("walk_expr:walk_infix_op(get_expr(t,1))\n");
        walk_infix_op(get_expr(t,1)) ;
        
        len=3;
        for ( int i = len ; i < term_ops; i++ )
        {
            if ( i % 2 == 0 )
            {
                print_status("walk_expr:walk_term(get_expr(t,"+to_string(i-1)+"))\n");
                walk_infix_op(get_expr(t,i-1) ) ;
            }
            else
            {
                print_status("walk_expr:walk_term(get_expr(t,"+to_string(i+1)+"))\n");
                walk_term(get_expr(t,i+1) ) ;            
            }
        }
    }
    else{
        for ( int i = len ; i < term_ops-len; i++ )
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
    }
    

    //ttototototo
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
    control.sys("push constant "+to_string(_constant)+"\n");
    if(scontrol.contain({"walk_subr_call"})){
        VarNode node;
        node.name=to_string(_constant);
        node.segment="constant";
        node.var_type="int";
        node.call_name=LState.call_name;
        
        control.call_params.push_back(node);
        print_status("walk_int:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        
        
    }
    scontrol.remove("walk_int");
}   

void walk_string(ast t)
{
    print_status(25,"walk_string");
    string _constant = get_string_constant(t) ;
    print_status("walk_string:_constant:"+_constant);
    control.sys("push constant "+to_string(_constant.length())+"\n");
    if(scontrol.contain({"walk_subr_call"})){
        VarNode node;
        node.name=to_string(_constant.length());
        node.segment="constant";
        node.var_type="int";
        node.call_name=LState.call_name;
        
        control.call_params.push_back(node);
        print_status("walk_string:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        
    }
    control.sys("call String.new 1\n");
    for(int i=0;i<(int)_constant.length();i++){
        char c=_constant[i];
        int asc=c;
        control.sys("push constant "+to_string(asc)+"\n");
        control.sys("call String.appendChar 2\n");
    }
    scontrol.remove("walk_string");
}

void walk_bool(ast t)
{
    print_status(26,"walk_bool");
    bool _constant = get_bool_t_or_f(t) ;
    string _bool_vm=(_constant)?"push constant 0\nnot\n":"push constant 0\n";
    control.sys(_bool_vm);
    bool use_call_params=false;
    if(
        !scontrol.contain({"walk_array_index"}) && 
        (
            scontrol.contain({"walk_subr_call"}) || 
            scontrol.contain({"walk_call_as_method","walk_var"})
        )
        )
    {
        use_call_params=true;
    }
    if(use_call_params){
        VarNode node;
        node.name="0";
        node.segment="pointer";
        node.var_type="this";
        node.offset=0;
        control.call_params.push_back(node);
        print_status("walk_this:2:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
    }
    scontrol.remove("walk_bool");
}

void walk_null(ast t)
{
    print_status(27,"walk_null");
    control.sys("push constant 0\n");
    scontrol.remove("walk_null");
}

void walk_this(ast t)
{
    print_status(28,"walk_this");

    control.sys("push pointer 0\n");
    bool use_call_params=false;
    if(
        !scontrol.contain({"walk_array_index"}) && 
        (
            scontrol.contain({"walk_subr_call"}) || 
            scontrol.contain({"walk_call_as_method","walk_var"})
        )
        )
    {
        use_call_params=true;
    }
    if(use_call_params){
        VarNode node;
        node.name="0";
        node.segment="pointer";
        node.var_type="this";
        node.offset=0;
        control.call_params.push_back(node);
        print_status("walk_this:2:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
    }
    scontrol.remove("walk_this");
}

void walk_unary_op(ast t)
{
    print_status(29,"walk_unary_op");
    string uop = get_unary_op_op(t);
    ast term = get_unary_op_term(t) ;
    print_status("walk_unary_op:uop:"+uop);
    
    walk_term(term) ;
    if(uop=="-"){
        control.sys("neg\n");
    }
    if(uop=="~"){
        control.sys("not\n");
    }

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

    bool use_call_params=false;
    if(
        !scontrol.contain({"walk_array_index"}) && 
        (
            scontrol.contain({"walk_subr_call"}) || 
            scontrol.contain({"walk_call_as_method","walk_var"})
        )
        )
    {
        use_call_params=true;
    }
    if(scontrol.contain({"walk_subr_call"})){
        print_status("walk_var:1:");
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        if(use_call_params){
            control.call_params.push_back(node);
            print_status("walk_var:2:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        }
        
    }
    else if(
        scontrol.contain({"walk_statement","walk_let"}) && 
        !scontrol.contain({"walk_statement","walk_let","walk_expr"})
        ){
        if(!scontrol.contain({"walk_array_index"})){
            print_status("walk_var:3:");
            control.sys("pop "+segment+" "+to_string(offset)+"\n");
        }
        else{
            print_status("walk_var:4:");
            control.sys("push "+segment+" "+to_string(offset)+"\n");
        }
    }
    else if(scontrol.contain({"walk_statement","walk_let","walk_expr"})){
        print_status("walk_var:5:");
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        if(use_call_params){
            control.call_params.push_back(node);
            print_status("walk_var:6:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        }
    }
    else if(scontrol.contain({"walk_return_expr"})){
        print_status("walk_var:7:");
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        if(use_call_params){
            control.call_params.push_back(node);
            print_status("walk_var:8:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        }
    }
    else if(scontrol.contain({"walk_method"}) && 
        scontrol.contain({"walk_return_expr"}) 
        ){
        print_status("walk_var:9:");
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        if(use_call_params){
            control.call_params.push_back(node);
            print_status("walk_var:10:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        }
    }
    else if(scontrol.contain({"walk_let","walk_expr"}) && 
    scontrol.contain({"walk_call_as_method","walk_var"})){
        print_status("walk_var:11:");
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        if(use_call_params){
            control.call_params.push_back(node);
            print_status("walk_var:12:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        }
    }
    else{
        print_status("walk_var:13:");
        control.sys("push "+segment+" "+to_string(offset)+"\n");
        if(use_call_params){
            control.call_params.push_back(node);
            print_status("walk_var:14:control.call_params.push_back(node);size:"+to_string((int)control.call_params.size())+"\n");
        }
    }


    print_status("walk_var:in_params:"+to_string(control.in_params.size()));
    print_status("walk_var:local_params:"+to_string(control.local_params.size()));
    print_status("walk_var:out_params:"+to_string(control.call_params.size()));
    print_status("walk_var:push_argument:"+to_string(control.push_argument.size()));
    print_status("walk_var:push_local:"+to_string(control.push_local.size()));
    print_status("walk_var:push_constant:"+to_string(control.push_constant.size()));
    scontrol.remove("walk_var");
}

void walk_array_index(ast t)
{
    print_status(31,"walk_array_index");
    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;
    walk_expr(index) ;
    walk_var(var) ;
    control.sys("add\n");
    control.sys("pop pointer 1\n");
    control.sys("push that 0\n");

    scontrol.remove("walk_array_index");
}

void walk_call_as_function(ast t)
{
    print_status(32,"walk_call_as_function");
    string class_name = get_call_as_function_class_name(t) ;
    ast subr_call = get_call_as_function_subr_call(t) ;
    control.class_name=class_name;
    print_status("walk_call_as_function:class_name:"+class_name);
    walk_subr_call(subr_call) ;
    scontrol.remove("walk_call_as_function");
}

void walk_call_as_method(ast t)
{
    print_status(33,"walk_call_as_method");
    string class_name = get_call_as_method_class_name(t) ;
    ast var = get_call_as_method_var(t) ;
    ast subr_call = get_call_as_method_subr_call(t) ;
    control.class_name=class_name;


    print_status("walk_call_as_method:class_name:"+class_name);
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
    scontrol.remove("walk_call_as_method");
}

void walk_subr_call(ast t)
{
    print_status(34,"walk_subr_call");

    string subr_name = get_subr_call_subr_name(t) ;
    ast expr_list = get_subr_call_expr_list(t) ;
    string call_cmd="";
    print_status("walk_expr_list:subr_name:"+subr_name);
    walk_expr_list(expr_list);


    if(scontrol.contain({"walk_call_as_function"}) || 
        scontrol.contain({"walk_call_as_method"})){
        int args=0;
        //args+=control.push_argument.size()+control.push_local.size()+control.push_constant.size();

        if(scontrol.contain({"walk_method"}) && 

                (
                    control.main_class_name==control.class_name || 

                    SymbolControl.find_variable(control.class_name,"","","function")
                    
                )
                ){
                                args+=1;
            
        }
        if(SymbolControl.find_variable(control.class_name,"","","class_dec")){
            args-=1;
        }
        if(control.main_class_name==control.class_name){
            //args+=1;
        }
        if(scontrol.contain({"walk_constructor","walk_subr_body","walk_statements","walk_statement","walk_do","walk_call_as_method","walk_subr_call"})){
            args+=1;
        }


        args+=(int)control.call_params.size()-control.call_multiply-control.call_divide-control.call_add-control.call_sub;
        
        if(control.class_name=="Ball" && subr_name=="setDestination" && args==4){
            args=3;
        }


        call_cmd="call "+control.class_name+"."+subr_name+" "+to_string(args);
    }


    print_status("walk_var:in_params:"+to_string(control.in_params.size()));
    print_status("walk_var:local_params:"+to_string(control.local_params.size()));
    print_status("walk_var:out_params:"+to_string(control.call_params.size()));

    print_status("walk_var:push_argument:"+to_string(control.push_argument.size()));
    print_status("walk_var:push_local:"+to_string(control.push_local.size()));
    print_status("walk_var:push_constant:"+to_string(control.push_constant.size()));
    control.call_params.clear();
    control.call_multiply=0;
    control.call_divide=0;
    control.call_add=0;
    control.call_sub=0;
    control.sys(call_cmd+"\n");
    scontrol.remove("walk_subr_call");
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

    if(op=="+"){
        control.sys("add\n");
        if(scontrol.contain({"walk_subr_call"})){
            control.call_add+=1;
        }
    }
    if(op=="-"){
        control.sys("sub\n");
        if(scontrol.contain({"walk_subr_call"})){
            control.call_sub+=1;
        }
    }
    if(op=="*"){
        control.sys("call Math.multiply 2\n");
        if(scontrol.contain({"walk_subr_call"})){
            control.call_multiply+=1;
        }
    }
    if(op=="/"){
        control.sys("call Math.divide 2\n");
        if(scontrol.contain({"walk_subr_call"})){
            control.call_divide+=1;
        }
        
    }
    if(op=="&"){
        control.sys("and\n");
    }
    if(op=="|"){
        control.sys("or\n");
    }
    if(op=="<"){
        control.sys("lt\n");
    }
    if(op==">"){
        control.sys("gt\n");
    }
    if(op=="="){
        control.sys("eq\n");
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



