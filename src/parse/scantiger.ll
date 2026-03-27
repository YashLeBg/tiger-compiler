                                                            /* -*- C++ -*- */
// %option defines the parameters with which the reflex will be launched
%option noyywrap
// To enable compatibility with bison
%option bison-complete
%option bison-cc-parser=parser
%option bison_cc_namespace=parse
%option bison-locations

%option lex=lex
// Add a param of function lex() generate in Lexer class
%option params="::parse::TigerDriver& td"
%option namespace=parse
// Name of the class generate by reflex
%option lexer=Lexer

%top{

#define YY_EXTERN_C extern "C" // For linkage rule

#include <cerrno>
#include <climits>
#include <regex>
#include <string>

#include <boost/lexical_cast.hpp>

#include <misc/contract.hh>
  // Using misc::escape is very useful to quote non printable characters.
  // For instance
  //
  //    std::cerr << misc::escape('\n') << '\n';
  //
  // reports about `\n' instead of an actual new-line character.
#include <misc/escape.hh>
#include <misc/symbol.hh>
#include <parse/parsetiger.hh>
#include <parse/tiger-driver.hh>

  // FIXME: Some code was deleted here (Define YY_USER_ACTION to update locations).
#define YY_USER_ACTION \
    td.location_.step();    \
    td.location_.columns(yyleng);
#define TOKEN(Type)                             \
  parser::make_ ## Type(td.location_)

#define TOKEN_VAL(Type, Value)                  \
  parser::make_ ## Type(Value, td.location_)

# define CHECK_EXTENSION()                              \
  do {                                                  \
    if (!td.enable_extensions_p_)                       \
      td.error_ << misc::error::error_type::scan        \
                << td.location_                         \
                << ": invalid identifier: `"            \
                << misc::escape(text()) << "'\n";       \
  } while (false)


%}

%x SC_COMMENT SC_STRING

/* Abbreviations.  */
int    [0-9]+ 
id        [a-zA-Z][a-zA-Z0-9_]*
id_ext  _[a-zA-Z][a-zA-Z0-9_]*
  /* FIXME: Some code was deleted here. */

%class{
  // FIXME: Some code was deleted here (Local variables).
    std::string cur_string;
    int nb_coms=0;
}

%%
/* The rules.  */
{int}         {
                int val = 0;
  // FIXME: Some code was deleted here (Decode, and check the value).

                val=boost::lexical_cast<int>(text());
                return TOKEN_VAL(INT, val);
              }
  /* FIXME: Some code was deleted here. */
[ \t\r]+  { td.location_.step();}
\n+   {td.location_.lines(yyleng); td.location_.step();}
"array" {return TOKEN(ARRAY);}
"if"    {return TOKEN(IF);}
"then"  {return TOKEN(THEN);}
"else"  {return TOKEN(ELSE);}
"while" {return TOKEN(WHILE);}
"for"   {return TOKEN(FOR);}
"to"    {return TOKEN(TO);}
"do"    {return TOKEN(DO);}
"let"   {return TOKEN(LET);}
"in"          {return TOKEN(IN);}
"end"         {return TOKEN(END);}
"of"          {return TOKEN(OF);}
"break"       {return TOKEN(BREAK);}
"nil"         {return TOKEN(NIL);}
"function"    {return TOKEN(FUNCTION);}
"var"         {return TOKEN(VAR);}
"type"        {return TOKEN(TYPE);}
"import"      {return TOKEN(IMPORT);}
"primitive"   {return TOKEN(PRIMITIVE);}
"class"       {
                if (td.enable_object_extensions_p_) 
                  return TOKEN(CLASS);
                else 
                  return TOKEN_VAL(ID, misc::symbol(text()));
              }
"extends"     {
                if (td.enable_object_extensions_p_) 
                  return TOKEN(EXTENDS);
                else 
                  return TOKEN_VAL(ID, misc::symbol(text()));
              }
