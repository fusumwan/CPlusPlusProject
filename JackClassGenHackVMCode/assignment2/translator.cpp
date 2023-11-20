// convert a document tree into VM code
#include "iobuffer.h"
#include "abstract-syntax-tree.h"
#include "assembler-internal.h"
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
// to make out programs a bit neater
using namespace std ;

using namespace CS_IO_Buffers ;
using namespace Hack_Virtual_Machine ;

// grammer to be parsed:
// vm_class ::= command* eoi
// command ::= vm_operator | vm_jump | vm_func | vm_stack
// vm_operator ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
// vm_jump ::= jump label
// vm_func ::= func label number
// vm_stack ::= stack segment number

// forward declare parsing functions - one per rule
static void translate_vm_class(ast root) ;
static void translate_vm_command(ast command) ;
static void translate_vm_operator(ast vm_op) ;
static void translate_vm_jump(ast jump) ;
static void translate_vm_func(ast func) ;
static void translate_vm_stack(ast stack) ;



////////////////////////////////////////////////////////////////

/************   PUT YOUR HELPER FUNCTIONS HERE   **************/




static vector<string> split(string str, string token){
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


class StateManager{
public:
	string class_name;
	string func_name;
	string label;
	string command;
	int return_line;
	std::vector<string> labellist;
	StateManager(){
		class_name="";
		func_name="";
		label="";
		command="";
		return_line=1;
	}

	template <typename T>
	string to_string(T value)
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}

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
	int indexof(std::string text, std::string pattern)
	{
		// where appears the pattern in the text?
		std::string::size_type loc = text.find(pattern, 0);
		if(loc != std::string::npos) 
		{
			return loc;
		} 
		else 
		{
			return -1;
		}
	}
	string replace(string str, std::string pattern, std::string replace_str){
		int replace_index=-1;
		replace_index=indexof(str,pattern);
		if(replace_index>-1){
			vector<string> str_arr=split(str,pattern);
			string tmp=str_arr[0]+replace_str+str_arr[1];
			str=tmp;
		}
		return str;
	}
	string remove_ch(string str,char remove){
		string result="";
		for(int i=0;i<str.length();i++){
			if(str[i]!='\n'){
				result+=str[i];
			}
		}
		return result;
	}
	void set_command(string cmd){
		command=cmd;
	}
	string get_classfunc(){
		return class_name+"."+func_name;
	}
	void set_label(string l){
		if(indexof(l,".")>-1){
			vector<string> cf=split(l,".");
			class_name=cf[0];
			func_name=cf[1];
		}
		label=l;
	}
	string get_label(){
		return label;
	}
	void set_class(string c){
		class_name=c;
	}
	string get_class_name(){
		return class_name;
	}
	void add_label(string str){
		labellist.push_back(str);
	}
	int find_label(string str){
		for(int i=0;i<(int)labellist.size();i++){
			if(labellist[i]==str){
				return i;
			}
		}
		return -1;
	}
};

static StateManager state;


/************      END OF HELPER FUNCTIONS       **************/

///////////////////////////////////////////////////////////////



// the function translate_vm_class() will be called by the main program
// its is passed the abstract syntax tree constructed by the parser
// it walks the abstract syntax tree and produces the equivalent VM code as output
static void translate_vm_class(ast root)
{
	// assumes we have a "class" node containing VM command nodes
	ast_mustbe_kind(root,ast_vm_class) ;

	// tell the output system we are starting to translate VM commands for a Jack class
	start_of_vm_class();

	// translate the commands in the order they were added to the class
	int ncommands = size_of_vm_class(root) ;
	for ( int i = 0 ; i < ncommands ; i++ )
	{
		translate_vm_command(get_vm_class(root,i)) ;
	}

	// tell the output system we have just finished translating VM commands for a Jack class
	end_of_vm_class() ;

}

// translate the current vm command - a bad node is a fatal error
static void translate_vm_command(ast command)
{
	switch(ast_node_kind(command))
	{
	case ast_vm_operator:
		translate_vm_operator(command) ;
		break ;
	case ast_vm_jump:
		translate_vm_jump(command) ;
		break ;
	case ast_vm_function:
		translate_vm_func(command) ;
		break ;
	case ast_vm_stack:
		translate_vm_stack(command) ;
		break ;
	default:
		fatal_error(0,"// bad node - expected vm_operator, vm_jump, vm_function or vm_stack\n") ;
		break ;
	}
}

