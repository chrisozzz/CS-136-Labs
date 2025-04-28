#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
using namespace std;

const string FILE_ONE = "fileOne.txt";
const string FILE_TWO = "fileTwo.txt";
const string INPUT_ERROR = {"Invalid input! Only enter an option number."};
const int WIDTH = 15;

class Queue;
class Node;
class FileManager;

void clearError(string error);
void printMenu();

enum Menu{
    DETERMINE_SIMILARITY=1,
    DISPLAY_STATS,
    EXIT_PROGRAM
};


class Node {
    private:
        char character;
        Node *next;

    public:
        Node();
        Node(char c);

        void setChar(char c);
        void setNext(Node *node);

        char getChar() const;
        Node* getNext() const;
};

class Queue {
    private:
        Node *qFront;
        Node *qRear;

    public:
        const Queue& operator=(const Queue&);

        bool isEmpty() const;

        /*
        Reinitialize the queue to be empty
        Precondition: Queue object exists
        Postcondition: queue front and rear pointer point to nullptr
        */
        void initializeQueue();
        char front() const;
        char back() const;
        void enqueue(const char &c);
        void dequeue();

        Queue();
        Queue(const Queue &other);
        ~Queue();
};

class FileManager{
    private:
        string file1;
        string file2; 
        int letters1, digits1, specials1;
        int letters2, digits2, specials2;  
        bool isProcessed; 
    
        /*
        Process each file by counting the # of letters, digits, and special characters 
        and adding each character to the queue.
        Precondition: All the parameter variables are initialized
        Postcondition: Return true if the file was properly processed and determines the values of the parameter variables.
        */
        bool processFile(string file, Queue &queue, int &letters, int &digits, int &specials);
    public:
        FileManager(string f1, string f2);

        /*
        Determine if the files are idential
        Precondition: 'file1' and 'file2' are initialized
        Postcondition: Print out to the console if the files are identical
        */
        void processFiles();
        
        /*
        Display the total amount of letters, digits, and special characters in each file
        Precondition: 'processFiles()' is called and
        Postcondition: The # of letters, digits, and special character is printed to the console in a table
        */
        void compareFiles();
};

int main(){
    cout << left;
    int choice = 0;
    FileManager fManage(FILE_ONE, FILE_TWO);

    while(choice != EXIT_PROGRAM){
        printMenu();

        cin >> choice;

        //if user puts in "1asdfasdf", etc.
        if(cin.peek() != '\n'){ choice = 0; }

        switch(choice){
            case DETERMINE_SIMILARITY:
                fManage.processFiles();
                break;

            case DISPLAY_STATS:
                fManage.compareFiles();
                break;

            case EXIT_PROGRAM:
                cout << "\nGoodbye!\n";
                break;

            default:
                clearError(INPUT_ERROR);
        }
    }
}


void clearError(string error){
    cout << "\n"+error+"\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printMenu(){
    cout << "\n\nMain Menu:\n"
            "1 Process Files / Determine similarity\n"
            "2 Output stats for each file\n"
            "3 Exit program\n"
            "Enter your choice: ";
}


FileManager::FileManager(string f1, string f2){
    file1 = f1;
    file2 = f2;
    isProcessed = false;
    letters1 = digits1 = specials1 = 0;
    letters2 = digits2 = specials2 = 0; 
}

void FileManager::processFiles(){
    Queue q1, q2;

    if(processFile(file1, q1, letters1, digits1, specials1) && 
    processFile(file2, q2, letters2, digits2, specials2)){
        isProcessed = true;
        bool areTheSame = true;
        while(!q1.isEmpty() && !q2.isEmpty()){
            if(q1.front() != q2.front()){
                areTheSame = false;
            }
            q1.dequeue();
            q2.dequeue();
        }
        areTheSame = areTheSame && (q1.isEmpty() && q2.isEmpty());

        if(areTheSame){
            cout << "\n"+file1+" & "+file2+" are identical.\n"; }
        else{
            cout << "\n"+file1+" & "+file2+" are not identical.\n";
        }
    }
}

void FileManager::compareFiles(){
    if(isProcessed){
        cout << "\n" << setw(WIDTH) << "" << setw(WIDTH-1) <<"| "+file1+" " << setw(WIDTH-1)<<"| "+file2+" "<<"\n"
             << setw(3*WIDTH) << setfill('-') <<""<<"\n" << setfill(' ')
             << setw(WIDTH) << "# of letters" << setw(WIDTH-1) << "| "+to_string(letters1)+" " << setw(WIDTH-1) << "| "+to_string(letters2)+" " << "\n"
             << setw(WIDTH) << "# of digits" << setw(WIDTH-1) << "| "+to_string(digits1)+" " << setw(WIDTH-1) << "| "+to_string(digits2)+" " << "\n"   
             << setw(WIDTH) << "# of specials" << setw(WIDTH-1) << "| "+to_string(specials1)+" " << setw(WIDTH-1) << "| "+to_string(specials2)+" " << "\n";
    }else{
        cout << "\nProcess the files first!\n";
    }
}

