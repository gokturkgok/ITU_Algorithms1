#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;


class radixNumbers{
private:
    int id;
    long long num;
    
public:
    radixNumbers(){}
    
    void set_numbers(int a, long long b){
        this->id = a;
        this->num = b;
    }
    
    int get_id(){return this->id;}
    long long get_number(){return this->num;}
    
    void setClass(radixNumbers& duplicate){
        this->id = duplicate.get_id();
        this->num = duplicate.get_number();
    }
    
    void readOps(ifstream &inFile){
        inFile >> id >> num ;
        set_numbers(id,num);
    }
    
    ~radixNumbers(){}
    
};


// WRITE TO FILE OPERATION FROM THE radixNumbers LIST
void writeOps(ofstream &outFile , radixNumbers *sample, int limit){
    outFile << fixed << left << setw(5) << "Id" << fixed << right << setw(12) << "Number" << endl;
    for(int counter = 0 ; counter < limit; counter++){
        outFile << fixed << left << setw(6)<< sample[counter].get_id() << "   "
        << fixed << setw(9)<< sample[counter].get_number() << endl;
    }
}

// returns the radixNumbers element which is the maximum
radixNumbers getMax(radixNumbers *liste, int n)
{
    radixNumbers mx;
    mx.setClass(liste[0]);
    for (int i = 1; i < n; i++)
        if (liste[i].get_number() > mx.get_number())
            mx.setClass(liste[i]);
    return mx;
}

void countSort(radixNumbers *countList, int n, int exp)
{
    radixNumbers *outputList = new radixNumbers[n];
    int i;
    int *count = new int[1000000000];
    
    for (i = 0; i < n; i++)
        count[ (countList[i].get_number()/exp)%10 ]++;
    
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    // Build the output class array list
    for (i = n - 1; i >= 0; i--)
    {
        outputList[count[ (countList[i].get_number()/exp)%10 ] - 1].setClass(countList[i]);
        count[ (countList[i].get_number()/exp)%10 ]--;
    }
    
    // set the sorted countList[]
    for (i = 0; i < n; i++)
        countList[i].setClass(outputList[i]) ;
    delete [] count;
    delete [] outputList;
}


void radixsort(radixNumbers *liste, int n)
{
    // Find the maximum number to know number of digits
    radixNumbers m,mx;
    m=getMax(liste, n);
    m.setClass(mx);
    
    // digit variable is used for passing the digits
    for (int digit = 1; (m.get_number())/digit > 0; digit *= 10)
        countSort(liste, n, digit);
}


// Driver program to test above functions
int main(int argc, char **argv)
{
    clock_t timer;
    int N = atoi(argv[1]);
    ifstream inputFile;
    ofstream outFile;
    
    radixNumbers *list = new radixNumbers[N];
    
    inputFile.open("numbers_to_sort.txt");
    int inputCounter = 0;
    if(inputFile.is_open()){
        while(inputCounter < N){
            list[inputCounter].readOps(inputFile);
            inputCounter++;
        }
    }
    timer = clock();
    inputFile.close();
    
    radixsort(list, N);
    timer = clock()-timer;
    
    cout << "\n clock ticks: " << timer << "\n seconds: "  << static_cast<double>(timer) / CLOCKS_PER_SEC << endl << endl;
    
    outFile.open("sorted_numbers.txt");
    if(outFile.is_open()){
        writeOps(outFile,list,N);
    }
    outFile.close();
    
    delete [] list;
    
    return 0;
}
