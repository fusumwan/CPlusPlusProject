#include <string>
#include <vector>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atol */
#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace CS_IO_Buffers ;
using namespace CS_Symbol_Tables ;
using namespace Jack_Compiler ;

// symbol table management so we can lookup declared variables
static symbols symbol_table = 0;
static symbols symbol_class_table = 0;
static symbols symbol_function_table=0;

// a global to record the next free position in the local segment
static int nextlocal = 0 ;
static int nextstatic = 0 ;
static int nextfield= 0 ; 
static bool debug=false;

// this function adds an identifier to the symbol table
// it returns a tree node representing the variable

static string main_class_name="";
static bool sys_error=false;
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
    ast declare_variable(Token _identifier,string _area,string _segment,string _type,bool _is_class){
        VarNode _node;
        _node.name = token_spelling(_identifier);
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
        _node.var=create_var(_node.name, _node.segment, _node.offset, _node.var_type);
        vars.push_back(_node);
        return _node.var;
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
               ) {
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
    ast lookup_variable(Token _identifier,string _area){

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
        return _node.var;
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
    }
};

static SymbolTable SymbolControl;


// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to parse a Jack class and construct the equivalent abstract syntax tree, ast.
//        The main function will print the ast formatted as XML
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// NOTE: use the fatal function below to print fatal error messages with the current
//       input file position marked.
//
// The skeleton code has one function per non-terminal in the Jack grammar
//  - comments describing how to create relevant ast nodes prefix most functions
//  - ast nodes are immutable - you must create all the sub-trees before creating a new node
//
// The structure of the skeleton code is just a suggestion
//  - it matches the grammar but does not quite match the ast structure
//  - you will need to change the parameters / results / functions to suit your own logic
//  - you can change it as much as you like
//
// NOTE: the best place to start is to write code to parse the input without attempting to create
//       any ast nodes. If correct this parser will report errors when it finds syntax errors but
//       correct programs will fail because the tree printer will not be provided with a valid ast.
//       Once the parsing works modify your code to create the ast nodes.
//
// NOTE: the return -1 ; statements are only present so that this skeleton code will compile.
//       these statements need to be replaced by code that creates ast nodes.
//

// fatal error handling
static void fatal(string message)
{
    fatal_error(0,"***** Error:\n" + token_context() + message) ;
}

// The Jack grammar to be recognised:
// program          ::= One or more classes, each class in a separate file named <class_name>'.Jack'
// class            ::= 'class' identifier '{' class_var_decs subr_decs '}'
// class_var_decs   ::= (static_var_dec | field_var_dec)*
// static_var_dec   ::= 'static' type identifier (',' identifier)* ';'
// field_var_dec    ::= 'field' type identifier (',' identifier)* ';'
// type             ::= 'int' | 'char' | 'boolean' | identifier
// vtype            ::= 'void' | type
// subr_decs        ::= (constructor | function | method)*
// constructor      ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// function         ::= 'function' vtype identifier '(' param_list ')' subr_body
// method           ::= 'method' vtype identifier '(' param_list ')' subr_body
// param_list       ::= ((type identifier) (',' type identifier)*)?
// subr_body        ::= '{' var_decs statements '}'
// var_decs         ::= var_dec*
// var_dec          ::= 'var' type identifier (',' identifier)* ';'

// statements       ::= statement*
// statement        ::= let | if | while | do | return
// let              ::= 'let' identifier index? '=' expr ';'
// if               ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// while            ::= 'while' '(' expr ')' '{' statements '}'
// do               ::= 'do' identifier (id_call | call) ';'
// return           ::= 'return' expr? ';'

// expr             ::= term (infix_op term)*
// term             ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// var_term         ::= identifier (index | id_call | call)?
// index            ::= '[' expr ']'
// id_call          ::= '.' identifier call
// call             ::= '(' expr_list ')'
// expr_list        ::= (expr (',' expr)*)?
// infix_op         ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// unary_op         ::= '-' | '~'



// forward declarations of one function per non-terminal rule in the Jack grammar - except for program
// NOTE: parse_type(), parse_vtype(), parse_unary_op() all return a Token not an ast
// NOTE: parse_static_var_dec(), parse_field_var_dec(), parse_var_dec() all return vector<ast> not an ast
//
ast parse_class() ;
ast parse_class_var_decs() ;
vector<ast> parse_static_var_dec() ;
vector<ast> parse_field_var_dec() ;
Token parse_type() ;
Token parse_vtype() ;
ast parse_subr_decs() ;
ast parse_constructor() ;
ast parse_function() ;
ast parse_method() ;
ast parse_param_list() ;
ast parse_subr_body() ;
ast parse_var_decs() ;
vector<ast> parse_var_dec() ;

ast parse_statements() ;
ast parse_statement() ;
ast parse_let() ;
ast parse_if() ;
ast parse_while() ;
ast parse_do() ;
ast parse_return() ;

ast parse_expr() ;
ast parse_term() ;
ast parse_var_term() ;
ast parse_index() ;
ast parse_id_call() ;
ast parse_call() ;
ast parse_expr_list() ;
ast parse_infix_op() ;
Token parse_unary_op() ;


void print_status(string function_name){
    if(debug){
        printf("%s: area:%s current_token:%s\n",
        function_name.c_str(),
        (LState.constructor)?"class":"function",
        token_spelling(current_token()).c_str());
    }
}

