/*
 * NAME :   BURAK SEN
 * ID   :   150170063
 * DATE :   09.02.2021
 */

#include <iostream>

#include <string>
using namespace std;


struct Keys{ //keys struct in this struct we store our values
    int x;
    int y;
    char z;
    void setValues(int x, int y, char z)
    {
	this->x = x;
	this->y = y;
	this->z = z;
    }
};

class BNode{ 	// BNode our BTree Node
    friend class BTree;
    Keys *keys; // array of keys
    int degree; // the minimum degree of the node
    BNode **children; // array of children pointers with 
    //this we can add child node at most numberof keys + 1 
    int numberofkeys;// number of keys
    bool leaf; // if these node is a leaf node or not
public:
    BNode(int degree, bool leaf); 
    void insertNonFull(Keys key, char keytype);
    void splitChild(int i, BNode*y);
    void traverse();
};

BNode::BNode(int degree, bool leaf) { // BNode constructer
    this->degree = degree; // set degree (t) of the node
    this->leaf = leaf; // set node's type if it is leaf node or not

    this->keys = new Keys[2 * this->degree - 1]; // allocate key memory 2*t-1
    this->children = new BNode*[2 * this->degree]; // allocate children pointers memory 2*t

    this->numberofkeys = 0; // set numberof keys as 0
}

void BNode::insertNonFull(Keys key, char keytype) { 
// insertNonFull function inserts if numberof keys less than or equal to 2 * degree -1
    int index = this->numberofkeys - 1; // set index to 0 - 9 system instead of 1 - 10
    if(this->leaf){ // if our node is a leaf node
        if(keytype == 'x'){ // check if our keytype is x
            while(index >= 0 && this->keys[index].x > key.x){ 
            // while index is not less than 0 we and our new key.x value is less then our node's indexth key.x  
                this->keys[index + 1] = this->keys[index]; //right shift our indexth key.x value
                index--; // and decrease the index
            }
        }// do the same things for other keytypes
        else if(keytype == 'y'){
            while(index >= 0 && this->keys[index].y > key.y){
                this->keys[index + 1] = this->keys[index];
                index--;
            }
        }
        else if(keytype == 'z'){
            while(index >= 0 && (int)this->keys[index].z > (int)key.z){
                this->keys[index + 1] = this->keys[index];
                index--;
            }
        }
	
	// when we right shift all of the values like this
	// [0]->[1]->[8]->[9]->[] --> [0]->[0]->[1]->[9]->[8]
	// our final key list would be like this
	// if our list shifted like this our index would be -1
	// [0]->[1]->[8]->[9]->[] --> [0]->[1]->[8]->[8]->[9]
	// if it would be like this our index would be 1
	// as we can see our indexes always less than 1 from the place that we insert the new keys index
	// if we want to insert the new key to this place we increase our index by 1 
	// then set our new key to the our list of keys index + 1th position
	
        this->keys[index + 1] = key;
        this->numberofkeys++; // and because we insert a new key we increase the number of keys by one
    }
    else{
    	// if our node is not a leaf node
    	// check if our keytype is x
        if(keytype == 'x'){
            while(index >= 0 && this->keys[index].x > key.x){
            // while index is not less than 0 we and our new key.x value is less then our node's indexth key.x  
                index--; // decrease the index
            }
        }// do the same things for other keytypes
        else if(keytype == 'y'){
            while(index >= 0 && this->keys[index].y > key.y){
                index--;
            }
        }
        else if(keytype == 'z'){
            while(index >= 0 && this->keys[index].z > key.z){
                index--;
            }
        }
	
	// beacuse we are in a non leaf node we will look for our child node
	// an for that we decreased ourindex number for finding the place
	// |[0]->|->[1]->|->[8]->|->[9]|
	// |	 |       |       |     |
	// these each pipe are child pointers
	// when we want to place for example 7 to the keys
	// we decrease the index until 1 then we are at the position |[0]->|->[1]
	// to go our destionation we add 1 to our index and control that child's number of keys
        if(this->children[index + 1]->numberofkeys == 2 * this->degree - 1){
        // if our child in index + 1 's numberofkeys equals to 2*t - 1 (maximum key num in a key array)
            this->splitChild(index + 1, this->children[index + 1]); 
            // we split our child because this child is full
            
            // check if our keytype is x
            if(keytype == 'x'){
                if(this->keys[index + 1].x < key.x){ // if our index + 1 th key is less hten our new key
                    index++; // increase our index
                }
            }// do this to other keytypes
            else if(keytype == 'y'){
                if(this->keys[index + 1].y < key.y){
                    index++;
                }
            }
            else if(keytype == 'z'){
                if(this->keys[index + 1].z < key.z){
                    index++;
                }
            }
        }
        // if our index increased at the above we go the the next child
        // if our index not increased at the aboce we go the prev child
        this->children[index + 1]->insertNonFull(key, keytype); // insert our key to the index + 1th child node.
    }
}

