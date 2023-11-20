// a skeleton implementation of a tokeniser

#include "tokeniser.h"
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

// to shorten the code
using namespace std ;

////////////////////////////////////////////////////////////////////////
template <typename T>
string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}


namespace Assignment_Tokeniser
{
	// the current input character, initiliased to ' ' which we ignore
	// the eof marker can be confused with a legal character but not one accepted by our tokeniser
	static char ch = ' ' ;

	// the current line number and column, initialised to line 1 column 0
	static int line_num = 1 ;
	static int column_num = 0 ;

	// the line number and column for the first character in the current token
	static int start_line = 0 ;
	static int start_column = 0 ;
	static string read_str="";
	static string total_input="";
	static vector<string> inputs;

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
	static int indexof(std::string text, std::string pattern)
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
	static string replace(string str, std::string pattern, std::string replace_str){
		int replace_index=-1;
		replace_index=indexof(str,pattern);
		if(replace_index>-1){
			vector<string> str_arr=split(str,pattern);
			string tmp=str_arr[0]+replace_str+str_arr[1];
			str=tmp;
		}
		return str;
	}
	static string remove_ch(string str,char remove){
		string result="";
		for(int i=0;i<str.length();i++){
			if(str[i]!='\n'){
				result+=str[i];
			}
		}
		return result;
	}

	static string inputToString(string input){
		string result="";
		for(int i=0;i<input.length();i++){
			switch(input[i]){
				case '\t':
				case '.':
				case '+':
				case '!':
				case '=':
				case '<':
				case '>':
				case '-':
				case '*':
				case '@':
				case '$':
				case '{':
				case '}':
				case '(':
				case ')':
				case '[':
				case ']':
				case ':':
				case ';':
				case ',':
				case '_':
				case '0'...'9':
				case 'a'...'z':
				case 'A'...'Z':
					result+=input[i];
				break;
			}
		}
		return result;
	}
	static TokenKind findKind(string spelling){
		if(spelling=="do"){
			return tk_do;
		}
		else if(spelling=="for"){
			return tk_for;
		}
		else if(spelling=="pointer"){

			return tk_pointer;
		}
		else if(spelling=="real"){
			return tk_real;
		}
		else if(spelling=="this"){
			return tk_this;
		}
		else{
			return tk_identifier;
		}
	}
	static TokenKind findOps(char c){
		switch(c)                  
		{
			case '@':
							return tk_at;
			case ';':
							return tk_semi;
			case ':':
							return tk_colon;
			case '!':
							return tk_not;
			case ',':
							return tk_comma;
			case '.':
							return tk_stop;
			case '{':       
							return tk_lcb;
			case '}':       
							return tk_rcb;
			case '(':
							return tk_lrb;
			case ')':
							return tk_rrb;
			case '[':
							return tk_lsb;
			case ']':
							return tk_rsb;
			case '/':
							return tk_div;
			default:
							return tk_oops;
		}
	}

	static string float_str(string spelling){
		spelling=replace(spelling,"e0","e+0");
		spelling=replace(spelling,".e",".0e");
		if(spelling.find('e')!=std::string::npos && 
			spelling.find("e+")==std::string::npos && 
			spelling.find("e-")==std::string::npos 
			){
			spelling=replace(spelling,"e","e+");
		}

		if(spelling[spelling.length()-1]=='.'){
			spelling+="0";
			
		}
		
		if(spelling.find('e')==std::string::npos){
			spelling+="e+0";
		}
		return spelling;
	}

	static string integer_str(string spelling){

		return spelling;
	}