bool is_declared(VarNode Node){
    if(
        Node.name!="" &&
        Node.var_type!=""
        ){
        return true;
    }
    return false;
}

bool is_func(string name){
    for(int i=0;i<(int)funclist.size();i++){
        if(funclist[i]==name){
            return true;
        }
    }
    return false;
}




// class ::= 'class' identifier '{' class_var_decs subr_decs '}'
// create_class(myclassname,class_var_decs,class_subrs)
ast parse_class()
{
    push_error_context("parse_class()") ;
    print_status("parse_class");
    funclist.clear();
    ast decls ;         // the declarations
    ast stat ;          // the single statement that is the class
    mustbe(tk_class);
    main_class_name=token_spelling(mustbe(tk_identifier));
    mustbe(tk_lcb);
    LState.class_dec=true;
    decls=parse_class_var_decs();
    LState.class_dec=false;
    stat=parse_subr_decs();
    mustbe(tk_rcb);
    if(debug){
        SymbolControl.print();
    }
    pop_error_context();
    sys_error=SymbolControl.is_duplicated();
    if(!sys_error){
        return create_class(main_class_name,decls,stat) ;
    }else{
        sys_error=false;
        return -1 ; 
    }
}

// class_var_decs ::= (static_var_dec | field_var_dec)*
// returns ast_class_var_decs: create_class_var_decs(vector<ast> decs)
// create_class_var_decs must be passed a vector of ast_var_dec
//
ast parse_class_var_decs()
{
    push_error_context("parse_class_var_decs()") ;
    print_status("parse_class_var_decs");

    vector<ast> stat;


    vector<ast> vars;
    

    while(have(tk_class_var)){
        if(have(tk_static)){
            stat=parse_static_var_dec();
            for(int i=0;i<stat.size();i++){
                vars.push_back(stat[i]);
            }
        }
        else{
            stat=parse_field_var_dec();
            for(int i=0;i<stat.size();i++){
                vars.push_back(stat[i]);
            }
        }
    }
    pop_error_context() ;
    return create_class_var_decs(vars);

}

// static_var_dec ::= 'static' type identifier (',' identifier)* ';'
// returns vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
vector<ast> parse_static_var_dec()
{
    vector<ast> decs ;
    push_error_context("parse_class()") ;
    print_status("parse_static_var_dec");

    mustbe(tk_static);
    string type=token_spelling(mustbe(tk_type));

    ast dec;
    Token id= mustbe(tk_identifier);
    string name=token_spelling(id);

    string segment="static";
    dec = SymbolControl.declare_variable(id,"class",segment,type,false);
    //dec = declare_variable(id,segment,type,-1);
    int offset=get_var_offset(dec);

    dec = create_var_dec(name,segment,offset,type);
    decs.push_back(dec);
    while(have(tk_comma)){
        mustbe(tk_comma);
        id= mustbe(tk_identifier);
        name=token_spelling(id);
        dec = SymbolControl.declare_variable(id,"class",segment,type,false);
        //dec = declare_variable(id,segment,type,-1);
        int offset=get_var_offset(dec);
        
        dec = create_var_dec(name,segment,offset,type);
        decs.push_back(dec);
    }
        
    mustbe(tk_semi);

    pop_error_context();
    return decs ;
}

// field_var_dec ::= 'field' type identifier (',' identifier)* ';'
// returns vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
vector<ast> parse_field_var_dec()
{
    vector<ast> decs ;
    push_error_context("parse_class()") ;
    print_status("parse_static_var_dec");

    mustbe(tk_field);
    string type=token_spelling(mustbe(tk_type));

    ast dec;
    Token id= mustbe(tk_identifier);
    string name=token_spelling(id);
    string segment="this";

    dec = SymbolControl.declare_variable(id,"class",segment,type,false);
    //dec = declare_variable(id,segment,type,-1);
    int offset=get_var_offset(dec);
    
    dec = create_var_dec(name,segment,offset,type);
    decs.push_back(dec);
    while(have(tk_comma)){
        mustbe(tk_comma);
        id= mustbe(tk_identifier);
        name=token_spelling(id);
        dec = SymbolControl.declare_variable(id,"class",segment,type,false);
        //dec = declare_variable(id,segment,type,-1);
        int offset=get_var_offset(dec);
        
        dec = create_var_dec(name,segment,offset,type);
        decs.push_back(dec);
    }
        
    mustbe(tk_semi);

    pop_error_context();
    return decs ;
    
}

// type ::= 'int' | 'char' | 'boolean' | identifier
// returns the Token for the type
Token parse_type()
{
    push_error_context("parse_type()") ;
    print_status("parse_type");
    if(have(tk_int)){
        return mustbe(tk_int);
    }
    if(have(tk_char)){
        return mustbe(tk_char);
    }
    if(have(tk_boolean)){
        return mustbe(tk_boolean);
    }
    if(have(tk_identifier)){
        return mustbe(tk_identifier);
    }
    sys_error=true;
    pop_error_context() ;
    return -1 ;
}

// vtype ::= 'void' | type
// returns the Token for the type
Token parse_vtype()
{
    Token vtype;
    push_error_context("parse_vtype()") ;
    print_status("parse_vtype");
    if(have(tk_void)){
        vtype=mustbe(tk_void);
        pop_error_context() ;
        return vtype;
    }
    vtype=mustbe(tk_type);
    pop_error_context() ;
    return vtype;
}

