// Christopher Osgood
// Lab 04

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
using namespace std;

const string INPUT_FILE_NAME = "in_cars.txt";
const string ERROR_FILE_NAME = "err_cars.txt";
const string LOW_INVENTORY_FILE = "lowInventoryCars.txt";
const int MAX_FILE_SIZE = 100;
const int REQUIRED_ID_SIZE = 8;
const int MIN_MODEL_ID_LENGTH = 3;
const char DISALLOWED_LETTER = 'O';
const char DISALLOWED_NUMBER = '0';
const double MIN_CAR_PRICE = 12'999.00;
const int ID_WIDTH = 10;
const int MODEL_WIDTH = 14;
const int MANUFAC_WIDTH = 14;
const int QUANT_WIDTH = 14;
const int PRICE_WIDTH = 14;
const int ERR_WIDTH = 40;
const double PRICE_RANGE = 0.12;
const int MIN_INVENTORY = 5;
const string ERROR_STRINGS[] = {", ID is not 8 characters long", ", First 4 characters in the ID are not all letters or has O",
                                   ", Last 4 characters in the ID are not all numeric or has 0", ", Model is less than 3 characters",
                                   ", Model is not only alphanumeric or starts with number", ", Quantity is less than 0", 
                                   ", Price is less than or equal to 12,999.00", "Invalid! Only enter a valid option",
                                   "Price must be above 12,999.00", "Please enter a valid number above 12,999.00"};

class Car;
class Inventory;

void readFile(Inventory &Inventory);
void clearError(string error);
void printMainMenu();
void printSpecificMenu();
void printSortMenu();
void printTableHeader(bool hasErrors=false);

enum CarInfo{
    CAR_ID,
    CAR_MODEL,
    CAR_QUANTITY,
    CAR_PRICE
};

enum MainMenuOptions{
    SEE_VALID_RECORDS = 1,
    SEE_INVALID_RECORDS,
    SEE_SPECIFIC_RECORDS,
    SORT_CARS,
    EXIT_PROGRAM
};

enum SpecificCarMenuOptions{
    SEARCH_CAR_MM = 1,
    SEARCH_PRICE,
    EXIT_SPECIFIC_CAR_MENU
};

enum SortMenuFields{
    SORT_ID=1,
    SORT_MODEL,
    SORT_MANUFACTURER,
    SORT_QUANTITY,
    SORT_PRICE,
    EXIT_SORT_MENU
};

enum Errors{
    ID_LENGTH_ERROR,
    ID_FIRST_FOUR_NOT_ALPHA_OR_O_ERROR,
    ID_LAST_FOUR_NOT_NUMERIC_OR_0_ERROR,
    MODEL_LENGTH_ERROR,
    MODEL_CHARACTER_ERROR,
    QUANTITY_ERROR,
    PRICE_ERROR,
    INVALID_MENU_OPTION,
    PRICE_TOO_LOW,
    PRICE_NOT_NUM
};

class Car {
    private:
        string carID;
        string carModel;
        string carManufacturer;
        int carQuantity;
        double carPrice;

        /*
        Validates the carID based on length and format rules.
        Precondition: The car ID string is initialized.
        Postcondition: Returns true if valid, false otherwise and appends to the error string.
        */
        bool isValidCarID(string &err);

        /*
        Checks that the first 4 characters of the CarID are in the correct format
        Precondition: The string is 4 characters long
        Postcondition: Returns true if valid, false otherwise.
        */
        bool isValidFirstHalfCarID(string frontHalf);

        /*
        Checks that the last 4 characters of the CarID are in the correct format
        Precondition: The string is 4 characters long
        Postcondition: Returns true if valid, false otherwise.
        */
        bool isValidSecondHalfCarID(string backHalf);

        /*
        Checks if a character is a numeric digit; used within isValidSecondHalfCarID & isValidFirstHalfCarID 
        Precondition: The input char is initialized.
        Postcondition: Returns true if the char is a number, false otherwise.
        */
        bool isNumber(char ch);
        
        /*
        Validates the car model based on length and format
        Precondition: The model string is initialized
        Postcondition: Return true if valid, false otherwise and appends to error string.
        */
        bool isValidCarModel(string &err);

        /*
        Validates if the quantity if valid (non-negative)
        Precondition: The quantity is an integer
        Postcondition: Return true if valid, false otherwise and appends to error string.
        */
        bool isValidQuantity(string &err);

        /*
        Returns if price is valid (greater than 12,999.00)
        Precondition: The price is a double.
        Postcondition: Returns true if valid, false otherwise and appends to error string.
        */
        bool isValidPrice(string &err);


    public:

        Car(){ carID = carModel = carManufacturer = ""; carQuantity = carPrice = 0; }

        Car(string id, string model, string manufac, int quant, double price){
            carID = id;
            carModel = model;
            carManufacturer = manufac;
            carQuantity = quant;
            carPrice = price;
        }

        //Setter
        void setCarData(string info){
            istringstream iss(info);

            iss >> carID >> carModel >> carManufacturer >> carQuantity >> carPrice;
        }

        //Getters 
        string getCarID() const { return carID; }
        string getCarModel() const { return carModel; }
        string getCarManufacturer() const { return carManufacturer; }
        double getCarPrice() const { return carPrice; }
        int getCarQuantity() const { return carQuantity; }

        /*
        Retrieve a specfiic data field as a string
        Precondition: 'field' represents one of the SortMenuFields enum options
        Postcondition: the data is returned as a string
        */
        string getDataField(int field);