	// is the token of the given kind or does it belong to the given grouping?
	bool token_is_in(Token token,TokenKind kind_or_grouping)
	{
		TokenKind kind = token_kind(token) ;

		// check identity first
		if ( kind == kind_or_grouping ) return true ;

		// this is best written as nested switch statements
		switch(kind_or_grouping)
		{
			case tk_number:
			if (kind == tk_integer || kind == tk_float)
				return true;
			else
				return false;
			break;
			case tk_keyword:
			if (kind == tk_do || 
				kind == tk_for || 
				kind == tk_pointer || 
				kind == tk_real || 
				kind == tk_this)
				return true;
			else
				return false;
			break;
			case tk_symbol:
			if (kind ==  tk_at || 
				kind == tk_semi || 
				kind == tk_colon || 
				kind == tk_not || 
				kind == tk_comma || 
				kind == tk_stop || 
				kind == tk_eq || 
				kind == tk_spaceship || 
				kind == tk_lcb || 
				kind == tk_rcb || 
				kind == tk_lrb || 
				kind == tk_rrb || 
				kind == tk_lsb || 
				kind == tk_rsb || 
				kind == tk_div)
				return true;
			else
				return false;
			break;
			default:
				return false ;
		}
	}

	static string Append(string str1,string str2){
		for(int i=0;i<str2.length();i++){
			str1+=str2[i];
		}
		return str1;
	}
	static bool debug=false;
	
	int tab_count(string str){
		int result=0;
		for(int i=0;i<str.length();i++){
			if(str[i]=='\t'){
				result+=1;
			}
		}
		return result;
	}

	static string return2line(string str, int token_column_pos,string input_str,string token_str,Token token){
		vector<string> lines=split(str,"\n");

		if(lines.size()<=2){

			string result="";
			for(int i=0;i<lines.size();i++){
				if(lines[i][lines[i].length()-1]=='\r' || 
					lines[i][lines[i].length()-1]=='\b' || 
					lines[i][lines[i].length()-1]=='\n'){

					
					if(lines[i][lines[i].length()-1]=='\r'){
						lines[i][lines[i].length()-1]='\n';
						//printf("Special2 r\n");
					}
					if(lines[i][lines[i].length()-1]=='\b'){
						//printf("Special2 b\n");
					}
					if(lines[i][lines[i].length()-1]=='\n'){
						//printf("Special2 n\n");
					}
				}
				if(i==(lines.size()-1)){
					string temp=lines[i].substr(0,token_column_pos+5);
					if(token_is_in(token,tk_identifier)){
						result+=temp+inputToString(token_str);
					}
					else if(token_is_in(token,tk_symbol)){
						result+=temp+inputToString(token_str);
					}
					else if(token_is_in(token,tk_integer)){
						result+=temp+inputToString(token_str);
					}
					else if(temp[temp.length()-1]!=' '){
						result+=lines[i].substr(0,token_column_pos+6);
					}else{
						result+=lines[i];
					}
				}
				else{
					result+=lines[i];
				}
				if(indexof(lines[i],"\n")>-1){
					
				}
				else if(i<lines.size()-1){
					result+="\n";
				}
			}
			return result;
		}else{
			string result="";
			for(int i=lines.size()-2;i<lines.size();i++){
				if(lines[i][lines[i].length()-1]=='\r' || 
					lines[i][lines[i].length()-1]=='\b' || 
					lines[i][lines[i].length()-1]=='\n'){

					
					if(lines[i][lines[i].length()-1]=='\r'){
						lines[i][lines[i].length()-1]='\n';
						//printf("Special2 r\n");
					}
					if(lines[i][lines[i].length()-1]=='\b'){
						//printf("Special2 b\n");
					}
					if(lines[i][lines[i].length()-1]=='\n'){
						//printf("Special2 n\n");
					}
				}
				if(i==(lines.size()-1)){
					string temp=lines[i].substr(0,token_column_pos+5);
					if(token_is_in(token,tk_identifier)){
						result+=temp+inputToString(token_str);
					}
					else if(token_is_in(token,tk_symbol)){
						result+=temp+inputToString(token_str);
					}
					else if(token_is_in(token,tk_integer)){
						result+=temp+inputToString(token_str);
					}
					else if(temp[temp.length()-1]!=' '){
						result+=lines[i].substr(0,token_column_pos+6);
					}else{
						result+=lines[i];
					}
				}
				else{
					result+=lines[i];
				}
				if(indexof(lines[i],"\n")>-1){
					
				}
				else if(i<lines.size()-1){
					result+="\n";
				}
			}
			return result;
		}
	}