// subr_decs ::= (constructor | function | method)*
// returns ast_subr_decs: create_subr_decs(vector<ast> subrs)
// create_subr_decs must be passed an vector of ast_subr
//
// ast_subr: create_subr(ast subr)
// create_subr must be passed one of: ast_constructor, ast_function or ast_method
//
ast parse_subr_decs()
{
    vector<ast> subrs;
    push_error_context("parse_subr_decs()");
    print_status("parse_subr_decs");
 
    while(
        have(tk_constructor) || 
        have(tk_function) || 
        have(tk_method) 
        ){
        ast decs;
        if(have(tk_constructor)){
            LState.constructor=true;
            decs=create_subr(parse_constructor());
            LState.constructor=false;
        }
        else if(have(tk_method)){
            LState.method=true;
            SymbolControl.reset_nextlocal(0);
            SymbolControl.reset_nextargument(1);
            decs=create_subr(parse_method());
            LState.method=false;
        }
        else if(have(tk_function)){
            LState.func=true;
            SymbolControl.reset_nextlocal(0);
            SymbolControl.reset_nextargument(0);
            decs=create_subr(parse_function());
            LState.func=false;
        }
        subrs.push_back(decs);
    }
    pop_error_context() ;
    return create_subr_decs(subrs);
}

// constructor ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// returns ast_constructor: create_constructor(string vtype,string name,ast params,ast body)
// . vtype: the constructor's return type, this must be it's class name
// . name: the constructor's name within its class
// . params: ast_param_list - the constructor's parameters
// . body: ast_subr_body - the constructor's body
//
ast parse_constructor(){
    push_error_context("parse_constructor()");
    print_status("parse_constructor");
    string vtype;
    string name="";
    ast params;
    ast body;
    mustbe(tk_constructor);
    vtype=token_spelling(parse_vtype());
    name=token_spelling(mustbe(tk_identifier));
    LState.call_name=name;
    funclist.push_back(name);
    mustbe(tk_lrb);
    params=parse_param_list();
    mustbe(tk_rrb);
    mustbe(tk_lcb);
    body=parse_subr_body();
    mustbe(tk_rcb);
    LState.call_name="";
    pop_error_context();
    


    return create_constructor(vtype,name,params,body);
}

// function ::= 'function' vtype identifier '(' param_list ')' subr_body
// returns ast_function: create_function(string vtype,string name,ast params,ast body)
// . vtype: the function's return type
// . name: the function's name within its class
// . params: ast_param_list - the function's parameters
// . body: ast_subr_body - the function's body
//
ast parse_function()
{
    push_error_context("parse_function()") ;
    print_status("parse_function");

    string vtype;
    string name;
    ast params;
    ast body;

    mustbe(tk_function);
    vtype=token_spelling(parse_vtype());
    name=token_spelling(mustbe(tk_identifier));
    LState.call_name=name;
    funclist.push_back(name);
    mustbe(tk_lrb);
    params=parse_param_list();
    mustbe(tk_rrb);
    mustbe(tk_lcb);
    body=parse_subr_body();
    mustbe(tk_rcb);
    LState.call_name="";
    pop_error_context();
    if(vtype=="")
        sys_error=true;
    return create_function(vtype,name,params,body);
}

// method ::= 'method' vtype identifier '(' param_list ')' subr_body
// returns ast_method: create_method(string vtype,string name,ast params,ast body)
// . vtype: the method's return type
// . name: the method;s name within its class
// . params: ast_param_list - the method's explicit parameters
// . body: ast_subr_body - the method's body
//
ast parse_method()
{
    push_error_context("parse_method()") ;
    print_status("parse_method");

    string vtype;
    string name;
    ast params;
    ast body;
    mustbe(tk_method);
    vtype=token_spelling(parse_vtype());
    name=token_spelling(mustbe(tk_identifier));
    LState.call_name=name;
    funclist.push_back(name);
    mustbe(tk_lrb);
    params=parse_param_list();
    mustbe(tk_rrb);
    mustbe(tk_lcb);
    body=parse_subr_body();
    mustbe(tk_rcb);
    LState.call_name="";
    pop_error_context() ;
    if(vtype=="")
        sys_error=true;
    return create_method(vtype,name,params,body);
}

// param_list ::= ((type identifier) (',' type identifier)*)?
// returns ast_param_list: create_param_list(vector<ast> params)
// create_param_list must be passed a vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
ast parse_param_list()
{
    push_error_context("parse_param_list()") ;
    print_status("parse_param_list");

    vector<ast> params;
    int count=0;
    
    while(have(tk_var) || 
        have(tk_int) || 
        have(tk_char) || 
        have(tk_boolean) || 
        have(tk_identifier)){
        ast dec;
        ast ast_var_dec;
        string name="";
        string type="var";
        string segment="";
        bool is_var=false;
        if(have(tk_var)){
            mustbe(tk_var);
            type="var";
            is_var=true;
        }
        if(have(tk_type)){
            type=token_spelling(mustbe(tk_type));
        }
        if(debug){ printf("parse_param_list:current_token:%s \n",token_spelling(current_token()).c_str()); }
            

        if(have(tk_identifier)){
            Token id= mustbe(tk_identifier);
            name=token_spelling(id);

            segment="argument";
            if(is_var)
                segment="local";


            dec = SymbolControl.declare_variable(id,"",segment,type,false);
            //dec = declare_variable(id,segment,type,-1);
            int offset=get_var_offset(dec);
            if(debug){ printf("parse_param_list:name:%s segment:%s type:%s  offset:%d\n",name.c_str(),segment.c_str(),type.c_str(),offset); }
            if(have(tk_comma))
                mustbe(tk_comma);

            ast_var_dec=create_var_dec(name,segment,offset,type);
            params.push_back(ast_var_dec);
        }
        count+=1;
    }

    pop_error_context() ;
    return create_param_list(params);
}

