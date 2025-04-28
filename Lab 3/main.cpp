#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "stackArray.h"
using namespace std;
const string DELIMITERS[] = {"(", ")", "[" ,"]" ,"{","}", "<!--","-->", 
                             "/*", "*/", "\"", "\"", "'", "'"};
const string STATUS[] = {"Unbalanced", "Balanced"};
const string INPUT_FILE = "in_file.txt";
const int EXPRESSION_SIZE = 60;
const int SYMBOL_SIZE = 40;
const int STATUS_SIZE = 30;

enum Delimiters{
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
    OPEN_SQUARE_BRACKET,
    CLOSED_SQUARE_BRACKET,
    OPEN_BRACKET,
    CLOSED_BRACKET,
    OPEN_HTML_TAG,
    CLOSED_HTML_TAG,
    OPEN_COMMENT,
    CLOSED_COMMENT,
    OPEN_STRING,
    CLOSED_STRING,
    OPEN_CHAR,
    CLOSED_CHAR
};
enum optionChoices{
    PROCESS_EXPRESSIONS=1,
    EXIT
};


void getValidOption(int &choice);
void processFile();
bool processExpression(string exp, string &symbols, string &error);
bool isOpenSymbol(string exp);
bool isClosedSymbol(string exp, Delimiters &delim);
string determineMultiDelim(string character, string expression, int &i);

int main(){
    int choice;
    while(choice != EXIT){
        choice = 0;
        cout << left; //left justify
        cout << "\n\n\nWhich option do you want?\n"
                "1 Process all expressions\n"
                "2 Exit\n";
        getValidOption(choice); //get valid input
        
        switch(choice){
            case 1:
                processFile();        
                break;
            case 2:
                cout << "Goodbye!\n";
                break;
        }
    }
}

/*
Reads expression from input file and prints in formatted table
Precondition: The input file exists and is accessible
Postcondition: Each expression is analyzed and printed in the table.
*/
void processFile(){
    ifstream inFile(INPUT_FILE);
    string expression;
    string expressionStatus;
    bool isBalanced;


    if(inFile.fail()){
        cout << "\nFailed to open input file\n";

    }else{
        // Format table
        cout << left << setw(STATUS_SIZE) << "| Status"
             << setw(SYMBOL_SIZE) << "| Symbols"
             << setw(EXPRESSION_SIZE) << "| Expression" << "\n"
             << setw(STATUS_SIZE+SYMBOL_SIZE+EXPRESSION_SIZE) << setfill('-') << "" << setfill(' ') << "\n";

        // Iterate through expression
        while(getline(inFile, expression)){
            string symbols, errorMessage;
            isBalanced = processExpression(expression, symbols, errorMessage);
            expressionStatus = STATUS[static_cast<int>(isBalanced)];

            if(!isBalanced){
                if(!errorMessage.empty()){
                    expressionStatus = errorMessage;
                }
                symbols.clear();
            }

            cout << setw(STATUS_SIZE) << "| "+expressionStatus
                 << setw(SYMBOL_SIZE) << "| "+symbols
                 << setw(EXPRESSION_SIZE) << "| "+expression << "\n";
        }
        inFile.close();
    }
    
}

