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
    int tab;
    std::vector<VarNode> in_params;
    std::vector<VarNode> call_params;

    SysControl(){
        cid=0;
        walk_subr_call_args=0;
        tab=0;
    }

    void stack_clear(){
        while (!var_stack.empty()) { 
            var_stack.pop(); 
        } 
        while (!op_stack.empty()) { 
            op_stack.pop(); 
        } 
    }

    void addtab(){
        tab+=1;
    }

    void subtab(){
        tab-=1;
    }

    string get_tab(){
        string tab_str="";
        for(int i=0;i<tab;i++){
            tab_str+="    ";
        }
        return tab_str;
    }

    void sys(string msg){
        sys(msg,true);
    }

    void sys(string msg,bool is_tab){
        if(msg!=""){
            vm.push_back(((is_tab)?get_tab():"")+msg);
            debug_msg.push_back("[code]: "+msg+"\n");
        }else{
            vm.push_back("\n");
        }
    }

    void line_between(){
        if(vm.size()>0){
            if(vm[vm.size()-1]==(get_tab()+"}\n")){
                vm.push_back("\n");
            }
        }
    }

    void del_last_blank(){
        if(vm.size()>0){
            if(vm[vm.size()-1]=="\n"){
                vm.erase(vm.begin()+vm.size()-1);
            }
        }
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
    }
    return (int)debug_msg.size();
}


// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to walk an abstract syntax tree, ast, of a Jack class
//        and pretty print the equivalent Jack source code.
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


void walk_class(ast t)
{
    print_status(1,"walk_class");
    string myclassname = get_class_class_name(t) ;
    ast var_decs = get_class_var_decs(t) ;
    ast subr_decs = get_class_subr_decs(t) ;
    print_status("walk_class:myclassname:"+myclassname);
    control.sys("class "+myclassname+"\n");
    control.sys("{\n");
    walk_class_var_decs(var_decs) ;
    walk_subr_decs(subr_decs) ;
    control.sys("}\n");



    scontrol.remove("walk_class");
    print_debug();
    control.print();
}

void walk_class_var_decs(ast t)
{
    print_status(2,"walk_class_var_decs");
    control.addtab();
    int ndecs = size_of_class_var_decs(t) ;
    print_status("walk_class_var_decs:ndecs:"+to_string(ndecs));
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_class_var_decs(t,i)) ;
        
    }
    if(ndecs>0){
        control.sys("");
    }
    control.subtab();
    scontrol.remove("walk_class_var_decs");
}

void walk_var_dec(ast t)
{
    print_status(3,"walk_var_dec");
    string name = get_var_dec_name(t) ;
    string type = get_var_dec_type(t) ;
    string segment = get_var_dec_segment(t) ;
    int offset = get_var_dec_offset(t) ;
    print_status("walk_var_dec:name:"+name+"\n");
    print_status("walk_var_dec:type:"+type+"\n");
    print_status("walk_var_dec:segment:"+segment+"\n");
    print_status("walk_var_dec:offset:"+to_string(offset)+"\n");
    print_status("walk_var_dec:declare_variable:"+name+"\n");
    VarNode node=SymbolControl.declare_variable(name,"",segment,type,false);
    if(segment=="argument"){
        if(scontrol.contain({"walk_param_list"})){
            control.in_params.push_back(node);
            print_status("walk_var_dec:control.in_params.push_back(node);size:"+to_string((int)control.in_params.size())+"\n");
        }
        if(scontrol.contain({"walk_param_list","walk_var_dec"})){
            control.sys(type+" "+name,false);
        }
    }
    else{
        if(segment=="local"){
            control.sys("var");
        }
        else if(segment=="static"){
            control.sys("static");
        }
        else if(segment=="this"){
            control.sys("field");
        }
        control.sys(" "+type+" "+name+" ;\n",false);
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
        if(i<size-1)
            control.sys("");
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
        walk_constructor(subr) ;
        break ;
    case ast_function:
        walk_function(subr) ;
        break ;
    case ast_method:
        walk_method(subr) ;
        break ;
    default:
        fatal_error(0,"Unexpected subroutine kind") ;
        break ;
    }

    scontrol.remove("walk_subr");
}