// subr_body ::= '{' var_decs statements '}'
// returns ast_subr_body: create_subr_body(ast decs,ast body)
// create_subr_body must be passed:
// . decs: ast_var_decs - the subroutine's local variable declarations
// . body: ast_statements - the statements within the body of the subroutinue
//
ast parse_subr_body()
{
    push_error_context("parse_subr_body()") ;
    print_status("parse_subr_body");

    ast decs;
    ast body;
    decs=parse_var_decs();
    if(have(tk_statement)){
        body=parse_statements();
    }
    pop_error_context() ;
    return create_subr_body(decs,body);
}

// var_decs ::= var_dec*
// returns ast_var_decs: create_var_decs(vector<ast> decs)
// create_var_decs must be passed a vector of ast_var_dec
//
ast parse_var_decs()
{
    vector<ast> decs;
    push_error_context("parse_var_decs()") ;
    print_status("parse_var_decs");
    string type="";
    while(have(tk_var) || have(tk_comma))
    {
        ast dec;
        ast ast_var_dec;
        
        if(have(tk_comma))
            mustbe(tk_comma);
        if(have(tk_var))
            mustbe(tk_var);
        if(have(tk_int)){
            type="int";
            mustbe(tk_int);
        }
        else if(have(tk_char)){
            type="char";
            mustbe(tk_char);
        }
        else if(have(tk_boolean)){
            type="boolean";
            mustbe(tk_boolean);
        }
        
        Token id;
        if(have(tk_identifier) && !have(tk_comma)){
            id= mustbe(tk_identifier);
            string name=token_spelling(id);

            VarNode Node=SymbolControl.find_node(name,"class","","");
            if((
                Node.is_class || 
                name==main_class_name || 
                !SymbolControl.find_variable(name,"","","")
                ) && 
                !have(tk_semi) && have(tk_identifier)){
                type=name;
                id= mustbe(tk_identifier);
                name=token_spelling(id);
            }
            string segment="local";
            dec = SymbolControl.declare_variable(id,"function",segment,type,false);
            int offset=get_var_offset(dec);
            ast_var_dec=create_var_dec(name,segment,offset,type);
            
            if(have(tk_semi))
                mustbe(tk_semi);
            
            decs.push_back(ast_var_dec) ;
        }
    }

    pop_error_context();
    return create_var_decs(decs);
}

// var_dec ::= 'var' type identifier (',' identifier)* ';'
// returns vector of ast_var_dec
//
// ast_var_dec: create_var_dec(string name,string segment,int offset,string type)
// create_var_dec must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
vector<ast> parse_var_dec()
{
    vector<ast> decs;
    push_error_context("parse_var_dec()") ;
    print_status("parse_var_dec");
    string name;
    string type;
    string segment;
    int offset;
    mustbe(tk_var);
    type= token_spelling(mustbe(tk_identifier));
    while(have(tk_comma)){
        mustbe(tk_comma);
        Token id= mustbe(tk_identifier);
        name=token_spelling(id);
        ast dec = SymbolControl.declare_variable(id,"class",segment,type,false);
        int offset=get_var_offset(dec);
        dec = create_var_dec(name,segment,offset,type);
        decs.push_back(dec);
    }
    mustbe(tk_semi);
    pop_error_context() ;
    return decs;
}


// statements ::= statement*
// create_statements(vector<ast> statements)
// create_statements must be passed a vector of ast_statement
//
ast parse_statements()
{
    vector<ast> statements;
    push_error_context("parse_statements()") ;
    print_status("parse_statements");

    while(have(tk_statement)){
        statements.push_back(parse_statement());
    }
    pop_error_context() ;
    return create_statements(statements);
}

// statement ::= let | if | while | do | return
// create_statement(ast statement)
// create_statement initialiser must be one of: ast_let, ast_let_array, ast_if, ast_if_else, ast_while, ast_do, ast_return or ast_return_expr
//
ast parse_statement()
{
    ast statement;
    push_error_context("parse_statement()") ;
    print_status("parse_statement");
    if(have(tk_statement)){
        ast ast_let, ast_let_array, ast_if, ast_if_else, ast_while, ast_do, ast_return, ast_return_expr;
        switch(token_kind()){
        case tk_let: 
            statement=parse_let();
        break;
        case tk_if:
            statement=parse_if();
        break;
        case tk_while:
            statement=parse_while();
        break;
        case tk_do:
            statement=parse_do();
        break;
        case tk_return:
            statement=parse_return();
            
        break;
        default:
            mustbe(tk_statement);
            break;
        }
    }
    pop_error_context() ;
    return create_statement(statement);
}

