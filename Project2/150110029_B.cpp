//
//  Gokturk GOK
//  150110029
//
//  AOA - Project 3
//  Part B
//

#include<iostream>
#include<fstream>  // file I/O
#include<iomanip>  // setw(),fixed
#include<ctime>   // clock()
#include<cmath>  // log2()
#include<cstdlib>

using namespace std;
// ******************* PLAYER CLASS ***************************
class player{
private:
    int id;
    int cp;
    
public:
    player(){}
    
    void set_numbers(int a, int b){
        this->id = a;
        this->cp = b;
    }
    int get_id(){ return this->id; }
    int get_cp(){ return this->cp; }
    
    void set_CapturedCP(int captured){ this->cp = this->cp + captured; }
    void set_UpdatedCP(int updatedCp){ this->cp = updatedCp; }
    
    ~player(){}
    
};
// *************************************************************

// to calculate the all Charisma Points in a clan
int allCpOfClan(player *list , int range){
    
    int total = 0;
    for(int i = 0 ; i < range ; i++){
        total += list[i].get_cp();
    }
    return total;
}
// READ FILE OPERATION AND FILL THE CLASS ELEMENT LINE BY LINE
void readOps(ifstream &inFile , player * inBuffer, int limit){
    int inputCounter = 0;
    int id,cp;
    while(inputCounter < limit){
        inFile >> id >> cp;
        inBuffer[inputCounter].set_numbers(id,cp);
        inputCounter++;
    }
    
}
// WRITE FILE OPERATION DATA FROM CLASS ARRAY
void writeOps(ofstream &outFile , player *outBuffer, int limit){
    outFile << fixed << right << setw(3) << "Id" << fixed << right << setw(9) << "CP" << endl;
    for(int counter = 0 ; counter < limit; counter++){
        outFile << fixed << left << setw(6)<< outBuffer[counter].get_id() << "   "
        << fixed << setw(9)<< outBuffer[counter].get_cp() << endl;
    }
}

// swap operation template for swapping 2 classes.
template <class T> void swapPlayers ( T& a, T& b )
{
    T c(a);
    a=b;
    b=c;
}

// ********************************* HEAP SORT *******************************************
void minHeapify( player *heapified, int k, int N){
    while ( k*2 + 1 < N ) {
        int left = 2*k + 1;
        int right = 2*k + 2;
        
        if (( right < N) && (heapified[left].get_cp() > heapified[right].get_cp()))
            left++;
        
        if (heapified[k].get_cp() > heapified[left].get_cp()) {
            swapPlayers( heapified[left], heapified[k] );
            k = left;
        }
        else
            return;
    }
}


void heapSort( player * list, int limit){
    
    for (int k = limit/2; k >= 0; k--) {
        minHeapify( list, k, limit);
    }
    
    while (limit-1 > 0) {
        // swap the maximum value as root  with last element of the heap
        swapPlayers(list[limit-1], list[0]);
        minHeapify(list, 0, limit-1);
        limit--; // previous max value will stay
    }
}
// ***********************************************************************************


