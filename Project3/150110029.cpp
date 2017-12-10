
//  BLG335E AOA 1
//  PROJECT - 4
//
//  GOKTURK GOK
//  150110029
//
//  12.12.2016
// EMPTY = "" , DELETED = "~~~~0~~~~"


#include<iostream>
#include<fstream>  // file I/O
#include<cstdlib>  // atoi()
#include<string>

using namespace std;

class myHash {
public:
    int _tableSize;
    int collision=0;
    
    myHash(int tableSize); // Creates a hash table that can store up to tableSize entries.(Constructor)
    
    void insert(string s); // Inserts w into the hash table.
    int hashFunc(string s); // Computes the hash value of w.
    
    bool retrieve(string s); // Finds index of a given word in the hash table, if it isn't inthe table, starts for spell checking and suggests similiar words if any found.
    bool remove(string s); // Removes given word from the dictionary.
    
    // ************* Additional Methods ******************
    bool spell_checker(string s);
    int search(string s);
    void readParseEvaluateFile(ifstream &inputFile);
    void writeOutputFile(ofstream &outputFile );
    // ***************************************************

    ~myHash();
private:
    string *myHashTable; // Stores words.
    int totalEntries=0; // Stores the current number of entries in the table.
    
};
// myHash constructor allocates myHash dynamically
myHash::myHash(int tableSize){
    this->myHashTable = new string[tableSize];
    this->_tableSize = tableSize;
}

// deallocate the table when it is destructed.
myHash::~myHash(){
    delete [] myHashTable;
}


// compute the slot index from the key s
int myHash::hashFunc(string s){
    
    long long int indexNo = 1; // it's 1 for multiplication method
    
    // calculation of the ascii(w1)*ascii(w2)*...*ascii(wN) mod _tableSize
    // ascii value of characters are taken with int(s[index])
    for(int i = 0; i < s.length() ; i++){
        long long int remainderAscii = int(s[i]) % _tableSize;
        indexNo *= remainderAscii;
        // every step of the multiplication takes the remainder to AVOID overload!
        indexNo = indexNo % _tableSize;
    }
    return (int)indexNo;
    
}
// EMPTY = "" , DELETED = "~~~~0~~~~"
void myHash::insert(string s){
    long long int index = hashFunc(s);
    bool isInserted = false;
    
    // check whether it is already inserted at its original slote or not
    if(myHashTable[index] == s){
        cout << "h(x) result is "<<index<< " for: " << s << endl;
        cout<<"WARNING: The word '"<<s<<"' is already in the dictionary!"<<endl;
        cout<<"COLLISIONS :" << collision << endl;
        cout<<"-------------------------------------------------------------------------"<<endl;
        isInserted = true;
    }
    
    // if it can be inserted in slot where it is suppose to be
    if(myHashTable[index] == "" || myHashTable[index] == "~~~~0~~~~"){
        
        this->myHashTable[index] = s;
        this->totalEntries++;
        
        cout<<"INSERT: The word '"<<s<<"' is put in the cell number "<<index<<endl;
        cout<<"COLLISIONS :" << collision << endl;
        cout<<"--------------------------------------------------------------------------"<<endl;
        isInserted = true;
    }
    
    long long int traverse = index +1 ;
    while(!isInserted){
        this->collision++;
        // If the current slot is NOT EMPTY("") or DELETED("~~~~0~~~~")
        if(!(myHashTable[traverse] == "" || myHashTable[traverse] == "~~~~0~~~~")){
            if(myHashTable[traverse] == s){
                cout << "h(x) result is "<<index<< " for: " << s << endl;
                cout<<"WARNING: The word '"<<s<<"' is already in the dictionary!"<<endl;
                cout<<"COLLISIONS :" << collision << endl;
                cout<<"------------------------------------------------------------------"<<endl;
                isInserted = true;
            }
            // If
            // eger tekrar kendisine geldiyse boş slot bulamamış demektir!
            if(traverse==index){
                cout << "h(x) result is "<<index<< " for: " << s << endl;
                cout<<"WARNING: There are no empty space in the hash table for the word: "<<s<<endl;
                cout<<"COLLISIONS :" << collision << endl;
                cout<<"-------------------------------------------------------------------------"<<endl;
                isInserted = true;
            }
        }
        // If the current slot IS EMPTY("") or DELETED("~~~~0~~~~")
        else{
            this->myHashTable[traverse] = s;
            this->totalEntries++;
            cout << "h(x) result is "<<index<< " for: " << s << endl;
            cout<<"INSERT: The word '"<<s<<"' is put in the cell number "<<traverse<<endl;
            cout<<"COLLISIONS :" << collision << endl;
            cout<<"-------------------------------------------------------------------------"<<endl;
            
            isInserted = true;
            break;
        }
        // next slot
        traverse++;
        // allows it to traverse as table size
        traverse = traverse % _tableSize;
        
    }
}