bool FileManager::processFile(const string fileName, Queue &queue, int &letters, int &digits, int &specials){
    bool successful = true;
    ifstream file(fileName);
    if(file.fail()){
        cout << "Failed to open "+fileName+"\n"; 
        successful = false; }
    else{
        char character;
        while(file.get(character)){
            queue.enqueue(character);

            if(isalpha(character)){ letters++; }
            else if(isdigit(character)) { digits++; }
            else{ specials++; }
        }
        file.close();
    }
    return successful;
}



bool Queue::isEmpty() const{
    return (qFront == nullptr);
}

void Queue::initializeQueue(){
    Node *temp;
    while(qFront != nullptr){
        temp = qFront;
        qFront = qFront->getNext();
        delete temp;
    }
    qRear = nullptr;
    temp = nullptr;
}

void Queue::enqueue(const char &c){
    try {
        Node *newNode = new Node(c);

        if(qFront == nullptr){
            qFront = qRear = newNode; }
        else{
            qRear->setNext(newNode);
            qRear = qRear->getNext();
        }} 
    catch (const bad_alloc &e){
        cout << "\nMemory allocation for new Node failed. Clearing queue.\n";
        initializeQueue();
    }
}

char Queue::front() const{
    if(qFront != nullptr){ return qFront->getChar(); }
    else { return '\0'; }
}

char Queue::back() const{
    if(qRear != nullptr){ return qRear->getChar(); }
    else { return '\0'; }
}

void Queue::dequeue(){
    Node *temp;
    if(!isEmpty()){
        temp = qFront;
        qFront = qFront->getNext();
        delete temp;
        if(qFront == nullptr){ qRear = nullptr; }

    }else{
        cout << "Can't remove from empty queue\n";
    }
}

const Queue& Queue::operator=(const Queue &other){
    if(this != &other){
        initializeQueue();
        Queue temp(other);
        while(!temp.isEmpty()){
            this->enqueue(temp.front());
            temp.dequeue();
        }
    }
    return *this;
}

Queue::Queue(){
    qFront = qRear = nullptr;
}

Queue::Queue(const Queue &other){
    qFront = qRear = nullptr;

    Node *temp = other.qFront;
    while(temp){
        enqueue(temp->getChar());
        temp = temp->getNext();
    }
}

Queue::~Queue(){
    initializeQueue();
}


Node::Node(){
    setChar('\0');
    setNext(nullptr);
}

Node::Node(char c){
    setChar(c);
    setNext(nullptr);
}

void Node::setChar(char c){
    character = c;
}

void Node::setNext(Node *node){
    next = node;
}

char Node::getChar() const {
    return character;
}

Node* Node::getNext() const {
    return next;
}

// FILES ARE IDENTICAL: 
// Hello, this is a test file.
// Everything should be identical except for one letter.
// Let's see if the program can detect it!
// This line has an 'a'.
/*
Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: blah

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 4

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 0

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 1blah

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 1 2

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 2

Process the files first!


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 1

fileOne.txt & fileTwo.txt are identical.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 2

               | fileOne.txt | fileTwo.txt 
---------------------------------------------
# of letters   | 109         | 109         
# of digits    | 0           | 0           
# of specials  | 34          | 34          


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 3

Goodbye!
*/

// FILES ARE DIFFERENT: 
// fileOne.txt:

// Hello, this is a test file.
// Everything should be identical except for one letter.
// Let's see if the program can detect it!
// This line has an 'a'.

// fileTwo.txt:

// Hello, this is a test file.
// Everything should be identical except for one letter.
// Let's see if the program can detect it!
// This line has an 'o'. blah blah blah
/*
Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: blah

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 1 wrong

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 2wrong

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 4

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 0

Invalid input! Only enter an option number.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 2

Process the files first!


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 1

fileOne.txt & fileTwo.txt are not identical.


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 2

               | fileOne.txt | fileTwo.txt 
---------------------------------------------
# of letters   | 109         | 121         
# of digits    | 0           | 0           
# of specials  | 34          | 37          


Main Menu:
1 Process Files / Determine similarity
2 Output stats for each file
3 Exit program
Enter your choice: 3

Goodbye!
*/