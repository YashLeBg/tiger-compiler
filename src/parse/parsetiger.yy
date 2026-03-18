                                                                // -*- C++ -*-
%require "3.8"
%language "c++"
// Set the namespace name to `parse', instead of `yy'.
%define api.prefix {parse}
%define api.namespace {parse}
%define api.parser.class {parser}
%define api.value.type variant
%define api.token.constructor

// We use a GLR parser because it is able to handle Shift-Reduce and
// Reduce-Reduce conflicts by forking and testing each way at runtime. GLR
// permits, by allowing few conflicts, more readable and maintainable grammars.
%glr-parser
%skeleton "glr2.cc"

// In TC, we expect the GLR to resolve one Shift-Reduce and zero Reduce-Reduce
// conflict at runtime. Use %expect and %expect-rr to tell Bison about it.
  // FIXME: Some code was deleted here (Other directives).
%expect 1 //accept only 1 shift/reduce conflit
%expect-rr 0 //do not accept any reduce/reduce conflict
%define parse.error verbose
%defines
%debug
// Prefix all the tokens with TOK_ to avoid colisions.
%define api.token.prefix {TOK_}

/* We use pointers to store the filename in the locations.  This saves
   space (pointers), time (no deep copy), but leaves the problem of
   deallocation.  This would be a perfect job for a misc::symbol
   object (passed by reference), however Bison locations require the
   filename to be passed as a pointer, thus forcing us to handle the
   allocation and deallocation of this object.

   Nevertheless, all is not lost: we can still use a misc::symbol
   object to allocate a flyweight (constant) string in the pool of
   symbols, extract it from the misc::symbol object, and use it to
   initialize the location.  The allocated data will be freed at the
   end of the program (see the documentation of misc::symbol and
   misc::unique).  */
%define api.filename.type {const std::string}
%locations

/*---------------------.
| Support for tokens.  |
`---------------------*/
%code requires
{
#include <string>
#include <misc/algorithm.hh>
#include <misc/separator.hh>
#include <misc/symbol.hh>
#include <parse/fwd.hh>

  // Pre-declare parse::parse to allow a ``reentrant'' parsing within
  // the parser.
  namespace parse
  {
    ast_type parse(Tweast& input);
  }
}

// The parsing context.
%param { ::parse::TigerDriver& td }
%parse-param { ::parse::Lexer& lexer }

%printer { yyo << $$; } <int> <std::string> <misc::symbol>;

%token <std::string>    STRING "string"
%token <misc::symbol>   ID     "identifier"
%token <int>            INT    "integer"


/*--------------------------------.
| Support for the non-terminals.  |
`--------------------------------*/

%code requires
{
# include <ast/fwd.hh>
// Provide the declarations of the following classes for the
// %destructor clauses below to work properly.
# include <ast/exp.hh>
# include <ast/var.hh>
# include <ast/ty.hh>
# include <ast/name-ty.hh>
# include <ast/field.hh>
# include <ast/field-init.hh>
# include <ast/function-dec.hh>
# include <ast/type-dec.hh>
# include <ast/var-dec.hh>
# include <ast/chunk.hh>
# include <ast/chunk-list.hh>
}

  // FIXME: Some code was deleted here (Printers and destructors).
//%printer {yyo<<$$;} <int>   <std::string><misc::symbol>;      //yyo = bison output,$$ token value, followed by the 3 types that will be applied to
/*-----------------------------------------.
| Code output in the implementation file.  |
`-----------------------------------------*/

%code
{
# include <parse/tweast.hh>
# include <misc/separator.hh>
# include <misc/symbol.hh>
# include <ast/all.hh>
# include <ast/libast.hh>
# include <parse/tiger-factory.hh>
# include <parse/tiger-driver.hh>
  namespace
  {

    /// Get the metavar from the specified map.
    template <typename T>
    T*
    metavar(parse::TigerDriver& td, unsigned key)
    {
      parse::Tweast* input = td.input_;
      return input->template take<T>(key);
    }

  }
}

%code
{
  #include <parse/scantiger.hh>  // header file generated with reflex --header-file
  #undef yylex
  #define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global lex()
}