void walk_constructor(ast t)
{
    print_status(6,"walk_constructor");
    control.addtab();
    string vtype = get_constructor_vtype(t) ;
    string name = get_constructor_name(t) ;
    ast param_list = get_constructor_param_list(t) ;
    ast subr_body = get_constructor_subr_body(t) ;
    print_status("walk_constructor:vtype:"+vtype);
    print_status("walk_constructor:name:"+name);
    control.sys("constructor "+vtype+" "+name+"(");
    walk_param_list(param_list) ;
    control.sys(")\n",false);
    control.sys("{\n");
    walk_subr_body(subr_body) ;
    control.del_last_blank();
    control.sys("}\n");
    control.subtab();
    scontrol.remove("walk_constructor");
}

void walk_function(ast t)
{
    print_status(7,"walk_function");
    control.addtab();
    string vtype = get_function_vtype(t) ;
    string name = get_function_name(t) ;
    ast param_list = get_function_param_list(t) ;
    ast subr_body = get_function_subr_body(t) ;
    print_status("walk_function:vtype:"+vtype);
    print_status("walk_function:name:"+name);

    control.sys("function "+vtype+" "+name+"(");
    walk_param_list(param_list) ;
    control.sys(")\n",false);
    control.sys("{\n");
    walk_subr_body(subr_body) ;
    control.del_last_blank();
    control.sys("}\n");
    control.subtab();
    scontrol.remove("walk_function");
}

void walk_method(ast t)
{
    print_status(8,"walk_method");
    control.addtab();
    string vtype = get_method_vtype(t) ;
    string name = get_method_name(t) ;
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;

    control.sys("method "+vtype+" "+name+"(");
    walk_param_list(param_list) ;
    control.sys(")\n",false);
    control.sys("{\n");
    print_status("walk_method:vtype:"+vtype);
    print_status("walk_method:name:"+name);
    walk_subr_body(subr_body) ;
    control.del_last_blank();
    control.sys("}\n");
    control.subtab();
    scontrol.remove("walk_method");
}


void walk_param_list(ast t)
{
    print_status(9,"walk_param_list");
    int ndecs = size_of_param_list(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_param_list(t,i)) ;
        if(i<ndecs-1){
            control.sys(",",false);
        }
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
    control.addtab();
    int ndecs = size_of_var_decs(t) ;
    print_status("walk_var_decs:ndecs:"+to_string(ndecs));
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_var_decs(t,i)) ;
    }
    if(ndecs>0){
        control.sys("");
    }
    control.subtab();
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
        if(i<nstatements-1){
            //control.sys("");
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
        break ;
    case ast_return_expr:
        walk_return_expr(statement) ;
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
    control.addtab();
    ast var = get_let_var(t) ;
    ast expr = get_let_expr(t) ;
    control.line_between();
    control.sys("let ",true);
    walk_var(var) ;
    control.sys(" = ",false);
    walk_expr(expr) ;
    control.sys(" ;\n",false);
    control.subtab();
    scontrol.remove("walk_let");
}

void walk_let_array(ast t)
{
    print_status(15,"walk_let_array");
    control.addtab();
    ast var = get_let_array_var(t) ;
    ast index = get_let_array_index(t) ;
    ast expr = get_let_array_expr(t) ;
    control.sys("let ");
    walk_var(var) ;
    control.sys("[",false);
    walk_expr(index) ;
    control.sys("]",false);
    control.sys(" = ",false);
    walk_expr(expr) ;
    control.sys(" ;\n",false);
    control.subtab();
    scontrol.remove("walk_let_array");
}

void walk_if(ast t)
{
    bool pre_statement=false;
    if(scontrol.contain({"walk_let"}) ||  
        scontrol.contain({"walk_if"}) ||  
        scontrol.contain({"walk_if_else"}) ||  
        scontrol.contain({"walk_while"}) 
        ){
        pre_statement=true;
    }
    print_status(16,"walk_if");
    control.addtab();
    ast condition = get_if_condition(t) ;
    ast if_true = get_if_if_true(t) ;
    control.line_between();
    control.sys("if (");
    walk_expr(condition) ;
    control.sys(")\n",false);
    control.sys("{\n");
    walk_statements(if_true) ;
    control.sys("}\n");
    if(!pre_statement){
        control.sys("");
    }
    control.subtab();
    scontrol.remove("walk_if");
}