        /*
        Format output string in the form of the table header
        Precondition: member variables are initialized
        Postcondition: return string is properaly formed according to the header format
        */
        string toString();

        /*
        Checks if a Car is valid by checking each field and appends an error if not.
        Precondition: The Car is initialized
        Postcondition: Returns true if it's a valid record, otherwise it appends to the error string.
        */
        bool isValidRecord(string &err);
};

class Inventory {
    private:
        Car validCars[MAX_FILE_SIZE];
        int numValidCars;

        /*
        Sort the list of valid cars based on the specified field using Bubble Sort
        Precondition: 'searchField' is a valid sorting field 
        Postcondition: 'validCars' is sorted in ascending order based on the given field
        */
        void sortCars(int searchField);

        /*
        /*
        Search for cars by a user input for model or manufacturer
        Precondition: 'validCars' is initiliazed and 'numValidCars' represents how many cars there are
        Postcondition: Prints all matching cars to the user input
        */
        void searchSpecificCarMM();
        
        /*
        Convert a string to all lowercase
        Precondition: input string 'str' is non-empty
        Postcondition: returns a new string that is all lowercase
        */
        string toLowerCase(string str);

        /*
        Search for cars within a price range of +-12% of the user input
        Precondition: 'validCars' and 'numValidCars' are initialized and accurate
        Postcondition: Print all matching cars within the price range in a table.
        */
        void searchSpecificCarPrice();

    public:

        Inventory(){ numValidCars = 0; }

        int getNumValidCars(){ return numValidCars; };

        void addValidCar(Car car);
                
        /*
        Print all valid car records in a formatted table.
        Precondition: The array contains Car structs.
        Postcondition: The records are displayed in the console.
        */
        void printValidCars();
        
        /*
        Reads and print all invalid car records from the error file.
        Precondition: The error exists and is formatted correctly.
        Postcondition: The invalid records are displayed in the console with errors.
        */
        void printInvalidCars();
        
        /*
        Prompt the user to select a sorting option and sort the car inventory
        Precondition: 'validCars' and 'numValidCars' are accutre and initialized
        Postcondition: The inventory is sorted, and the original 'validCars' changes order
        */
        void promptSortCars();
        
        /*
        Prompt the user to choose a specific car search method and display the results
        Precondition: 'validCars' and 'numValidCars' are accurate
        Postcondition: The search results are displayed based on the chosen method
        */
        void promptSearchSpecificCars();

        /*
        Display cars with low inventory and exit the program.
        Precondition: 'validCars' and 'numValidCars' are accurate
        Postcondition: Cars with low inventory are displayed, and the program exits.
        */
        void exitProgram();    
};

int main(){
    Inventory carInventory;

    readFile(carInventory);

    int menuChoice;
    string errorMessage;

    cout << fixed << setprecision(2);
    while(menuChoice != EXIT_PROGRAM){
        printMainMenu();
        
        cin >> menuChoice;

        if(cin.fail() || cin.peek() != '\n' || 
           menuChoice < SEE_VALID_RECORDS || menuChoice > EXIT_PROGRAM){
            menuChoice = 0;
            errorMessage =  ERROR_STRINGS[INVALID_MENU_OPTION]; }

        switch (menuChoice){
            case SEE_VALID_RECORDS:
                carInventory.printValidCars();
                break;

            case SEE_INVALID_RECORDS:
                carInventory.printInvalidCars();
                break;

            case SEE_SPECIFIC_RECORDS:
                carInventory.promptSearchSpecificCars();
                break;

            case SORT_CARS:
                carInventory.promptSortCars();
                break;

            case EXIT_PROGRAM:
                carInventory.exitProgram();
                break;

            default:
                clearError(errorMessage);
        }
    }
}

/*
Reads the input file and stores valid Car records in an array while put invalid ones in the error text file.
Precondition: The input file exists and is formatted correctly.
Postcondition: Valid records are stored in the array, and invalid records are written to the error file.
*/
void readFile(Inventory &inventory){
    ifstream inputFile(INPUT_FILE_NAME);
    ofstream errorFile(ERROR_FILE_NAME);

    string strCarData;
    Car tempCar;

    if (inputFile.fail()){
        cout << "\nInput file not found!"; } 
    
    else if (errorFile.fail()){
        cout << "\nError file not found!"; } 
    
    else {
        while(getline(inputFile, strCarData)){
            string carErrorMessage = "";

            if(!strCarData.empty()){
                tempCar.setCarData(strCarData);

                if(inventory.getNumValidCars() < MAX_FILE_SIZE){
                    if(tempCar.isValidRecord(carErrorMessage)){
                        inventory.addValidCar(tempCar); } 

                    else{

                        errorFile << strCarData + carErrorMessage + "\n";
                    }
                }
            }
        }
        inputFile.close();
        errorFile.close();
    }
}

/*
Clears error, input buffer, and prints out the error.
Precondition: string input that is the error
Postcondition: cin is cleared from error and input buffer
*/
void clearError(string error){
    cout << "\n"+error+"\n";
    cin.clear();
    cin.ignore(100, '\n');
}

void printSortMenu(){
    cout << "\n\nHow would you like to sort by: \n" 
            "1 ID\n"
            "2 Model\n"
            "3 Manufacturer\n"
            "4 Quantity\n"
            "5 Price\n"
            "6 Go back to Main menu\n"
            "Enter a menu option: ";
}

