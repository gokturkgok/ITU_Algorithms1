//
//  Analysis of Algorithms 1
//  Proje-1
//  21.10.2016
//
//  Göktürk Gök
//  150110029
//

#include <fstream>   // file operations fstream,istream
#include <iostream>  // cout and cin
#include <cstdlib>
#include <ctime>     // clock()
#include <math.h>    // sqrt(),abs()
#include <cmath>
#include <iomanip>   // setw(),setprecision()


using namespace std;

// ****************************** Warehouse Class ********************************************
class Warehouse{
    
private:
    int id;
    int x_axis;
    int y_axis;
    float distance;
    
public:
    Warehouse(){}

    void set_warehouse(int i , int x , int y){
        id = i;
        x_axis = x;
        y_axis = y;
    }
    
    // distance calculator
    void set_distance(int targetX , int targetY){
        this->distance = sqrt(pow((x_axis - targetX),2)+pow((y_axis - targetY),2));
    }
    
    float get_distance(){ return distance; }
    
    // for the id comparison in linear search
    int get_id(){return this->id;}   // to get ID of the current warehouse
    
    // file operations
    void readInputs(ifstream &, int, int);
    void writeOutputs(ofstream & );
    
    ~Warehouse(); // destructor
    
};

// ************************** File READ/WRITE OPERATIONS *************************************
void Warehouse::readInputs(ifstream &inFile, int targetX, int targetY){
   
    inFile >> id >> x_axis >> y_axis ;
    set_distance(targetX, targetY);
    
}
void Warehouse::writeOutputs( ofstream &outFile ){
    
    outFile << setprecision(8);
    outFile << fixed << left << setw(10) << id << fixed << left << setw(15) << x_axis ;
    outFile << fixed << left << setw(15) << y_axis << fixed << left << setw(25) << setprecision(2)<< get_distance() << endl;
    
}

Warehouse::~Warehouse(){};   // destructor of warehouse object

// ****************************************************************************************

// *********************************** INSERTION SORTER ***********************************

void insertion_sorter(Warehouse *list, int range){
    Warehouse sample;
    int i, j;
    Warehouse temp;
    for (i = 1; i < range; i++) {
        sample = list[i];
        j = i-1;
        while (j >= 0 && list[j].get_distance() > sample.get_distance()) {
            temp = list[j+1];
            list[j+1] = list[j];
            list[j] = temp;
            j--;
        }
    }
}
// ****************************************************************************************

// ***************************** LINEAR SEARCHER ******************************************

// to find and get the maximum element of unsorted array
Warehouse findMaxElement(Warehouse *element , int size){
    
    Warehouse maxTemp = element[0];
    
       for(int i = 1; i < size; i++){
        
        if(element[i].get_distance() > maxTemp.get_distance()){
    
                maxTemp = element[i];
        }
       
    }
    return maxTemp;
}

Warehouse *linear_searcher(Warehouse *list, int N, int K)
{
    Warehouse *temp = new Warehouse[K];
    Warehouse maxTempElement ;
    
    for(int i = 0; i < K ; i++){
        temp[i] = list[i];
    }
    
    for (int j = K ; j < N; j++) {
        maxTempElement = findMaxElement(temp, K);
        for (int z = 0; z < K; z++) {
            if(list[j].get_distance() < maxTempElement.get_distance() ){
               
                if(temp[z].get_id() == maxTempElement.get_id()){
                    temp[z] = list[j];
                    break;
                }
            }
        }
    }
    return temp;
}
// ****************************************************************************************

// ******************************** MERGE SORTER ******************************************

void merger(Warehouse *list, int range, int middle){
    int i, j, k;
    // allocation for the temporary memory to merge
    Warehouse * temp = new Warehouse[range];
    
    for (i = 0, j = middle, k = 0; k < range; k++) {
        temp[k] = j == range ? list[i++]
        : i == middle ? list[j++]
        : list[j].get_distance() < list[i].get_distance() ? list[j++]
        : list[i++];
    }
    for (i = 0; i < range; i++) {
        list[i] = temp[i];
    }
    delete[] temp;
}

void merge_sorter(Warehouse *list, int range){
    if (range < 2)
        return;
    int middle = range / 2;
    merge_sorter(list, middle);   // first half recursive
    merge_sorter(list + middle, range - middle); // second half recursive
    merger(list, range, middle);  // to merge the separate halves
}
// ****************************************************************************************

// main function with command line arguments
int main(int argc, char** argv){
    
    ifstream inFile;        // warehouselocations.txt
    ofstream outFile;       // output.txt
    string whichAlgorithm;  // Algorithm Type : IS , MS , LS
    
    int counter = 0;
    
    clock_t timer;      // to get execution time
    
    // arguments from command line arguments
    int N = atoi(argv[1]);          // N
    int K = atoi(argv[2]);          // K
    whichAlgorithm = argv[3];       // IS , MS , LS
    int target_x = atoi(argv[4]);   // Target X Coordinate
    int target_y = atoi(argv[5]);   // Target Y Coordinate
    
    
    if (argc < 5){
        cerr << "Usage: ./a.out N K <IS , MS , LS> < Target x-axis > < Target y-axis >" << endl;
        exit(1);
    }
    
    // memory allocation for the closest K-Warehouses
    Warehouse *list = new Warehouse[N] ;
    
    inFile.open("warehouselocations.txt");
    
    if(inFile.is_open()){
        while(counter < N ){
            list[counter].readInputs(inFile, target_x, target_y);
            counter++;
        }
    }
    else
        cerr << " warehouse.txt couldn't be opened & read ! " << endl;
    
    inFile.close();
    
    // to keep the list of desired K-warehouse list from linear_searcher function
    Warehouse *tempLinearSearchList = new Warehouse[K];
    
    
    if (whichAlgorithm == "IS"){
        
        // to calculate the execution time for sorting,
        // afte timer set, then function will work , finally we can take the execution time
        // by subtracting them
        timer = clock();
        insertion_sorter(list, N);
        timer = clock() - timer ;   // to calculate exact running time of insertion_sorter
        
    }
    else if(whichAlgorithm == "MS"){
        timer = clock();
        merge_sorter(list, N);
        timer = clock() - timer;
    }
    else if(whichAlgorithm == "LS"){
        
        timer = clock();
        tempLinearSearchList = linear_searcher(list, N, K);
        timer = clock() - timer;
    }
    
    cout << whichAlgorithm << "\n clock ticks: " << timer << "\n seconds: "  << static_cast<double>(timer) / CLOCKS_PER_SEC << endl;
    
    // output operations
    counter = 0;
    outFile.open("output.txt");
    if(outFile.is_open()){
        outFile << fixed << left << setw(10) << "id" << fixed << left << setw(15) << "x-axis" << fixed << left << setw(15) << "y-axis";
        outFile << fixed << left << setw(15) << "Closest Warehouses \n";
        
        if(whichAlgorithm == "IS"){
            while(counter < K){
                list[counter].writeOutputs( outFile );
                counter++;
            }
        
        }
        else if(whichAlgorithm == "MS"){
            while(counter < K){
                list[counter].writeOutputs( outFile );
                counter++;
            }
        }
        else if(whichAlgorithm == "LS"){
            while(counter < K){
                tempLinearSearchList[counter].writeOutputs(outFile);
                counter++;
            }
        }
    }
    else{
        cerr << "Unable to write output.txt" << endl;
    }
    
    outFile.close();
    
    delete [] tempLinearSearchList;
    delete [] list;
    
    return 0;
    
}
