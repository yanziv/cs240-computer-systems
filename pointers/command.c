/**
 * CS 240 Pointers: A simple command parsing library
 *
 * Assignment description:
 * https://cs.wellesley.edu/~cs240/assignments/pointers/
 */
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "command.h"

/**
 * command_char_is_space:
 *
 * Determine whether a character is a space for purposes of command parsing.
 * Do not change this function.
 *
 * Notes:
 *
 * - This function is an example helper function to encourage you to
 *   define and use new helper functions where useful.
 * - To support optional extensions, this function checks a broader
 *   definition of "space" including tabs, newlines, and carriage
 *   returns. However, we will never evaluate your code on command
 *   lines containing the following characters.  Our tests will use
 *   only ' ' as a space character.
 * - C has no Boolean data type, but there is no reason one cannot be
 *   defined.  The "stdbool.h" library defines `bool` as an alias for
 *   an integer type and `true`/`false` as `1`/`0` under the hood.
 *
 * Parameters:
 *
 * - ch: character to inspect
 *
 * Return:
 *
 * - true if the given character is a space character for purposes of
 *   command parsing
 *
 * - false otherwise
 */
bool command_char_is_space(char ch) {
  switch (ch) {
  case ' ':
  case '\t':
  case '\n':
  case '\r':
    return true;
  default:
    return false;
  }
}

/**
 * command_parse:
 *
 * Parse a command-line string, report foreground/background status,
 * and return a command array.  Each element in the command array
 * corresponds to one word in the command string, in order.
 *
 * A command with '&' after the last word is a background command.
 * The '&' is not part of the resulting command array.  Repetitions or
 * other placements of '&' constitute invalid command lines.
 *
 * Parameters:
 *
 * - line: non-NULL pointer to a command-line string.
 *
 * - status: non-NULL pointer to the location where this function
 *   should store the (1) foreground or (0) background status of the
 *   given command line.
 * 
 * Return:
 *
 * - a pointer (char**) to a NULL-terminated array of strings if given
 *   a valid command-line string.
 *
 * - NULL if given a command-line string with invalid placement of '&'
 *
 * For valid commands, in addition to returning the command array
 * address, command_parse shall set the contents at the status address
 * given by the `status` pointer argument to the appropriate
 * foreground or background value.
 */
char** command_parse(char* line, int* status) {
  // Check arguments: must be non-NULL pointers.
  assert(line!=NULL);
  assert(status!=NULL);

  // IMPLEMENT ME
  char* lineIter = line;
  int charcounter = 0; //counts number of characters in each word
  int wordcounter = 0; //Count how many words are in the command line

  /* 
  * Checking for ampersand: 
  * it may appear at most once
  * & may appear only after the final word, followed only by zero or more spaces to the end
  * a valid command containing no ampersand is a foreground command, returns 0 
  * containing one ampersand is a background command, returns 1
  */ 
  int ampcount = 0; 
  bool found = false; 
  while(*lineIter != '\0'){ //check that lineIter did not reach the end of the line
    if (ampcount > 1){
      //printf("ampcount > 1\n");
      return NULL;
    }
    if (*lineIter == '&'){
      found = true;
      ampcount ++;
    }

    // return NULL when there's a non-space character that follows that ampersand 
    else if (!command_char_is_space(*lineIter) && found == true){
      //printf("there's a non-space character that follows that ampersand\n");
      return NULL;
    }
    lineIter++;
  }
  lineIter = line; 

  // set status to either 0 or 1 based on the number of ampersand we have in the VALID command line
  if (ampcount == 0){//foreground
    *status = 1;
  }
  else{//background
    *status = 0;
  }

  /*
   * Counting the number of words in a VALID command line
   */
  while(*lineIter != '\0'){
    if(!command_char_is_space(*lineIter) && (*lineIter != '&')){ //increment char count if not a space character and not an &
      charcounter++;
    }
    else{ 
      if (charcounter!=0){
        wordcounter++;
        charcounter = 0;
      }
    }
    lineIter++;
  }
  //checking if string does not end with a space --> increment if there is a word at the end of the string
  if (charcounter!=0){
    wordcounter++;
  }
  charcounter = 0; 

  //memory allocation for array
  printf("allocating array of size %d\n",wordcounter+1);
  char** array = (char**) malloc((wordcounter + 1)*(sizeof(char*))); 

  *(array + wordcounter) = NULL; //set the last word in array to be NULL
  //printf("malloced array at locatiton %p\n",array);
  
  char** arr = array; //pointer that points to command line array
  char* wordIter; 
  char* word; 
  lineIter = line; 
  char* lineIndex = lineIter;
  //printf("array starts at: %p\n", array); 

  /*
   * Counting length of each word
   * memory allocating each pointer in the array
   * fill each pointer with the word
   */
  while(*lineIter != '\0'){
     if(!command_char_is_space(*lineIter) && (*lineIter != '&')){
      charcounter++;
    }
    else{
      if (charcounter!=0){
        word = (char*) malloc((charcounter + 1)*(sizeof(char))); 
        wordIter = word;
        //copying word from command line into word pointer 
        for (int x = 0; x < charcounter; x++){
		      *wordIter = *lineIndex; //assign character in word
          wordIter++; //increment position in word to enter next character
          lineIndex++; //increment lineIndex
        }
        *wordIter = '\0'; //add null character to signify end of word
        *arr = word; //assign pointer in array to point to word
        arr++; //increment to look at next pointer in array
        charcounter = 0; //restart charcounter for next word
      }
      lineIndex++; //if encountering a space, increment index
    }
      lineIter++;

      if (charcounter > 0 && (*lineIter == '\0')){ //the last word ends with a NULL character, ex. "parse me."
        word = (char*) malloc((charcounter + 1)*(sizeof(char))); 
        wordIter = word;
        for (int x = 0; x < charcounter; x++){
		      *wordIter = *lineIndex; //assign character in word
          wordIter++; //increment position in word to enter next character
          lineIndex++; //increment lineIndex
        }
        *wordIter = '\0'; //add null character to signify end of word
        *arr = word;
        //printf("filling slot %d of array at location %p with %s\n",(int)(arr-array),arr,word);
      }
  }
  //printf("array ends at: %p\n", arr); 
  //printf("fifth string is %s %s %s %s\n",*(array),*(array+1),*(array+2), *(array+3));

  return array; 
}

/**
 * command_show:
 *
 * Print the structure of a command array.
 *
 * Parameters:
 *
 * - command: a non-NULL pointer to the command array to print.
 */
void command_show(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command!=NULL);

  // IMPLEMENT ME
  char* pointer = *command;
  printf("{"); 
  while (*pointer != '\0'){
    printf("\n  \"%d\",",*pointer); 
    pointer ++;
  }
  printf("  NULL\n}\n");
}

/**
 * command_free:
 *
 * Free all parts of a command array created by command_parse.
 *
 * Parameters:
 *
 * - command: non-NULL pointer to the command array to free.
 */
void command_free(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command!=NULL);
  int i = 0;
  while(*(command+i)!= NULL){ //*(command + i) is the pointer notation for command[i]
    free(*(command+i)); // free command[i] we created thru malloc 
    i++; //increment i to free each pointer 
  }
  free(command); //free the whole arrary 
  // IMPLEMENT ME

}