// let ::= 'let' identifier index? '=' expr ';'
// return one of: ast_let or ast_let_array
//
// create_let(ast var,ast expr)
// . var: ast_var - the variable being assigned to
// . expr: ast_expr - the variable's new value
//
// create_let_array(ast var,ast index,ast expr)
// . var: ast_var - the variable for the array being assigned to
// . index: ast_expr - the array index
// . expr: ast_expr - the array element's new value
//
ast parse_let(){
    bool b_array=false;
    push_error_context("parse_let()") ;
    print_status("parse_let");
    
    ast ast_var;
    ast ast_index;
    ast ast_expr;
    mustbe(tk_let);
    Token id= mustbe(tk_identifier);
    string name=token_spelling(id);
    string type="";

    VarNode Node;
    
    if(SymbolControl.find_variable(name,"class","","")){
        Node=SymbolControl.find_node(name,"class","","");
        if(debug){
            printf("parse_let(1):%s area:%s segment:%s\n",name.c_str(),"class",Node.segment.c_str());
        }
        ast_var=Node.var;
    }
    else if(SymbolControl.find_variable(name,"function","","")){
        Node=SymbolControl.find_node(name,"function","argument",l_call());
        if(Node.name==""){
            Node=SymbolControl.find_node(name,"function","local",l_call());
        }
        if(debug){
            printf("parse_let(2):%s area:%s segment:%s\n",name.c_str(),"function",Node.segment.c_str());
        }
        ast_var=Node.var;
    }
    else{
        if(debug){
            printf("parse_let(3):%s area:%s segment:%s\n",name.c_str(),"function","local");
        }
        ast_var=SymbolControl.declare_variable(
            id,
            "function",
            "local","int",false);
    }
    
    if(have(tk_lsb)){
        b_array=true;
        ast_index=parse_index();
    }
    mustbe(tk_eq);
    ast_expr=parse_expr();
    mustbe(tk_semi);
    pop_error_context();
    if(b_array){
        return create_let_array(ast_var,ast_index,ast_expr);
    }else{
        return create_let(ast_var,ast_expr);
    }
    
    sys_error=true;
    pop_error_context();
    return -1;
}

// if ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// return one of: ast_if or ast_if_else
//
// create_if(ast condition,ast if_true)
// . condition: ast_expr - the if condition
// . if_true: ast_statements - the if true branch
//
// create_if_else(ast condition,ast if_true,ast if_false)
// . condition: ast_expr - the if condition
// . if_true: ast_statements - the if true branch
// . if_false: ast_statements - the if false branch
//
ast parse_if()
{
    push_error_context("parse_if()") ;
    print_status("parse_if");
 
    if(have(tk_if)){
        mustbe(tk_if);

        mustbe(tk_lrb);
        ast condition=parse_expr();
        mustbe(tk_rrb);

        mustbe(tk_lcb);
        ast if_true=parse_statements();
        mustbe(tk_rcb);

        if(have(tk_else)){
            mustbe(tk_else);

            mustbe(tk_lcb);
            ast if_false=parse_statements();
            mustbe(tk_rcb);

            pop_error_context();
            return create_if_else(condition,if_true,if_false);
        }
        else{
            pop_error_context();
            return create_if(condition,if_true);
        }
    }
    sys_error=true;
    pop_error_context();
    return -1;
}

// while ::= 'while' '(' expr ')' '{' statements '}'
// returns ast_while: create_while(ast condition,ast body)
// . condition: ast_expr - the loop condition
// . body: ast_statements - the loop body
//
ast parse_while()
{
    ast ast_expr;
    ast ast_statements;
    push_error_context("parse_while()") ;
    print_status("parse_while");

    if(!have(tk_while)){
        pop_error_context() ;
        return create_while(ast_expr,ast_statements);
    }else{
        mustbe(tk_while);
        mustbe(tk_lrb);
        ast_expr=parse_expr();
        mustbe(tk_rrb);
        mustbe(tk_lcb);
        ast_statements=parse_statements();
        mustbe(tk_rcb);
        pop_error_context();
        return create_while(ast_expr,ast_statements);
    }
}

// do ::= 'do' identifier (call | id_call) ';'
// returns ast_do: create_do(ast call)
// create_do must be passed one of: ast_call_as_function or ast_call_as_method
//
// ast_call_as_function: create_call_as_function(string class_name,ast subr_call)
// create_ast_call_as_function must be passed:
// . class_name: name of the function's class
// . subr_call: ast_subr_call - the function's name within it's class and it's explicit arguments
//
// ast_call_as_method: create_call_as_method(string class_name,ast object,ast subr_call)
// create_ast_call_as_method must be passed:
// . class_name: name of the method's class
// . object: ast_expr - the object the method is applied to
// . subr_call: ast_subr_call - the method's name within it's class and it's explicit arguments
//
ast parse_do()
{
    string name;
    ast object;
    ast subr_call;
    push_error_context("parse_do()") ;
    print_status("parse_do");

    mustbe(tk_do);
    name=token_spelling(mustbe(tk_identifier));

    VarNode Node;
    Node=SymbolControl.find_node(name,"class","","");
    if(Node.name==""){
        Node=SymbolControl.find_node(name,"function","argument",l_call());
        if(Node.name==""){
            Node=SymbolControl.find_node(name,"function","local",l_call());
        }
    }
    if(debug){ printf("parse_do:create_call_as_method: get_name:%s name:%s type:%s is_declared:%s is_func:%s\n",
        name.c_str(),Node.name.c_str(),Node.var_type.c_str(),(is_declared(Node))?"T":"F",(is_func(name))?"T":"F" ); }
    bool call_method=false;
    if(is_declared(Node)){
        call_method=true;
    }
    if(!is_declared(Node) && !is_func(Node.name) && have(tk_lrb)){
        call_method=true;
    }
    
    if(call_method){
        string class_name="";
        class_name=Node.var_type;
        if(class_name==""){
            class_name=main_class_name;
            object=create_this();
            string subr_name=name;
            if(have(tk_lrb)){
                mustbe(tk_lrb);
            }
            ast expr_list=parse_expr_list();
            if(have(tk_rrb)){
                mustbe(tk_rrb);
            }
            subr_call= create_subr_call(subr_name,expr_list);
        }else{
            object=Node.var;
            if(have(tk_stop)){
                subr_call=parse_id_call();
            }else{
                subr_call=parse_call();
            }
        }
        if(debug){ printf("parse_do:create_call_as_method: name:%s \n",name.c_str()); }
        mustbe(tk_semi);

        pop_error_context();
        return create_do(create_call_as_method(class_name,object,subr_call));
    }else{
        ast subr_call;
        if(have(tk_stop)){
            subr_call=parse_id_call();
        }else{
            subr_call=parse_call();
        }
        mustbe(tk_semi);
        pop_error_context() ;
        if(debug){ printf("parse_do:create_call_as_function: name:%s \n",name.c_str()); }
        return create_do(create_call_as_function(name,subr_call));

    }
            
            
    sys_error=true;
}

