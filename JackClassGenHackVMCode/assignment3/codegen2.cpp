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
            if(vars[i].call_name!=call_name && 
                vars[i].segment=="argument" && 
                _area=="class"
                ){
                //check_argument=false;
            }
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
    string tk_statement;
};



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
    TokenString tk;
    string status;
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
    if(debug){
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
}

void print_status(double num,string msg){
    if(debug){ 
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
        debug_msg.push_back(status+":"+to_string(num)+" "+msg+"\n");
    }
}

int print_debug(){
    if(debug){
        write_to_output("==============debug==============\n");
        for(int i=0;i<(int)debug_msg.size();i++){
            write_to_output(debug_msg[i]);
        }
        write_to_output("==============debug==============\n");
        SymbolControl.print();
    }
    return (int)debug_msg.size();
}

void walk_class(ast t){

    print_status(0,"walk_class");
    string myclassname = get_class_class_name(t) ;
    control.class_name=myclassname;
    LState.class_dec=true;
    control.tk.tk_statement="walk_class";
    ast var_decs = get_class_var_decs(t) ;
    control.tk.tk_statement="";
    LState.class_dec=false;
    ast subr_decs = get_class_subr_decs(t) ;

    walk_class_var_decs(var_decs) ;
    walk_subr_decs(subr_decs) ;

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
    SymbolControl.declare_variable(name,"",segment,type,false);

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
}

void walk_subr(ast t)
{
    print_status(5,"walk_subr");
    ast subr = get_subr_subr(t) ;
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
    
    control.params_tag="{"+control.class_name+"."+name+".params"+"}";
    control.sys("function "+control.class_name+"."+name+"."+control.params_tag+"\n");

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
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
}

