#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000;

struct Course {// Houses course information.

    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {

private:
    // Define structures for courses.
    struct Node {
        Course course;
        Node* right;
        Node* left;

        // Constructor.
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // Course initialization.
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    void inOrder(Node* node);
    int size = 0;

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
};

// Default constructor.
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Insert course.
void BinarySearchTree::Insert(Course aCourse) {

    Node* currentNode = root;
    if (root == NULL) {
        root = new Node(aCourse);
    }

    else {

        while (currentNode != NULL) {

            if (aCourse.courseId < currentNode->course.courseId) {

                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(aCourse);
                    currentNode = NULL;
                }

                else {
                    currentNode = currentNode->left;
                }
            }

            else {

                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(aCourse);
                    currentNode = NULL;
                }

                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}

// Course removal.
void BinarySearchTree::Remove(string courseId) {
    Node* par = NULL;
    Node* curr = root;

    while (curr != NULL) {

        if (curr->course.courseId == courseId) {

            if (curr->left == NULL && curr->right == NULL) {

                if (par == NULL) {
                    root = nullptr;
                }

                else if (par->left == curr) {
                    par->left = NULL;
                }

                else {
                    par->right = NULL;
                }
            }
            else if (curr->right == NULL) {

                if (par == NULL) {
                    root = curr->left;
                }

                else if (par->left == curr) {
                    par->left = curr->left;
                }

                else {
                    par->right = curr->left;
                }
            }
            else if (curr->left == NULL) {
                if (par == NULL) {
                    root = curr->right;
                }
                else if (par->left == curr) {
                    par->left = curr->right;
                }
                else {
                    par->right = curr->right;
                }
            }
            else {
                Node* suc = curr->right;

                while (suc->left != NULL) {
                    suc = suc->left;

                }

                Node successorData = Node(suc->course);
                Remove(suc->course.courseId);
                curr->course = successorData.course;
            }
            return;
        }
        else if (curr->course.courseId < courseId) {
            par = curr;
            curr = curr->right;
        }
        else {
            par = curr;
            curr = curr->left;
        }
    }
    cout << "Value NOT Found." << endl;
    return;
}

// Course search.
Course BinarySearchTree::Search(string courseId) {

    Course aCourse;
    Node* currentNode = root;

    while (currentNode != NULL) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
// Value is NOT found.
    return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {
    if (node == NULL) {
        return;
    }

    inOrder(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);
}

int BinarySearchTree::Size() {
    return size;
}

vector<string> Split(string lineFeed) {
    char delim = ',';

    lineFeed += delim;// Delimiter.
    vector<string> lineTokens;
    string temp = "";

    for (int i = 0; i < lineFeed.length(); i++) {
        if (lineFeed[i] == delim)
        {
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

// Courses load.
void loadCourses(string csvPath, BinarySearchTree* courseList) {

    ifstream inFS;
    string line;
    vector<string> stringTokens; 

    inFS.open(csvPath); // Read file.

    if (!inFS.is_open()) {// Error handler.
        cout << "Error, File could NOT be opened.\n" << endl;
        return;
    }

    while (!inFS.eof()) {

        Course aCourse;// Create new struct per line.

        getline(inFS, line);
        stringTokens = Split(line);

        if (stringTokens.size() < 2) { 

            cout << "Error. Line Skip.\n" << endl;
        }

        else {

            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {

                aCourse.preList.push_back(stringTokens.at(i));
            }
            courseList->Insert(aCourse);
        }
    }
    inFS.close(); // File closes.
}

/*
Prints: courseId and courseName
Prereq list: if there is no prereq, send relevant msg to output.
*/
void displayCourse(Course aCourse) {

    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (aCourse.preList.empty()) {// If there are no prereqs, print none.
        cout << "NONE" << endl;
    }

    else {

        for (unsigned int i = 0; i < aCourse.preList.size(); i++) {
             cout << aCourse.preList.at(i);
            
            if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {
                cout << ", ";
            }            
        }
    }

    cout << endl;
}

void convertCase(string &toConvert) {

    for (unsigned int i = 0; i < toConvert.length(); i++) {

        if (isalpha(toConvert[i])) {

            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

int main(int argc, char* argv[]) {

// cmd args.
    string csvPath, aCourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        aCourseKey = argv[2];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    }

    // Define table that holds the courses.
    BinarySearchTree* courseList = new BinarySearchTree();

    Course course;  
    bool goodInput;
    int choice = 0;

    while (choice != 9) {// Display menu and receive user input.

        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        // Clear.
        aCourseKey = "";    
        string anyKey = " ";
        choice = 0;

        try {
            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) {
                goodInput = true;
            }
            else {// Throw Error.
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:
                // Complete the call method to load courses.
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " Courses Read\n" << endl;
                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 2:
                courseList->InOrder();
                cout << "Enter \'y\' to continue.\n" << endl;
                cin >> anyKey;

                break;

            case 3:

                cout << "Which course do you want details on?\n " << endl;
                cin >> aCourseKey;

                convertCase(aCourseKey); // Convert the case of the user input.

                course = courseList->Search(aCourseKey);

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "Course ID: " << aCourseKey << " not found.\n" << endl;
                }

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 4:

                cout << " Which course will be deleted?\n " << endl;
                cin >> aCourseKey;

                convertCase(aCourseKey); // Convert the case of user input.
                courseList->Remove(aCourseKey);
                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:
                exit;
                break;

            default:

                throw 2;
            }
        }

        catch (int err) {
            std::cout << " Validate Input.\n" << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        // Clear extra input and errors generated by bad input.
        cin.clear();
        cin.ignore();        

        // Clear console.
        system("cls");
    }

    cout << "Goodbye." << endl; // Exit message.
    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}