/*
Checks if an expression is balanced using a stack
Precondition: 'expression' is line from the input file
Postcondition: returns true if 'expression' is balanced. updates symbols with pairs. updates errors if has error
*/
bool processExpression(string expression, string &symbols, string &error){
    stackArray stack;
    string character;
    Delimiters delim;
    bool charInStack = false, quoteInStack = false;
    int charDistance = 0;
    bool currCharIsOpen = true;
    bool currStringIsOpen = true;

    if(expression.size() <= EXPRESSION_SIZE){
        for(int i = 0; i < expression.size(); i++){
            character = expression[i];

            character = determineMultiDelim(character, expression, i);

            //If there is a char delimiter in the stack increase the distance from the open delimiter
            if(charInStack){
                charDistance++;
            }
            
            bool isChar = (character == DELIMITERS[OPEN_CHAR]);
            bool isString = (character == DELIMITERS[OPEN_STRING]);
            bool goThroughOpenCh, goThroughOpenStr; //If we need to check it's the open symbol

            if((currCharIsOpen && isChar) || (currCharIsOpen && !isChar)){
                goThroughOpenCh = true;
            }else if(!currCharIsOpen && isChar){
                goThroughOpenCh = false;
            }
            if((currStringIsOpen && isString) || (currStringIsOpen && !isString)){
                goThroughOpenStr = true;
            }else if(!currStringIsOpen && isString){
                goThroughOpenStr = false;
            }

            if(isOpenSymbol(character) && (goThroughOpenCh && goThroughOpenStr)){
            
                if(isChar && !charInStack){
                    charInStack = true;
                    currCharIsOpen = false;
                    stack.push(character);
                }else if(isString && !quoteInStack){
                    quoteInStack = true; 
                    currStringIsOpen = false;
                    stack.push(character);
                }else if(!isChar && !isString){
                    stack.push(character);
                }
            }else if(isClosedSymbol(character, delim)){
                string stackTopDelim = stack.top();
                string closedDelim = DELIMITERS[delim-1];

                if(charInStack){ //If char is in stack, check if is '' or 'X' (aka valid char)
                    if(charDistance <= 2){
                        charInStack = false;
                        goThroughOpenCh = true;
                        charDistance = 0;
                    }else{
                        error += "More than 1 character in ''";
                        stack.push(character);
                        charInStack = false; //More than 1 in char so just ignore the one in the stack.
                    }
                }

                if(quoteInStack && !goThroughOpenStr){ //If quote is in stack and it's the open quote, reset
                    quoteInStack = false; 
                    goThroughOpenStr = true;
                }

                if(stackTopDelim == closedDelim){ 
                    symbols.insert(0, stackTopDelim+" ");
                    symbols.append(character+" ");
                    stack.pop();

                }else{ //If no corresponding symbol, add to stack so it's not empty
                    stack.push(character);
                }
            }
        }
    }else{
        error += "Expression is too long";
        stack.push(""); // push something on the stack so it's not balanced;
    }

    return (stack.isEmpty());
}

/*
Determines multi-character delimiters and updates the index
Precondition: 'character' is a single character from 'expression' and 'i' is a valid index
Postcondition: Returns the correct delimiter and updates i;
*/
string determineMultiDelim(string character, string expression, int &i){
    string delim = character;

    if(character == "<" && expression.substr(i, 4) == DELIMITERS[OPEN_HTML_TAG]){
        delim = DELIMITERS[OPEN_HTML_TAG]; 
        i+=3; 


    }else if(character == ">" && expression.substr(i-2, 3) == DELIMITERS[CLOSED_HTML_TAG]){
        delim = DELIMITERS[CLOSED_HTML_TAG];


    }else if(character == "/" && expression.substr(i,2) == DELIMITERS[OPEN_COMMENT]){
        delim = DELIMITERS[OPEN_COMMENT];
        i++;


    }else if(character == "*" && expression.substr(i, 2) == DELIMITERS[CLOSED_COMMENT]){
        delim = DELIMITERS[CLOSED_COMMENT];
        i++;
    }

    return delim;
}

/*
Checks if a string represents an opening delimiter
Precondition: 'exp' is a string containing a single symbol
Postcondition: return true if 'exp' is an opening delim, false otherwise
*/
bool isOpenSymbol(string exp){
    return (exp == DELIMITERS[OPEN_BRACKET] || exp == DELIMITERS[OPEN_SQUARE_BRACKET] ||
           exp == DELIMITERS[OPEN_HTML_TAG] || exp == DELIMITERS[OPEN_COMMENT] ||
           exp == DELIMITERS[OPEN_PARENTHESIS] || exp == DELIMITERS[OPEN_CHAR] ||
           exp == DELIMITERS[OPEN_STRING]);
}