"method"      {
                if (td.enable_object_extensions_p_) 
                  return TOKEN(METHOD);
                else 
                  return TOKEN_VAL(ID, misc::symbol(text()));
              }
"new"         {
                if (td.enable_object_extensions_p_) 
                  return TOKEN(NEW);
                else 
                  return TOKEN_VAL(ID, misc::symbol(text()));
              }
","           {return TOKEN(COMMA);}
":"           {return TOKEN(COLON);}
";"           {return TOKEN(SEMI);}
"("           {return TOKEN(LPAREN);}
")"           {return TOKEN(RPAREN);}
"["           {return TOKEN(LBRACK);}
"]"           {return TOKEN(RBRACK);}
"{"           {return TOKEN(LBRACE);}
"}"           {return TOKEN(RBRACE);}
"."           {return TOKEN(DOT);}
"+"           {return TOKEN(PLUS);}
"-"           {return TOKEN(MINUS);}
"*"           {return TOKEN(TIMES);}
"/"           {return TOKEN(DIVIDE);}
"="           {return TOKEN(EQ);}
"<>"          {return TOKEN(NE);}
"<"           {return TOKEN(LT);}
"<="          {return TOKEN(LE);}
">"           {return TOKEN(GT);}
">="          {return TOKEN(GE);}
"&"           {return TOKEN(AND);}
"|"           {return TOKEN(OR);}
":="          {return TOKEN(ASSIGN);}

//above its classics cases easy to understand
//but now we r entering some tricky cases we ll explain each one.
//ps:checextension check if u r running your compiler with options allowing using of those words.

{id}  {return TOKEN_VAL(ID,misc::symbol(text()));}  //just for return the name of function haishi variable
"_cast"     {CHECK_EXTENSION(); return TOKEN(CAST);}    //used for convert a type to another else (casting bro...)
"_chunks"   {CHECK_EXTENSION(); return TOKEN(CHUNKS);}  //use for testing : injecting some declarations blocks in the ast.
"_exp"      {CHECK_EXTENSION(); return TOKEN(EXP);}
"_namety"   {CHECK_EXTENSION(); return TOKEN(NAMETY);}  //use for tweast : its to assigned a type to a number and using the number instead.
"_main"     {return TOKEN_VAL(ID, misc::symbol(text()));}
{id_ext}  {
    if (!td.enable_extensions_p_)
      td.error_ << misc::error::error_type::scan
                << td.location_
                << ": invalid identifier: `"
                << misc::escape(text()) << "'\n";
    return TOKEN_VAL(ID, misc::symbol(text()));
  } //same for external id begning with _

"/*"        {nb_coms=1;start(SC_COMMENT);}//That part is just multiple line commentary scanner send error if ther is needed.
<SC_COMMENT>{
"/*"    {nb_coms++;}
"*/"    {nb_coms--; if(nb_coms==0) start(INITIAL);}
\n+     {td.location_.lines(yyleng); td.location_.step();}
.       {}
<<EOF>> {td.error_<<misc::error::error_type::scan<<td.location_<<": the comment is not finished\n"; start(INITIAL);}
}

"\""        {cur_string.clear(); start(SC_STRING);}//That one is for strings mananing backslash,newline,tab,... .Send error if needed.
<SC_STRING>{
"\""    {start(INITIAL);return TOKEN_VAL(STRING,cur_string);}
\\n     {cur_string+='\n';}
\\t     {cur_string+='\t';}
\\\"    {cur_string+='\"';}
\\\\    {cur_string+='\\';}
\n      {td.error_<<misc::error::error_type::scan<<td.location_<<": nezline in string\n";td.location_.lines(yyleng); td.location_.step();}
<<EOF>> {td.error_<<misc::error::error_type::scan<<td.location_<<": the sring is not terminated\n"; start(INITIAL);}
.       {cur_string+=text();}
}
.    {td.error_ << misc::error::error_type::scan << td.location_ << ":that char is not correct -> " << misc::escape(text()) << "\n";}

%%



