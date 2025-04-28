// Christopher Osgood
// Lab 01

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

const string INPUT_FILE_NAME = "in_cars.txt";
const string ERROR_FILE_NAME = "err_cars.txt";
const int MAX_FILE_SIZE = 100;
const int REQUIRED_ID_SIZE = 8;
const int MIN_MODEL_ID_LENGTH = 3;
const char DISALLOWED_LETTER = 'O';
const char DISALLOWED_NUMBER = '0';
const double MIN_CAR_PRICE = 12'999.00;
const string carRecordErrors[7] = {", ID is not 8 characters long", ", First 4 characters in the ID are not all letters or has O",
                                   ", Last 4 characters in the ID are not all numeric or has 0", ", Model is less than 3 characters",
                                   ", Model is not only alphanumeric or starts with number", ", Quantity is less than 0", ", Price is less than or equal to 12,999.00"};

enum CarRecordInfo{
    CAR_ID,
    CAR_MODEL,
    CAR_QUANTITY,
    CAR_PRICE
};

enum MenuOptions{
    SEE_VALID_RECORDS = 1,
    SEE_INVALID_RECORDS = 2,
    EXIT = 3
};

enum CarRecordErrors{
    ID_LENGTH_ERROR,
    ID_FIRST_FOUR_NOT_ALPHA_OR_O_ERROR,
    ID_LAST_FOUR_NOT_NUMERIC_OR_0_ERROR,
    MODEL_LENGTH_ERROR,
    MODEL_CHARACTER_ERROR,
    QUANTITY_ERROR,
    PRICE_ERROR
};

struct CarRecord {
    string carID;
    string carModel;
    int carQuantity;
    double carPrice;

    CarRecord() {}

    CarRecord(string ID, string model, int quant, double price){
        carID = ID;
        carModel = model;
        carQuantity = quant;
        carPrice = price;
    }
};

void readFile(CarRecord[], int&);
void printValidCarRecords(CarRecord[], int);
void printInvalidCarRecords();

CarRecord createCarRecord(string);
bool isValidRecord(CarRecord, string &);
bool isValidCarID(string, string &);
bool isValidFirstHalfCarID(string);
bool isValidSecondHalfCarID(string);
bool isNumber(char);
bool isValidCarModel(string, string &);
bool isValidQuantity(int, string &);
bool isValidPrice(double, string &);


int main(){
    CarRecord validCarRecords[MAX_FILE_SIZE];
    int numValidCars = 0;
    readFile(validCarRecords, numValidCars);
    int menuChoice;
    bool inMenu = true;

    while(inMenu){
        cout << "\nWhat would you like to do: \n" 
             << "1. See all valid cars\n"
             << "2. See all invalid cars\n"
             << "3. Exit\n"
             << "Enter a number (1-3): ";
        cin >> menuChoice;
        switch (menuChoice){
            case SEE_VALID_RECORDS:
                printValidCarRecords(validCarRecords, numValidCars);
                break;
            case SEE_INVALID_RECORDS:
                printInvalidCarRecords();
                break;
            case EXIT:
                cout << "Goodbye!";
                inMenu = false;
                break;
        }
    }
}

/*
Reads the input file and stores valid Car records in an array while put invalid ones in the error text file.
Precondition: The input file exists and is formatted correctly.
Postcondition: Valid records are stored in the array, and invalid records are written to the error file.
*/
void readFile(CarRecord cars[], int &index){
    ifstream inputFile;
    ofstream errorFile;

    string tempCarData;
    CarRecord tempCarRecord;

    inputFile.open(INPUT_FILE_NAME);
    errorFile.open(ERROR_FILE_NAME);

    if (inputFile.fail()){
        cout << "\nInput file not found!";
    } else if (errorFile.fail()){
        cout << "\nError file not found!";
    } else {
        while(getline(inputFile, tempCarData)){
            string errorMessage = "";
            if(!tempCarData.empty()){
                tempCarRecord = createCarRecord(tempCarData);
                if(index < MAX_FILE_SIZE){
                    if(isValidRecord(tempCarRecord, errorMessage)){
                        cars[index] = tempCarRecord;
                        index++;
                    } else {

                        errorFile << tempCarData << errorMessage << "\n";
                    }
                }
            }
        }
    }
    inputFile.close();
    errorFile.close();
}

