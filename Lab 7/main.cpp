// Christopher Osgood
// Lab #07

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

const string FILE_NAME = "students.txt";
const int EXTRACT_FACTOR = 10'000'000;
const int BOUNDARY_FACTOR = 10'000;
const string ERRORS_STRINGS[] = {"Invalid table size. Please enter 10, 100, or 1000.",
                                  "Invalid input. Please try again."};

enum MainMenu{
    SEARCH_ID=1,
    ARCHIVE_STUDENT,
    PRINT_TABLE,
    PRINT_DELETED,
    PRINT_UNPROCESSED,
    EXIT
};
enum Errors{
    INVALID_TABLE_SIZE,
    INVALID_INPUT
};

class HashTable;
class Student;

Student* readFile(string fileName, int tableSize);

void printMainMenu();
void clearError(string error);

int extractLast7Digits(int num);
int BoundaryFold(int num);
int flip(int num);
int getHashKey(int value, int tableSize);

class Student{
    private: 
        string firstName = "n/a";
        string lastName = "n/a";
        int studentID = -1;
        int numCredits = -1;
        bool archived = false;

    public:
        Student(){}
        Student(string studentInfo){ setStudentInfo(studentInfo); }

        void setStudentInfo(string studentInfo);

        string getFirstName(){ return firstName; }
        string getLastName(){ return lastName; }
        int getStudentID(){ return studentID; }
        int getNumCredits(){ return numCredits; }
        bool isArchived(){ return archived; }

        void toString(){
            cout << "\nStudent ID: " << studentID
                 << "\nFirst Name: " << firstName
                 << "\nLast Name: " << lastName
                 << "\nNumber of Credits: " << numCredits << endl;
        }
};
class HashTable{
    private:
        Student hashTable[] = {Student()};
        Student overflowTable[] = {Student()};
        int tableSize = 0;
        int overflowTableSize = 0;

    public:

        void setTableSize(int size);
        void createTable();

        bool addStudent(const Student& student);
        void loadFromFile(string file);
        void searchStudent(int ID);
        void archiveStudent(int ID);
        void printStudents();


        HashTable(){}
        HashTable(int tableSize);

};

int main(){
    int tableSize = 0;
    
    while(tableSize != 10 && tableSize != 100 && tableSize != 1000){
        cout << "Enter the size of the hash table (10, 100, 1000): ";
        cin >> tableSize;

        if(cin.peek() != '\n'){
            tableSize = 0; }

        switch(tableSize){
            case 10:
            case 100:
            case 1000:
                break;
            default:
                clearError(ERRORS_STRINGS[INVALID_TABLE_SIZE]);
        }
    }
    int overflowTableSize = tableSize * 0.2;


    int mainMenu = 0;

    while(mainMenu != EXIT){
        printMainMenu();
        cin >> mainMenu;

        if(cin.peek() != '\n'){
            mainMenu = 0; }

        switch(mainMenu){
            case SEARCH_ID:
                cout << "Searching for a student by ID..." << endl;
                break;

            case ARCHIVE_STUDENT:
                cout << "Archiving a student..." << endl;
                break;

            case PRINT_TABLE:
                cout << "Printing the hash table..." << endl;
                break;  

            case PRINT_DELETED:
                cout << "Printing the deleted students..." << endl;
                break;

            case PRINT_UNPROCESSED:
                cout << "Printing the unprocessed students..." << endl;
                break;

            case EXIT:
                cout << "Exiting the program..." << endl;
                break;

            default:
                clearError(ERRORS_STRINGS[INVALID_INPUT]);
        }
    }


    Student* students = nullptr;
    students = readFile(FILE_NAME, tableSize);
}

void clearError(string error){
    cout << "\n"+error+"\n" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void printMainMenu(){
    cout << "\nMain Menu\n"
            "1 Search for a student by ID\n"
            "2 Archive a student\n"
            "3 Print the hash table\n"
            "4 Print the deleted students\n"
            "5 Print the unprocessed students\n"
            "6 Exit\n"
            "Enter your choice: ";
}


void HashTable::createTable(){
    hashTable = new Student[tableSize];
    overflowTable = new Student[overflowTableSize];
}
HashTable::HashTable(int tableSize){
    setTableSize(tableSize);
    createTable();
}



Student* readFile(string fileName, int tableSize){
    ifstream inFile(fileName);
    Student* students = nullptr;

    if(!inFile){
        cout << "Error opening file." << endl; }
    else{

        string studentInfo;
        int index = 0;
        students = new Student[tableSize];

        while(getline(inFile, studentInfo) && index < tableSize){
            students[index].setStudentInfo(studentInfo);
            int hashKey = getHashKey(students[index].getStudentID(), tableSize);
            students[index].toString();
            cout << "Hash Key: " << hashKey << endl;
            index++;
        }
        inFile.close();
    }
    return students;
}

int extractLast7Digits(int num){
    return num % EXTRACT_FACTOR;
}
int BoundaryFold(int num){
    int firstHalf = num / BOUNDARY_FACTOR;
    int secondHalf = num % BOUNDARY_FACTOR;

    return flip(firstHalf) + flip(secondHalf);
}
int flip(int num){
    int flippedNum = 0;
    while(num > 0){
        flippedNum = flippedNum * 10 + num % 10;
        num /= 10;
    }
    return flippedNum;
}
int getHashKey(int value, int tableSize){
    int last7Digits = extractLast7Digits(value);
    int boundaryFoldedNum = BoundaryFold(last7Digits);
    int boundarySquared = boundaryFoldedNum * boundaryFoldedNum;
    string num = to_string(boundarySquared);
    int middleIndex = (num.length()-1) / 2;
    int hashKey = -1;

    switch(tableSize){
        case 10:
            hashKey = stoi(num.substr(middleIndex, 1));
            break;
        case 100:
            hashKey = stoi(num.substr(middleIndex, 2));
            break;
        case 1000:
            hashKey = stoi(num.substr(middleIndex, 3));
            break;
    }

    return hashKey;
}


void Student::setStudentInfo(string studentInfO){
    istringstream iss(studentInfO);
    iss >> studentID >> firstName >> lastName >> numCredits;
}