void BNode::splitChild(int i, BNode *y) { // this function splits our child node to two nodes.
    BNode *newchild = new BNode(y->degree, y->leaf); // create a new child node with our nodes parameters
    newchild->numberofkeys = this->degree - 1; // set its degree as t - 1

    for(int j = 0; j < this->degree - 1; j++){ // we copy the last t - 1 key of our node to new child
        newchild->keys[j] = y->keys[j + this->degree];
    }

    if(!y->leaf){// if our node is not a leaf node
        for(int j = 0; j < this->degree; j++){ // we copy the las t children of our node to new node's children
            newchild->children[j] = y->children[j + this->degree];
        }
    }

    y->numberofkeys = this->degree - 1; // decrease the number of keys in y to t - 1

    for(int j = this->numberofkeys; j >= i + 1; j--){ 
    //because we are going to have a new child in this node in the index i + 1th position
    // we right shift our children pointer in the array until we reach the position 
        this->children[j + 1] = this->children[j];
    }
    //and we set our new child to the index + 1th position
    this->children[i + 1] = newchild;

    //when we created the new child we have to arrange the keys
    //from y we move the middle key to this node
    // for that we right shift until the middle of the keys
    for(int j = this->numberofkeys - 1; j >= i; j--){
        this->keys[j + 1] = this->keys[j];
    }

	//we set y's middle key to this node
    this->keys[i] = y->keys[this->degree - 1];

	//and because we increase the number of keys in this node we increase the numeber of keys parameter
    this->numberofkeys++;
}


void BNode::traverse() {// In this function we print our nodes
    int i;

    for(i = 0; i < this->numberofkeys; i++) {//print each key
        cout << "(" << this->keys[i].x << "," << this->keys[i].y << "," << this->keys[i].z << ")";
    }

	// go each children of this node
    for(i = 0; i < this->numberofkeys + 1; i++){
        if(!this->leaf) { // if childrens not leaf nodes
            std::cout << std::endl; // because we will go down by one we print newline
            this->children[i]->traverse(); // go to the children
        }
    }
}

class BTree{ //BTree
    BNode *root;// root node
    int degree; // tree degree
    char keytype; // store keytype
public:
    BTree(int degree, char keytype){ // BTree contstructor
        this->root = nullptr; // set root null
        this->degree = degree; // set degree
        this->keytype = keytype; // set keytype
    }
    void insert(Keys key);
    void traverse();
};

void BTree::insert(Keys key) { //Insert key function
    if(this->root == nullptr){ // if our root is null our tree is empty 
        this->root = new BNode(this->degree, true); // allocate a node in the root
        this->root->keys[0] = key; // set our first key in the root
        this->root->numberofkeys = 1; // set our number of keys in the root
    }
    else{
        if(this->root->numberofkeys == 2 * this->degree -1){// if our root's number of keys equal to max
            BNode *newroot = new BNode(this->degree, false); // we create newroot with non leaf node

            newroot->children[0] = root; // we set our root to newroot's child
            
            newroot->splitChild(0, root); // we split our newroots children

            int i = 0; // index to 0
            if(this->keytype == 'x'){//check if our keytype is x
                if(newroot->keys[0].x < key.x){ // check our newroot's first key is less then new key
                    i++; // if our new key is greater increase our index
                }
            }// do this for all keytypes
            else if(this->keytype == 'y'){
                if(newroot->keys[0].y < key.y){
                    i++;
                }
            }
            else if(this->keytype == 'z'){
                if(newroot->keys[0].z < key.z){
                    i++;
                }
            }
	
	// because we want to insert a newkey to the list we found our child index
	// and with this index we can insert our new key to the tree with insertnonfull function
	// we used insertnonfull function because we already split our child
            newroot->children[i]->insertNonFull(key,this->keytype);
	// and we set our root as newroot
            this->root = newroot;
        }
        else{
        // if our root's number of keys is not equal to max we can insert new key to the root
            this->root->insertNonFull(key,this->keytype);
        }
    }
}

void BTree::traverse() { // in the BTRee traverse funciton we just check if our tree is empty or not
//and if it is not empty we call the traverse function from the root node.
    if(this->root != nullptr)
        this->root->traverse();
}

int main() {
    string frst;
    string scnd;
    string thrd;
    // get first three lines
    std::getline(std::cin, frst,'\n');
    std::getline(std::cin, scnd,'\n');
    std::getline(std::cin, thrd,'\n');
	
    //set first three lines in their respective formats
    int first = std::stoi(frst);
    int second = std::stoi(scnd);
    char third = *thrd.c_str();

    //create our tree with our paramters
    BTree *mytree;
    mytree = new BTree(second,third);
    
    string sx;
    string sy;
    string sz;
    
    int x;
    int y;
    char z;
    
    Keys *key;

    // for loop or all values
    for(int i = 0; i < first; i++){
    	//get values
        getline(std::cin, sx, ' ');
        getline(std::cin, sy, ' ');
        getline(std::cin, sz, '\n');
        //convert values
        x = std::stoi(sx);
        y = std::stoi(sy);
        z = *sz.c_str();
        //create new key
        key = new Keys;
        key->setValues(x,y,z);
        // insert new key
        mytree->insert(*key);
    }

    // print all
    mytree->traverse();

    return 0;
}