/*
Print all valid car records in a formatted table.
Precondition: The array contains CarRecord structs.
Postcondition: The records are displayed in the console.
*/
void printValidCarRecords(CarRecord cars[], int validIndex){
    cout << setw(10) << "Car ID" << "|"
         << setw(12) << "Model" << "|" 
         << setw(10) << "Quantity" << "|" 
         << setw(10) << "Price" << endl;
    cout << string(45, '-') << endl;
    for(int i = 0; i < validIndex; i++){
        CarRecord car = cars[i];
        cout << setw(10) << car.carID << "|"
             << setw(12) << car.carModel << "|"
             << setw(10) << car.carQuantity << "|"
             << setw(10) << fixed << setprecision(2) << car.carPrice << endl;
    }
}

/*
Reads and print all invalid car records from the error file.
Precondition: The error exists and is formatted correctly.
Postcondition: The invalid records are displayed in the console with errors.
*/
void printInvalidCarRecords(){
    ifstream errorFile("err_cars.txt");
    string errorStr;
    string ID, model;
    int quant;
    double price;

    cout << setw(10) << "Car ID" << "|"
         << setw(12) << "Model" << "|" 
         << setw(10) << "Quantity" << "|" 
         << setw(10) << "Price" << "|"
         << setw(40) << left << "Errors" << endl;
    cout << string(60, '-') << endl;
    while(getline(errorFile, errorStr)){
        istringstream iss(errorStr);
        iss >> ID >> model >> quant >> price;
        getline(iss, errorStr);

        cout << setw(10) << right << ID << "|"
             << setw(12) << model << "|" 
             << setw(10) << quant << "|" 
             << setw(10) << fixed << setprecision(2) << price << "|"
             << setw(40) << left << errorStr.substr(2) << endl;
    }
}

/*
Parses a string to create a CarRecord struct object
Precondition: The string contains data in the correct format
Postcondition: A CarRecord is returned
*/
CarRecord createCarRecord(string carInfoStr){
    istringstream iss(carInfoStr);
    string carData[4];
    int tempIndex = 0;

    while(iss >> carData[tempIndex]){
        tempIndex++;
    }
    CarRecord carRec = {carData[CAR_ID], carData[CAR_MODEL], stoi(carData[CAR_QUANTITY]), stod(carData[CAR_PRICE])};

    return carRec;
}

/*
Checks if a CarRecord is valid by checking each field and appends an error if not.
Precondition: The CarRecord is initialized
Postcondition: Returns true if it's a valid record, otherwise it appends to the error string.
*/
bool isValidRecord(CarRecord car, string &err){
    bool result = true;
    if(!isValidCarID(car.carID, err)){ result = false;}
    if(!isValidCarModel(car.carModel, err)){ result = false;}
    if(!isValidQuantity(car.carQuantity, err)){result = false;}
    if(!isValidPrice(car.carPrice, err)){result = false;}
    return result;
}

/*
Validates the carID based on length and format rules.
Precondition: The car ID string is initialized.
Postcondition: Returns true if valid, false otherwise and appends to the error string.
*/
bool isValidCarID(string id, string &err){
    bool results = true;
    if(id.length() != REQUIRED_ID_SIZE){
        err += carRecordErrors[ID_LENGTH_ERROR];
        results = false;
    }else if(!isValidFirstHalfCarID(id.substr(0, 4))){
        err += carRecordErrors[ID_FIRST_FOUR_NOT_ALPHA_OR_O_ERROR];
        results = false;
    }
    if(!isValidSecondHalfCarID(id.substr(4))){
        err += carRecordErrors[ID_LAST_FOUR_NOT_NUMERIC_OR_0_ERROR];
        results = false;
    }
    return results;
}

