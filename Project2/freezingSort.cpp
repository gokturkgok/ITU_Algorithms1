//*********************************
// GOKTURK GOK
//  150110029

//  AOA_3
//  PART A - FREEZE SORT ANALYSIS
//*********************************
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

void freezingSort(number *arr,int n){
    for (int freeze_count = 0; freeze_count < n; freeze_count++)
    {
        //Freeze from left
        for (int i = freeze_count; i < (n + freeze_count) / 2 +1; i++)
        {
            if (i != n - i + freeze_count && arr[i].get_number() > arr[n - i + freeze_count].get_number())
            {
                swapNumbers(arr[n - i + freeze_count], arr[i]);
            }
        }
        //Freeze from right
        if (freeze_count != 0)
        {
            for (int i = n - freeze_count; i > (n - freeze_count) / 2; i--)
            {
                if (i != n - i - freeze_count && arr[n - i - freeze_count].get_number() > arr[i].get_number())
                {
                    swapNumbers(arr[n - i - freeze_count], arr[i]);
                }
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
    cout << "      Unsorted Array   " << endl;
    for(int i = 0; i < 13 ; i++){
        cout <<  "arr[" << fixed << right << setw(2)<<i<<fixed <<left<<setw(2)<< "]: "<< "   "
        << fixed << right << setw(3)<< arr[i].get_id() <<  " " << fixed << right << setw(9)<< arr[i].get_number() << endl;
    }
    
    freezingSort(arr,13);
    cout << "      Sorted Array   " << endl;
    
    cout  << fixed << right << setw(2) << "Arrays" <<fixed <<left<<setw(2)<< ": "<< "   " << fixed << right << setw(3)<< "Index" <<  " " << fixed << right << setw(9)<< "Number" << endl;
    for(int i = 0; i < 13 ; i++){
        cout <<  "arr[" << fixed << right << setw(2)<<i<<fixed <<left<<setw(2)<< "]: "<< "   "
        << fixed << right << setw(3)<< arr[i].get_id() <<  " " << fixed << right << setw(9)<< arr[i].get_number() << endl;
    }
    cout << endl;
    cout << endl;
    
    
    return 0;
}