//
//
//  aoa 5
//  GOKTURK GOK
//  150110029
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

class RbNode{
    
private:
    // RbNode and RbTree connection
    friend class RedBlackTree;
    
protected:
    string personName;
    char gender;
    int age;
    string color;
    
    int size;
    int num_adult,num_non_adult;
    
    RbNode *parent,*left,*right;
    
public:
    RbNode(){
        personName = "";
        gender = '\0';
        age = 0;
        color = "";
        size = 0;
        
        parent = NULL;
        left = NULL;
        right = NULL;
    }
    
    RbNode(string n , char g , int a){
        personName = n;
        gender = g;
        age = a;
        color = "";
        num_adult = 0;
        num_non_adult = 0;
        size = 1;
        
        parent = NULL;
        left = NULL;
        right = NULL;
    }
    
    string getPersonName(){ return personName; }
    string getColor(){ return color; }
    
    
    void setColor(string colo){ color = colo; }
    void setAdultNum(int adNum){ num_adult = adNum; }
    void setNonAdultNum(int nonAdNum){ num_non_adult = nonAdNum; }
    //void setincreasedAge(int x){ age += x; }
    
    ~RbNode();
    
};

class RedBlackTree{
protected:
    RbNode *Root;
    RbNode *NIL ;
    
    int nTh_adult_num ;
    int nTh_non_adult_num ;
    
public:
    // construction of red black tree
    // Root's parent is NIL and NIL's parent should be NULL
    // NILs' should be BLACK all the time.
    RedBlackTree(){
        this->NIL = new RbNode();
        this->NIL->setColor("BLACK");
        this->NIL->parent = NULL;
        // size of the sentinels(NIL) should be set to zero.
        // because they are not an actual node to be operated
        this->NIL->size = 0;
        
        this->Root = NIL;
        
        nTh_adult_num = 0;
        nTh_non_adult_num = 0;
    }
    // insert operations for the new node to be implemented
    void insertNewRbNode(RbNode*);
    void insertFixUpTree(RbNode*);
    
    // rotation functions
    void leftRotateTree(RbNode*);
    void rightRotateTree(RbNode*);
    
    void readImplementArrangeTree(ifstream&);
    void printRedBlackTree(RbNode*,string);
    
    void fixedSizeOfTree(RbNode *);
    
    
    void fixedAdultAndNonAdultNums(RbNode *);
    
    void num_adult(RbNode *,int);
    void num_non_adult(RbNode *,int);
    
    void increase_one_age(RbNode *);
    
    RbNode * getTheRoot(){ return Root; }
    
    ~RedBlackTree();
};