// translate vm operator command into assembly language
static void translate_vm_operator(ast vm_op)
{
	// extract command specific info from the ast node passed in
	string command = get_vm_operator_command(vm_op) ;

	// tell the output system what kind of VM command we are now trying to implement
	start_of_vm_operator_command(command) ;

	/************   ADD CODE BETWEEN HERE   **************/

	// use the output_assembler() function to implement this VM command in Hack Assembler
	// careful use of helper functions you can define above will keep your code simple
	// ...
	if(command=="add"){
		output_assembler("// "+command) ;
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("A=A-1");
		output_assembler("M=D+M");
	}
	else if(command=="sub"){
		output_assembler("// "+command) ;
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("A=A-1");
		output_assembler("M=M-D");
	}
	else if(command=="and"){
		output_assembler("// "+command) ;

		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("M=D&M");
		output_assembler("@SP");
		output_assembler("M=M+1");

	}
	else if(command=="eq"){
		output_assembler("// "+command) ;
		int true_line=state.return_line;
		state.return_line+=1;
		int false_line=state.return_line;
		state.return_line+=1;
		int end_line=state.return_line;
		state.return_line+=1;

		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M-D");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(true_line)+"_TRUE");
		output_assembler("D;JEQ");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(false_line)+"_FALSE");
		output_assembler("0;JMP");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(true_line)+"_TRUE)");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=-1");
		output_assembler("@SP");
		output_assembler("M=M+1");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(end_line)+"_END");
		output_assembler("0;JMP");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(false_line)+"_FALSE)");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=0");
		output_assembler("@SP");
		output_assembler("M=M+1");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(end_line)+"_END)");
	}
	else if(command=="gt"){
		output_assembler("// "+command) ;
		int true_line=state.return_line;
		state.return_line+=1;
		int false_line=state.return_line;
		state.return_line+=1;
		int end_line=state.return_line;
		state.return_line+=1;
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M-D");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(true_line)+"_TRUE");
		output_assembler("D;JGT");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(false_line)+"_FALSE");
		output_assembler("0;JMP");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(true_line)+"_TRUE)");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=-1");
		output_assembler("@SP");
		output_assembler("M=M+1");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(end_line)+"_END");
		output_assembler("0;JMP");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(false_line)+"_FALSE)");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=0");
		output_assembler("@SP");
		output_assembler("M=M+1");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(end_line)+"_END)");
	}
	else if(command=="lt"){
		output_assembler("// "+command);
		int true_line=state.return_line;
		state.return_line+=1;
		int false_line=state.return_line;
		state.return_line+=1;
		int end_line=state.return_line;
		state.return_line+=1;

		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M-D");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(true_line)+"_TRUE");
		output_assembler("D;JLT");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(false_line)+"_FALSE");
		output_assembler("0;JMP");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(true_line)+"_TRUE)");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=-1");
		output_assembler("@SP");
		output_assembler("M=M+1");
		output_assembler("@"+state.get_classfunc()+"$COMP_"+to_string(end_line)+"_END");
		output_assembler("0;JMP");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(false_line)+"_FALSE)");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=0");
		output_assembler("@SP");
		output_assembler("M=M+1");
		output_assembler("("+state.get_classfunc()+"$COMP_"+to_string(end_line)+"_END)");
	}
	else if(command=="neg"){
		output_assembler("// "+command);
		output_assembler("@SP");
		output_assembler("A=M-1");
		output_assembler("M=-M");

	}
	else if(command=="not"){
		output_assembler("// "+command);
		output_assembler("@SP");
		output_assembler("A=M-1");
		output_assembler("M=!M");

		
	}
	else if(command=="or"){
		output_assembler("// "+command);
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("M=D|M");
		output_assembler("@SP");
		output_assembler("M=M+1");
	}
	else if(command=="return"){
		output_assembler("// "+command);
		// FRAME = LCL
		output_assembler("@LCL");
		output_assembler("D=M");
		output_assembler("@5");
		output_assembler("A=D-A");
		output_assembler("D=M");
		output_assembler("@R13");
		output_assembler("M=D");    // FRAME = LCL

		
		//*ARG = pop
		output_assembler("@SP");
		output_assembler("A=M-1");
		output_assembler("D=M");
		output_assembler("@ARG");
		output_assembler("A=M");
		output_assembler("M=D");


		//SP=ARG+1
		output_assembler("@ARG");
		output_assembler("D=M+1");
		output_assembler("@SP");
		output_assembler("M=D");

		//THAT=*(LCL-1)
		output_assembler("@LCL");
		output_assembler("A=M-1");
		output_assembler("D=M");
		output_assembler("@THAT");
		output_assembler("M=D");

		//THIS = *(LCL-2)
		output_assembler("@LCL");
		output_assembler("D=M");
		output_assembler("@2");
		output_assembler("A=D-A");
		output_assembler("D=M");
		output_assembler("@THIS");
		output_assembler("M=D");

		//ARG = *(LCL-3)
		output_assembler("@LCL");
		output_assembler("D=M");
		output_assembler("@3");
		output_assembler("A=D-A");
		output_assembler("D=M");
		output_assembler("@ARG");
		output_assembler("M=D");

		//LCL = *(LCL-4)
		output_assembler("@LCL");
		output_assembler("D=M");
		output_assembler("@4");
		output_assembler("A=D-A");
		output_assembler("D=M");
		output_assembler("@LCL");
		output_assembler("M=D");

		//goto retAddr
		output_assembler("@R13");
		output_assembler("A=M");
		output_assembler("0;JMP");
		
	}

	/************         AND HERE          **************/

	// tell the output system that we have just finished trying to implement a VM command
	end_of_vm_command() ;
}