// Definition of the tokens, and their pretty-printing.
%token AND          "&"
       ARRAY        "array"
       ASSIGN       ":="
       BREAK        "break"
       CAST         "_cast"
       CLASS        "class"
       COLON        ":"
       COMMA        ","
       DIVIDE       "/"
       DO           "do"
       DOT          "."
       ELSE         "else"
       END          "end"
       EQ           "="
       EXTENDS      "extends"
       FOR          "for"
       FUNCTION     "function"
       GE           ">="
       GT           ">"
       IF           "if"
       IMPORT       "import"
       IN           "in"
       LBRACE       "{"
       LBRACK       "["
       LE           "<="
       LET          "let"
       LPAREN       "("
       LT           "<"
       MINUS        "-"
       METHOD       "method"
       NE           "<>"
       NEW          "new"
       NIL          "nil"
       OF           "of"
       OR           "|"
       PLUS         "+"
       PRIMITIVE    "primitive"
       RBRACE       "}"
       RBRACK       "]"
       RPAREN       ")"
       SEMI         ";"
       THEN         "then"
       TIMES        "*"
       TO           "to"
       TYPE         "type"
       VAR          "var"
       WHILE        "while"
       EOF 0        "end of file"

%type <ast::Exp*>             exp
%type <ast::ChunkList*>       chunks

%type <ast::TypeChunk*>       tychunk
%type <ast::TypeDec*>         tydec
%type <ast::NameTy*>          typeid
%type <ast::Ty*>              ty

%type <ast::Field*>           tyfield
%type <ast::fields_type*>     tyfields tyfields.1
  // FIXME: Some code was deleted here (More %types).
//This part is declarations of all cpp ast type for building the AST
%type <ast::Var*>             lvalue
%type <ast::exps_type*>       exps exps.1 args args.1
%type <ast::fieldinits_type*> recfields recfields.1
%type <ast::FieldInit*>       recfield
%type <ast::VarChunk*>        varchunk formals formals.1
%type <ast::VarDec*>          vardec
%type <ast::FunctionChunk*>   funchunk
%type <ast::MethodChunk*>     methodchunk
%type <ast::FunctionDec*>     fundec
%type <ast::MethodDec*>       methoddec
%type <ast::ChunkList*>       classfields
  // FIXME: Some code was deleted here (Priorities/associativities).
%precedence DO
%precedence OF
%precedence THEN    //those 2 lines ll tie the closet else to the then to solve shift/reduce ambiguity
%precedence ELSE

%nonassoc ASSIGN       //right assign in bidding arrithmetic
%left OR            //or priority<and priority
%left AND
%nonassoc EQ NE GT GE LT LE //comparisons cannot be chained
%left PLUS MINUS    //comparison priority<plusminus priority<div/mult priority
%left TIMES DIVIDE
%precedence UMINUS  //highest mathematic prio
// Solving conflicts on:
// let type foo = bar
//     type baz = bat
// which can be understood as a list of two TypeChunk containing
// a unique TypeDec each, or a single TypeChunk containing two TypeDec.
// We want the latter.
%precedence CHUNKS
%precedence TYPE
  // FIXME: Some code was deleted here (Other declarations).
//context-dependent precedence rules dor diff type of chuncks
%precedence FUNCTION
%precedence PRIMITIVE
%precedence CLASS
%precedence METHOD
%precedence VAR
%start program

%%
program:
  /* Parsing a source program.  */
  exp
   { td.ast_ = $1; }
| /* Parsing an imported file.  */
  chunks
   { td.ast_ = make_LetExp(@$, $1, make_SeqExp(@$, make_exps_type())); }
;

exp:
  INT
   { $$ = make_IntExp(@$, $1); }
  // FIXME: Some code was deleted here (More rules).