/*
Checks if given string 'exp' is a closing delimeter and assigns 'delim' the correct value
Precondition: 'exp' is a string containing the correct symbols
Postcondition: returns true is exp is a closing delimiter and assings it to 'delim'
*/
bool isClosedSymbol(string exp, Delimiters &delim){
    bool isClosed = true;
    if(exp == DELIMITERS[CLOSED_BRACKET]){
        delim = CLOSED_BRACKET;
    }else if(exp == DELIMITERS[CLOSED_SQUARE_BRACKET]){
        delim = CLOSED_SQUARE_BRACKET;
    }else if(exp == DELIMITERS[CLOSED_HTML_TAG]){
        delim = CLOSED_HTML_TAG;
    }else if(exp == DELIMITERS[CLOSED_COMMENT]){
        delim = CLOSED_COMMENT;
    }else if(exp == DELIMITERS[CLOSED_PARENTHESIS]){
        delim = CLOSED_PARENTHESIS;
    }else if(exp == DELIMITERS[CLOSED_CHAR]){
        delim = CLOSED_CHAR;
    }else if(exp == DELIMITERS[CLOSED_STRING]){
        delim = CLOSED_STRING;
    }else{
        isClosed = false;
    }
    return isClosed;
}

/*
Promps user for a valid menu choice (1 or 2) and updates 'choice'
Precondition: 'choice' is an integer reference
Postcondition: 'choice' is set to either PROCESS_EXPRESSIONS(1) or EXIT(2)
*/
void getValidOption(int &choice){ 
    string input;
    while(choice != PROCESS_EXPRESSIONS && choice != EXIT){
        cout << "Enter your options: ";
        getline(cin, input);
        istringstream iss(input);
        iss >> choice;
        if(iss.fail() || !iss.eof() || (choice != PROCESS_EXPRESSIONS && choice != EXIT)){
            cout << "\nPlease only input either '1' or '2'!\n\n";
            choice = 0;
        }
    }
}


