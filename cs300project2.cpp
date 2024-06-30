//============================================================================
// Name        : cs300project2.cpp
// Author      : William Cook
// Version     : 1.0
// Description : Course Planner
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;


//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


//structure for course object
struct Course{
    string courseNum;
    string courseName;
    vector<string> preReqs;
};

//structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    //default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }


};


//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void printInOrder();
    void Insert(Course course);
    Course Search(string courseNum);

};

//Default Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

//Destructor
BinarySearchTree::~BinarySearchTree() {
    delete root;
}

//print tree in order
//starts the recursion of the inOrder function
void BinarySearchTree::printInOrder() {
    this->inOrder(root);
}

//in order function to print the BST in alphanumeric from least to greatest
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNum << ": "
            << node->course.courseName << endl;;


        inOrder(node->right);
    }
}

//inserts a course to BST
//if root occupied, call addNode
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}


//adds node to tree
//compares greater or lesser
    //whichever comnparison if unoccupied add node -- if occupied call recursively from that node
void BinarySearchTree::addNode(Node* node, Course course) {

    if (node->course.courseNum.compare(course.courseNum) > 0) {
        //if greater check left pointer
            //if null insert -- otherwise recursively call from that node
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        //since the node is not greater
            //if null insert at right pointer -- otherwise recursively call from that node
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
}



//method to print a particular course
//used in search function
void printCourse(Course course) {

    //check if course was found in search function or dummy object sent (not found)
    if (!course.courseNum.empty()){
    

        cout << course.courseNum << ": " << course.courseName << endl;

        //check for prereqs, if they exist, print them out
        if (course.preReqs.size() > 0) {
            cout << "Prerequisites for this course are: ";

            for (int i = 0; i < course.preReqs.size(); i++) {
                cout << course.preReqs.at(i) << " ";
            }

            cout << endl << endl;
        }
    }
    else {
        cout << "The course was not found. Please try again." << endl << endl;
    }

    return;
}


//search the tree for a course by the courseNum
//@returns the course object if matches to search for
Course BinarySearchTree::Search(string num) {
    Node* current = root;

    while (current != nullptr) {
        //if current node matches, return it
        if (current->course.courseNum.compare(num) == 0) {
            return current->course;
        }

        if (num.compare(current->course.courseNum) < 0) {
            //if the number is smaller than current, traverse left
            current = current->left;
        }
        else {
            //else traverse right
            current = current->right;
        }
    }

    Course aCourse;
    return aCourse;
}


//parses a string separated by commas 
//@returns a vector containing the tokens
vector<string> Parser(string line) {

    char delim = ',';

    //ensures last token read if no comma
    line += delim;

    vector<string> tokens;
    string temp = "";

    //increment by character through the line sent
    for (int i = 0; i < line.length(); i++) {

        //if we hit a delimiter store the temp string in the vector and reset temp
            //and statement prevents hanging commas
        if (line[i] == delim && temp != ",") {
            tokens.push_back(temp); 
            temp = "";
            i++;
        }

        //add each character until we hit the delimiter
        temp += line[i];
    }

    return tokens;

}


//method to load courses into the BST
void loadCourses(string fileName, BinarySearchTree* bst) {
    cout << "Loading course information file: " << fileName << endl << endl;

    //begin loading courses
    ifstream fin;
    fin.open(fileName);

    //error check if file didn't open
    if (!fin.is_open()) {
        cout << "Couldn't open file." << endl << endl;
        return;
    }

    cout << "File opened" << endl;

    string line;
    vector<string> tokens;
    int lineCounter = 0;


    while (!fin.eof()) {

        Course tempCourse;

        getline(fin, line);

        
        if (line != "") {
            tokens = Parser(line); //call function to parse the line

            //error check for less than 2 values
            if (tokens.size() < 2) {
                cout << "Incorrect formatting at line: " << lineCounter << endl << endl;
                return;
            }
            else {
                tempCourse.courseNum = tokens.at(0);
                tempCourse.courseName = tokens.at(1);

                //push back pre reqs into vector (if any)
                for (int i = 2; i < tokens.size(); i++) {
                    tempCourse.preReqs.push_back(tokens.at(i));
                }

                //insert the completed object into the tree
                bst->Insert(tempCourse);

            }

            lineCounter++;

        }
    }


    fin.close();
    cout << "All done!" << endl;
    cout << "Lines read: " << lineCounter << endl << endl;

}

//method to convert a string to uppercase
//used to ensure search method matches input
void makeCapital(string& baseString) {
    for (int i = 0; i < baseString.size(); i++) {
        if (isalpha(baseString.at(i))) {
            baseString.at(i) = toupper(baseString.at(i));
        }
    }
}

//method to print menu
void printMenu() {
        cout << "Menu: " << endl;
        cout << "  1. Load courses from file" << endl;
        cout << "  2. Display all courses" << endl;
        cout << "  3. Search for a particular course" << endl;
        cout << "  4. Exit" << endl << endl;
        cout << "What would you like to do? ";
}

//main
int main()
{

    //Define a binary search tree to hold the courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    //file that holds course data
    string fileName = "CS 300 ABCU_Advising_Program_Input.csv";

    cout << "Welcome to the course planner!" << endl << endl;

    int choice = 0;
    while (choice != 4) {
        printMenu();
        cin >> choice;
        cout << endl;

        //used for case 3 course search
        string courseChoice;

        switch (choice) {

        case 1:
            loadCourses(fileName, bst);
            break;

        case 2:
            bst->printInOrder();
            cout << endl;
            break;

        case 3:
            cout << "Enter the course number you'd like to know more about: ";
            
            cin >> courseChoice;
            cout << endl << endl;
            //convert the input so not case sensitive
            makeCapital(courseChoice);


            //call search method to find the course
            course = bst->Search(courseChoice);
            //then print it out
            printCourse(course);

            break;

        default:
            if (choice != 4) {
            cout << "Invalid input, please try again." << endl << endl;
            }
            
            break;
      
        

        }//end switch
        
    }//end loop

        cout << "Thanks for using course planner!" << endl;
        return 0;

}