void printSpecificMenu(){
    cout << "\n\nHow would you like to search: \n"
            "1 Car model or manufacturer\n"
            "2 Price Range\n"
            "3 Go back to main menu\n"
            "Enter your choice: ";
}

void printMainMenu(){
    cout << "\n\nWhat would you like to do: \n" 
                "1 See all valid cars\n"
                "2 See all invalid cars\n"
                "3 Search for specific car(s)\n"
                "4 Sort cars\n"
                "5 Save Low Inventory/Exit\n"
                "Enter a menu option: ";
}

void printTableHeader(bool hasErrors){
    cout << left << setw(10) << "|Car ID "
                << setw(MODEL_WIDTH) << "|Model " 
                << setw(MANUFAC_WIDTH) << "|Manufacturer "
                << setw(QUANT_WIDTH) << "|Quantity " 
                << "|" << setw(PRICE_WIDTH-1) << right << "Price";
    if(hasErrors){
        cout << setw(ERR_WIDTH) << left << "|Errors " << "\n";
        cout << setw(ID_WIDTH+MODEL_WIDTH+MANUFAC_WIDTH+QUANT_WIDTH+PRICE_WIDTH+ERR_WIDTH) << setfill('-') << "" << setfill(' ') << "\n"; }
    
    else{
        cout << "\n" << setw(ID_WIDTH+MODEL_WIDTH+MANUFAC_WIDTH+QUANT_WIDTH+PRICE_WIDTH) << setfill('-') << "" << setfill(' ') << "\n";
    }
}


bool Car::isValidCarID(string &err){
    bool results = true;
    if(carID.length() != REQUIRED_ID_SIZE){
        err += ERROR_STRINGS[ID_LENGTH_ERROR];
        results = false; }
    
    else if(!isValidFirstHalfCarID(carID.substr(0, 4))){
        err += ERROR_STRINGS[ID_FIRST_FOUR_NOT_ALPHA_OR_O_ERROR];
        results = false; }

    if(!isValidSecondHalfCarID(carID.substr(4))){
        err += ERROR_STRINGS[ID_LAST_FOUR_NOT_NUMERIC_OR_0_ERROR];
        results = false; }
    
    return results;
}

bool Car::isValidFirstHalfCarID(string frontHalf){
    bool result = true;
    for(int i = 0; i < frontHalf.length(); i++){
        char character = frontHalf[i];
        
        if(character == DISALLOWED_LETTER || isNumber(character)){
            result = false; }

    }
    return result;
}

bool Car::isValidSecondHalfCarID(string backHalf){
    bool result = true;
    for(int i = 0; i < backHalf.length(); i++){
        char character = backHalf[i];
        
        if(!isNumber(character) || (character == DISALLOWED_NUMBER)){
            result = false; }

    }
    return result;
}

bool Car::isNumber(char ch){
    bool result = false;
    if (ch >= '0' && ch <= '9') {
        result = true;
    }
    return result;
}

bool Car::isValidCarModel(string &err){
    bool result = true;
    if(carModel.length() < MIN_MODEL_ID_LENGTH){
        err += ERROR_STRINGS[MODEL_LENGTH_ERROR];
        result = false; }

    char firstCharacter = carModel[0];
    if(isNumber(firstCharacter)){
        err += ERROR_STRINGS[MODEL_CHARACTER_ERROR];
        result = false; }

    return result;
}

bool Car::isValidQuantity(string &err){
    bool result = true;
    if(carQuantity < 0){ 
        err += ERROR_STRINGS[QUANTITY_ERROR];
        result = false; }

    return result;
}

bool Car::isValidPrice(string &err){
    bool result = true;
    if(carPrice <= MIN_CAR_PRICE){ 
        err += ERROR_STRINGS[PRICE_ERROR];
        result = false; }

    return result;
}

string Car::getDataField(int field){
    string data;
    switch(field){
        case SORT_ID:
            data = carID;
            break;

        case SORT_MODEL:
            data = carModel;
            break;

        case SORT_MANUFACTURER:
            data = carManufacturer;
            break;

        case SORT_QUANTITY:
            data = to_string(carQuantity);
            break;

        case SORT_PRICE:
            data = to_string(carPrice);
            break;
    }
    return data;
}

string Car::toString(){
    ostringstream oss;
    oss << left << setw(ID_WIDTH) << "|"+carID
        << setw(MODEL_WIDTH) << "|"+carModel
        << setw(MANUFAC_WIDTH) << "|"+carManufacturer
        << setw(QUANT_WIDTH) << "|"+to_string(carQuantity)
        << "|" << setw(PRICE_WIDTH-1) << right << fixed << setprecision(2) << carPrice << "\n";
    return oss.str();
}

bool Car::isValidRecord(string &err){
    bool result = true;
    if(!isValidCarID(err)){ result = false;}
    if(!isValidCarModel(err)){ result = false;}
    if(!isValidQuantity(err)){result = false;}
    if(!isValidPrice(err)){result = false;}
    return result;
}


void Inventory::sortCars(int searchField){
    if(searchField != EXIT_SORT_MENU){    
        for(int i = numValidCars-1; i>=0; i--){
            for(int j = 0; j < i; j++){
                
                Car thisCar = validCars[j];
                Car nextCar = validCars[j+1];

                if(searchField == SORT_PRICE || searchField == SORT_QUANTITY){
                    if(stod(thisCar.getDataField(searchField)) > stod(nextCar.getDataField(searchField))){
                        validCars[j] = nextCar;
                        validCars[j+1] = thisCar; }}
                else{
                    if(thisCar.getDataField(searchField) > nextCar.getDataField(searchField)){
                        validCars[j] = nextCar;
                        validCars[j+1] = thisCar;
                    }    
                }
            }
        }
    }
}