// translate vm operator command into assembly language
static void translate_vm_jump(ast jump)
{
	// extract command specific info from the ast node passed in
	string command = get_vm_jump_command(jump) ;
	string label = get_vm_jump_label(jump) ;

	// tell the output system what kind of VM command we are now trying to implement
	start_of_vm_jump_command(command,label) ;

	/************   ADD CODE BETWEEN HERE   **************/

	// use the output_assembler() function to implement this VM command in Hack Assembler
	// careful use of helper functions you can define above will keep your code simple
	// ...
	if(command=="label"){
		output_assembler("("+state.get_classfunc()+"$"+label+")");
	}
	else if(command=="if-goto"){
		output_assembler("// "+command);
		int return_line=state.return_line;

		output_assembler("@SP");
		output_assembler("AM=M-1");
		output_assembler("D=M");
		output_assembler("@"+state.get_classfunc()+"$"+label);
		output_assembler("D;JNE");
	}
	else if(command=="goto"){
		output_assembler("// "+command);
		output_assembler("@"+state.get_classfunc()+"$"+label);
		output_assembler("0;JMP");
	}

	/************         AND HERE          **************/

	// tell the output system that we have just finished trying to implement a VM command
	end_of_vm_command() ;
}

// translate vm operator command into assembly language
static void translate_vm_func(ast func)
{
	// extract command specific info from the ast node passed in
	string command = get_vm_function_command(func) ;
	string label = get_vm_function_label(func) ;

	
	int number = get_vm_function_number(func) ;

	

	// tell the output system what kind of VM command we are now trying to implement
	start_of_vm_func_command(command,label,number) ;

	/************   ADD CODE BETWEEN HERE   **************/

	// use the output_assembler() function to implement this VM command in Hack Assembler
	// careful use of helper functions you can define above will keep your code simple
	// ...
	if ( command == "function")
	{
		state.set_label(label);
		state.set_command(command);
		//output_assembler("// "+command+" "+to_string(number)) ;
		output_assembler("// "+command+" "+label+" "+to_string(number)) ;
		output_assembler("("+label+")") ;

		for(int i=0;i<number;i++){
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("M=0");
			output_assembler("@SP");
			output_assembler("M=M+1");
		}
		
	}
	else if( command == "call"){
		output_assembler("// "+command+" "+label+" "+to_string(number)) ;
		int return_line=state.return_line;
		
		//push return address
		string return_label="";

		if(state.find_label(return_label)==-1){
			return_label=state.get_classfunc()+"$RETURN_ADDRESS_"+to_string(return_line);
			state.add_label(label);
			state.return_line+=1;
		}

		output_assembler("@"+return_label);
		// push return-address
		output_assembler("D=A");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=D");
		output_assembler("@SP");
		output_assembler("M=M+1");  // push return-address
		// push LCL
		output_assembler("@LCL");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=D");
		output_assembler("@SP");
		output_assembler("M=M+1");  // push LCL
		// push ARG
		output_assembler("@ARG");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=D");
		output_assembler("@SP");
		output_assembler("M=M+1");  // push ARG
		// push THIS
		output_assembler("@THIS");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=D");
		output_assembler("@SP");
		output_assembler("M=M+1");  // push THIS
		// push THAT
		output_assembler("@THAT");
		output_assembler("D=M");
		output_assembler("@SP");
		output_assembler("A=M");
		output_assembler("M=D");
		output_assembler("@SP");
		output_assembler("M=M+1");  // push THAT
		
		// ARG = SP-n-5
		output_assembler("@SP");
		output_assembler("D=M");
		output_assembler("@"+to_string(number));
		output_assembler("D=D-A");
		output_assembler("@5");
		output_assembler("D=D-A");
		output_assembler("@ARG");
		output_assembler("M=D");

		// LCL = SP
		output_assembler("@SP");
		output_assembler("D=M");
		output_assembler("@LCL");
		output_assembler("M=D");    // LCL = SP
		output_assembler("@"+label);
		output_assembler("0;JMP");
		
		output_assembler("("+return_label+")");
	}
	/************         AND HERE          **************/

	// tell the output system that we have just finished trying to implement a VM command
	end_of_vm_command() ;
}