/*
Checks that the first 4 characters of the CarID are in the correct format
Precondition: The string is 4 characters long
Postcondition: Returns true if valid, false otherwise.
*/
bool isValidFirstHalfCarID(string frontHalf){
    bool result = true;
    for(int i = 0; i < frontHalf.length(); i++){
        char character = frontHalf[i];
        if(character == DISALLOWED_LETTER || isNumber(character)){
            result = false;
        }
    }
    return result;
}

/*
Checks that the last 4 characters of the CarID are in the correct format
Precondition: The string is 4 characters long
Postcondition: Returns true if valid, false otherwise.
*/
bool isValidSecondHalfCarID(string backHalf){
    bool result = true;
    for(int i = 0; i < backHalf.length(); i++){
        char character = backHalf[i];
        if(!isNumber(character) || (character == DISALLOWED_NUMBER)){
            result = false;
        }
    }
    return result;
}

/*
Checks if a character is a numeric digit; used within isValidSecondHalfCarID & isValidFirstHalfCarID 
Precondition: The input char is initialized.
Postcondition: Returns true if the char is a number, false otherwise.
*/
bool isNumber(char ch){
    if (ch >= '0' && ch <= '9') {
        return true;
    }
    return false;
}

/*
Validates the car model based on length and format
Precondition: The model string is initialized
Postcondition: Return true if valid, false otherwise and appends to error string.
*/
bool isValidCarModel(string model, string &err){
    bool result = true;
    if(model.length() < MIN_MODEL_ID_LENGTH){
        err += carRecordErrors[MODEL_LENGTH_ERROR];
        result = false;
    }
    char firstCharacter = model[0];
    if(isNumber(firstCharacter)){
        err += carRecordErrors[MODEL_CHARACTER_ERROR];
        result = false;
    }
    return result;
}

/*
Validates if the quantity if valid (non-negative)
Precondition: The quantity is an integer
Postcondition: Return true if valid, false otherwise and appends to error string.
*/
bool isValidQuantity(int quant, string &err){
    bool result = true;
    if(quant < 0){ 
        err += carRecordErrors[QUANTITY_ERROR];
        result = false;
    }
    return result;
}

/*
Returns if price is valid (greater than 12,999.00)
Precondition: The price is a double.
Postcondition: Returns true if valid, false otherwise and appends to error string.
*/
bool isValidPrice(double price, string &err){
    bool result = true;
    if(price <= MIN_CAR_PRICE){ 
        err += carRecordErrors[PRICE_ERROR];
        result = false;
    }
    return result;
}