void walk_if_else(ast t)
{
    bool pre_statement=false;
    if(scontrol.contain({"walk_let"}) ||  
        scontrol.contain({"walk_if"}) ||  
        scontrol.contain({"walk_if_else"}) ||  
        scontrol.contain({"walk_while"}) 
        ){
        pre_statement=true;
    }
    print_status(17,"walk_if_else");
    control.addtab();
    ast condition = get_if_else_condition(t) ;
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;
    control.line_between();
    control.sys("if (");
    walk_expr(condition) ;
    control.sys(")\n",false);
    control.sys("{\n");
    walk_statements(if_true) ;
    control.sys("}\n");
    control.sys("else\n");
    control.sys("{\n");
    walk_statements(if_false) ;
    control.sys("}\n");
    if(!pre_statement){
        control.sys("");
    }
    control.subtab();
    scontrol.remove("walk_if_else");
}

void walk_while(ast t)
{
    bool pre_statement=false;
    if(scontrol.contain({"walk_let"}) ||  
        scontrol.contain({"walk_if"}) ||  
        scontrol.contain({"walk_if_else"}) ||  
        scontrol.contain({"walk_while"}) 
        ){
        pre_statement=true;
    }
    print_status(18,"walk_while");
    control.addtab();
    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;
    control.line_between();
    control.sys("while (");
    walk_expr(condition) ;
    control.sys(")\n",false);
    control.sys("{\n");
    walk_statements(body) ;
    control.sys("}\n");
    if(!pre_statement){
        control.sys("");
    }
    control.subtab();
    scontrol.remove("walk_while");
}

void walk_do(ast t)
{
    print_status(19,"walk_do");
    control.addtab();
    ast call = get_do_call(t) ;
    control.line_between();
    control.sys("do ");
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
    control.sys(" ;\n",false);
    control.subtab();
    scontrol.remove("walk_do");
}

void walk_return(ast t)
{
    print_status(20,"walk_return");
    control.addtab();
    control.sys("return ;\n");
    control.subtab();
    scontrol.remove("walk_return");
}

void walk_return_expr(ast t)
{
    print_status(21,"walk_return_expr");
    control.addtab();
    ast expr = get_return_expr(t) ;
    control.sys("return ");
    walk_expr(expr) ;
    control.sys(" ;\n",false);
    control.subtab();
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
        control.sys("(",false);
        walk_expr(term) ;
        control.sys(")",false);
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

    control.sys(to_string(_constant),false);
    scontrol.remove("walk_int");
}

void walk_string(ast t)
{
    print_status(25,"walk_string");
    string _constant = get_string_constant(t) ;
    print_status("walk_string:_constant:"+_constant);
    control.sys("\""+_constant+"\"",false);
    scontrol.remove("walk_string");
}

void walk_bool(ast t)
{
    print_status(26,"walk_bool");
    bool _constant = get_bool_t_or_f(t) ;
    print_status("walk_bool:_constant:"+to_string(_constant));

    control.sys((_constant)?"true":"false",false);
    scontrol.remove("walk_bool");
}

void walk_null(ast t)
{
    print_status(27,"walk_null");

    control.sys("null",false);

    scontrol.remove("walk_null");
}

void walk_this(ast t)
{
    print_status(28,"walk_this");
    if(scontrol.contain({"walk_term","walk_call_as_method","walk_this"})){

    }
    else if(scontrol.contain({"walk_do"}) && 
        !scontrol.contain({"walk_subr_call","walk_expr_list","walk_expr","walk_term"})){
    }
    else if(scontrol.contain({"walk_subr_call","walk_expr_list","walk_expr","walk_term"})){
        control.sys("this",false);
    }
    else{
        control.sys("this",false);
    }
    scontrol.remove("walk_this");
}

void walk_unary_op(ast t)
{
    print_status(29,"walk_unary_op");
    string uop = get_unary_op_op(t);
    ast term = get_unary_op_term(t) ;
    print_status("walk_unary_op:uop:"+uop);
    control.sys(uop,false);
    walk_term(term) ;
    scontrol.remove("walk_unary_op");
}