//all that block defines all the ways on how an expression can be wrote in tiger  and how to tuurn it on cpp object
// "is it that symbole?" ->     {cpp code that will be executed } $$=rule res,function, @$=localisation, X element
| STRING                            { $$ = make_StringExp(@$, $1); }
| NIL                               { $$ = make_NilExp(@$); }
| lvalue                            { $$ = $1; }
| LPAREN exps RPAREN                { $$ = make_SeqExp(@$, $2); }
| ID LPAREN args RPAREN             { $$ = make_CallExp(@$, $1, $3); }
| lvalue DOT ID LPAREN args RPAREN  { $$ = make_MethodCallExp(@$, $3, $5, $1); }
//| typeid LBRACE recfields RBRACE    { $$ = make_RecordExp(@$, $1, $3); }
| ID LBRACK exp RBRACK OF exp                 { $$ = make_ArrayExp(@$, make_NameTy(@1, $1), $3, $6); }
| NAMETY "(" INT ")" LBRACK exp RBRACK OF exp { $$ = make_ArrayExp(@$, metavar<ast::NameTy>(td, $3), $6, $9); }
| lvalue ASSIGN exp                 { $$ = make_AssignExp(@$, $1, $3); }
| IF exp THEN exp %prec THEN        { $$ = make_IfExp(@$, $2, $4); }
| IF exp THEN exp ELSE exp          { $$ = make_IfExp(@$, $2, $4, $6); }
| WHILE exp DO exp                  { $$ = make_WhileExp(@$, $2, $4); }
| FOR ID ASSIGN exp TO exp DO exp   { $$ = make_ForExp(@$, make_VarDec(@2, $2, nullptr, $4), $6, $8); }
| BREAK                             { $$ = make_BreakExp(@$); }
| LET chunks IN exps END            { $$ = make_LetExp(@$, $2, make_SeqExp(@4, $4)); }
| exp PLUS exp                      { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::add, $3); }
| exp MINUS exp                     { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::sub, $3); }
| exp TIMES exp                     { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::mul, $3); }
| exp DIVIDE exp                    { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::div, $3); }
| exp EQ exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::eq, $3); }
| exp NE exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::ne, $3); }
| exp LT exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::lt, $3); }
| exp LE exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::le, $3); }
| exp GT exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::gt, $3); }
| exp GE exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::ge, $3); }
| MINUS exp %prec UMINUS            { $$ = make_OpExp(@$, make_IntExp(@1, 0), ast::OpExp::Oper::sub, $2); }
| exp AND exp                       { $$ = make_IfExp(@$, $1, $3, make_IntExp(@3, 0)); }
| exp OR exp                        { $$ = make_IfExp(@$, $1, make_IntExp(@1, 1), $3); }
| NEW typeid                        { $$ = make_ObjectExp(@$, $2); }
| CAST LPAREN exp COMMA ty RPAREN   { $$ = make_CastExp(@$, $3, $5); }
;
//that block is for everything that can be on the left of ":=" 
//works on the same way as before
lvalue:
  ID                                { $$ = make_SimpleVar(@$, $1); }
| lvalue DOT ID                     { $$ = make_FieldVar(@$, $1, $3); }
| lvalue LBRACK exp RBRACK          { $$ = make_SubscriptVar(@$, $1, $3); }
;
//manages expression list. separator ";"
exps:
  %empty                            { $$ = make_exps_type(); }
| exps.1                            { $$ = $1; }
;
exps.1:
  exp                               { $$ = make_exps_type($1); }
| exps.1 SEMI exp                   { $$ = $1; $$->emplace_back($3); }
;
//manages arguments list.separator ","
args:
  %empty                            { $$ = make_exps_type(); }
| args.1                            { $$ = $1; }
;
args.1:
  exp                               { $$ = make_exps_type($1); }
| args.1 COMMA exp                  { $$ = $1; $$->emplace_back($3); }
;
//manages record list. separator ","
recfields:
  %empty                            { $$ = make_fieldinits_type(); }
| recfields.1                       { $$ = $1; }
;
recfields.1:
  recfield                          { $$ = make_fieldinits_type($1); }
| recfields.1 COMMA recfield        { $$ = $1; $$->emplace_back($3); }
;
recfield:
  ID EQ exp                         { $$ = make_FieldInit(@$, $1, $3); }
;
/*---------------.
| Declarations.  |
`---------------*/

%token CHUNKS "_chunks";
chunks:
  /* Chunks are contiguous series of declarations of the same type
     (TypeDec, FunctionDec...) to which we allow certain specfic behavior like
     self referencing.
     They are held by a ChunkList, that can be empty, like in this case:
        let
        in
            ..
        end
     which is why we end the recursion with a %empty. */
%empty                  { $$ = make_ChunkList(@$); }
| tychunk   chunks        { $$ = $2; $$->push_front($1); }
| varchunk  chunks        { $$ = $2; $$->push_front($1); }
| funchunk  chunks        { $$ = $2; $$->push_front($1); }
;