// With 100+ valid car records
/*
What would you like to do: 
1. See all valid cars
2. See all invalid cars
3. Exit
Enter a number (1-3): 1
    Car ID|       Model|  Quantity|     Price
---------------------------------------------
  BPXZ3478|         RX5|        10|  15000.00
  MPKZ1265|      ModelX|         5|  20000.00
  ABCD2345|         R9X|         3|  14000.00
  LMNZ9876|        T100|         0|  13000.00
  ASDF8765|      ModelY|         3|  25000.00
  BNGH2345|       Supra|         2|  14000.00
  QWAS4567|        F150|         0|  15500.00
  XZAC1357|      Tundra|         8|  22000.00
  WERT6789|      Accord|         6|  17000.00
  JKLM5678|      Altima|         2|  14500.00
  PLMK3457|     Charger|         5|  17500.00
  XCVB6789|    Explorer|         8|  21000.00
  HGTR2341|    Corvette|         3|  26000.00
  BNVC7894|     Impreza|         2|  18000.00
  MNBV5612|      Tacoma|         4|  19000.00
  LKJH4321|     Outback|         7|  15500.00
  ERDF9876|  Challenger|         6|  23000.00
  TYHG3458|      Bronco|         1|  19500.00
  CVBN3459|      Sonata|         9|  17000.00
  XDRT2398|  Pathfinder|         5|  22000.00
  PLNB7623|      Malibu|         4|  16500.00
  BPXZ3478|         RX5|        10|  15000.00
  MPKZ1265|      ModelX|         5|  20000.00
  ABCD2345|         R9X|         3|  14000.00
  LMNZ9876|        T100|         0|  13000.00
  ASDF8765|      ModelY|         3|  25000.00
  BNGH2345|       Supra|         2|  14000.00
  QWAS4567|        F150|         0|  15500.00
  XZAC1357|      Tundra|         8|  22000.00
  WERT6789|      Accord|         6|  17000.00
  JKLM5678|      Altima|         2|  14500.00
  PLMK3457|     Charger|         5|  17500.00
  XCVB6789|    Explorer|         8|  21000.00
  HGTR2341|    Corvette|         3|  26000.00
  BNVC7894|     Impreza|         2|  18000.00
  MNBV5612|      Tacoma|         4|  19000.00
  LKJH4321|     Outback|         7|  15500.00
  ERDF9876|  Challenger|         6|  23000.00
  TYHG3458|      Bronco|         1|  19500.00
  CVBN3459|      Sonata|         9|  17000.00
  XDRT2398|  Pathfinder|         5|  22000.00
  PLNB7623|      Malibu|         4|  16500.00
  BPXZ3478|         RX5|        10|  15000.00
  MPKZ1265|      ModelX|         5|  20000.00
  ABCD2345|         R9X|         3|  14000.00
  LMNZ9876|        T100|         0|  13000.00
  ASDF8765|      ModelY|         3|  25000.00
  BNGH2345|       Supra|         2|  14000.00
  QWAS4567|        F150|         0|  15500.00
  XZAC1357|      Tundra|         8|  22000.00
  WERT6789|      Accord|         6|  17000.00
  JKLM5678|      Altima|         2|  14500.00
  PLMK3457|     Charger|         5|  17500.00
  XCVB6789|    Explorer|         8|  21000.00
  HGTR2341|    Corvette|         3|  26000.00
  BNVC7894|     Impreza|         2|  18000.00
  MNBV5612|      Tacoma|         4|  19000.00
  LKJH4321|     Outback|         7|  15500.00
  ERDF9876|  Challenger|         6|  23000.00
  TYHG3458|      Bronco|         1|  19500.00
  CVBN3459|      Sonata|         9|  17000.00
  XDRT2398|  Pathfinder|         5|  22000.00
  PLNB7623|      Malibu|         4|  16500.00
  BPXZ3478|         RX5|        10|  15000.00
  MPKZ1265|      ModelX|         5|  20000.00
  ABCD2345|         R9X|         3|  14000.00
  LMNZ9876|        T100|         0|  13000.00
  ASDF8765|      ModelY|         3|  25000.00
  BNGH2345|       Supra|         2|  14000.00
  QWAS4567|        F150|         0|  15500.00
  XZAC1357|      Tundra|         8|  22000.00
  WERT6789|      Accord|         6|  17000.00
  JKLM5678|      Altima|         2|  14500.00
  PLMK3457|     Charger|         5|  17500.00
  XCVB6789|    Explorer|         8|  21000.00
  HGTR2341|    Corvette|         3|  26000.00
  BNVC7894|     Impreza|         2|  18000.00
  MNBV5612|      Tacoma|         4|  19000.00
  LKJH4321|     Outback|         7|  15500.00
  ERDF9876|  Challenger|         6|  23000.00
  TYHG3458|      Bronco|         1|  19500.00
  CVBN3459|      Sonata|         9|  17000.00
  XDRT2398|  Pathfinder|         5|  22000.00
  PLNB7623|      Malibu|         4|  16500.00
  BPXZ3478|         RX5|        10|  15000.00
  MPKZ1265|      ModelX|         5|  20000.00
  ABCD2345|         R9X|         3|  14000.00
  LMNZ9876|        T100|         0|  13000.00
  ASDF8765|      ModelY|         3|  25000.00
  BNGH2345|       Supra|         2|  14000.00
  QWAS4567|        F150|         0|  15500.00
  XZAC1357|      Tundra|         8|  22000.00
  WERT6789|      Accord|         6|  17000.00
  JKLM5678|      Altima|         2|  14500.00
  PLMK3457|     Charger|         5|  17500.00
  XCVB6789|    Explorer|         8|  21000.00
  HGTR2341|    Corvette|         3|  26000.00
  BNVC7894|     Impreza|         2|  18000.00
  MNBV5612|      Tacoma|         4|  19000.00
  LKJH4321|     Outback|         7|  15500.00

What would you like to do: 
1. See all valid cars
2. See all invalid cars
3. Exit
Enter a number (1-3): 2
    Car ID|       Model|  Quantity|     Price|Errors                                  
------------------------------------------------------------
      AAAA|         AAA|         0|  13000.00|ID is not 8 characters long             
  OOOO1111|         AAA|         0|  13000.00|First 4 characters in the ID are not all letters or has O
  AAAA0000|         AAA|         0|  13000.00|Last 4 characters in the ID are not all numeric or has 0
  AAAA1111|          AA|         0|  13000.00|Model is less than 3 characters         
  AAAA1111|         1AA|         0|  13000.00|Model is not only alphanumeric or starts with number
  AAAA1111|         AAA|        -1|  13000.00|Quantity is less than 0                 
  AAAA1111|         AAA|         0|      0.00|Price is less than or equal to 12,999.00
  MPKZ1205|         RX5|        10|  15000.00|Last 4 characters in the ID are not all numeric or has 0
  ABCD2340|         R9X|         3|  14000.00|Last 4 characters in the ID are not all numeric or has 0
  QWER1111|          A1|         2|  12000.00|Model is less than 3 characters, Price is less than or equal to 12,999.00
  ZXCV5678|        9Car|         4|  18000.00|Model is not only alphanumeric or starts with number
  TYUI3456|       Camry|        -5|  20000.00|Quantity is less than 0                 
  HGFE7654|     Mustang|         1|  12999.00|Price is less than or equal to 12,999.00
  VBNM4321|          RX|         7|  16000.00|Model is less than 3 characters         
  UIOP1234|       Civic|         4|  13500.00|First 4 characters in the ID are not all letters or has O
  OPLK1276|     Sorento|         0|  14500.00|First 4 characters in the ID are not all letters or has O
  MKOI8765|     Elantra|         3|  14000.00|First 4 characters in the ID are not all letters or has O
  OOOO0000|          TA|        -1|   1000.00|First 4 characters in the ID are not all letters or has O, Last 4 characters in the ID are not all numeric or has 0, Model is less than 3 characters, Quantity is less than 0, Price is less than or equal to 12,999.00
  MPKZ1205|         RX5|        10|  15000.00|Last 4 characters in the ID are not all numeric or has 0
  ABCD2340|         R9X|         3|  14000.00|Last 4 characters in the ID are not all numeric or has 0
  QWER1111|          A1|         2|  12000.00|Model is less than 3 characters, Price is less than or equal to 12,999.00
  ZXCV5678|        9Car|         4|  18000.00|Model is not only alphanumeric or starts with number
  TYUI3456|       Camry|        -5|  20000.00|Quantity is less than 0                 
  HGFE7654|     Mustang|         1|  12999.00|Price is less than or equal to 12,999.00
  VBNM4321|          RX|         7|  16000.00|Model is less than 3 characters         
  UIOP1234|       Civic|         4|  13500.00|First 4 characters in the ID are not all letters or has O
  OPLK1276|     Sorento|         0|  14500.00|First 4 characters in the ID are not all letters or has O
  MKOI8765|     Elantra|         3|  14000.00|First 4 characters in the ID are not all letters or has O
  OOOO0000|          TA|        -1|   1000.00|First 4 characters in the ID are not all letters or has O, Last 4 characters in the ID are not all numeric or has 0, Model is less than 3 characters, Quantity is less than 0, Price is less than or equal to 12,999.00
  MPKZ1205|         RX5|        10|  15000.00|Last 4 characters in the ID are not all numeric or has 0
  ABCD2340|         R9X|         3|  14000.00|Last 4 characters in the ID are not all numeric or has 0
  QWER1111|          A1|         2|  12000.00|Model is less than 3 characters, Price is less than or equal to 12,999.00
  ZXCV5678|        9Car|         4|  18000.00|Model is not only alphanumeric or starts with number
  TYUI3456|       Camry|        -5|  20000.00|Quantity is less than 0                 
  HGFE7654|     Mustang|         1|  12999.00|Price is less than or equal to 12,999.00
  VBNM4321|          RX|         7|  16000.00|Model is less than 3 characters         
  UIOP1234|       Civic|         4|  13500.00|First 4 characters in the ID are not all letters or has O
  OPLK1276|     Sorento|         0|  14500.00|First 4 characters in the ID are not all letters or has O
  MKOI8765|     Elantra|         3|  14000.00|First 4 characters in the ID are not all letters or has O
  OOOO0000|          TA|        -1|   1000.00|First 4 characters in the ID are not all letters or has O, Last 4 characters in the ID are not all numeric or has 0, Model is less than 3 characters, Quantity is less than 0, Price is less than or equal to 12,999.00
  MPKZ1205|         RX5|        10|  15000.00|Last 4 characters in the ID are not all numeric or has 0
  ABCD2340|         R9X|         3|  14000.00|Last 4 characters in the ID are not all numeric or has 0
  QWER1111|          A1|         2|  12000.00|Model is less than 3 characters, Price is less than or equal to 12,999.00
  ZXCV5678|        9Car|         4|  18000.00|Model is not only alphanumeric or starts with number
  TYUI3456|       Camry|        -5|  20000.00|Quantity is less than 0                 
  HGFE7654|     Mustang|         1|  12999.00|Price is less than or equal to 12,999.00
  VBNM4321|          RX|         7|  16000.00|Model is less than 3 characters         
  UIOP1234|       Civic|         4|  13500.00|First 4 characters in the ID are not all letters or has O
  OPLK1276|     Sorento|         0|  14500.00|First 4 characters in the ID are not all letters or has O
  MKOI8765|     Elantra|         3|  14000.00|First 4 characters in the ID are not all letters or has O
  OOOO0000|          TA|        -1|   1000.00|First 4 characters in the ID are not all letters or has O, Last 4 characters in the ID are not all numeric or has 0, Model is less than 3 characters, Quantity is less than 0, Price is less than or equal to 12,999.00
  MPKZ1205|         RX5|        10|  15000.00|Last 4 characters in the ID are not all numeric or has 0
  ABCD2340|         R9X|         3|  14000.00|Last 4 characters in the ID are not all numeric or has 0
  QWER1111|          A1|         2|  12000.00|Model is less than 3 characters, Price is less than or equal to 12,999.00
  ZXCV5678|        9Car|         4|  18000.00|Model is not only alphanumeric or starts with number
  TYUI3456|       Camry|        -5|  20000.00|Quantity is less than 0                 
  HGFE7654|     Mustang|         1|  12999.00|Price is less than or equal to 12,999.00
  VBNM4321|          RX|         7|  16000.00|Model is less than 3 characters         
  UIOP1234|       Civic|         4|  13500.00|First 4 characters in the ID are not all letters or has O

What would you like to do: 
1. See all valid cars
2. See all invalid cars
3. Exit
Enter a number (1-3): 3
Goodbye!
*/

