#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
/*
    //My BST Tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('a',2));
    bt.insert(std::make_pair('b',4));
    bt.insert(std::make_pair('c',4));
    bt.insert(std::make_pair('d',9));
    bt.insert(std::make_pair('e',4));

    cout << "----- TESTING ITERATOR ------" << endl;
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
*/

/*
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('b',2));
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('c',3));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }

    bt.clear();


    cout << "Erasing b" << endl;
    bt.remove('b');
    cout << "Erasing a" << endl;
    bt.remove('a');
    cout << "Erasing c" << endl;
    bt.remove('c');
    cout << "DONE" << endl;
*/

    /*
    // My AVL Tests
    AVLTree<char, int> at;
    at.insert(std::make_pair('e', 12));
    at.insert(std::make_pair('g', 10));
    at.print();
    at.insert(std::make_pair('f', 15));
    at.print();
    at.insert(std::make_pair('b', 5));
    at.print();
    at.insert(std::make_pair('a', 3));
    at.print();
    at.insert(std::make_pair('d', 10));
    at.print();
    at.insert(std::make_pair('b', 8));
    at.print();
 */

    BinarySearchTree<int, double> bst;
	bst.insert(std::make_pair(1, 1.0));
    cout << "here" << endl;
	bst.remove(1);
    cout << "end" << endl;
	

/*
    // AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    at.insert(std::make_pair('b',2));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');
*/

    return 0;
}