// return ::= 'return' expr? ';'
// returns one of: ast_return or ast_return_expr
//
// ast_return: create_return()
//
// ast_return_expr: create_return_expr(ast expr)
// create_return_expr must be passed an ast_expr
//
ast parse_return()
{
    push_error_context("parse_return()") ;
    print_status("parse_return");
 
    ast expr;
    if(have(tk_return)){
        mustbe(tk_return);
        if(have(tk_semi)){
            mustbe(tk_semi);
            pop_error_context();
            return create_return();
        }
        else{
            expr=parse_expr();
            pop_error_context();
            mustbe(tk_semi);
            return create_return_expr(expr);
        }
    }
    return -1;
}

// expr ::= term (infix_op term)*
// returns ast_expr: create_expr(vector<ast> expr)
// the vector passed to create_expr:
// . must be an odd length of at least 1, ie 1, 3, 5, ...
// . the odd positions must contain an ast_term
// . the even positions must contain an ast_infix_op
//
ast parse_expr()
{
    vector<ast> expr;
    ast ast_expr;
    push_error_context("parse_expr()") ;
    print_status("parse_expr");
 
    int count=0;
    while((have(tk_term) || have(tk_infix_op)) ){
        if(have(tk_term) && count%2==0){
            if(debug){printf("parse_expr:parse_term count:%d\n",count);}
            ast_expr=parse_term();
            expr.push_back(ast_expr);
        }
        else if(have(tk_infix_op))
        {
            if(debug){printf("parse_expr:parse_infix_op count:%d\n",count);}
            ast_expr=parse_infix_op();
            expr.push_back(ast_expr);
        }
        count+=1;
    }
    pop_error_context() ;
    return create_expr(expr);    
}

// term ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// returns ast_term: create_term(ast term)
// create_term must be passed one of: ast_int, ast_string, ast_bool, ast_null, ast_this, ast_expr,
//                                    ast_unary_op, ast_var, ast_array_index, ast_call_as_function, ast_call_as_method
//
// ast_int: create_int(int _constant)
// create_int must be passed an integer value in the range 0 to 32767
//
// ast_string: create_string(string _constant)
// create_string must be passed any legal Jack string literal
//
// ast_bool: create_bool(bool t_or_f)
// create_bool must be passed true or false
//
// ast_null: create_null()
//
// ast_this: create_this()
//
// ast_unary_op: create_unary_op(string op,ast term)
// create_unary_op must be passed:
// . op: the unary op
// . term: ast_term
//
ast parse_term()
{
    ast ast_term;
    push_error_context("parse_term()");
    print_status("parse_term");
    
    if(have(tk_identifier)){
        if(debug){printf("parse_term:tk_identifier\n");}
        ast_term=parse_var_term();
    }
    else if(have(tk_integerConstant)){
        if(debug){printf("parse_term:tk_integerConstant\n");}
        Token id= mustbe(tk_integerConstant);
        string name=token_spelling(id);
        ast_term=create_int(atoi(name.c_str()));
    }
    else if(have(tk_stringConstant)){
        if(debug){printf("parse_term:tk_stringConstant\n");}
        Token id= mustbe(tk_stringConstant);
        string name=token_spelling(id);
        ast_term=create_string(name);
    }
    else if(have(tk_sub) || have(tk_not)){
        if(debug){printf("parse_term:have(tk_sub) || have(tk_not)\n");}
        Token op=parse_unary_op();
        string name=token_spelling(op);
        ast_term=create_unary_op(name,parse_term());
    }
    else if(have(tk_true)){
        if(debug){printf("parse_term:have(tk_true)\n");}
        string boolStr=token_spelling(mustbe(tk_true));
        if(boolStr=="true"){
            ast_term=create_bool(true);
        }
    }
    else if(have(tk_false)){
        if(debug){printf("parse_term:have(tk_false)\n");}
        string boolStr=token_spelling(mustbe(tk_false));
        if(boolStr=="false"){
            ast_term=create_bool(false);
        }
    }
    else if(have(tk_null)){
        if(debug){printf("parse_term:have(tk_null)\n");}
        mustbe(tk_null);
        ast_term=create_null();
    }
    else if(have(tk_this)){
        if(debug){printf("parse_term:have(tk_this)\n");}
        mustbe(tk_this);
        ast_term=create_this();
    }
    else if(have(tk_lrb)){
        if(debug){printf("parse_term:have(tk_lrb)n");}
        mustbe(tk_lrb);
        ast_term=parse_expr();
        if(have(tk_rrb))
            mustbe(tk_rrb);
    }
    pop_error_context();
    return create_term(ast_term); ;
}