// Put the tags with /* & */ at end so they didn't interfere.
/*
Which option do you want?
1 Process all expressions
2 Exit
Enter your options: blah 

Please only input either '1' or '2'!

Enter your options: all options

Please only input either '1' or '2'!

Enter your options: 1!

Please only input either '1' or '2'!

Enter your options: 2!!!!

Please only input either '1' or '2'!

Enter your options: GIVE ME 1!!!!!!!!

Please only input either '1' or '2'!

Enter your options: 2190381203821

Please only input either '1' or '2'!

Enter your options: 1 1 1 111 

Please only input either '1' or '2'!

Enter your options: 1
| Status                      | Symbols                               | Expression                                                
----------------------------------------------------------------------------------------------------------------------------------
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} '{done}"                    
| Balanced                    | ( { } )                               | (hkj   hkj{hj  shj})                                      
| Unbalanced                  |                                       | (hkj   hkj{)hj  shj}                                      
| Balanced                    | [ { ( ) } ]                           | [abc {def (ghi)}]                                         
| Unbalanced                  |                                       | [abc {def (ghi])}                                         
| Balanced                    | " "                                   | "Hello"                                                   
| Balanced                    | " "                                   | "Unclosed string"                                         
| More than 1 character in '' |                                       | 'XXXX'                                                    
| Balanced                    | ' '                                   | 'Y'                                                       
| Balanced                    | ( [ { } ] )                           | /( [ { } ] )                                              
| Unbalanced                  |                                       | ( [ { ] } )                                               
| Balanced                    | " { [ ( ' ' ) ] } "                   | "Balanced 'A' (test) [check] {done}"                      
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} {done}"                     
| Balanced                    | <!-- -->                              | <!--HELLLO-->                                             
| Unbalanced                  |                                       | <!--wRONG                                                 
| Unbalanced                  |                                       | -->LSDKFJ                                                 
| Unbalanced                  |                                       | )                                                         
| Unbalanced                  |                                       | }                                                         
| Balanced                    | " ' ' "                               | "'X'"                                                     
| Balanced                    | " "                                   | ""                                                        
| Balanced                    | ' '                                   | ''                                                        
| Balanced                    | " " " "                               | """"                                                      
| Balanced                    | ' ' ' '                               | ''''                                                      
| Unbalanced                  |                                       | '"'"                                                      
| Balanced                    | ' ' ' '                               | 'A''A'                                                    
| Balanced                    | <!-- <!-- --> -->                     | <!--<!--AA-->-->                                          
| Balanced                    |                                       | AAAAAAAAAAAA                                              
| Balanced                    | " " " " " " " " " " " " " "           | """"""""""""""                                            
| Balanced                    | ' ' ' ' ' ' ' ' ' ' ' '               | ''''''''''''                                              
| Unbalanced                  |                                       | """""""""""""""                                           
| Unbalanced                  |                                       | '''''''''''''                                             



Which option do you want?
1 Process all expressions
2 Exit
Enter your options: 1
| Status                      | Symbols                               | Expression                                                
----------------------------------------------------------------------------------------------------------------------------------
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} '{done}"                    
| Balanced                    | ( { } )                               | (hkj   hkj{hj  shj})                                      
| Unbalanced                  |                                       | (hkj   hkj{)hj  shj}                                      
| Balanced                    | [ { ( ) } ]                           | [abc {def (ghi)}]                                         
| Unbalanced                  |                                       | [abc {def (ghi])}                                         
| Balanced                    | " "                                   | "Hello"                                                   
| Balanced                    | " "                                   | "Unclosed string"                                         
| More than 1 character in '' |                                       | 'XXXX'                                                    
| Balanced                    | ' '                                   | 'Y'                                                       
| Balanced                    | ( [ { } ] )                           | /( [ { } ] )                                              
| Unbalanced                  |                                       | ( [ { ] } )                                               
| Balanced                    | " { [ ( ' ' ) ] } "                   | "Balanced 'A' (test) [check] {done}"                      
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} {done}"                     
| Balanced                    | <!-- -->                              | <!--HELLLO-->                                             
| Unbalanced                  |                                       | <!--wRONG                                                 
| Unbalanced                  |                                       | -->LSDKFJ                                                 
| Unbalanced                  |                                       | )                                                         
| Unbalanced                  |                                       | }                                                         
| Balanced                    | " ' ' "                               | "'X'"                                                     
| Balanced                    | " "                                   | ""                                                        
| Balanced                    | ' '                                   | ''                                                        
| Balanced                    | " " " "                               | """"                                                      
| Balanced                    | ' ' ' '                               | ''''                                                      
| Unbalanced                  |                                       | '"'"                                                      
| Balanced                    | ' ' ' '                               | 'A''A'                                                    
| Balanced                    | <!-- <!-- --> -->                     | <!--<!--AA-->-->                                          
| Balanced                    |                                       | AAAAAAAAAAAA                                              
| Balanced                    | " " " " " " " " " " " " " "           | """"""""""""""                                            
| Balanced                    | ' ' ' ' ' ' ' ' ' ' ' '               | ''''''''''''                                              
| Unbalanced                  |                                       | """""""""""""""                                           
| Unbalanced                  |                                       | '''''''''''''                                             



Which option do you want?
1 Process all expressions
2 Exit
Enter your options: 1
| Status                      | Symbols                               | Expression                                                
----------------------------------------------------------------------------------------------------------------------------------
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} '{done}"                    
| Balanced                    | ( { } )                               | (hkj   hkj{hj  shj})                                      
| Unbalanced                  |                                       | (hkj   hkj{)hj  shj}                                      
| Balanced                    | [ { ( ) } ]                           | [abc {def (ghi)}]                                         
| Unbalanced                  |                                       | [abc {def (ghi])}                                         
| Balanced                    | " "                                   | "Hello"                                                   
| Balanced                    | " "                                   | "Unclosed string"                                         
| More than 1 character in '' |                                       | 'XXXX'                                                    
| Balanced                    | ' '                                   | 'Y'                                                       
| Balanced                    | ( [ { } ] )                           | /( [ { } ] )                                              
| Unbalanced                  |                                       | ( [ { ] } )                                               
| Balanced                    | " { [ ( ' ' ) ] } "                   | "Balanced 'A' (test) [check] {done}"                      
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} {done}"                     
| Balanced                    | <!-- -->                              | <!--HELLLO-->                                             
| Unbalanced                  |                                       | <!--wRONG                                                 
| Unbalanced                  |                                       | -->LSDKFJ                                                 
| Unbalanced                  |                                       | )                                                         
| Unbalanced                  |                                       | }                                                         
| Balanced                    | " ' ' "                               | "'X'"                                                     
| Balanced                    | " "                                   | ""                                                        
| Balanced                    | ' '                                   | ''                                                        
| Balanced                    | " " " "                               | """"                                                      
| Balanced                    | ' ' ' '                               | ''''                                                      
| Unbalanced                  |                                       | '"'"                                                      
| Balanced                    | ' ' ' '                               | 'A''A'                                                    
| Balanced                    | <!-- <!-- --> -->                     | <!--<!--AA-->-->                                          
| Balanced                    |                                       | AAAAAAAAAAAA                                              
| Balanced                    | " " " " " " " " " " " " " "           | """"""""""""""                                            
| Balanced                    | ' ' ' ' ' ' ' ' ' ' ' '               | ''''''''''''                                              
| Unbalanced                  |                                       | """""""""""""""                                           
| Unbalanced                  |                                       | '''''''''''''                                             



Which option do you want?
1 Process all expressions
2 Exit
Enter your options: alsdkfj1

Please only input either '1' or '2'!

Enter your options: 1
| Status                      | Symbols                               | Expression                                                
----------------------------------------------------------------------------------------------------------------------------------
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} '{done}"                    
| Balanced                    | ( { } )                               | (hkj   hkj{hj  shj})                                      
| Unbalanced                  |                                       | (hkj   hkj{)hj  shj}                                      
| Balanced                    | [ { ( ) } ]                           | [abc {def (ghi)}]                                         
| Unbalanced                  |                                       | [abc {def (ghi])}                                         
| Balanced                    | " "                                   | "Hello"                                                   
| Balanced                    | " "                                   | "Unclosed string"                                         
| More than 1 character in '' |                                       | 'XXXX'                                                    
| Balanced                    | ' '                                   | 'Y'                                                       
| Balanced                    | ( [ { } ] )                           | /( [ { } ] )                                              
| Unbalanced                  |                                       | ( [ { ] } )                                               
| Balanced                    | " { [ ( ' ' ) ] } "                   | "Balanced 'A' (test) [check] {done}"                      
| More than 1 character in '' |                                       | "Unbalanced 'A (test] [check} {done}"                     
| Balanced                    | <!-- -->                              | <!--HELLLO-->                                             
| Unbalanced                  |                                       | <!--wRONG                                                 
| Unbalanced                  |                                       | -->LSDKFJ                                                 
| Unbalanced                  |                                       | )                                                         
| Unbalanced                  |                                       | }                                                         
| Balanced                    | " ' ' "                               | "'X'"                                                     
| Balanced                    | " "                                   | ""                                                        
| Balanced                    | ' '                                   | ''                                                        
| Balanced                    | " " " "                               | """"                                                      
| Balanced                    | ' ' ' '                               | ''''                                                      
| Unbalanced                  |                                       | '"'"                                                      
| Balanced                    | ' ' ' '                               | 'A''A'                                                    
| Balanced                    | <!-- <!-- --> -->                     | <!--<!--AA-->-->                                          
| Balanced                    |                                       | AAAAAAAAAAAA                                              
| Balanced                    | " " " " " " " " " " " " " "           | """"""""""""""                                            
| Balanced                    | ' ' ' ' ' ' ' ' ' ' ' '               | ''''''''''''                                              
| Unbalanced                  |                                       | """""""""""""""                                           
| Unbalanced                  |                                       | '''''''''''''                                             



Which option do you want?
1 Process all expressions
2 Exit
Enter your options: 2
Goodbye!
*/
// Expressions with /* & */
//| Expression is too long      |                                       | /* Nested properly in this case here and it exceeds the limit */
//| Balanced                    | <!-- /* [ { ( ) } ] */ -->            | <!--/*{ Hello this expression (is 60 character long)}[]*/-->
//| Balanced                    | /* /* /* */ */ */                     | /*/*/*AA*/*/*/                                            
//| Balanced                    | <!-- " /* ( { [ ' ' ] } ) */ " -->    | <!--"/*({['A']})*/"-->                                    
//| Unbalanced                  |                                       | /*wRONG                                                   
//| Unbalanced                  |                                       | */WRONG                                                   
//| Balanced                    | /* */                                 | /*HELLO*/                                                 