//With <100 records
/*
What would you like to do: 
1. See all valid cars
2. See all invalid cars
3. Exit
Enter a number (1-3): 1
    Car ID|       Model|  Quantity|     Price
---------------------------------------------
  BPXZ3478|         RX5|        10|  15000.00
  MPKZ1265|      ModelX|         5|  20000.00
  ABCD2345|         R9X|         3|  14000.00
  LMNZ9876|        T100|         0|  13000.00
  ASDF8765|      ModelY|         3|  25000.00
  BNGH2345|       Supra|         2|  14000.00
  QWAS4567|        F150|         0|  15500.00
  XZAC1357|      Tundra|         8|  22000.00
  WERT6789|      Accord|         6|  17000.00
  JKLM5678|      Altima|         2|  14500.00
  PLMK3457|     Charger|         5|  17500.00
  XCVB6789|    Explorer|         8|  21000.00
  HGTR2341|    Corvette|         3|  26000.00
  BNVC7894|     Impreza|         2|  18000.00
  MNBV5612|      Tacoma|         4|  19000.00
  LKJH4321|     Outback|         7|  15500.00
  ERDF9876|  Challenger|         6|  23000.00
  TYHG3458|      Bronco|         1|  19500.00
  CVBN3459|      Sonata|         9|  17000.00
  XDRT2398|  Pathfinder|         5|  22000.00
  PLNB7623|      Malibu|         4|  16500.00

What would you like to do: 
1. See all valid cars
2. See all invalid cars
3. Exit
Enter a number (1-3): 2
    Car ID|       Model|  Quantity|     Price|Errors                                  
------------------------------------------------------------
      AAAA|         AAA|         0|  13000.00|ID is not 8 characters long             
  OOOO1111|         AAA|         0|  13000.00|First 4 characters in the ID are not all letters or has O
  AAAA0000|         AAA|         0|  13000.00|Last 4 characters in the ID are not all numeric or has 0
  AAAA1111|          AA|         0|  13000.00|Model is less than 3 characters         
  AAAA1111|         1AA|         0|  13000.00|Model is not only alphanumeric or starts with number
  AAAA1111|         AAA|        -1|  13000.00|Quantity is less than 0                 
  AAAA1111|         AAA|         0|      0.00|Price is less than or equal to 12,999.00
  MPKZ1205|         RX5|        10|  15000.00|Last 4 characters in the ID are not all numeric or has 0
  ABCD2340|         R9X|         3|  14000.00|Last 4 characters in the ID are not all numeric or has 0
  QWER1111|          A1|         2|  12000.00|Model is less than 3 characters, Price is less than or equal to 12,999.00
  ZXCV5678|        9Car|         4|  18000.00|Model is not only alphanumeric or starts with number
  TYUI3456|       Camry|        -5|  20000.00|Quantity is less than 0                 
  HGFE7654|     Mustang|         1|  12999.00|Price is less than or equal to 12,999.00
  VBNM4321|          RX|         7|  16000.00|Model is less than 3 characters         
  UIOP1234|       Civic|         4|  13500.00|First 4 characters in the ID are not all letters or has O
  OPLK1276|     Sorento|         0|  14500.00|First 4 characters in the ID are not all letters or has O
  MKOI8765|     Elantra|         3|  14000.00|First 4 characters in the ID are not all letters or has O
  OOOO0000|          TA|        -1|   1000.00|First 4 characters in the ID are not all letters or has O, Last 4 characters in the ID are not all numeric or has 0, Model is less than 3 characters, Quantity is less than 0, Price is less than or equal to 12,999.00

What would you like to do: 
1. See all valid cars
2. See all invalid cars
3. Exit
Enter a number (1-3): 3
Goodbye!
*/