void RedBlackTree::insertNewRbNode(RbNode *newPerson){
    RbNode *y = NIL;
    RbNode *x = Root;
    
    // preparing to locate new node
    while( x != NIL){
        y = x;
        if(newPerson->getPersonName() < x->getPersonName()){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    newPerson->parent = y;
    if(y == NIL){ // if tree is empty
        Root = newPerson;
        Root->parent = NIL;
        
    }
    else if (newPerson->getPersonName() < y->getPersonName()){
        y->left = newPerson;
    }
    else{
        y->right = newPerson;
    }
    
    newPerson->left = NIL;
    newPerson->right = NIL;
    newPerson->setColor("RED");
    insertFixUpTree(newPerson);
    
}

void RedBlackTree::insertFixUpTree(RbNode *insertedNode){
    
    while(insertedNode->parent->getColor() == "RED"){
        if(insertedNode->parent == insertedNode->parent->parent->left){
            
            RbNode *y = insertedNode->parent->parent->right;
            
            // if uncle is red then make that parent BLACK
            if(y->getColor() == "RED"){
                insertedNode->parent->setColor("BLACK");
                y->setColor("BLACK");
                insertedNode->parent->parent->setColor("RED");
                insertedNode = insertedNode->parent->parent;
            }
            else {
                if (insertedNode == insertedNode->parent->right){
                    insertedNode = insertedNode->parent;
                    leftRotateTree(insertedNode);
                }
                insertedNode->parent->setColor("BLACK");
                insertedNode->parent->parent->setColor("RED");
                rightRotateTree(insertedNode->parent->parent);
            }
            
        }
        else{
            // y: uncle of the inserted node
            RbNode *y = insertedNode->parent->parent->left;
            
            if(y->getColor() == "RED"){
                insertedNode->parent->setColor("BLACK");
                y->setColor("BLACK");
                insertedNode->parent->parent->setColor("RED");
                insertedNode = insertedNode->parent->parent;
            }
            else {
                if (insertedNode == insertedNode->parent->left){
                    
                    insertedNode = insertedNode->parent;
                    rightRotateTree(insertedNode);
                }
                insertedNode->parent->setColor("BLACK");
                insertedNode->parent->parent->setColor("RED");
                leftRotateTree(insertedNode->parent->parent);
            }
        }
    }
    // Keeps the color of the root always as BlACK
    Root->setColor("BLACK");
    
    // After the whole insert operations fixing and updating the size of nodes are handled here
    fixedSizeOfTree(Root);
    
}

void RedBlackTree::leftRotateTree(RbNode *toLeft){
    RbNode *rotator = toLeft->right;
    toLeft->right = rotator->left;     // turn left subtree of rotator into right subtree of toLeft
    
    if(rotator->left != NIL){
        rotator->left->parent = toLeft;
    }
    // linking parent of the toLeft to rotator
    // makes toLeft and rotator siblings
    rotator->parent = toLeft->parent;
    
    // if parent of the inserted node is NIL then it means that there are no inserted nodes into tree yet
    // that is why it is gonna be located on the Root as first inserted node
    if(toLeft->parent == NIL){
        Root = rotator;
    }
    else if( toLeft == toLeft->parent->left ){
        toLeft->parent->left = rotator;
    }
    else{
        toLeft->parent->right = rotator;
    }
    // putting the toLeft on rotator's left node
    rotator->left = toLeft;
    toLeft->parent = rotator;
    
    // fixed-size operations on rotation for the only two nodes which are changed on tree
    rotator->size = toLeft->size;
    toLeft->size = toLeft->left->size + toLeft->right->size + 1;
}


void RedBlackTree::rightRotateTree(RbNode *toRight){
    RbNode *rotator = toRight->left;
    toRight->left = rotator->right;     // turn right subtree of rotator into left subtree of toRight
    
    if(rotator->right != NIL){
        rotator->right->parent = toRight;
    }
    
    // linking parent of the toRight to rotator
    rotator->parent = toRight->parent;
    // if parent of the inserted node is NIL then it means that there are no inserted nodes into tree yet
    // that is why it is gonna be located on the Root as first inserted node
    if(toRight->parent == NIL){
        Root = rotator;
    }
    else if( toRight == toRight->parent->right ){
        toRight->parent->right = rotator;
    }
    else{
        toRight->parent->left = rotator;
    }
    
    // locate the toRight on rotator's right node
    rotator->right = toRight;
    toRight->parent = rotator;
    
    // fixed-size operations on rotation for the only two nodes which are changed on tree
    rotator->size = toRight->size;
    toRight->size = toRight->left->size + toRight->right->size + 1;
    
}

// after all new nodes implemented correctly to the RBTree then
// by traversing all the nodes as left-right-root(postorder walk)
// because we take size of the left and right then add 1 to calculate the size of its parent
// all up to date size of the nodes are updated to be ready for the
// OS-SELECT operation
// as ,root->size = root->left->size + root->right->size + 1 means post order traversing
void RedBlackTree::fixedSizeOfTree( RbNode *NodeWalker ){
    
    if( NodeWalker->left != NIL ) fixedSizeOfTree(NodeWalker->left);
    if( NodeWalker->right != NIL ) fixedSizeOfTree(NodeWalker->right);
    NodeWalker->size = NodeWalker->left->size + NodeWalker->right->size + 1;
    
}

// Set the rank of both num_adult and num_non_adult of the nodes in the tree
// by traversing the inorder because of the structure of the RbTree
void RedBlackTree::fixedAdultAndNonAdultNums( RbNode *NodeWalker ){
    
    // left subtree traversing
    if( NodeWalker->left != NIL ) fixedAdultAndNonAdultNums(NodeWalker->left);
    // set the num_adult by increasing as 1 and set it as its rank for adult number
    // if age of node is more or equal than 18
    if(NodeWalker->age >= 18){
        nTh_adult_num++;
        NodeWalker->setAdultNum(nTh_adult_num);
    }
    // set the num_non_adult by increasing as 1 and set it as its rank for non_adult number
    // if age of node is less than 18
    else{
        nTh_non_adult_num++;
        NodeWalker->setNonAdultNum(nTh_non_adult_num);
    }
    // right subtree traversing
    if( NodeWalker->right != NIL ) fixedAdultAndNonAdultNums(NodeWalker->right);
    
}


void RedBlackTree::num_adult(RbNode * walker,int nTh_adult){
    /*
     if(nTh_adult == walker->num_adult)
     cout << endl << walker->num_adult <<"th adult: "<<  walker->personName ;
     else if(nTh_adult < walker->num_adult){
     num_adult(walker->left, nTh_adult);
     }
     else{
     num_adult(walker->right, nTh_adult - walker->num_adult);
     }
     */
    
    if(walker->left != NIL) num_adult(walker->left, nTh_adult);
    if(nTh_adult == walker->num_adult){
        cout << endl << walker->num_adult <<"th adult: "<<  walker->personName ;
    }
    if(walker->right != NIL) num_adult(walker->right, nTh_adult );
    
}



void RedBlackTree::num_non_adult(RbNode * walker,int nTh_non_adult){
    
    if(walker->left != NIL) num_non_adult(walker->left, nTh_non_adult);
    if(nTh_non_adult == walker->num_non_adult){
        cout << endl << walker->num_non_adult <<"rd non-adult: "<<  walker->personName << endl;
    }
    if(walker->right != NIL) num_non_adult(walker->right, nTh_non_adult );
    
}
/*
 void RedBlackTree::increase_one_age(RbNode *walker){
 if(walker->left != NIL) increase_one_age(walker->left);
 walker->setincreasedAge(1);
 if(walker->right != NIL) increase_one_age(walker->right);
 }
 */
void RedBlackTree::readImplementArrangeTree(ifstream &inFile){
    
    string inputName;
    char inputGender;
    int inputAge;
    
    while(!inFile.eof()){
        inFile >> inputName >> inputGender >> inputAge;
        RbNode *person = new RbNode(inputName,inputGender,inputAge);
        insertNewRbNode(person);
    }
    
}


void RedBlackTree::printRedBlackTree(RbNode *printer , string level){
    
    if(printer){
        if(printer != NIL){
            cout << level <<"(" << printer->color[0] << ") " << printer->personName << " " << printer->gender << " " << printer->age << endl;
            printRedBlackTree(printer->left,level + "-");
            printRedBlackTree(printer->right,level + "-");
        }
    }
    
}


int main(int argc, char **argv){
    
    ifstream inputFile;
    ofstream outputFile;
    char *inputFileName = argv[1]; // input file name
    
    RedBlackTree *Tree = new RedBlackTree;
    
    inputFile.open(inputFileName);
    if(inputFile.is_open()){ Tree->readImplementArrangeTree(inputFile); }
    
    
    Tree->printRedBlackTree(Tree->getTheRoot(),"");
    //Tree->increase_one_age(Tree->getTheRoot());
    Tree->fixedAdultAndNonAdultNums(Tree->getTheRoot());
    
    Tree->num_adult(Tree->getTheRoot(), 5);
    Tree->num_non_adult(Tree->getTheRoot(), 3);
    
    return 0;
    
    
}