void walk_var(ast t)
{
    print_status(30,"walk_var");
    string name = get_var_name(t) ;
    string type = get_var_type(t) ;
    string segment = get_var_segment(t) ;
    int offset = get_var_offset(t) ;
    print_status("get_var_name:name:"+name);
    print_status("get_var_name:type:"+type);
    print_status("get_var_name:segment:"+segment);
    print_status("get_var_name:offset:"+to_string(offset));
    bool use_tab=false;
    if(scontrol.contain({"walk_let"}) &&  
        scontrol.contain({"walk_let_array"})
        ){
        if(!scontrol.contain({"walk_let_array","walk_expr","walk_term","walk_var"})){
            use_tab=true;
        }
    }
    control.sys(name,use_tab);
    if(scontrol.contain({"walk_let","walk_expr","walk_term"}) && 
        scontrol.contain({"walk_call_as_method"})){
        control.sys(".",false);
    }
    else if(scontrol.contain({"walk_do","walk_call_as_method"})){
        if(!scontrol.contain({"walk_do","walk_call_as_method","walk_subr_call","walk_expr_list","walk_expr","walk_term","walk_var"})){
            control.sys(".",false);
        }
    }

    scontrol.remove("walk_var");
}

void walk_array_index(ast t)
{
    print_status(31,"walk_array_index");
    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;

    walk_var(var) ;
    control.sys("[",false);
    walk_expr(index) ;
    control.sys("]",false);

    scontrol.remove("walk_array_index");
}

void walk_call_as_function(ast t)
{
    print_status(32,"walk_call_as_function");
    control.addtab();
    string class_name = get_call_as_function_class_name(t) ;
    ast subr_call = get_call_as_function_subr_call(t) ;
    print_status("walk_call_as_function:class_name:"+class_name);
    bool use_tab=true;
    if(scontrol.contain({"walk_let"}) || 
        scontrol.contain({"walk_let_array"}) || 
        scontrol.contain({"walk_while","walk_expr","walk_term","walk_call_as_function"})){
        use_tab=false;
    }
    if(!scontrol.contain({"walk_let"}) &&  
        !scontrol.contain({"walk_let_array"}) &&  
        !scontrol.contain({"walk_expr"})   
        ){
        use_tab=false;
    }
    control.sys(class_name+".",use_tab);
    walk_subr_call(subr_call);
    control.subtab();
    scontrol.remove("walk_call_as_function");
}

void walk_call_as_method(ast t)
{
    print_status(33,"walk_call_as_method");
    string class_name = get_call_as_method_class_name(t) ;
    ast var = get_call_as_method_var(t) ;
    ast subr_call = get_call_as_method_subr_call(t) ;

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
    print_status("walk_subr_call:subr_name:"+subr_name);
    control.sys(subr_name,false);
    control.sys("(",false);
    walk_expr_list(expr_list) ;
    control.sys(")",false);

    scontrol.remove("walk_subr_call");
}

void walk_expr_list(ast t)
{
    print_status(35,"walk_expr_list");
    int nexpressions = size_of_expr_list(t) ;
    print_status("walk_expr_list:nexpressions:"+to_string(nexpressions));
    for ( int i = 0 ; i < nexpressions ; i++ )
    {
        walk_expr(get_expr_list(t,i)) ;
        if(scontrol.contain({"walk_call_as_method","walk_subr_call"}) || 
            scontrol.contain({"walk_call_as_function","walk_subr_call"})
            ){
            if(i<nexpressions-1){
                control.sys(",",false);
            }
        }
    }

    scontrol.remove("walk_expr_list");
}

void walk_infix_op(ast t)
{
    print_status(36,"walk_infix_op");
    string op = get_infix_op_op(t) ;
    print_status("walk_infix_op:op:"+to_string(op));
    control.sys(" "+op+" ",false);
    scontrol.remove("walk_infix_op");
}

// main program
int main(int argc,char **argv)
{
    // walk an AST parsed from XML and pretty print equivalent Jack code
    walk_class(ast_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

