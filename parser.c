/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"          /* when the keytoktab is added   */
#include "lexer.h"              /* when the lexer     is added   */
#include "symtab.h"             /* when the symtab    is added   */
/* #include "optab.h"       */       /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
//enum tvalues { program = 257, id, input, output,var,integer,begin,end };
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
/* static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
                     var,id,',',id,',',id,':',integer,';',
                     begin,
                     id,assign,id,'+',id,'*',number,
                     end,'.',
                     '$' }; */

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
/* static int pget_token(){  
   static int i=0;
   if (tokens[i] != '$') return tokens[i++]; else return '$';
} */

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
   addp_name(get_lexeme());
   match(id); 
   match('('); 
   match(input);
   match(','); 
   match(output); 
   match(')'); 
   match(';');
}   

/***********VAR PART***********/

static void var_part();
static void var_dec_list();
static void var_dec();
static void id_list();
static void type();

static void var_part(){
   if(DEBUG) printf("\n *** In var_part");
   match(var);
   var_dec_list();
}
static void var_dec_list(){
   if(DEBUG){printf("\n *** In var_dec_list");}
   var_dec();
   if (lookahead == id) 
      var_dec_list();
}
static void var_dec(){
   if(DEBUG){printf("\n *** In var_dec");}
   id_list();
   match(':');
   type();
   match(';');
}
static void id_list(){
   if(DEBUG){printf("\n *** In id_list");}

   if(lookahead == id){
      addv_name(get_lexeme());
      match(id);
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
      setv_type(error);
   }
   
}

/***********STAT PART***********/
static void stat_part();
static void stat_list();
static void stat();
static void assign_stat();
static toktyp expr();
static toktyp term();
static toktyp factor();
static toktyp operand();

static void stat_part(){
   if(DEBUG)   printf("\n ***In stat_part");
   match(begin);
   stat_list();
   match(end);
   match('.');
}
static void stat_list(){
   stat(); 
   if (lookahead == ';'){
      match(';'); 
      stat_list(); 
   }
}
static void stat(){
   assign_stat();
}
static void assign_stat(){
   match(id);
   match(assign);
   expr();
}
static toktyp expr(){
   if(DEBUG){printf("\n *** In expr");}
   toktyp tmp = term();
   if(lookahead == '+'){
      match('+');
      tmp += expr();
   }
   return tmp;
}
static toktyp term(){
   toktyp tmp = factor();

   if(lookahead == '*'){
      match('*');
      tmp *= term();
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
      match(id);
      return id;
   }
   else if(lookahead == number){
      match(number);
      return number;
   }
   else{
      printf("\n Incompatible operand!");
      return error;
   }
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser(){
   if (DEBUG) printf("\n *** In  parser");
   lookahead = get_token();       // get the first token
   program_header();               // call the first grammar rule
   var_part();
   stat_part();
   p_symtab();                     // Print Symtab
   return is_parse_ok;             // status indicator
}


/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/