// translate vm operator command into assembly language
static void translate_vm_stack(ast stack)
{
	// extract command specific info from the ast node passed in
	string command = get_vm_stack_command(stack) ;
	string segment = get_vm_stack_segment(stack) ;
	int number = get_vm_stack_offset(stack) ;

	// tell the output system what kind of VM command we are now trying to implement
	start_of_vm_stack_command(command,segment,number) ;

	/************   ADD CODE BETWEEN HERE   **************/

	// use the output_assembler() function to implement this VM command in Hack Assembler
	// careful use of helper functions you can define above will keep your code simple
	// ...
	if ( command == "push")
	{
		if(segment=="static" && state.get_label()!=""){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@"+state.get_class_name()+"."+to_string(number));
			output_assembler("D=M");
			output_assembler("@SP");
			output_assembler("AM=M+1");
			output_assembler("A=A-1");
			output_assembler("M=D");
		}
		else if(segment=="local"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@LCL");
			output_assembler("D=M");
			output_assembler("@13");
			output_assembler("M=D");
			output_assembler("@"+to_string(number));
			output_assembler("D=A");

			int loop_line=state.return_line;
			state.return_line+=1;
			int end_line=state.return_line;
			state.return_line+=1;
			output_assembler("("+state.get_classfunc()+"$PUSH_LCL_"+to_string(loop_line)+"_LOOP)");
			output_assembler("D=D-1");
			output_assembler("@"+state.get_classfunc()+"$PUSH_LCL_"+to_string(end_line)+"_END");
			output_assembler("D;JLT");
			output_assembler("@R13");
			output_assembler("M=M+1");
			output_assembler("@"+state.get_classfunc()+"$PUSH_LCL_"+to_string(loop_line)+"_LOOP");
			output_assembler("D;JGE");
			output_assembler("("+state.get_classfunc()+"$PUSH_LCL_"+to_string(end_line)+"_END)");
			output_assembler("@R13");
			output_assembler("A=M");
			output_assembler("D=M");
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("M=M+1");
		}
		else if(segment=="argument"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			int loop_line=state.return_line;
			state.return_line+=1;
			int end_line=state.return_line;
			state.return_line+=1;
			
			output_assembler("@"+to_string(number));
			output_assembler("D=A");
			output_assembler("@ARG");
			output_assembler("A=M");
			output_assembler("D=D+A");
			output_assembler("A=D");
			output_assembler("D=M");
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("M=M+1");
		}
		else if(segment=="pointer"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@R"+to_string(number+3));
			output_assembler("D=M");
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("M=M+1");
		}
		else if(segment=="temp"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@R"+to_string(number+5));
			output_assembler("D=M");
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("M=M+1");
		}
		else if(segment=="this"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));

			output_assembler("@"+to_string(number));
			output_assembler("D=A");
			output_assembler("@THIS");
			output_assembler("A=M");
			output_assembler("D=D+A");
			output_assembler("A=D");
			output_assembler("D=M");
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("M=M+1");
		}
		else if(segment=="that"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));

			output_assembler("@"+to_string(number));
			output_assembler("D=A");
			output_assembler("@THAT");
			output_assembler("A=M");
			output_assembler("D=D+A");
			output_assembler("A=D");
			output_assembler("D=M");
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("M=M+1");
		}
		else if(segment=="constant"){
			output_assembler("// "+command+"XXX "+segment+" "+to_string(number));
			output_assembler("@"+to_string(number));
			output_assembler("D=A");
			output_assembler("@SP");
			output_assembler("AM=M+1");
			output_assembler("A=A-1");
			output_assembler("M=D");
		}
	}
	else if(command=="pop"){
		if(segment=="temp"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@SP");
			output_assembler("AM=M-1");
			output_assembler("D=M");
			output_assembler("@R"+to_string(number+5));
			output_assembler("M=D");
		}
		else if(segment=="static"){

			int loop_line=state.return_line;
			state.return_line+=1;
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			
			output_assembler("@SP");
			output_assembler("M=M-1");
			output_assembler("@SP");
			output_assembler("A=M");
			output_assembler("D=M");
			output_assembler("@"+state.get_class_name()+"."+to_string(number));
			output_assembler("M=D");
		}
		else if(segment=="local"){
			
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@LCL");
			output_assembler("D=M");
			output_assembler("@"+to_string(number));
			output_assembler("D=D+A");
			output_assembler("@R13");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("AM=M-1");
			output_assembler("D=M");
			output_assembler("@R13");
			output_assembler("A=M");
			output_assembler("M=D");
			
		}
		else if(segment=="argument"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			
			
			output_assembler("@ARG");
			output_assembler("D=M");
			output_assembler("@"+to_string(number));
			output_assembler("D=D+A");
			output_assembler("@R13");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("AM=M-1");
			output_assembler("D=M");
			output_assembler("@R13");
			output_assembler("A=M");
			output_assembler("M=D");
		}
		else if(segment=="pointer"){
			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@SP");
			output_assembler("AM=M-1");
			output_assembler("D=M");
			output_assembler("@R"+to_string(number+3));
			output_assembler("M=D");
		}
		else if(segment=="that"){

			output_assembler("// "+command+" "+segment+" "+to_string(number));
			output_assembler("@THAT");
			output_assembler("D=M");
			output_assembler("@"+to_string(number));
			output_assembler("D=D+A");
			output_assembler("@R13");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("AM=M-1");
			output_assembler("D=M");
			output_assembler("@R13");
			output_assembler("A=M");
			output_assembler("M=D");


		}
		else if(segment=="this"){


			output_assembler("@THIS");
			output_assembler("D=M");
			output_assembler("@"+to_string(number));
			output_assembler("D=D+A");
			output_assembler("@R13");
			output_assembler("M=D");
			output_assembler("@SP");
			output_assembler("AM=M-1");
			output_assembler("D=M");
			output_assembler("@R13");
			output_assembler("A=M");
			output_assembler("M=D");

		}
	}

	/************         AND HERE          **************/

	// tell the output system that we have just finished trying to implement a VM command
	end_of_vm_command() ;
}

// main program
int main(int argc,char **argv)
{
	// parse abstract syntax tree and pass to the translator
	translate_vm_class(ast_parse_xml()) ;

	// flush output and errors
	print_output() ;
	print_errors() ;
}