// var_term ::= identifier (index | id_call | call)?
// returns one of: ast_var, ast_array_index, ast_call_as_function or ast_call_as_method
//
// ast_var: create_var(string name,string segment,int offset,string type)
// create_ast_var must be passed:
// . name: the variable's name
// . segment: the variable's segment
// . offset: the variable's position in it's segment
// . type: the variable's type
//
// ast_array_index: create_array_index(ast var,ast index)
// create_ast_array_index must be passed:
// . var: ast_var - the array variable
// . index: ast_expr - the index into the array
//
// ast_call_as_function: create_call_as_function(string class_name,ast subr_call)
// create_ast_call_as_function must be passed:
// . class_name: name of the function's class
// . subr_call: ast_subr_call - the function's name within it's class and it's explicit arguments
//
// ast_call_as_method: create_call_as_method(string class_name,ast object,ast subr_call)
// create_ast_call_as_method must be passed:
// . class_name: name of the method's class
// . object: ast_expr - the object the method is applied to
// . subr_call: ast_subr_call - the method's name within it's class and it's explicit arguments
//
ast parse_var_term()
{
    push_error_context("parse_var_term()") ;
    print_status("parse_var_term");
    
    ast ast_term;
    Token id= mustbe(tk_identifier);
    string name=token_spelling(id);
    VarNode Node;
    Node=SymbolControl.find_node(name,"class","","");
    if(Node.name==""){
        Node=SymbolControl.find_node(name,"function","argument",l_call());
        if(Node.name==""){
            Node=SymbolControl.find_node(name,"function","local",l_call());
        }
    }

    //printf("%s\n", name.c_str());
    string ctype=Node.var_type;
    if(debug){ 
        printf("parse_var_term: name:%s call:%s type:%s\n",name.c_str(),l_call().c_str(),ctype.c_str()); 
        Node.print();
    }

    if((main_class_name==name || 
        !SymbolControl.find_variable(name,"","","")) && have(tk_stop)){
        if(debug){printf("parse_var_term:main_class_name==name && have(tk_stop)\n");}
        if(debug){ printf("create_call_as_function: Name:%s  type:%s\n",name.c_str(),ctype.c_str()); }

        ast subr_call;
        mustbe(tk_stop);
        string fname=token_spelling(mustbe(tk_identifier));
        mustbe(tk_lrb);
        ast expr_list=parse_expr_list();
        mustbe(tk_rrb);
        if(debug){ printf("create_subr_call: fname:%s \n",fname.c_str()); }
        subr_call= create_subr_call(fname,expr_list);
        ast_term=create_call_as_function(name,subr_call);
    }
    else if(
        ctype=="" && have(tk_lrb) 
        ){
        if(debug){printf("ctype=='' && have(tk_lrb)\n");}
        
        ast object=create_this();

        if(debug){ printf("create_call_as_method: Name:%s  type:%s\n",name.c_str(),ctype.c_str()); }

        mustbe(tk_lrb);
        ast expr_list=parse_expr_list();
        mustbe(tk_rrb);
        if(debug){ printf("create_subr_call: fname:%s \n",name.c_str()); }
        ast subr_call= create_subr_call(name,expr_list);
        ast_term=create_call_as_method(main_class_name,object,subr_call);
    }
    else if((is_declared(Node) && !is_func(Node.name) && have(tk_stop))){
        if(debug){printf("parse_var_term:(is_declared(Node) && !is_func(Node.name)\n");}

        string classname=ctype;
        ast object=Node.var;

        mustbe(tk_stop);

        string fname=token_spelling(mustbe(tk_identifier));
        if(debug){ 
            printf("create_call_as_method: Name:%s fname:%s type:%s\n",
                name.c_str(),fname.c_str(),ctype.c_str()); 
        }

        mustbe(tk_lrb);
        ast expr_list=parse_expr_list();
        mustbe(tk_rrb);

        if(debug){ printf("create_subr_call: fname:%s \n",fname.c_str()); }
        ast subr_call= create_subr_call(fname,expr_list);
        ast_term=create_call_as_method(classname,object,subr_call);
    }
    else if(
        (SymbolControl.find_variable(ctype,"class","","") || 
        main_class_name==ctype)
        && ctype!="" && have(tk_stop)){
        if(debug){printf("parse_var_term:SymbolControl.find_variable\n");}

        string classname=ctype;
        ast object=Node.var;

        
        mustbe(tk_stop);

        string fname=token_spelling(mustbe(tk_identifier));
        if(debug){ 
            printf("create_call_as_method: Name:%s fname:%s type:%s\n",
                name.c_str(),fname.c_str(),ctype.c_str()); 
        }

        mustbe(tk_lrb);
        ast expr_list=parse_expr_list();
        mustbe(tk_rrb);

        if(debug){ printf("create_subr_call: fname:%s \n",fname.c_str()); }
        ast subr_call= create_subr_call(fname,expr_list);
        ast_term=create_call_as_method(classname,object,subr_call);
    }
    else if(have(tk_stop)){
        if(debug){printf("parse_var_term:have(tk_stop)\n");}
        ast_term=parse_id_call();
    }
    else if(have(tk_lrb)){
        if(debug){printf("parse_var_term:have(tk_lrb)\n");}
        ast_term=parse_call();
    }
    else if(have(tk_lsb)){
        if(debug){printf("parse_var_term:have(tk_lsb)\n");}
        ast var;
        if(SymbolControl.find_variable(name,"class","","")){
            var= SymbolControl.find_node(name,"class","","").var;
        }
        else{
            var= SymbolControl.find_node(name,"function","","").var;
        }
        ast index=parse_index();
        ast_term=create_array_index(var,index);
    }
    else{
        if(debug){
            printf("parse_var_term:create_var:name:%s segment:%s offset:%d\n",
                Node.name.c_str(),
                Node.segment.c_str(),
                Node.offset);
        }
        ast_term=Node.var;      
    }
    pop_error_context() ;
    return ast_term ;
}