	string lineString(int line){
		string str;
		if(line<10){
			str+="   ";
		}else if(line>=10 && line <99){
			str+="  ";
		}
		else if(line>=100 && line <999){
			str+=" ";
		}
		else if(line>=1000 && line <9999){
			str+="";
		}
		return str+to_string(line)+": ";
	}

	// generate a context string for the given token
	// it shows the line before the token,
	// the line containing the token up to the end of the token, and
	// a line with a ^ marking the token's position
	// the lines are prefixed by line numbers, right justified in four spaces, followed by ': '
	// if a token extends over more than one line, only the part of the token on the first line is included
	string token_context(Token token){
		int count=1;
		int arrow_space=0;
		int line_space=0;
		string str="";
		int line_count=1;
		int line_number=token_line(token);
		string token_str=token_spelling(token);
		string input_str="";
		int token_column_pos=token_column(token);
		int column_pos=0;
		bool is_substr=false;

		int input_str_len=0;
		int token_str_len=0;
		if(debug){
			printf("token_str:%s|t:%d|n:%d|r:%d|b:%d|%lu\n",
					token_str.c_str(),
					tab_count(token_str),
					indexof(token_str,"\n"),
					indexof(token_str,"\r"),
					indexof(token_str,"\b"),
					token_str.length());
			printf("input.size:%d\n",(int)inputs.size());
			printf("Global Read str :%s\n",read_str.c_str());
		}
		if(inputs.size()==0){
			inputs.push_back(read_str);
		}
		for(int i=0;i<inputs.size();i++){
				
				if(line_count==line_number && 
					column_pos>=token_column_pos && 
					token_kind(token)!=tk_eol_comment && 
					token_kind(token)!=tk_adhoc_comment){
					
					if(i-1>=0){
						input_str=inputs[i-1];
					}
					else{
						input_str=inputs[i];
					}
					input_str_len=(int)inputToString(input_str).length();
					
					if(debug){
						printf("input_str:%s\n",input_str.c_str());
					}
					//str=remove_ch(str,'\n');
					break;
				}
				else if(line_count>line_number){
					break;
				}

				if(i==0 && str==""){
					str+=lineString(count);
					count+=1;
					arrow_space=0;
					line_space=str.length();
				}
				

				column_pos+=inputs[i].length();
				str+=inputs[i];
				if(debug){
					printf("Input:%s|t:%d|n:%d|r:%d|b:%d|%lu\n",
					inputs[i].c_str(),
					tab_count(inputs[i]),
					indexof(inputs[i],"\n"),
					indexof(inputs[i],"\r"),
					indexof(inputs[i],"\b"),
					inputs[i].length());
					
				}
				
				arrow_space=str.length()-line_space-inputs[i].length();
				if(i>=0 && 
					(indexof(inputs[i],"\n")>-1)){

					if(line_count==line_number && 
						column_pos>=token_column_pos && 
						token_kind(token)!=tk_eol_comment && 
						token_kind(token)!=tk_adhoc_comment){
						
						input_str=inputs[i];
						input_str_len=(int)inputToString(input_str).length();
						//str=remove_ch(str,'\n');
						if(debug){
							printf("input_str:%s\n",input_str.c_str());
						}
						break;
					}
					else if(line_count+1>line_number){
						break;
					}
					str+=lineString(count);
					count+=1;
					arrow_space=0;
					line_space=str.length();
					column_pos=0;
					line_count+=1;
				}
				
		}

		if(str[str.length()-1]=='\n'){
			str=str.substr(0,str.length()-1);
		}
		string arrow;
		arrow+="\n";
		arrow+="      ";

		for(int i=0;i<token_column_pos-1;i++){
			arrow+=" ";
		}
		arrow+="^\n";
		if(debug){
			printf("str:%s\n",str.c_str());
		}

		str=return2line(str,token_column_pos,input_str,token_str,token);
		while(str[str.length()-1]=='\n'){
			str=str.substr(0,str.length()-1);
		}
		str+=arrow;
		return str;

	}

