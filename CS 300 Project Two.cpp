#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

struct Course {
    string courseId; 
    string title;
    vector<string> preReqCourses;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }
    Node(Course currentCourse) :
        Node() {
        course = currentCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseId);
};

BinarySearchTree::~BinarySearchTree() {

}

BinarySearchTree::BinarySearchTree() {

    root = nullptr;
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::Insert(Course currentCourse) {
    if (root == nullptr) {
        root = new Node(currentCourse);
    }
    else {
        addNode(root, currentCourse);
    }
}

Course BinarySearchTree::Search(string courseId) {
    Node* currentNode = root;
    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        }
        if (currentNode->course.courseId > courseId) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }

    Course course;
    return course;
}

void BinarySearchTree::addNode(Node* node, Course course) {
    if (node->course.courseId > course.courseId) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseId << ", " << node->course.title << endl;
        inOrder(node->right);
    }
}

void menuDisplay() {
    cout << endl;
    cout << "   **Course Menu**" << endl;
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl;
    cout << "Make a selection:   ";
}

void loadCourses(string courseFile, BinarySearchTree* courseTree) {
    ifstream file(courseFile);
    if (file.is_open()) {
        string line;
        cout << endl;
        cout << endl;
        cout << "File opened successfully." << endl;
        while (getline(file, line)) {
            stringstream s_stream(line);
            vector<string> splitLine;
            while (s_stream.good()) {
                string substring;
                getline(s_stream, substring, ',');
                splitLine.push_back(substring);
            }
            if (splitLine.size() < 2) {
                cout << "Course is missing parameters, cancelling file read";
                return;
            }
            Course newCourse;
            newCourse.courseId = splitLine[0];
            newCourse.title = splitLine[1];
            if (splitLine.size() > 2) {
                for (int i = 2; i < splitLine.size(); i++) {
                    if (splitLine[i] != "") {
                        newCourse.preReqCourses.push_back(splitLine[i]);
                    }
                }
            }
            courseTree->Insert(newCourse);
        }
    }
    else {
        cout << "File open error." << endl;
    }
}

int main(int argc, char* argv[]) {
    BinarySearchTree* courseTree;
    courseTree = new BinarySearchTree();
    Course course;
    string courseFile, key;
    int userSelec = 0;
    
    cout << "Welcome to the course planner." << endl;
    
    while (userSelec != 9) {
        menuDisplay();

        cin >> userSelec;

        switch (userSelec) {
        case 1:
            cout << "Enter name of file to be opened:   ";
            cin >> courseFile;
            loadCourses(courseFile, courseTree);
            break;
        case 2:
            cout << "Here is a sample schedule:" << endl << endl;
            courseTree->InOrder();
            break;
        case 3:
            cout << "What course do you want to know about? ";
            cin >> key;
            course = courseTree->Search(key);

            if (!course.courseId.empty()) {
                cout << course.courseId << ", " << course.title << endl;
                cout << "Prerequisites: ";
                for (auto const& course : course.preReqCourses) {
                    std::cout << ",  " << course;
                }
                cout << endl;
            }
            else {
                cout << key << " not in file." << endl;
            }
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << userSelec << " is not a valid option.";
        }
    }
    return 0;
}