// read game by game & update the clan lists according to games by checking whether
// the attacker is Leader, Henchmen or Soldier. After each game, related clan list is sorted
// to be kept up to date for the next operations
void readAndEvaluateLogs(ifstream &inFile , player* sortedListA , player* sortedListB , int limit){
    
    int inputCounter = 0;
    
    
    char clanOfAttacker;
    int GameId, AttackerPosition, AttackedPosition;
    while(inputCounter < limit){
        
        inFile >> GameId  >> clanOfAttacker >> AttackerPosition >> AttackedPosition;
        
        int attackerPlayerIndex = AttackerPosition;
        int attackedPlayerIndex = AttackedPosition;
        // CLAN A ----------
        if(clanOfAttacker == 'A'){
            
            // leader ----> enemy
            if( attackerPlayerIndex == 0 ){
                
                int captured = sortedListB[attackedPlayerIndex].get_cp() / 2;
                sortedListA[attackerPlayerIndex].set_CapturedCP(captured);
                
                int damagerPoint = sortedListB[attackedPlayerIndex].get_cp();
                int attackedCpChange = damagerPoint - damagerPoint / 2;
                if(attackedCpChange < 0){
                    sortedListB[attackedPlayerIndex].set_UpdatedCP(0);
                }
                else{
                    sortedListB[attackedPlayerIndex].set_UpdatedCP(attackedCpChange);
                }
            }
            // henchmen -----> enemy
            if( attackerPlayerIndex >= 1 && attackerPlayerIndex <= 7 ){
                
                int captured = 500;
                sortedListA[attackerPlayerIndex].set_CapturedCP(captured);
                
                int attackedCpChange = sortedListB[attackedPlayerIndex].get_cp() - 500;
                // all CP values should be equal or greater than zero!
                if(attackedCpChange < 0){
                    sortedListB[attackedPlayerIndex].set_UpdatedCP(0);
                }
                else{
                    sortedListB[attackedPlayerIndex].set_UpdatedCP(attackedCpChange);
                }
                
            }
            
            // soldier -------> enemy
            if( attackerPlayerIndex > 7 ){
                
                int heightOfAttacker = log2(attackerPlayerIndex+1);
                int heightOfAttacked = log2(attackedPlayerIndex+1);
                
                int captured = 30 * (abs(heightOfAttacker - heightOfAttacked) + 1);
                sortedListA[attackerPlayerIndex].set_CapturedCP(captured);
                
                int attackedCpChange = sortedListB[attackedPlayerIndex].get_cp() - 120;
                if(attackedCpChange < 0){
                    sortedListB[attackedPlayerIndex].set_UpdatedCP(0);
                }
                else{
                    sortedListB[attackedPlayerIndex].set_UpdatedCP(attackedCpChange);
                }
                
            }
            heapSort(sortedListA,10000);
            heapSort(sortedListB,10000);
            
        }
        // CLAN B ---------------
        if(clanOfAttacker == 'B'){
            
            // leader ----> enemy
            if( attackerPlayerIndex == 0 ){
                
                int captured = sortedListA[attackedPlayerIndex].get_cp() / 2;
                sortedListB[attackerPlayerIndex].set_CapturedCP(captured);
                
                int damagerPoint = sortedListB[attackedPlayerIndex].get_cp();
                int attackedCpChange = damagerPoint - damagerPoint / 2;
                if(attackedCpChange < 0){
                    sortedListA[attackedPlayerIndex].set_UpdatedCP(0);
                }
                else{
                    sortedListA[attackedPlayerIndex].set_UpdatedCP(attackedCpChange);
                }
                
            }
            // henchmen ------> enemy
            if( attackerPlayerIndex >= 1 && attackerPlayerIndex <= 7 ){
                
                int captured = 500;
                sortedListB[attackerPlayerIndex].set_CapturedCP(captured);
                
                int attackedCpChange = sortedListA[attackedPlayerIndex].get_cp() - 500;
                // all CP values should be equal or greater than zero!
                if(attackedCpChange < 0){
                    sortedListA[attackedPlayerIndex].set_UpdatedCP(0);
                }
                else{
                    sortedListA[attackedPlayerIndex].set_UpdatedCP(attackedCpChange);
                }
                
            }
            
            // soldier -------> enemy
            if( attackerPlayerIndex > 7 ){
                
                // making all indexes +1 to evaluate the true value of height
                int heightOfAttacker = log2(attackerPlayerIndex+1);
                int heightOfAttacked = log2(attackedPlayerIndex+1);
                
                // Hit point of Soldier as "captured"
                int captured = 30 * (abs(heightOfAttacker - heightOfAttacked) + 1);
                sortedListB[attackerPlayerIndex].set_CapturedCP(captured);
                
                // attackedCpChange is changed to its new cp after damage
                int attackedCpChange = sortedListA[attackedPlayerIndex].get_cp() - 120;
                if(attackedCpChange < 0){
                    sortedListA[attackedPlayerIndex].set_UpdatedCP(0);
                }
                else{
                    sortedListA[attackedPlayerIndex].set_UpdatedCP(attackedCpChange);
                }
            }
            heapSort(sortedListA,10000);
            heapSort(sortedListB,10000);
        }
        inputCounter++;
    }
    
}




int main(int argc, char **argv)
{
    ifstream inputFileA,inputFileB,inputFileLog;
    ofstream outFile;
    clock_t timer;
    
    int question = atoi(argv[1]);
    
    if( question ==1){
        
        // N REPRESENTS THE NUMBER OF PLAYERS TO BE SORTED
        int N = atoi(argv[2]);
        player *list = new player[N];
        
        inputFileA.open("ClanA.txt");
        if(inputFileA.is_open()){ readOps(inputFileA, list , N); }
        inputFileA.close();
        
        timer = clock();
        heapSort(list, N);
        timer = clock()-timer;
        
        cout << "Running time of Heap Sort with "<< N << " numbers: " << endl;
        cout << "\n clock ticks: " << timer << "\n seconds: " << static_cast<double>(timer) / CLOCKS_PER_SEC << endl;
        
        outFile.open("A_sorted.txt");
        if(outFile.is_open()){  writeOps(outFile,list,N);  }
        outFile.close();
        
        delete [] list;
        
        return 0;
    }
    if ( question == 2 ){
        
        int range = 10000;
        
        player *listA = new player[range];
        player *listB = new player[range];
        
        inputFileA.open("ClanA.txt");
        if(inputFileA.is_open()){ readOps(inputFileA, listA , range); }
        inputFileA.close();
        
        heapSort(listA, range);
        
        inputFileB.open("ClanB.txt");
        if(inputFileB.is_open()){ readOps(inputFileB, listB , range); }
        inputFileB.close();
        
        heapSort(listB, range);
        
        
        inputFileLog.open("gamelogs.txt");
        timer = clock();
        
        if(inputFileLog.is_open()){ readAndEvaluateLogs(inputFileLog, listA, listB, range); }
        timer = clock()-timer;
        
        inputFileLog.close();
        
        
        cout << "Handling the results of games 2 sorting at each one is taken : " << endl;
        cout << "\n clock ticks: " << timer << "\n seconds: "  << static_cast<double>(timer) / CLOCKS_PER_SEC << endl << endl;
        
        
        cout << "Clan A : "<< allCpOfClan(listA, range) << " CP" << endl;
        cout << "Clan B : "<< allCpOfClan(listB, range) << " CP" << endl << endl;
        
        // deciding which clan is winner by displaying the total CP's of each clan
        if(allCpOfClan(listA, range) > allCpOfClan(listB, range)){
            
            cout << "Winner is Clan A : "<< allCpOfClan(listA, range) << " CP" << endl;
            outFile.open("A_results.txt");
            
            if(outFile.is_open()){  writeOps(outFile,listA,range);  }
            
            outFile.close();
        }
        else{
            cout << "Winner is Clan B : "<< allCpOfClan(listB, range) << " CP" << endl;
        }
        
        outFile.open("A_results.txt");
        if(outFile.is_open()){  writeOps(outFile,listA,range);  }
        outFile.close();
        
        
        
        delete [] listA;
        delete [] listB;
        
        return 0;
    }
    
}
