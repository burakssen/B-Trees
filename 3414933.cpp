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
    void setValues(int x, int y, char z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};


class BNode{ // BNode our BTree Node
    friend class BTree;
    Keys *keys;// array of keys
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
    void remove(Keys key, char keytype);
    void removeFromLeaf(int index);
    void removeFromNonLeaf(int index, char keytype);
    void getKeyFromPreviousChild(int index);
    void getKeyFromNextChild(int index);
    void merge(int index);
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

void BNode::remove(Keys key,char keytype) {

    int index = 0; // set index as 0

    if(keytype == 'x'){// check if our keytype is x
        while(index < this->numberofkeys && this->keys[index].x < key.x ){
            // while index is less then this node's number of keys
            // and this node's index th key less then our new key
            index++; //increase the index
        }
    }// do the same steps for each keytype
    else if(keytype == 'y'){
        while(index < this->numberofkeys && this->keys[index].y < key.y){
            index++;
        }
    }
    else if(keytype == 'z'){
        while(index < this->numberofkeys && this->keys[index].z < key.z ){
            index++;
        }
    }

    if(keytype == 'x'){// check if our keytype is x
        if(index < this->numberofkeys && this->keys[index].x == key.x){
            // if the index less than the node's number of keys and if our key in that index is equal to our key
            if(this->leaf){// if our node is a leaf node
                this->removeFromLeaf(index); // remove the indexth key from a leaf node
            }
            else{
                this->removeFromNonLeaf(index, keytype); // remove the indexth key from a non leaf node
            }
        }
        else{//else
            if(this->leaf){// if our node is a leaf node
                return;// return from the function
            }

            bool keyexist = false;// set a boolean for checking if key exists
            if(index == this->numberofkeys){ // if our index is equal to number of keys
                keyexist = true; // key exists true
            }

            if(this->children[index]->numberofkeys < this->degree){ // if node's indexth child's number of keys less then nodes degree
                // if index is not 0 and this node's index - 1 th child's number of key is greater than or equal to this degree
                if(index != 0 && this->children[index - 1]->numberofkeys >= this->degree){
                    // get key from the previous child
                    this->getKeyFromPreviousChild(index);
                }   // if index is not this number of keys and this node's index + 1 th child's number of key is greater than or equal to this degree
                else if(index != this->numberofkeys && this->children[index + 1]->numberofkeys >= this->degree){
                    // get key from the next child
                    this->getKeyFromNextChild(index);
                }
                else{
                    // if our index is not equal to this node's number of keys
                    if(index != this->numberofkeys){
                        // merge our node's index th child with index + 1 th child
                        this->merge(index);
                    }
                    else{
                        // merge our node's index - 1 th child with index th child
                        this->merge(index - 1);
                    }
                }
            }

            if(keyexist && index > this->numberofkeys){ // if keyexists and index is greater than the number of keys of this node
                this->children[index - 1]->remove(key, keytype); // remove the key from the this node's index - 1 th child
            }
            else{
                this->children[index]->remove(key, keytype); // remove the key from the this node's index th child
            }
        }
    }//do the same steps to the other keytypes
    else if(keytype == 'y'){
        if(index < this->numberofkeys && this->keys[index].y == key.y){
            if(this->leaf){
                this->removeFromLeaf(index);
            }
            else{
                this->removeFromNonLeaf(index, keytype);
            }
        }
        else{
            if(this->leaf){
                return;
            }

            bool keyexist = false;
            if(index == this->numberofkeys){
                keyexist = true;
            }

            if(this->children[index]->numberofkeys < this->degree){
                // if index is not 0 and this node's index - 1 th child's number of key is greater than or equal to this degree
                if(index != 0 && this->children[index - 1]->numberofkeys >= this->degree){
                    // get key from the previous child
                    this->getKeyFromPreviousChild(index);
                }   // if index is not this number of keys and this node's index + 1 th child's number of key is greater than or equal to this degree
                else if(index != this->numberofkeys && this->children[index + 1]->numberofkeys >= this->degree){
                    // get key from the next child
                    this->getKeyFromNextChild(index);
                }
                else{
                    // if our index is not equal to this node's number of keys
                    if(index != this->numberofkeys){
                        // merge our node's index th child with index + 1 th child
                        this->merge(index);
                    }
                    else{
                        // merge our node's index - 1 th child with index th child
                        this->merge(index - 1);
                    }
                }
            }

            if(keyexist && index > this->numberofkeys){
                this->children[index - 1]->remove(key, keytype);
            }
            else{
                this->children[index]->remove(key, keytype);
            }
        }
    }
    else if(keytype == 'z'){
        if(index < this->numberofkeys && this->keys[index].z == key.z){
            if(this->leaf){
                this->removeFromLeaf(index);
            }
            else{
                this->removeFromNonLeaf(index, keytype);
            }
        }
        else{
            if(this->leaf){
                return;
            }

            bool keyexist = false;
            if(index == this->numberofkeys){
                keyexist = true;
            }

            if(this->children[index]->numberofkeys < this->degree){
                // if index is not 0 and this node's index - 1 th child's number of key is greater than or equal to this degree
                if(index != 0 && this->children[index - 1]->numberofkeys >= this->degree){
                    // get key from the previous child
                    this->getKeyFromPreviousChild(index);
                }   // if index is not this number of keys and this node's index + 1 th child's number of key is greater than or equal to this degree
                else if(index != this->numberofkeys && this->children[index + 1]->numberofkeys >= this->degree){
                    // get key from the next child
                    this->getKeyFromNextChild(index);
                }
                else{
                    // if our index is not equal to this node's number of keys
                    if(index != this->numberofkeys){
                        // merge our node's index th child with index + 1 th child
                        this->merge(index);
                    }
                    else{
                        // merge our node's index - 1 th child with index th child
                        this->merge(index - 1);
                    }
                }
            }

            if(keyexist && index > this->numberofkeys){
                this->children[index - 1]->remove(key, keytype);
            }
            else{
                this->children[index]->remove(key, keytype);
            }
        }
    }
}

// this function removes indexth key from the node
void BNode::removeFromLeaf(int index) {
    // we override the key that we want to delete
    // this function basically do this
    // [0]->[8]->[9]->[10]->[4]
    // if we want to delete 9 from this list
    // [0]->[8]->[10]->[4]->[4]
    // we basically left shit the array from the keys index that we want to delete
    for(int i = index + 1; i < this->numberofkeys; ++i){
        this->keys[i - 1] = this->keys[i];
    }
    // [0]->[8]->[10]->[4] and here we decrease the number of key of this node
    this->numberofkeys--;
}

// this function removes index th key from this non leaf node
void BNode::removeFromNonLeaf(int index, char keytype) {
    Keys key = this->keys[index];
    // get the index th key of this node

    //if this node's index th children's number of keys is greater and equal to this node's degree
    if(this->children[index]->numberofkeys >= this->degree){

        // if the child that precedes our key has a degree of t
        // we find the predecessor of our key in the subtree of this node's indexth child.
        // get the index th child
        BNode *current = this->children[index];

        while(!current->leaf){ // while this child is not a leaf node
            current = current->children[current->numberofkeys]; // go to the rightmost node (which is a leaf node)
        }

        this->keys[index] = current->keys[current->numberofkeys - 1]; // we replace the key with predecessor
        // and delete the predecessor in this node's indexth child in recursive aproach
        this->children[index]->remove(current->keys[current->numberofkeys - 1], keytype);
    } //if this node's index + 1 th children's number of keys is greater and equal to this node's degree
    else if(this->children[index + 1]->numberofkeys >= this->degree){
        // if the child that success our key has a degree of t
        // we find the successor of our key in the subtree of this node's indexth child.

        BNode *current = this->children[index + 1];
        while(!current->leaf){// while this child is not a leaf node
            current = current->children[0];// go to the leftmost node (which is a leaf node)
        }

        this->keys[index] = current->keys[0];;// we replace the key with successor
        // and delete the successor in this node's indexth child in recursive aproach
        this->children[index + 1]->remove(current->keys[0], keytype);
    }
    else{
        // if both this node's index th and index + 1 th children has less keys than this degree
        // merge our key and this node's index th and index + 1 th children
        this->merge(index);
        // after merge operation our index th child has 2*this->degree - 1
        this->children[index]->remove(key, keytype);
    }
}

// this function removes one key from node's index - 1 th child and inserts that key to the index th child
void BNode::getKeyFromPreviousChild(int index) {

    BNode *child = this->children[index]; // get the index th child
    BNode *sibling = this->children[index - 1]; // get the index - 1 th child

    //right shift all keys of the index th child
    for(int i = child->numberofkeys - 1; i >= 0; --i){
        child->keys[i + 1] = child->keys[i];
    }

    // if our child is not a leaf node
    if(!child->leaf){
        // right shift all children pointers of the index th child
        for(int i = child->numberofkeys; i >= 0; --i){
            child->children[i + 1] = child->children[i];
        }
    }

    //set the index th child's 0th key as this node's index - 1 th key
    child->keys[0] = this->keys[index - 1];

    // if our child is not leaf node
    if(!child->leaf){
        // set the index th child's 0th children pointer to our index - 1 th child's right most children pointer
        child->children[0] = sibling->children[sibling->numberofkeys];
    }

    // and move the index - 1 th child's rightmost key to the
    // this->keys index - 1 th place
    this->keys[index - 1] = sibling->keys[sibling->numberofkeys - 1];

    // and arrange number of keys
    child->numberofkeys++;
    sibling->numberofkeys--;
}

void BNode::getKeyFromNextChild(int index) {
    BNode *child = this->children[index];// get the index th child
    BNode *sibling = this->children[index + 1];// get the index + 1 th child

    // set the index th child's rightmost key as this node's index th key
    child->keys[child->numberofkeys] = this->keys[index];

    // if index th child is not leaf
    if(!child->leaf){
        //set index th child's rightmost child pointer as the index + 1 th childs first child pointer
        child->children[child->numberofkeys + 1] = sibling->children[0];
    }

    // set the index th key of this node as index + 1 th childs first key
    this->keys[index] = sibling->keys[0];

    // left shift all keys of the index + 1 th child
    for(int i = 1; i < sibling->numberofkeys; ++i){
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if(!sibling->leaf){ // if index + 1 th child is not a leaf node
        // left shift all children pointers of the index + 1 th child
        for(int i = 1; i <= sibling->numberofkeys; ++i){
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    // arrange number of keys
    child->numberofkeys++;
    sibling->numberofkeys--;
}

// this function merges the index th child and index + 1 th child
void BNode::merge(int index) {

    BNode *child = this->children[index]; // get index th child
    BNode *sibling = this->children[index + 1]; // get index + 1 th child

    // set index th child's t-1 th key as this node's index th key
    child->keys[this->degree - 1] = this->keys[index];

    // get keys from index + 1 th child to index th child
    for(int i = 0; i < sibling->numberofkeys; ++i){
        child->keys[i + this->degree] = sibling->keys[i];
    }

    // if index th child is not a leaf node
    if(!child->leaf){
        // get children pointers from index + 1 th child to index th child
        for(int i = 0; i <= sibling->numberofkeys; ++i){
            child->children[i + this->degree] = sibling->children[i];
        }
    }

    // beacuse we moved he index th key of this node to index th child' t-1 th key position
    // we can left shift the keys of this node
    for(int i = index + 1; i < this->numberofkeys; ++i){
        this->keys[i - 1] = this->keys[i];
    }

    // because we also moved the child pointers of this node
    // we can again left shift them
    for(int i = index + 2; i <= this->numberofkeys; ++i){
        this->children[i - 1] = this->children[i];
    }

    // update the key counts
    child->numberofkeys += sibling->numberofkeys + 1;
    this->numberofkeys--;

    //and delete the sibling
    delete sibling;
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
    void remove(Keys key, char keytype);
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

void BTree::remove(Keys key, char keytype) {
    //in this function we remove the key

    if(!this->root){ // if root is not exists
        return; // return from the function
    }

    this->root->remove(key, keytype); // call the remove function of the root node

    if(this->root->numberofkeys == 0){ // if this root do not have any keys
        BNode *temp = this->root; // get the root

        if(this->root->leaf){ // if this root is a leaf node
            this->root = nullptr; // set root as a leaf node
        }
        else{ // else
            this->root = this->root->children[0]; // set root's first child as root
        }

        delete temp; // and delete the temp
    }
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

    string deletevar;

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

    //get the delete value
    getline(std::cin, deletevar, '\n');

    if(third == 'x'){ // if our keytype is x
        // set the key values
        key->setValues(std::stoi(deletevar),0,0);
        // and delete the key from the tree
        mytree->remove(*key, third);
    }//do this for all the keytypes
    else if (third == 'y'){
        key->setValues(0,std::stoi(deletevar),0);
        mytree->remove(*key, third);
    }
    else{
        key->setValues(0, 0, *deletevar.c_str());
        mytree->remove(*key, third);
    }

    // print all
    mytree->traverse();

    return 0;
}