// index ::= '[' expr ']'
// returns ast_expr
ast parse_index()
{
    ast ast_expr;
    push_error_context("parse_index()") ;
    print_status("parse_index");
    
    mustbe(tk_lsb);
    ast_expr=parse_expr();
    mustbe(tk_rsb);
    pop_error_context() ;
    return ast_expr;
}

// id_call ::= '.' identifier call
// returns ast_subr_call: create_subr_call(string subr_name,ast expr_list)
// create_subr_call must be passed:
// . subr_name: the constructor, function or method's name within its class
// . expr_list: ast_expr_list - the explicit arguments to the call
//
ast parse_id_call()
{
    string subr_name;
    ast expr_list;
    push_error_context("parse_id_call()") ;
    print_status("parse_id_call");
    
    mustbe(tk_stop);
    subr_name=token_spelling(mustbe(tk_identifier));
    mustbe(tk_lrb);
    expr_list=parse_expr_list();
    mustbe(tk_rrb);
    pop_error_context() ;
    return create_subr_call(subr_name,expr_list);
}

// call ::= '(' expr_list ')'
// returns ast_expr_list
//
ast parse_call()
{
    ast expr_list;
    push_error_context("parse_call()") ;
    print_status("parse_call");
    mustbe(tk_lrb);
    expr_list=parse_expr_list();
    mustbe(tk_rrb);
    pop_error_context() ;
    return expr_list;
}

// expr_list ::= (expr (',' expr)*)?
// returns ast_expr_list: create_expr_list(vector<ast> exprs)
// create_expr_list must be passed: a vector of ast_expr
//
ast parse_expr_list()
{
    vector<ast> exprs;
    push_error_context("parse_expr_list()") ;
    print_status("parse_call");

    int count=0;
    bool next=true;
    if(have(tk_term)){
        do{
            if(debug){ printf("parse_expr_list:parse_expr\n"); }
            ast expr=parse_expr();
            exprs.push_back(expr);
            if(have(tk_comma))
                mustbe(tk_comma);
            else
                next=false;
            count+=1;
        }while(next);
    }
    pop_error_context() ;
    return create_expr_list(exprs);
}

// infix_op ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// returns ast_op: create_op(string infix_op)
// create_op must be passed:
// infix_op: the infix op
//
ast parse_infix_op()
{
    string infix_op="";
    push_error_context("parse_infix_op()") ;
    print_status("parse_infix_op");
    if(have(tk_infix_op)){
        if(have(tk_add)){
            mustbe(tk_add);
            infix_op="+";
        }
        if(have(tk_sub)){
            mustbe(tk_sub);
            infix_op="-";
        }
        if(have(tk_times)){
            mustbe(tk_times);
            infix_op="*";
        }
        if(have(tk_divide)){
            mustbe(tk_divide);
            infix_op="/";
        }
        if(have(tk_and)){
            mustbe(tk_and);
            infix_op="&";
        }
        if(have(tk_or)){
            mustbe(tk_or);
            infix_op="|";
        }
        if(have(tk_lt)){
            mustbe(tk_lt);
            infix_op="<";
        }
        if(have(tk_gt)){
            mustbe(tk_gt);
            infix_op=">";
        }
        if(have(tk_eq)){
            mustbe(tk_eq);
            infix_op="=";
        }
    }
    pop_error_context() ;
    return create_infix_op(infix_op);
}

// unary_op ::= '-' | '~'
// returns Token for the unary_op
//
Token parse_unary_op()
{
    push_error_context("parse_unary_op()") ;
    print_status("parse_unary_op");
    if(have(tk_sub)){
        return mustbe(tk_sub);
    }
    else{
        return mustbe(tk_not);
    }

}

ast jack_parser()
{
    // read the first token to get the tokeniser initialised
    next_token() ;
    symbol_class_table = create_ints() ;
    symbol_function_table = create_ints();
    // construct tree and return as result
    return parse_class() ;
}

// main program
int main(int argc,char **argv)
{
    
    // parse a Jack class and print the abstract syntax tree as XML
    ast_print_as_xml(jack_parser(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}