	// read next character if not at the end of input
	// and update the line and column numbers
	static void nextch()
	{
		if ( ch == EOF )
		{ 

			return ;
		}

		if ( ch == '\n' )           // if last ch was newline ...
		{
			line_num++ ;            // increment line number
			column_num = 0 ;        // reset column number
			if(read_str!=""){
				//inputs.push_back(read_str);
			}



			inputs.push_back(read_str);
			read_str="";
		}
		

		ch=read_char() ;            // read the next character, probably from stdin but this could change during testing
		
		if(ch==' ' && read_str.length()>0){
			if(inputToString(read_str)!=""){
				inputs.push_back(read_str);
				read_str="";
			}
		}
		
		if(ch=='\t'){
			column_num+=4;
			read_str+="    ";
			total_input+="    ";
		}else{
			column_num++;
			read_str+=ch;
			total_input+=ch;
		}

		if(ch==' ' && read_str.length()>0){
			if(inputToString(read_str)!=""){
				inputs.push_back(read_str);
				read_str="";
			}
		}


									// additional code will be required here to handle preprocessing of '\t' and '\r'
									// you should also consider building a line by line copy of the input for use by token_context()
	}

	////////////////////////////////////////////////////////////////////////
	// called when we find end of input or we an error
	Token parse_eoi()
	{
		// simulate end of input in case this is handling a bad token rather than a real end of input
		ch = EOF ;

		// return an eoi token
		return new_token(tk_eoi,"",start_line,start_column) ;
	}
	

	static Token parse_single_char_symbol(TokenKind kind,string spelling)
	{
		// always read the next character - we have read past the end of the token
		nextch() ;
		if(kind==tk_oops){
			return parse_eoi(); 
		}
		// return a new Token object
		string_to_token_kind(spelling);
		return new_token(kind,spelling,start_line,start_column) ;
	}
	static Token parse_comment(string spelling)
	{
		TokenKind kind=tk_eol_comment;
		string single_quote="'";
		char quote=single_quote[0];
		// always read the next character - we have read past the end of the token
		nextch();
		while ( isalpha(ch) || 
			isdigit(ch) || 
			ch==' ' || 
			ch=='.' || 
			ch=='"' || 
			ch==',' || 
			ch==':' || 
			ch=='=' ||
			ch=='+' ||
			ch=='-' ||
			ch=='*' ||
			ch=='/' ||
			ch==';' ||
			ch=='(' ||
			ch==')' ||
			ch=='[' ||
			ch==']' ||
			ch==quote ||
			ch=='\n' || 
			ch=='\r' ||
			ch=='&')
		{
			
			if((ch=='\n' || ch=='\r') && kind==tk_eol_comment){
				break;
			}
			if(ch!='\r'){
				spelling += ch;
			}
			if(spelling=="//"){
				spelling="";
			}
			if(spelling=="/*"){
				spelling="";
				kind=tk_adhoc_comment;
			}
			if(spelling[spelling.length()-2]=='*' && 
				spelling[spelling.length()-1]=='/'){
				spelling=spelling.substr(0,spelling.length()-2);
				nextch();
				break;
			}
			nextch();
		}
		
		string_to_token_kind(spelling);
		// return a new Token object
		return new_token(kind,spelling,start_line,start_column) ;
	}
	static Token parse_string_symbol(string spelling)
	{
		spelling="";
		bool b_str=false;
		nextch() ;
		while ( isalpha(ch) || 
			isdigit(ch) || 
			ch==' ' || 
			ch=='.' || 
			ch=='"' || 
			ch==',' || 
			ch==':' || 
			ch=='=' ||
			ch=='+' ||
			ch=='-' ||
			ch=='*' ||
			ch=='/' ||
			ch==';' ||
			ch=='[' ||
			ch==']' ||
			ch=='\n' || 
			ch=='&')
		{
			if(ch!='"'){
				spelling += ch ;
			}
			nextch();
		}
		string_to_token_kind(spelling);
		// return a new Token object
		return new_token(tk_string,spelling,start_line,start_column) ;
	}