void walk_subr_body(ast t)
{
    print_status(10,"walk_subr_body");
    ast decs = get_subr_body_decs(t) ;
    ast body = get_subr_body_body(t) ;
    walk_var_decs(decs) ;
    walk_statements(body) ;
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
        if(control.tk.tk_statement=="IF_ELSE" || nstatements>0){
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
}


void walk_statement(ast t)
{
    print_status(13,"walk_statement");
    ast statement = get_statement_statement(t) ;

    switch(ast_node_kind(statement))
    {
    case ast_let:
        control.tk.tk_statement="LET";
        LState.let=true;
        walk_let(statement) ;
        LState.let=false;
        break ;
    case ast_let_array:
        control.tk.tk_statement="LET_ARRAY";
        LState.let=true;
        walk_let_array(statement) ;
        LState.let=false;
        break ;
    case ast_if:
        control.tk.tk_statement="IF";
        walk_if(statement) ;
        break ;
    case ast_if_else:
        control.tk.tk_statement="IF_ELSE";
        walk_if_else(statement) ;
        break ;
    case ast_while:
        control.tk.tk_statement="WHILE";
        walk_while(statement) ;
        break ;
    case ast_do:
        control.tk.tk_statement="DO";
        walk_do(statement) ;
        break ;
    case ast_return:
        control.tk.tk_statement="RETURN";
        walk_return(statement) ;
        break ;
    case ast_return_expr:
        control.tk.tk_statement="RETURN_EXPR";
        walk_return_expr(statement) ;
        break ;
    case ast_statements:
        control.tk.tk_statement="STATEMENTS";
        walk_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }
    control.tk.tk_statement="";
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
}

void walk_if_else(ast t)
{
    print_status(17,"walk_if_else");
    ast condition = get_if_else_condition(t) ;
    int tk_if=control.tk.tk_if;

    control.sys("push constant 0\n");
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

}

void walk_while(ast t)
{
    print_status(18,"walk_while");
    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;
    int tk_while=control.tk.tk_while;

    control.sys("label WHILE_EXP"+to_string(tk_while)+"\n");
    control.sys("push constant 0\n");
    walk_expr(condition) ;
    walk_statements(body) ;
    control.sys("goto WHILE_EXP"+to_string(tk_while)+"\n");
    control.sys("label WHILE_END"+to_string(tk_while)+"\n");
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
}

void walk_return(ast t)
{
    print_status(20,"walk_return");
    control.sys("push constant 0\n");
    control.sys("return\n");
}

void walk_return_expr(ast t)
{
    print_status(21,"walk_return_expr");
    ast expr = get_return_expr(t) ;
    control.stack_status="walk_return_expr";
    walk_expr(expr) ;
    control.stack_status="";
    control.sys("return\n");
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
}

void walk_int(ast t)
{
    print_status(24,"walk_int");
    int _constant = get_int_constant(t) ;
    print_status("walk_int:_constant:"+to_string(_constant));
    if(control.stack_status=="stack_push"){
        variable v;
        v.name=to_string(_constant);
        v.type="int";
        v.segment="constant";
        v.offset=-1;
        control.var_stack.push(v);
    }
    else{
        control.sys("push constant "+to_string(_constant)+"\n");
    }
}   

void walk_string(ast t)
{
    print_status(25,"walk_string");
    string _constant = get_string_constant(t) ;
    print_status("walk_string:_constant:"+_constant);
}

void walk_bool(ast t)
{
    print_status(26,"walk_bool");
    bool _constant = get_bool_t_or_f(t) ;
    string _r=((_constant)?"true":"false");
    print_status("walk_bool:_constant:"+_r);
    if(control.tk.tk_statement=="IF" || control.tk.tk_statement=="IF_ELSE"){
        control.sys(((_constant)?"not\n":""));

    }
    if(control.tk.tk_statement=="WHILE"){
        int tk_while=control.tk.tk_while;
        control.sys(((_constant)?"not\nnot\n":"not\n"));
        control.sys("if-goto "+control.tk.tk_statement+"_END"+to_string(tk_while)+"\n");
        control.tk.tk_while+=1;
    }
}

void walk_null(ast t)
{
    print_status(27,"walk_null");
}

void walk_this(ast t)
{
    print_status(28,"walk_this");
    control.sys("push constant 1\n");
    control.sys("call Memory.alloc 1\n");
    control.sys("pop pointer 0\n");
    control.sys("push pointer 0\n");
}

void walk_unary_op(ast t)
{
    print_status(29,"walk_unary_op");
    string uop = get_unary_op_op(t);
    ast term = get_unary_op_term(t) ;
    print_status("walk_unary_op:uop:"+uop);
    walk_term(term) ;
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
    
    if(control.stack_status=="stack_push"){
        variable v;
        v.name=name;
        v.type=type;
        v.segment=segment;
        v.offset=offset;
        control.var_stack.push(v);
    }
    else if(control.stack_status=="walk_let"){
        control.sys("pop "+segment+" "+to_string(offset)+"\n");
        VarNode node=SymbolControl.find_node(name,"function","local","");
        if(node.name!="" && LState.func){
            control.sys("push local 0\n");
        }else{
            print_status(30.1,"walk_var:Not Find:"+name+"\n");
        }
    }
    else if(control.stack_status=="walk_return_expr"){
        VarNode node=SymbolControl.find_node(name,"",segment,"");
        if(node.name!="" && LState.method){
            control.sys("push argument "+to_string(offset)+"\n");
            control.sys("pop pointer "+to_string(offset)+"\n");
        }
        control.sys("push "+segment+" "+to_string(offset)+"\n");
    }
    else{
        control.sys("push "+segment+" "+to_string(offset)+"\n");
    }
}

void walk_array_index(ast t)
{
    print_status(31,"walk_array_index");
    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;

    walk_var(var) ;
    walk_expr(index) ;
}

void walk_call_as_function(ast t)
{
    print_status(32,"walk_call_as_function");
    string class_name = get_call_as_function_class_name(t) ;
    ast subr_call = get_call_as_function_subr_call(t) ;
    print_status("walk_call_as_function:class_name:"+class_name);
    control.sys("call "+class_name+".");
    control.status="walk_call_as_function";
    walk_subr_call(subr_call) ;
    control.status="";
}

void walk_call_as_method(ast t)
{
    print_status(33,"walk_call_as_method");
    string class_name = get_call_as_method_class_name(t) ;
    ast var = get_call_as_method_var(t) ;
    ast subr_call = get_call_as_method_subr_call(t) ;
    print_status("walk_call_as_method:class_name:"+class_name);
    control.sys("call "+class_name+".");
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
}

void walk_subr_call(ast t)
{
    print_status(34,"walk_subr_call");
    string subr_name = get_subr_call_subr_name(t) ;
    ast expr_list = get_subr_call_expr_list(t) ;
    print_status("walk_expr_list:subr_name:"+subr_name);
    if(control.status=="walk_call_as_function" || control.status=="walk_call_as_method"){
        if(control.status=="walk_call_as_method"){
            control.sys(subr_name+" 1\n");
        }
        else{
            control.sys(subr_name+" 0\n");
        }
        
    }
    walk_expr_list(expr_list) ;
}

void walk_expr_list(ast t)
{
    print_status(35,"walk_expr_list");
    int nexpressions = size_of_expr_list(t) ;
    print_status("walk_expr_list:nexpressions:"+to_string(nexpressions));
    for ( int i = 0 ; i < nexpressions ; i++ )
    {
        walk_expr(get_expr_list(t,i)) ;
    }
}

void walk_infix_op(ast t)
{
    print_status(36,"walk_infix_op");
    string op = get_infix_op_op(t) ;
    print_status("walk_infix_op:op:"+op);
    if(control.stack_status=="stack_push"){
        control.op_stack.push(op);
    }else{
        if(op=="+"){
            control.sys("add\n");
        }
    }
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