void Inventory::searchSpecificCarMM(){
    string carMM;
    string rightCars = "";
    cout << "\nWhat manufacturer or model car do you want to search for: ";
    cin >> carMM;
    carMM = toLowerCase(carMM);

    for(int i = 0; i < numValidCars; i++){
        Car currCar = validCars[i];
        string model = toLowerCase(currCar.getCarModel());
        string manufacturer = toLowerCase(currCar.getCarManufacturer());
        if(model.find(carMM) != string::npos || manufacturer.find(carMM) != string::npos){
            rightCars += currCar.toString();
        }
    }
    if(rightCars.empty()){
        cout << "\nThere are no cars with that manufacturer or model!\n";}
    else {
        printTableHeader();
        cout << rightCars; }
}

string Inventory::toLowerCase(string str){
    string lower;
    for(char letter : str){
        lower.push_back(static_cast<char>(tolower(letter)));
    }    
    return lower;
}

void Inventory::searchSpecificCarPrice(){
    int price = 0;

    while(price < MIN_CAR_PRICE){
        cout << "What is your target price (+-12%): ";
        cin >> price;

        if(cin.fail() || cin.peek() != '\n'){
            price = 0;
            clearError(ERROR_STRINGS[PRICE_NOT_NUM]); }

        else if(price < MIN_CAR_PRICE){
            price = 0;
            clearError(ERROR_STRINGS[PRICE_TOO_LOW]); }

        else{
            int lowerPrice = price*(1-PRICE_RANGE);
            int upperPrice = price*(1+PRICE_RANGE);

            string rightCars = "";
            for(int i = 0; i < numValidCars; i++){
                Car currCar = validCars[i];
                if(currCar.getCarPrice() >= lowerPrice && currCar.getCarPrice() <= upperPrice){
                    
                    if(currCar.getCarQuantity() < 5){
                        ostringstream oss;
                        oss << left << setw(ID_WIDTH) << "|"+currCar.getCarID()
                            << setw(MODEL_WIDTH) << "|"+currCar.getCarModel()
                            << setw(MANUFAC_WIDTH) << "|"+currCar.getCarManufacturer()
                            << setw(QUANT_WIDTH) << "|Low Inventory"
                            "|" << setw(PRICE_WIDTH-1) << right << fixed << setprecision(2) << currCar.getCarPrice() << "\n";
                        rightCars += oss.str(); }
                    else {
                        rightCars += currCar.toString(); }
                }
            }

            if(rightCars.empty()){
                cout << "\nThere are no cars in that price range!\n"; }
            else{
                cout << "Cars between " << lowerPrice << "-" <<upperPrice << ":\n";
                printTableHeader();
                cout << rightCars; }
        }
    }
}

void Inventory::addValidCar(Car car){
    if(numValidCars != MAX_FILE_SIZE){
        validCars[numValidCars++] = car;
    }
}

void Inventory::printValidCars(){
    printTableHeader();
    
    for(int i = 0; i < numValidCars; i++){
        Car car = validCars[i];
        cout << car.toString();
    }
}

void Inventory::printInvalidCars(){
    ifstream errorFile(ERROR_FILE_NAME);

    string badCarRecordStr;
    string ID, model, manufac;
    int quant;
    double price;

    printTableHeader(true);


    while(getline(errorFile, badCarRecordStr)){
        istringstream iss(badCarRecordStr);
        iss >> ID >> model >> manufac >> quant >> price;
        getline(iss, badCarRecordStr);


        cout << left << setw(ID_WIDTH) << "|"+ID
            << setw(MODEL_WIDTH) << "|"+model
            << setw(MANUFAC_WIDTH) << "|"+manufac
            << setw(QUANT_WIDTH) << "|"+to_string(quant)
            << "|" << setw(PRICE_WIDTH-1) << right << price
            << setw(ERR_WIDTH) << left << "|"+badCarRecordStr.substr(2) << "\n";
    }
    errorFile.close();
}

void Inventory::promptSortCars(){
    int searchOptionField = 0;

    while(searchOptionField != EXIT_SORT_MENU){    
        printSortMenu();

        cin >> searchOptionField;

        if(cin.fail() || cin.peek() != '\n' || 
        searchOptionField < SEE_VALID_RECORDS || searchOptionField > EXIT_SORT_MENU){
            searchOptionField = 0;
            clearError(ERROR_STRINGS[INVALID_MENU_OPTION]); }
        else if(searchOptionField != EXIT_SORT_MENU){
            sortCars(searchOptionField);
            printValidCars(); }
        else {
            cout << "\nGoing back to Main Menu\n";
        }
    }
}

void Inventory::promptSearchSpecificCars(){
    int specificCarMenu = 0;
    string error;

    while(specificCarMenu != EXIT_SPECIFIC_CAR_MENU){
        printSpecificMenu();
        
        cin >> specificCarMenu;

        if(cin.fail() || cin.peek() != '\n' || 
            specificCarMenu < SEARCH_CAR_MM || 
            specificCarMenu > EXIT_SPECIFIC_CAR_MENU){
                specificCarMenu = 0;
                error =  ERROR_STRINGS[INVALID_MENU_OPTION]; }
        
        switch(specificCarMenu){
            case SEARCH_CAR_MM:
                searchSpecificCarMM();
                break;
            
            case SEARCH_PRICE:
                searchSpecificCarPrice();
                break;

            case EXIT_SPECIFIC_CAR_MENU:
                cout << "\nGoing back to Main Menu\n";
                break;

            default:
                clearError(error);
        }
    }

}