bool myHash::spell_checker(string s){
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    bool checker = false;
    // buffer is used for modifying and check
    string buffer = s;
    // allows us to keep collision number while using the search() function
    // because search can increase the collision but not allowed here !
    int keepCollision = collision;
    
    cout <<"SUGGESTIONS for "<<s<<": ";
    // visit, modify and search given 'word' character by character
    for (int i = 0; i < s.length() ; i++) {
        for (int j = 0; j < alphabet.length(); j++) {
            // to avoid looking for the same character the word has
            if(s[i] != alphabet[j]){
                s[i] = alphabet[j];
                long long int index = search(s);
                
                // if suggested word is found then display it
                if(index != -1){
                    if(myHashTable[index] != buffer){
                        /*notFoundCounter++;
                         if(NotFoundCounter != 1){
                         firstNotFoundCollision = collision
                         }*/
                        cout << myHashTable[index] << ", ";
                        checker = true;
                    }
                }
            }
        }
        // to keep the original string 's' for modifying again as original
        s = buffer;
    }
    collision = keepCollision;
    cout <<endl;
    
    if (checker) return true;
    else return false;
}

bool myHash::retrieve(string s){
    bool isFound = false;
    long long int slotNumber = hashFunc(s);
    
    // check whether it is already located at its original slote or not
    if(myHashTable[slotNumber] == s){
        cout<<"RETRIEVE: The word '"<<s<<"' found in the dictionary with index: "<<slotNumber<<endl;
        cout<<"COLLISIONS :" << collision << endl;
        cout<<"-------------------------------------------------------------------------"<<endl;
        isFound = true;
        return true;
    }
    // check from slotNumber where the key is suppose to be located by the end of table
    for(long long int index = slotNumber +1 ; index < this->_tableSize  ; index++){
        this->collision++;
        if(myHashTable[index] == s){
            cout<<"RETRIEVE: The word '"<<s<<"' found in the dictionary with index: "<<index<<endl;
            cout<<"COLLISIONS :" << collision << endl;
            cout<<"-------------------------------------------------------------------------"<<endl;
            isFound = true;
            return true;
            break;
        }
    }
    
    if(!isFound){
        // check from beginning of table by the slotNumber if it cannot be found before
        for(int j = 0; j < slotNumber ; j++ ){
            // traversing the table increase the collision
            this->collision++;
            if(myHashTable[j] == s){
                cout<<"RETRIEVE: The word '"<<s<<"' found in the dictionary with index: "<<j<<endl;
                cout<<"COLLISIONS :" << collision << endl;
                cout<<"-------------------------------------------------------------------------"<<endl;
                isFound = true;
                return true;
                break;
            }
            
        }
        
    }
    // If it can not be found because of traversing all the table
    // collision equals the table size of the hash table.
    this->collision = this->_tableSize;
    return false;
    
}