	static Token parse_char_symbol(string spelling)
	{
		// always read the next character - we have read past the end of the token
		nextch() ;
		// append digits to spelling until we read past the end of the integer
		while ( isalpha(ch) || 
			isdigit(ch) || 
			ch=='_' || 
			ch=='.' || 
			ch=='$' )
		{
			spelling += ch ;
			nextch();
		}

		
	
		// return a new Token object
		if(findKind(spelling)==tk_identifier){
			string_to_token_kind(spelling);
		
			return new_token(tk_identifier,spelling,start_line,start_column);
		}else{
			TokenKind kind=string_to_token_kind(spelling);
			
			return new_token(findKind(inputToString(spelling)),spelling,start_line,start_column);
		}
	}


	// parse a number - always read one extra character
	// we know spelling is the first digit of the number
	static Token parse_number(string spelling)
	{
		// always read the next character
		nextch() ;

		// append digits to spelling until we read past the end of the integer
		while ( isdigit(ch) || ch=='.' || ch=='e' || ch=='E' || ch=='+' || ch=='-'){
			if(spelling.length()>=1 && spelling.find('.')==std::string::npos){
				if(spelling[0]=='0' && 
					spelling.find('e')==std::string::npos && 
					spelling[spelling.length()-1]=='0' 
					&& ch=='0'){
					break;
				}
				if(spelling[0]=='0' && 
					spelling.find('e')==std::string::npos && 
					spelling[spelling.length()-1]=='0' && 
					isdigit(ch)){
					break;
				}
			}
			if(ch=='E'){
				ch='e';
			}
			spelling += ch ;

			if(indexof(spelling,"e-0")>-1){
				nextch();
				break;
			}
			nextch();
		}
		if(spelling[spelling.length()-2]=='.' && spelling[spelling.length()-1]=='e'){
			return parse_eoi(); 
		}

		spelling=replace(spelling,"e0","e+0");
		if(spelling.find('.')!=std::string::npos || 
			spelling.find('e')!=std::string::npos){
			
			return new_token(tk_float,float_str(spelling),start_line,start_column);
		}
		else{
			
			return new_token(tk_integer,integer_str(spelling),start_line,start_column);
		}
	}

	

	// return the next token object by reading more of the input
	Token next_token()
	{
		// you must read input using the nextch() function
		// the last character read is in the static variable ch
		// always read one character past the end of the token being returned

		string spelling = "" ;

		// this loop reads one character at a time until it reaches end of input
		while ( ch != EOF )
		{
			start_line = line_num ;                 // remember current position in case we find a token
			start_column = column_num ;
			spelling=ch;
			switch(ch)                              // ch is always the next char to read
			{
			case ' ':                               // ignore space and newline
			case '\t':
			case '\r':
			case '\n':
				nextch() ;                          // read one more character and try again
				break ;
			case '0'...'9':
				return parse_number(spelling);
				break;
				// single character symbols
			case '/':
				return parse_comment(spelling);
				break;
			case '"':
				return parse_string_symbol(spelling);
				break;
			case '.':
			case '+':
			case '!':
			case '=':
			case '<':
			case '>':
			case '-':
			case '*':
			  
			case '@':
			case '{':       
			case '}':       
			case '(':
			case ')':
			case '[':
			case ']':
			case ':':
			case ';':
			case ',':
			
				return parse_single_char_symbol(findOps(ch),spelling);
			case 'a'...'z':
			case 'A'...'Z':
			case '_':
				return parse_char_symbol(spelling) ;
				break;

													// add additional case labels here for characters that can start tokens
													// call a parse_*(kind,spelling) function to complete and return each kind of token
													// this should follow the style used in the workshops
													// but remember that the token grammar is different in this assignment
			default:
				return parse_eoi();                // the next character cannot start a token, this is an error, return an EOI token
			}
		}

		start_line = line_num ;                     // remember current position so EOI token is correct
		start_column = column_num ;

		return parse_eoi() ;                         // return an EOI token
	}
}