void Inventory::exitProgram(){
    ofstream lowInventory(LOW_INVENTORY_FILE);
    
    if(lowInventory.fail()){
        cout << "\nCouldn't open " + LOW_INVENTORY_FILE + "\n"; }
    else {
        cout << "\nCars with Low Inventory (<5): \n";
        string lowInventoryCars = "";
        printTableHeader();

        for(int i = 0; i < numValidCars; i++){
            if(validCars[i].getCarQuantity() < MIN_INVENTORY){
                lowInventoryCars += validCars[i].toString();
                lowInventory << validCars[i].toString();
            }
        }
        cout << lowInventoryCars + "\n\nGoodbye!\n";
    }
    lowInventory.close();
}






//OUTPUT
/*
What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: a

Invalid! Only enter a valid option


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: alpha

Invalid! Only enter a valid option


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: -5

Invalid! Only enter a valid option


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: 0

Invalid! Only enter a valid option


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: 1
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00
|WERT6789 |Accord       |Honda        |6            |     17000.00
|CHEV6743 |Camaro       |Chevrolet    |8            |     52000.00
|MAZD8964 |CX5          |Mazda        |25           |     35000.00
|TESL4321 |ModelS       |Tesla        |5            |     79999.00
|SUBA2147 |Outback      |Subaru       |11           |     33000.00
|NISS8675 |Altima       |Nissan       |14           |     29000.00
|BUIC1387 |Enclave      |Buick        |6            |     40000.00
|JAGU4561 |FType        |Jaguar       |4            |     73000.00
|LEXU7319 |RX350        |Lexus        |7            |     49000.00
|HYUN3245 |Sonata       |Hyundai      |19           |     27000.00
|KIAI4128 |Sportage     |Kia          |21           |     32000.00
|BPXZ3478 |RX5          |Mazda        |10           |     15000.00
|MPKZ1265 |ModelX       |Tesla        |5            |     20000.00
|ABCD2345 |R9X          |Nissan       |3            |     14000.00
|LMNZ9876 |T100         |Toyota       |0            |     13000.00
|ASDF8765 |ModelY       |Tesla        |3            |     25000.00
|BNGH2345 |Supra        |Toyota       |2            |     14000.00
|QWAS4567 |F150         |Ford         |0            |     15500.00
|XZAC1357 |Tundra       |Toyota       |8            |     22000.00
|JKLM5678 |Altima       |Nissan       |2            |     14500.00
|PLMK3457 |Charger      |Dodge        |5            |     17500.00
|XCVB6789 |Explorer     |Ford         |8            |     21000.00
|HGTR2341 |Corvette     |Chevrolet    |3            |     26000.00
|BNVC7894 |Impreza      |Subaru       |2            |     18000.00
|MNBV5612 |Tacoma       |Toyota       |4            |     19000.00
|LKJH4321 |Outback      |Subaru       |7            |     15500.00
|ERDF9876 |Challenger   |Dodge        |6            |     23000.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00
|CVBN3459 |Sonata       |Hyundai      |9            |     17000.00
|XDRT2398 |Pathfinder   |Nissan       |5            |     22000.00
|PLNB7623 |Malibu       |Chevrolet    |4            |     16500.00


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: 2
|Car ID   |Model        |Manufacturer |Quantity     |        Price|Errors                                 
----------------------------------------------------------------------------------------------------------
|FORD2385 |Mustang      |Ford         |12           |     45000.00|First 4 characters in the ID are not all letters or has O
|HOND1572 |Accord       |Honda        |20           |     30000.00|First 4 characters in the ID are not all letters or has O
|TOYT3481 |Corolla      |Toyota       |15           |     28000.00|First 4 characters in the ID are not all letters or has O
|BMWW5267 |X5           |BMW          |10           |     65000.00|Model is less than 3 characters        
|AUDI7435 |A4           |Audi         |18           |     43000.00|Model is less than 3 characters        
|DODG5632 |Charger      |Dodge        |9            |     41000.00|First 4 characters in the ID are not all letters or has O
|FERR6523 |F8           |Ferrari      |3            |    280000.00|Model is less than 3 characters        
|VOLK3749 |Golf         |Volkswagen   |22           |     27000.00|First 4 characters in the ID are not all letters or has O
|PORC6754 |911          |Porsche      |2            |    120000.00|First 4 characters in the ID are not all letters or has O, Model is not only alphanumeric or starts with number
|VOLV8723 |XC90         |Volvo        |13           |     56000.00|First 4 characters in the ID are not all letters or has O
|VBNM4321 |RX           |Mazda        |7            |     16000.00|Model is less than 3 characters        
|MPKZ1205 |RX5          |Mazda        |10           |     15000.00|Last 4 characters in the ID are not all numeric or has 0
|ABCD2340 |R9X          |Nissan       |3            |     14000.00|Last 4 characters in the ID are not all numeric or has 0
|QWER1111 |A1           |Audi         |2            |     12000.00|Model is less than 3 characters, Price is less than or equal to 12,999.00
|ZXCV5678 |9Car         |BadManufac   |4            |     18000.00|Model is not only alphanumeric or starts with number
|TYUI3456 |Camry        |Toyota       |-5           |     20000.00|Quantity is less than 0                
|HGFE7654 |Mustang      |Ford         |1            |     12999.00|Price is less than or equal to 12,999.00
|UIOP1234 |Civic        |Honda        |4            |     13500.00|First 4 characters in the ID are not all letters or has O
|OPLK1276 |Sorento      |Kia          |0            |     14500.00|First 4 characters in the ID are not all letters or has O
|MKOI8765 |Elantra      |Hyundai      |3            |     14000.00|First 4 characters in the ID are not all letters or has O
|OOOO0000 |TA           |BadManufac   |-1           |      1000.00|First 4 characters in the ID are not all letters or has O, Last 4 characters in the ID are not all numeric or has 0, Model is less than 3 characters, Quantity is less than 0, Price is less than or equal to 12,999.00
|AAAA     |AAA          |BadManufac   |0            |     13000.00|ID is not 8 characters long            
|OOOO1111 |AAA          |BadManufac   |0            |     13000.00|First 4 characters in the ID are not all letters or has O
|AAAA0000 |AAA          |BadManufac   |0            |     13000.00|Last 4 characters in the ID are not all numeric or has 0
|AAAA1111 |AA           |BadManufac   |0            |     13000.00|Model is less than 3 characters        
|AAAA1111 |1AA          |BadManufac   |0            |     13000.00|Model is not only alphanumeric or starts with number
|AAAA1111 |AAA          |BadManufac   |-1           |     13000.00|Quantity is less than 0                
|AAAA1111 |AAA          |BadManufac   |0            |         0.00|Price is less than or equal to 12,999.00


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: 3


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: a

Invalid! Only enter a valid option


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: beta

Invalid! Only enter a valid option


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: -2

Invalid! Only enter a valid option


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: 0

Invalid! Only enter a valid option


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: 1

What manufacturer or model car do you want to search for: ford
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00
|QWAS4567 |F150         |Ford         |0            |     15500.00
|XCVB6789 |Explorer     |Ford         |8            |     21000.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: 2
What is your target price (+-12%): 15000
Cars between 13200-16800:
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|BPXZ3478 |RX5          |Mazda        |10           |     15000.00
|ABCD2345 |R9X          |Nissan       |Low Inventory|     14000.00
|BNGH2345 |Supra        |Toyota       |Low Inventory|     14000.00
|QWAS4567 |F150         |Ford         |Low Inventory|     15500.00
|JKLM5678 |Altima       |Nissan       |Low Inventory|     14500.00
|LKJH4321 |Outback      |Subaru       |7            |     15500.00
|PLNB7623 |Malibu       |Chevrolet    |Low Inventory|     16500.00


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: 1

What manufacturer or model car do you want to search for: hyundai
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|HYUN3245 |Sonata       |Hyundai      |19           |     27000.00
|CVBN3459 |Sonata       |Hyundai      |9            |     17000.00


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: 1

What manufacturer or model car do you want to search for: asdf

There are no cars with that manufacturer or model!


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: 2
What is your target price (+-12%): 200000

There are no cars in that price range!


How would you like to search: 
1 Car model or manufacturer
2 Price Range
3 Go back to main menu
Enter your choice: 3

Going back to Main Menu


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: 4


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: a

Invalid! Only enter a valid option


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: alpha

Invalid! Only enter a valid option


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: -2

Invalid! Only enter a valid option


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: 0

Invalid! Only enter a valid option


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: 1
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|ABCD2345 |R9X          |Nissan       |3            |     14000.00
|ASDF8765 |ModelY       |Tesla        |3            |     25000.00
|BNGH2345 |Supra        |Toyota       |2            |     14000.00
|BNVC7894 |Impreza      |Subaru       |2            |     18000.00
|BPXZ3478 |RX5          |Mazda        |10           |     15000.00
|BUIC1387 |Enclave      |Buick        |6            |     40000.00
|CHEV6743 |Camaro       |Chevrolet    |8            |     52000.00
|CVBN3459 |Sonata       |Hyundai      |9            |     17000.00
|ERDF9876 |Challenger   |Dodge        |6            |     23000.00
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00
|HGTR2341 |Corvette     |Chevrolet    |3            |     26000.00
|HYUN3245 |Sonata       |Hyundai      |19           |     27000.00
|JAGU4561 |FType        |Jaguar       |4            |     73000.00
|JKLM5678 |Altima       |Nissan       |2            |     14500.00
|KIAI4128 |Sportage     |Kia          |21           |     32000.00
|LEXU7319 |RX350        |Lexus        |7            |     49000.00
|LKJH4321 |Outback      |Subaru       |7            |     15500.00
|LMNZ9876 |T100         |Toyota       |0            |     13000.00
|MAZD8964 |CX5          |Mazda        |25           |     35000.00
|MNBV5612 |Tacoma       |Toyota       |4            |     19000.00
|MPKZ1265 |ModelX       |Tesla        |5            |     20000.00
|NISS8675 |Altima       |Nissan       |14           |     29000.00
|PLMK3457 |Charger      |Dodge        |5            |     17500.00
|PLNB7623 |Malibu       |Chevrolet    |4            |     16500.00
|QWAS4567 |F150         |Ford         |0            |     15500.00
|SUBA2147 |Outback      |Subaru       |11           |     33000.00
|TESL4321 |ModelS       |Tesla        |5            |     79999.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00
|WERT6789 |Accord       |Honda        |6            |     17000.00
|XCVB6789 |Explorer     |Ford         |8            |     21000.00
|XDRT2398 |Pathfinder   |Nissan       |5            |     22000.00
|XZAC1357 |Tundra       |Toyota       |8            |     22000.00


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: 2
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|WERT6789 |Accord       |Honda        |6            |     17000.00
|JKLM5678 |Altima       |Nissan       |2            |     14500.00
|NISS8675 |Altima       |Nissan       |14           |     29000.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00
|MAZD8964 |CX5          |Mazda        |25           |     35000.00
|CHEV6743 |Camaro       |Chevrolet    |8            |     52000.00
|ERDF9876 |Challenger   |Dodge        |6            |     23000.00
|PLMK3457 |Charger      |Dodge        |5            |     17500.00
|HGTR2341 |Corvette     |Chevrolet    |3            |     26000.00
|BUIC1387 |Enclave      |Buick        |6            |     40000.00
|XCVB6789 |Explorer     |Ford         |8            |     21000.00
|QWAS4567 |F150         |Ford         |0            |     15500.00
|JAGU4561 |FType        |Jaguar       |4            |     73000.00
|BNVC7894 |Impreza      |Subaru       |2            |     18000.00
|PLNB7623 |Malibu       |Chevrolet    |4            |     16500.00
|TESL4321 |ModelS       |Tesla        |5            |     79999.00
|MPKZ1265 |ModelX       |Tesla        |5            |     20000.00
|ASDF8765 |ModelY       |Tesla        |3            |     25000.00
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00
|LKJH4321 |Outback      |Subaru       |7            |     15500.00
|SUBA2147 |Outback      |Subaru       |11           |     33000.00
|XDRT2398 |Pathfinder   |Nissan       |5            |     22000.00
|ABCD2345 |R9X          |Nissan       |3            |     14000.00
|LEXU7319 |RX350        |Lexus        |7            |     49000.00
|BPXZ3478 |RX5          |Mazda        |10           |     15000.00
|CVBN3459 |Sonata       |Hyundai      |9            |     17000.00
|HYUN3245 |Sonata       |Hyundai      |19           |     27000.00
|KIAI4128 |Sportage     |Kia          |21           |     32000.00
|BNGH2345 |Supra        |Toyota       |2            |     14000.00
|LMNZ9876 |T100         |Toyota       |0            |     13000.00
|MNBV5612 |Tacoma       |Toyota       |4            |     19000.00
|XZAC1357 |Tundra       |Toyota       |8            |     22000.00


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: 3
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|BUIC1387 |Enclave      |Buick        |6            |     40000.00
|CHEV6743 |Camaro       |Chevrolet    |8            |     52000.00
|HGTR2341 |Corvette     |Chevrolet    |3            |     26000.00
|PLNB7623 |Malibu       |Chevrolet    |4            |     16500.00
|ERDF9876 |Challenger   |Dodge        |6            |     23000.00
|PLMK3457 |Charger      |Dodge        |5            |     17500.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00
|XCVB6789 |Explorer     |Ford         |8            |     21000.00
|QWAS4567 |F150         |Ford         |0            |     15500.00
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00
|WERT6789 |Accord       |Honda        |6            |     17000.00
|CVBN3459 |Sonata       |Hyundai      |9            |     17000.00
|HYUN3245 |Sonata       |Hyundai      |19           |     27000.00
|JAGU4561 |FType        |Jaguar       |4            |     73000.00
|KIAI4128 |Sportage     |Kia          |21           |     32000.00
|LEXU7319 |RX350        |Lexus        |7            |     49000.00
|MAZD8964 |CX5          |Mazda        |25           |     35000.00
|BPXZ3478 |RX5          |Mazda        |10           |     15000.00
|JKLM5678 |Altima       |Nissan       |2            |     14500.00
|NISS8675 |Altima       |Nissan       |14           |     29000.00
|XDRT2398 |Pathfinder   |Nissan       |5            |     22000.00
|ABCD2345 |R9X          |Nissan       |3            |     14000.00
|BNVC7894 |Impreza      |Subaru       |2            |     18000.00
|LKJH4321 |Outback      |Subaru       |7            |     15500.00
|SUBA2147 |Outback      |Subaru       |11           |     33000.00
|TESL4321 |ModelS       |Tesla        |5            |     79999.00
|MPKZ1265 |ModelX       |Tesla        |5            |     20000.00
|ASDF8765 |ModelY       |Tesla        |3            |     25000.00
|BNGH2345 |Supra        |Toyota       |2            |     14000.00
|LMNZ9876 |T100         |Toyota       |0            |     13000.00
|MNBV5612 |Tacoma       |Toyota       |4            |     19000.00
|XZAC1357 |Tundra       |Toyota       |8            |     22000.00


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: 4
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|QWAS4567 |F150         |Ford         |0            |     15500.00
|LMNZ9876 |T100         |Toyota       |0            |     13000.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00
|JKLM5678 |Altima       |Nissan       |2            |     14500.00
|BNVC7894 |Impreza      |Subaru       |2            |     18000.00
|BNGH2345 |Supra        |Toyota       |2            |     14000.00
|HGTR2341 |Corvette     |Chevrolet    |3            |     26000.00
|ABCD2345 |R9X          |Nissan       |3            |     14000.00
|ASDF8765 |ModelY       |Tesla        |3            |     25000.00
|PLNB7623 |Malibu       |Chevrolet    |4            |     16500.00
|JAGU4561 |FType        |Jaguar       |4            |     73000.00
|MNBV5612 |Tacoma       |Toyota       |4            |     19000.00
|PLMK3457 |Charger      |Dodge        |5            |     17500.00
|XDRT2398 |Pathfinder   |Nissan       |5            |     22000.00
|TESL4321 |ModelS       |Tesla        |5            |     79999.00
|MPKZ1265 |ModelX       |Tesla        |5            |     20000.00
|BUIC1387 |Enclave      |Buick        |6            |     40000.00
|ERDF9876 |Challenger   |Dodge        |6            |     23000.00
|WERT6789 |Accord       |Honda        |6            |     17000.00
|LEXU7319 |RX350        |Lexus        |7            |     49000.00
|LKJH4321 |Outback      |Subaru       |7            |     15500.00
|CHEV6743 |Camaro       |Chevrolet    |8            |     52000.00
|XCVB6789 |Explorer     |Ford         |8            |     21000.00
|XZAC1357 |Tundra       |Toyota       |8            |     22000.00
|CVBN3459 |Sonata       |Hyundai      |9            |     17000.00
|BPXZ3478 |RX5          |Mazda        |10           |     15000.00
|SUBA2147 |Outback      |Subaru       |11           |     33000.00
|NISS8675 |Altima       |Nissan       |14           |     29000.00
|HYUN3245 |Sonata       |Hyundai      |19           |     27000.00
|KIAI4128 |Sportage     |Kia          |21           |     32000.00
|MAZD8964 |CX5          |Mazda        |25           |     35000.00


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: 5
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|LMNZ9876 |T100         |Toyota       |0            |     13000.00
|BNGH2345 |Supra        |Toyota       |2            |     14000.00
|ABCD2345 |R9X          |Nissan       |3            |     14000.00
|JKLM5678 |Altima       |Nissan       |2            |     14500.00
|BPXZ3478 |RX5          |Mazda        |10           |     15000.00
|QWAS4567 |F150         |Ford         |0            |     15500.00
|LKJH4321 |Outback      |Subaru       |7            |     15500.00
|PLNB7623 |Malibu       |Chevrolet    |4            |     16500.00
|WERT6789 |Accord       |Honda        |6            |     17000.00
|CVBN3459 |Sonata       |Hyundai      |9            |     17000.00
|PLMK3457 |Charger      |Dodge        |5            |     17500.00
|BNVC7894 |Impreza      |Subaru       |2            |     18000.00
|MNBV5612 |Tacoma       |Toyota       |4            |     19000.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00
|MPKZ1265 |ModelX       |Tesla        |5            |     20000.00
|XCVB6789 |Explorer     |Ford         |8            |     21000.00
|XDRT2398 |Pathfinder   |Nissan       |5            |     22000.00
|XZAC1357 |Tundra       |Toyota       |8            |     22000.00
|ERDF9876 |Challenger   |Dodge        |6            |     23000.00
|ASDF8765 |ModelY       |Tesla        |3            |     25000.00
|HGTR2341 |Corvette     |Chevrolet    |3            |     26000.00
|HYUN3245 |Sonata       |Hyundai      |19           |     27000.00
|NISS8675 |Altima       |Nissan       |14           |     29000.00
|KIAI4128 |Sportage     |Kia          |21           |     32000.00
|SUBA2147 |Outback      |Subaru       |11           |     33000.00
|MAZD8964 |CX5          |Mazda        |25           |     35000.00
|BUIC1387 |Enclave      |Buick        |6            |     40000.00
|LEXU7319 |RX350        |Lexus        |7            |     49000.00
|CHEV6743 |Camaro       |Chevrolet    |8            |     52000.00
|JAGU4561 |FType        |Jaguar       |4            |     73000.00
|TESL4321 |ModelS       |Tesla        |5            |     79999.00
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00


How would you like to sort by: 
1 ID
2 Model
3 Manufacturer
4 Quantity
5 Price
6 Go back to Main menu
Enter a menu option: 6

Going back to Main Menu


What would you like to do: 
1 See all valid cars
2 See all invalid cars
3 Search for specific car(s)
4 Sort cars
5 Save Low Inventory/Exit
Enter a menu option: 5

Cars with Low Inventory (<5): 
|Car ID   |Model        |Manufacturer |Quantity     |        Price
------------------------------------------------------------------
|LMNZ9876 |T100         |Toyota       |0            |     13000.00
|BNGH2345 |Supra        |Toyota       |2            |     14000.00
|ABCD2345 |R9X          |Nissan       |3            |     14000.00
|JKLM5678 |Altima       |Nissan       |2            |     14500.00
|QWAS4567 |F150         |Ford         |0            |     15500.00
|PLNB7623 |Malibu       |Chevrolet    |4            |     16500.00
|BNVC7894 |Impreza      |Subaru       |2            |     18000.00
|MNBV5612 |Tacoma       |Toyota       |4            |     19000.00
|TYHG3458 |Bronco       |Ford         |1            |     19500.00
|ASDF8765 |ModelY       |Tesla        |3            |     25000.00
|HGTR2341 |Corvette     |Chevrolet    |3            |     26000.00
|JAGU4561 |FType        |Jaguar       |4            |     73000.00
|HGFE7654 |Mustang      |Ford         |1            |   1811999.00


Goodbye!
*/