int myHash::search(string s){
    bool isFound = false;
    long long int index = hashFunc(s);
    // if it is located its original slot in table
    if(myHashTable[index] == s){
        isFound = true;
        return (int)index;
    }
    // if it cannot be found at its slot than traverse the table after it
    // this operations apply the Linear Probing
    for(long long int counter = index +1 ; counter < _tableSize  ; counter++){
        this->collision++;
        
        // while searching if there is an "EMPTY" slot after its original home slot ( = hashFunc(word) )
        // then the word can not be found because if the word would have been in the table slot
        // after its original home slot , word could have been inserted there ( next empty slot )
        if(myHashTable[counter] == ""){
            isFound = true;
            return -1;
            break;
        }
        if(myHashTable[counter] == s){
            isFound = true;
            return (int) counter;
            break;
        }
    }
    // Traverse from beginning of the table by the original slot
    // If cannot be found at any place after the original slot index
    if(!isFound){
        for(int counter = 0 ; counter < index  ; counter++){
            this->collision++;
            if(myHashTable[counter] == ""){
                isFound = true;
                return -1;
                break;
            }
            if(myHashTable[counter] == s){
                return counter;
                break;
            }
        }
    }
    // if it cannot be found in table by traversing all slots in table
    // then collision equals the table size
    this->collision = this->_tableSize;
    return -1;
    
}

bool myHash::remove(string s){
    long long int slot = search(s);
    // if search() function returns as -1 which means it is not found,
    // then do 'Lazy Deletion'.
    if( slot != -1){
        myHashTable[slot] = "~~~~0~~~~"; // lazy deletion
        this->totalEntries--;
        return true;
    }
    else{
        return false;
    }
}

// Read the input file given as argument (.txt)
// Parse the line as 'command' and 'word'
// Evaluate the operations according to taken command
void myHash::readParseEvaluateFile(ifstream &inFile ){
    
    long long int totalColisions = 0;
    // _tableSize = n;
    
    
    string row,command,word;
    
    while (!inFile.eof()) {
        inFile >> row;
        string separator = ":";
        // separating operations for the row taken from input file
        command = row.substr(0, row.find(separator)); // token command before ":"
        word = row.erase(0,row.find(separator) + separator.length()); // token word after":"
        
        if(command == "insert"){
            insert(word);
            totalColisions += this->collision;
            this->collision = 0;
        }
        else if(command == "retrieve"){
            if( retrieve(word)){
                totalColisions += this->collision;
                this->collision = 0;
            }
            else{
                cout<<"The word '"<<word<<"' couldn't be found in dictionary."<<endl;
                
                cout << "Looking for possible suggestions." << endl;
                // Suggestions are released
                if(spell_checker(word)){}
                cout<<"COLLISIONS :" << collision << endl;
                cout<<"-------------------------------------------------------------------------"<<endl;
                
                totalColisions += this->collision;
                this->collision = 0;
            }
            
        }
        else if(command == "delete"){
            if(remove(word)){
                cout<<"REMOVE: The word '"<<word<<"' is removed from the dictionary."<<endl;
                cout<<"COLLISIONS :" << collision << endl;
                cout<<"-------------------------------------------------------------------------"<<endl;
                
                totalColisions += this->collision;
                this->collision = 0;
            }
            else{
                cout << "REMOVE: The word '"<<word<<"' desired to be removed is NOT found and REMOVED!" << endl;
                cout<<"COLLISIONS :" << collision << endl;
                cout<<"-------------------------------------------------------------------------"<<endl;
                
                totalColisions += this->collision;
                this->collision = 0;
            }
        }
        else{
            //cerr << "Wrong command has been read !" << endl;
            break;
        }
        // to prepare the separators for the next row
        command = command.erase();
        row = row.erase();
        
    }
    cout << "======================================================"<<endl;
    cout << "TOTAL COLLISIONS: "<<totalColisions<<endl;
}

void myHash::writeOutputFile(ofstream & outputFile){
    for(int i = 0; i < _tableSize; i++){
        outputFile << i << ": " << myHashTable[i] << endl;
    }
    cout << "Final form of hash table is saved as 150110029_output.txt" << endl;
}

int main(int argc, char **argv){
    ifstream inputFile;
    ofstream outputFile;
    int N;  // for hash table length
    
    char *inputFileName = argv[1]; // input file name
    N = atoi(argv[2]);  // hash table length
    
    myHash myHashTable(N);
    inputFile.open(inputFileName);
    
    if (inputFile.is_open()) {
        myHashTable.readParseEvaluateFile(inputFile);
    }
    
    outputFile.open("150110029_output.txt");
    if(outputFile.is_open())
        myHashTable.writeOutputFile(outputFile);
    outputFile.close();
    
    return 0;
    
}