varchunk:
  vardec %prec CHUNKS     { $$ = make_VarChunk(@1); $$->push_front(*$1); }
| vardec varchunk         { $$ = $2; $$->push_front(*$1); }
;

funchunk:
  fundec %prec CHUNKS     { $$ = make_FunctionChunk(@1); $$->push_front(*$1); }
| fundec funchunk         { $$ = $2; $$->push_front(*$1); }
;

vardec:
  "var" ID ":=" exp                 { $$ = make_VarDec(@$, $2, nullptr, $4); }
| "var" ID ":" typeid ":=" exp      { $$ = make_VarDec(@$, $2, $4, $6); }
;
formals:
  %empty                            { $$ = make_VarChunk(@$); }
| formals.1                         { $$ = $1; }
;

formals.1:
  tyfield                           { $$ = make_VarChunk(@$); $$->push_front(*make_VarDec(@$, $1->name_get(), &($1->type_name_get()), nullptr)); }
| formals.1 "," tyfield             { $$ = $1; $$->emplace_back(*make_VarDec(@3, $3->name_get(), &($3->type_name_get()), nullptr)); }
;
fundec:
  "function" ID "(" formals ")" "=" exp               { $$ = make_FunctionDec(@$, $2, $4, nullptr, $7); }
| "function" ID "(" formals ")" ":" typeid "=" exp    { $$ = make_FunctionDec(@$, $2, $4, $7, $9); }
| "primitive" ID "(" formals ")"                      { $$ = make_FunctionDec(@$, $2, $4, nullptr, nullptr); }
| "primitive" ID "(" formals ")" ":" typeid           { $$ = make_FunctionDec(@$, $2, $4, $7, nullptr); }
;
classfields:
  %empty                            { $$ = make_ChunkList(@$); }
| varchunk classfields              { $$ = $2; $$->push_front($1); }
| methodchunk classfields           { $$ = $2; $$->push_front($1); }
;

methodchunk:
  methoddec %prec CHUNKS            { $$ = make_MethodChunk(@1); $$->push_front(*$1); }
| methoddec methodchunk             { $$ = $2; $$->push_front(*$1); }
;

methoddec:
  "method" ID "(" formals ")" "=" exp               { $$ = make_MethodDec(@$, $2, $4, nullptr, $7); }
| "method" ID "(" formals ")" ":" typeid "=" exp    { $$ = make_MethodDec(@$, $2, $4, $7, $9); }
;

/*--------------------.
| Type Declarations.  |
`--------------------*/

tychunk:
  /* Use `%prec CHUNKS' to do context-dependent precedence and resolve a
     shift-reduce conflict. */
  tydec %prec CHUNKS  { $$ = make_TypeChunk(@1); $$->push_front(*$1); }
| tydec tychunk       { $$ = $2; $$->push_front(*$1); }
;

tydec:
  "type" ID "=" ty { $$ = make_TypeDec(@$, $2, $4); }
;

ty:
  typeid               { $$ = $1; }
| "{" tyfields "}"     { $$ = make_RecordTy(@$, $2); }
| "array" "of" typeid  { $$ = make_ArrayTy(@$, $3); }
| "class" "extends" typeid "{" classfields "}" { $$ = make_ClassTy(@$, $3, $5); }
| "class" "{" classfields "}"                  { $$ = make_ClassTy(@$, make_NameTy(@$, "Object"), $3); }
;

tyfields:
  %empty               { $$ = make_fields_type(); }
| tyfields.1           { $$ = $1; }
;

tyfields.1:
  tyfields.1 "," tyfield { $$ = $1; $$->emplace_back($3); }
| tyfield                { $$ = make_fields_type($1); }
;

tyfield:
  ID ":" typeid     { $$ = make_Field(@$, $1, $3); }
;

%token NAMETY "_namety";
typeid:
  ID                    { $$ = make_NameTy(@$, $1); }
  /* This is a metavariable. It it used internally by TWEASTs to retrieve
     already parsed nodes when given an input to parse. */
| NAMETY "(" INT ")"    { $$ = metavar<ast::NameTy>(td, $3); }
;

%%

void
parse::parser::error(const location_type& l, const std::string& m)
{
  // FIXME: Some code was deleted here.
  td.error_ << l << ": " << m << std::endl;
}
