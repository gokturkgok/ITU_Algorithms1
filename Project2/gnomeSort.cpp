
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;

class number{
private:
    int id,number;
public:
    void set_number(int a, int b){
        this->id = a;
        this->number = b;
    }
    
    int get_id(){
        return this->id;
    }
    
    int get_number(){
        return this->number;
    }
    
};

// swap operation template for swapping 2 classes.
template <class T> void swapNumbers ( T& a, T& b )
{
    T c(a);
    a=b;
    b=c;
}

int getMax(int arr[], int n)
{
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}


void gnomeSort(number *a, int size){
    
    int i = 1;
    int j = 2;
    
    while (i < size){
        
        if(a[i-1].get_number() >= a[i].get_number()){
            i = j;
            j++;
        }
        else{
            swapNumbers(a[i-1], a[i]);
            i--;
            if(i == 0){
                i = j;
                j++;
            }
        }
    }
}


int main(){
    
    int array[] = {12,3,1,313,45,21,44,5,53,2,32,151,44};
    number *arr = new number[13];
    for(int i= 0; i < 13 ; i++){
        arr[i].set_number(i, array[i]);
    }
    
    gnomeSort(arr,13);
    
    cout  << fixed << right << setw(2) << "Arrays" <<fixed <<left<<setw(2)<< ": "<< "   " << fixed << right << setw(3)<< "Index" <<  " " << fixed << right << setw(9)<< "Number" << endl;
    for(int i = 0; i < 13 ; i++){
        cout <<  "arr[" << fixed << right << setw(2)<<i<<fixed <<left<<setw(2)<< "]: "<< "   "
        << fixed << right << setw(3)<< arr[i].get_id() <<  " " << fixed << right << setw(9)<< arr[i].get_number() << endl;
    }
    cout << endl;
    cout << endl;
    
    
    return 0;
}