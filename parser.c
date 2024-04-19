//Laborant Isak Nilsson, 2024-04
/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"          /* when the keytoktab is added   */
#include "lexer.h"              /* when the lexer     is added   */
#include "symtab.h"             /* when the symtab    is added   */
#include "optab.h"              /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
static int  lookahead=0;
static int  is_parse_ok=true;

/**********************************************************************/
/* Declerations                                                       */
/**********************************************************************/
// *** Var Part *** //
static void var_part();
static void var_dec_list();
static void var_dec();
static void id_list();
static void type();
// *** Stat Part *** //
static void stat_part();
static void stat_list();
static void stat();
static void assign_stat();
static toktyp expr();
static toktyp term();
static toktyp factor();
static toktyp operand();
// *** Error Handling *** //
static void error_id();
static void error_symbol();
static void error_operand();
static void error_type();
static void error_undec();
static void error_dupl();
static void error_asign();
static void error_emptyFile();
static void erro_afterParse();



/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/

/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t){
   if(DEBUG) printf("\n --------In match expected: %4s, found: %4s",tok2lex(t), tok2lex(lookahead));
   if (lookahead == t) lookahead = get_token();
   else {
      is_parse_ok=0;
      printf("\n *** Unexpected Token: expected: %4s found: %4s (in match)",
              tok2lex(t), tok2lex(lookahead));
      }
}



/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/

static void program_header(){
   if (DEBUG) printf("\n *** In  program_header");
   match(program);
   if(lookahead == id){
      addp_name(get_lexeme());
      match(id); 
   }
   else{
      error_id();
      addp_name("Error");
   }
   match('('); 
   match(input);
   match(','); 
   match(output); 
   match(')'); 
   match(';');
}

/***************  VAR PART  ***************/

static void var_part(){
   if(DEBUG) printf("\n *** In var_part");
   if(lookahead == var){
   match(var);
   }
   else{
      error_symbol("var");
   }
   var_dec_list();
}
static void var_dec_list(){
   if(DEBUG){printf("\n *** In var_dec_list");}
   var_dec();
   if (lookahead == id){
      var_dec_list();
   }
}
static void var_dec(){
   if(DEBUG){printf("\n *** In var_dec");}
   id_list();
   if(lookahead == ':'){
      match(':');
   }
   else{
      error_symbol(":");
   }
   type();
   if(lookahead == ';'){
      match(';');
   }
   else{
      error_symbol(";");
   }
}
static void id_list(){
   if(DEBUG){printf("\n *** In id_list");}

   if(lookahead == id){
      if(!find_name(get_lexeme())){
         addv_name(get_lexeme());
      }
      else{
         error_dupl();
      }
      match(id);
   }
   else{
      error_id();
   }
   if (lookahead == ','){
      match(',');
      id_list();
   }
}
static void type(){
   if(DEBUG){printf("\n *** In type");}

   if(lookahead == integer){
      setv_type(integer);
      match(integer);
   }
   else if (lookahead == real){
      setv_type(real);
      match(real);
   }
   else if(lookahead == boolean){
      setv_type(boolean);
      match(boolean);
   }
   else{
      error_type();
      setv_type(error);
   }
}

/***************  STAT PART  ***************/

static void stat_part(){
   if(DEBUG)   printf("\n ***In stat_part");
   if(lookahead == begin){
      match(begin);
   }
   else{
      error_symbol("begin");
   }

   stat_list();

   if(lookahead == end){
      match(end);
   }
   else{
      error_symbol("end");
   }

   if(lookahead == '.'){
      match('.');
   }
   else{
      error_symbol(".");
   }

   if(lookahead != '$'){
      erro_afterParse();
      while(lookahead != '$'){
         printf("%s \t",get_lexeme(lookahead));
         match(lookahead);
      }
   }

}
static void stat_list(){
   stat(); 
   if (lookahead == ';'){
      match(';'); 
   }
   else{
      error_symbol(";");         // *** MABY? *** //
   }
   stat_list(); 

}
static void stat(){
   if(DEBUG){printf("\n *** In stat");}
   assign_stat();
}
static void assign_stat(){
   if(DEBUG){printf("\n *** In assign stat");}

   toktyp rightNum,leftNum = error;

   if(lookahead == id){
      if(!find_name(get_lexeme())){
         error_undec();
      }
      else{
         leftNum = get_ntype(get_lexeme());
      }
   match(id);
   }
   else{
      error_id();
   }
   if(lookahead == assign){
   match(assign);
   }
   else{
      error_symbol(":=");
   }
   rightNum = expr();

   if(leftNum != rightNum){
      error_asign(tok2lex(leftNum),tok2lex(rightNum));
   }

}
static toktyp expr(){
   if(DEBUG){printf("\n *** In expr");}
   toktyp tmp = term();
   if(lookahead == '+'){
      match('+');
      tmp = get_otype('+',tmp,expr());
   }
   return tmp;
}
static toktyp term(){
   toktyp tmp = factor();

   if(lookahead == '*'){
      match('*');
      tmp = get_otype('*',tmp,term());
   }
   return tmp;
}
static toktyp factor(){
   if(DEBUG){printf("\n *** In factor");}
   toktyp tmp;
   if(lookahead == '('){

      match('(');
      tmp = expr();
      match(')');
   }
   else{
      tmp = operand();
   }
   return tmp;
}
static toktyp operand(){
   if(DEBUG){printf("\n *** In Opernad");}

   if(lookahead == id){
      if(!find_name(get_lexeme())){
         error_undec();
      }
      match(id);
      return get_ntype(get_lexeme());
   }
   else if(lookahead == number){
      match(number);
      return number;
   }
   else{
      error_operand();
      return error;
   }
}
/***************  Error Handling  ***************/
// *** SYNTAX ERRORS ***//
static void error_id(){
   printf("\nSYNTAX ERROR: \"ID\" expected, Found %s",get_lexeme());
   is_parse_ok = false;
}
static void error_symbol(char * s){
   printf("\nSYNTAX ERROR: expected \"symbol(%s)\", Found %s",s,get_lexeme());
   is_parse_ok = false;
}
static void error_operand(){
   printf("\nSYNTAX ERROR: expected \"Operand\"");
   is_parse_ok = false;
}
static void error_type(){
   printf("\nSYNTAX ERROR: expected \"Type name\", Found %s",get_lexeme());
   is_parse_ok = false;
}
static void error_emptyFile(){
   printf("SYNTAX ERROR: Inputfile is empty");
   is_parse_ok = false;
}
static void erro_afterParse(){
   printf("SYNTAX ERROR: Symbols existing after parse ending");
   is_parse_ok = false;
}
// *** SEMANTIC ERRORS ***//
static void error_undec(){
   printf("\nSEMANTIC ERROR: \"ID not declared\", %s",get_lexeme());
   is_parse_ok = false;
}
static void error_dupl(){
   printf("\nSEMANTIC ERROR: \"ID already declared\", %s",get_lexeme());
   is_parse_ok = false;
}
static void error_asign(char * l, char * r){
   printf("SEMATIC ERROR: \"%s\" cant := \"%s\"",l,r);
   is_parse_ok = false;
}


/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser(){
   if (DEBUG) printf("\n *** In  parser");
   lookahead = get_token();       // get the first token
   if(lookahead == '$' || lookahead < 0){
      error_emptyFile();
      return is_parse_ok;
   }
   program_header();               // call the first grammar rule
   var_part();
   stat_part();
   p_symtab();                     // Print Symtab
   return is_parse_ok;             // status indicator
}


/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/