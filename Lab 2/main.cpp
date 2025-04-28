// Christopher Osgood
// Lab #02 RE-DO

#include <iostream>
using namespace std;
const string ERROR_STRINGS[] = {"Invalid! Only enter an option number", 
                                "Invalid! Must only be a positive whole number greater than 0."};

enum class MENU_OPTIONS{
    NO_MENU,
    SEQUENCE_ONE,
    SEQUENCE_TWO,
    EXIT
};

enum ERRORS{
    INVALID_MENU_INPUT,
    INVALID_SEQUENCE_INPUT
};


void printMenu();
void clearError(string error);
int getValidIntegerInput();
void calculateSequenceOne(int numTerms, int currTerm=1, double sum=1);
void calculateSequenceTwo(int numTerms, int currTerm=1, int sum=1, bool firstIteration=true);



int main(){
    MENU_OPTIONS selectedMenu = MENU_OPTIONS::NO_MENU;
    string errorMessage;
    int intSelectedMenu;

    while(selectedMenu != MENU_OPTIONS::EXIT){
        intSelectedMenu = 0;
        errorMessage = "";

        printMenu();

        cin >> intSelectedMenu;
        selectedMenu = static_cast<MENU_OPTIONS>(intSelectedMenu);

        if(cin.fail() || cin.peek() != '\n' || 
           intSelectedMenu < static_cast<int>(MENU_OPTIONS::SEQUENCE_ONE) || 
           intSelectedMenu > static_cast<int>(MENU_OPTIONS::EXIT)){
            selectedMenu = MENU_OPTIONS::NO_MENU; 
            errorMessage = ERROR_STRINGS[INVALID_MENU_INPUT]; }


        switch (selectedMenu){
            case MENU_OPTIONS::SEQUENCE_ONE:
                calculateSequenceOne(getValidIntegerInput());;
                break;

            case MENU_OPTIONS::SEQUENCE_TWO:
                calculateSequenceTwo(getValidIntegerInput());
                break;

            case MENU_OPTIONS::EXIT:
                cout << "\nGoodbye!\n";
                break;

            default:
                clearError(errorMessage);
        }
    }
}

/*
Clears errors and input buffer from cin
Precondition: An error is passed as a string
Postcondition: Error and buffer are cleared
*/
void clearError(string error){
    cout << "\n"+error+"\n";
    cin.clear();
    cin.ignore(100, '\n');
}

/*
Prints the menu
Postcondition: Outputs menu to the console.
*/
void printMenu(){
    cout << "\n\n\nWhich sequence do you want?\n"
                "1 Σ[1/((n-1)+n)]\n"
                "2 Σ[n*n]\n"
                "3 Exit\n"
                "Enter your choice: ";
}

/*
Prompts the user for a valid positive integer input greater than 0.
Precondition: input must be greater than 0 to exit the loop.
Postcondition: Returns a valid positive integer greater than 0.
*/
int getValidIntegerInput(){
    int number = 0;
    while(number <= 0){
        cout << "\nHow many terms: ";
        cin >> number;
        if(cin.fail() || cin.peek() != '\n' || number <= 0){
            clearError(ERROR_STRINGS[INVALID_SEQUENCE_INPUT]);
            number = 0;
        }
    }
    
    return number;
}

/*
Prints out the sequence Σ[1/((n-1)+n)] and the sum to the console.
Precondition: numTerms must be a positive integer (>= 1)
Postcondition: Prints the sequence term by term and its sum to the console.
*/
void calculateSequenceOne(int numTerms, int currTerm, double sum){
    if(numTerms == 1 || currTerm == 1){ // Print '1' no matter how many terms.
        cout << "For n="<<numTerms<<" 1";

        if(numTerms > 1){ // If more than 1 term, then call recursion.
            
            calculateSequenceOne(numTerms, currTerm+1, sum);
        }
    }else{ //recursive call
        if(currTerm < numTerms){ // if not at final term, add to the sequence
            cout << "+1/("<<currTerm-1<<"+"<<currTerm<<")";
            sum += 1.0/(2*currTerm-1);
            calculateSequenceOne(numTerms, currTerm+1, sum);

        }else if(currTerm == numTerms){ // if at final term, finish sequence 
            sum += 1.0/(2*currTerm-1);
            cout << "+1/("<<currTerm-1<<"+"<<currTerm<<")="<<sum<<"\n";
        }
    }
}

/*
Prints out the sequence Σ[n*n] and the sum to the console.
Precondition: numTerms is greater than or equal to 1.
Postcondition: Print the sequence term by term and its sum to the console.
*/
void calculateSequenceTwo(int numTerms, int currTerm, int sum, bool firstIteration){
    //If we want just one term, there is no "="
    if(numTerms == 1 && currTerm == 1 && firstIteration){
        cout << "For n=1 1\n";
    }else if(numTerms == 1){ // base case (more than 1 term)
        cout << "1=" << sum << "\n";
    }else{
        if(firstIteration){
            cout << "For n="<<numTerms<<" ";
        }
        cout << "("<<numTerms<<"*"<<numTerms<<")"<<"+";
        calculateSequenceTwo(numTerms-1, currTerm+=1,sum+=numTerms*numTerms, false);
    }
}


//OUTPUT w/ TESTS
/*
Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: a

Invalid! Only enter an option number



Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: MANY LETTERS

Invalid! Only enter an option number



Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: -4

Invalid! Only enter an option number



Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: 0

Invalid! Only enter an option number



Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: 10

Invalid! Only enter an option number



Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: 1

How many terms: a

Invalid! Must only be a positive whole number greater than 0.

How many terms: alpha

Invalid! Must only be a positive whole number greater than 0.

How many terms: -2

Invalid! Must only be a positive whole number greater than 0.

How many terms: 0

Invalid! Must only be a positive whole number greater than 0.

How many terms: 5
For n=5 1+1/(1+2)+1/(2+3)+1/(3+4)+1/(4+5)=1.7873



Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: 2

How many terms: a   

Invalid! Must only be a positive whole number greater than 0.

How many terms: beta

Invalid! Must only be a positive whole number greater than 0.

How many terms: -8

Invalid! Must only be a positive whole number greater than 0.

How many terms: 0

Invalid! Must only be a positive whole number greater than 0.

How many terms: 8
For n=8 (8*8)+(7*7)+(6*6)+(5*5)+(4*4)+(3*3)+(2*2)+1=204



Which sequence do you want?
1 Σ[1/((n-1)+n)]
2 Σ[n*n]
3 Exit
Enter your choice: 3

Goodbye!
*/