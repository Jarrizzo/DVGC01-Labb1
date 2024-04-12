/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
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
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
	} tab;
	
/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
    {"id", 	     	 id},
	{"number", 		 number},
	{":=", 	         assign},
	{"undef", 		 undef},
	{"predef", 		 predef},
	{"tempty", 		 tempty},
	{"error",        error},
	{"type",         typ},
	{"$",            '$'},
	{"(",            '('},
	{")",            ')'},
	{"*",            '*'},
	{"+",            '+'},
	{",",            ','},
	{"-",            '-'},
	{".",            '.'},
	{"/",            '/'},
	{":",            ':'},
	{";",            ';'},
	{"=",            '='},
	{"TERROR", 	     nfound}
        };


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 		input},
	{"output", 		output},
	{"var", 		var},
	{"begin", 		begin},
	{"end", 		end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 		real},
	{"KERROR", 		nfound}
	} ;
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab(){  
	printf("\nProgram keyWords");

	int ksize = sizeof(tokentab) / sizeof(tab) - 1;
	int tsize = sizeof(keywordtab) / sizeof(tab) - 1;

	for(int i = 0; i < ksize; i++){
		printf("\n%s		%i",tokentab[i].text,tokentab[i].token);
	}
	printf("\nProgram tokens");
	for(int i = 0;i < tsize;i++){
		printf("\n%s			%i",keywordtab[i].text,keywordtab[i].token);
	}
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex){

	int ksize = sizeof(tokentab) / sizeof(tab) - 1;
	int tsize = sizeof(keywordtab) / sizeof(tab) - 1;

	// *** Check if number *** //
	if(fplex[0] <= 48 && fplex[0] >= 57){
		return number;
	}

	// *** Check if token *** //
	for(int i = 0; i < ksize;i++){
		if(!strcmp(fplex,tokentab[i].text)){
			return tokentab[i].token;
		}
	}
	// *** Check if keyword *** //
	for(int i = 0; i < tsize; i++){
		if(!strcmp(fplex, keywordtab[i].text)){
			return keywordtab[i].token;
		}
	}
	
	return 0;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex){

	int tsize = sizeof(keywordtab) / sizeof(tab);

	for(int i = 0; i < tsize;i++){
		if(!strcmp(fplex,keywordtab[i].text)){
			return keywordtab[i].token;
		}
	}

   return id;
   }

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok){

	int ksize = sizeof(tokentab) / sizeof(tab) - 1;
	int tsize = sizeof(keywordtab) / sizeof(tab) - 1;


	// *** Check for token *** //
	for(int i = 0; i < ksize; i++){
		if(ftok == tokentab[i].token){
			return tokentab[i].text;
		}
	}
	// *** Check for keyword *** //
	for(int i = 0; i < tsize; i++){
		if(ftok == keywordtab[i].token){
			return keywordtab[i].text;
		}
	}
	return "-1";

}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/