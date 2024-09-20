#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    string title;
    string author;
    string ISBN;
    int copyCount;

public:
    Book(string t, string a, string isbn, int copyCount = 1) 
        : title(t), author(a), ISBN(isbn), copyCount(copyCount) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return ISBN; }
    int getAvailability() const { return copyCount; }

    bool borrowBook() {
        if (copyCount > 0) {
            copyCount--;
            return true;
        }
        return false;
    }

    void returnBook() { copyCount++; }

    void displayBookDetails() const {
        cout << "----------------------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Title: " << title << "  ||   " << "Author: " << author << "  ||   " << "ISBN: " << ISBN << "  ||   " << "Available: " << (copyCount > 0 ? "Yes" : "No") << endl;
        cout << "----------------------------------------------------------------------------------------------------------------------------------------" << endl;
    }
};

struct BSTNode {
    Book* book;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Book* b) : book(b), left(nullptr), right(nullptr) {}
};

class Member {
private:
    string name;
    string memberID;

public:
    Member(string n, string id) : name(n), memberID(id) {}

    string getName() const { return name; }
    string getMemberID() const { return memberID; }

    void displayMemberDetails() const {
        cout << "------------------------------------------------------------------" << endl;
        cout << "Member Name: " << name << "  ||   " << "Member ID: " << memberID << endl;
        cout << "------------------------------------------------------------------" << endl;
    }
};

struct MemberNode {
    Member* member;
    MemberNode* next;

    MemberNode(Member* m) : member(m), next(nullptr) {}
};

class Library {
private:
    BSTNode* root;
    int bookCount;
    MemberNode* memberRoot;

    BSTNode* insertBook(BSTNode* node, Book* book) {
        if (node == nullptr) {
            return new BSTNode(book);
        }

        if (book->getISBN() < node->book->getISBN()) {
            node->left = insertBook(node->left, book);
        } else if (book->getISBN() > node->book->getISBN()) {
            node->right = insertBook(node->right, book);
        } else {
            cout << "Book with the same ISBN already exists." << endl;
        }
        return node;
    }

    BSTNode* searchBook(BSTNode* node,const string& isbn){
        if (node == nullptr || node->book->getISBN() == isbn)
        {
            return node;
        }
        if (isbn < node->book->getISBN())
        {
            return searchBook(node -> left, isbn);
        }else{
            return searchBook(node -> right, isbn);
        }
    }
    
    BSTNode* removeBook(BSTNode* node, const string& isbn) {
    if (node == nullptr) return node;

    if (isbn < node->book->getISBN()) {
        node->left = removeBook(node->left, isbn);
    } else if (isbn > node->book->getISBN()) {
        node->right = removeBook(node->right, isbn);
    } else {
        // Node to be deleted found
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }

        // Node with two children: Get the inorder successor
        BSTNode* temp = node->right;
        while (temp && temp->left != nullptr) temp = temp->left;
        node->book = temp->book;
        node->right = removeBook(node->right, temp->book->getISBN());
    }
    return node;
    }

void removeBookFromLibrary(const string& isbn) {
    root = removeBook(root, isbn);
    cout << "Book with ISBN " << isbn << " has been removed." << endl;
}

    void inOrderDisplay(BSTNode* node) {
        if (node != nullptr) {
            inOrderDisplay(node->left);
            node->book->displayBookDetails();
            inOrderDisplay(node->right);
        }
    }

public:
    Library() : root(nullptr), memberRoot(nullptr) {}

    void addBook(string title, string author, string isbn, int copyCount = 1) {
        Book* newBook = new Book(title, author, isbn, copyCount);
        root = insertBook(root, newBook);
        bookCount++;
        cout << "BOOK ADDED TO THE LIBRARY: " << title << endl;
    }

    void addMember(string name, string id) {
        Member* newMember = new Member(name, id);
        MemberNode* newNode = new MemberNode(newMember);
        if (memberRoot == nullptr) {
            memberRoot = newNode;
        } else {
            MemberNode* temp = memberRoot;
            while (temp -> next != nullptr) {
                temp = temp->next;
            }
            temp -> next = newNode;
        }
    }

    void displayMembers() {
        MemberNode* temp = memberRoot;
        while (temp != nullptr) {
            temp->member->displayMemberDetails();
            temp = temp->next;
        }
    }

    bool borrowBook(string memberID,string isbn){
        MemberNode* temp = memberRoot;
        while (temp != nullptr)
        {
            if (temp -> member -> getMemberID() == memberID) // Corrected here
            {
                BSTNode* booknode = searchBook(root,isbn);
                if (booknode && booknode -> book -> borrowBook())
                {
                    cout<< temp -> member -> getName() << " Borrwed the book " << booknode ->book->getTitle()<<endl;
                    return true;
                }else{
                    cout << "Book not available for borrowing" << endl;
                    return false;
                }
            }
            temp = temp -> next;
        }
        cout<< "Member not Found."<<endl;
        return false;
    }

    void searchBookByTitle(const string& title) {
       bool found = false;
       searchByTitle(root, title, found);
       if (!found) {
           cout << "No books found with the title: " << title << endl;
       }
    }
    
    void searchByTitle(BSTNode* node, const string& title, bool& found) {
        if (node != nullptr) {
            searchByTitle(node->left, title, found);
            if (node->book->getTitle() == title) {
                node->book->displayBookDetails();
                found = true;
            }
            searchByTitle(node->right, title, found);
        }
    }
    
    void searchBookByAuthor(const string& author) {
        bool found = false;
        searchByAuthor(root, author, found);
        if (!found) {
            cout << "No books found by author: " << author << endl;
        }
    }
    
    void searchByAuthor(BSTNode* node, const string& author, bool& found) {
        if (node != nullptr) {
            searchByAuthor(node->left, author, found);
            if (node->book->getAuthor() == author) {
                node->book->displayBookDetails();
                found = true;
            }
            searchByAuthor(node->right, author, found);
        }
    }


    void returnBook(string isbn){
        BSTNode* booknode = searchBook(root,isbn);
        if (booknode)
        {
            booknode -> book ->returnBook();
            cout<< "the book"<<booknode ->book->getTitle()<<"has been returned."<<endl;
        }else{
            cout<<"book not found."<<endl;
        } 
    }
  
    void generateReport(){
        cout<<"Book Report"<<endl;
        inOrderDisplay(root);

        cout<<"Member Report"<<endl;
        displayBooks();
    }
  
    void displayBooks() {
        inOrderDisplay(root);
    }
  
    ~Library() {
    clearBST(root);
    clearMembers(memberRoot);
    }
    
    void clearBST(BSTNode* node) {
        if (node != nullptr) {
            clearBST(node->left);
            clearBST(node->right);
            delete node->book; // Free the book memory
            delete node;
        }
    }
    
    void clearMembers(MemberNode* node) {
        while (node != nullptr) {
            MemberNode* temp = node;
            node = node->next;
            delete temp->member; // Free the member memory
            delete temp;
        }
    }
};
void function_1insert(Library& l) {
    int op;
    string title, author, isbn;
    int copyCount;

    while (true) {
        cout << "------------------------------------------------------------------" << endl;
        cout << "1. Do You want single Entry" << endl;
        cout << "2. Do you want multiple Entry" << endl;
        cout << "3. Display Books" << endl;
        cout << "4. Exit" << endl;
        cout << "------------------------------------------------------------------" << endl;
        cout << "Enter Options: ";
        cin >> op;

        switch (op) {
        case 1:
            cout << "Enter your Book Title: ";
            cin >> title;
            cout << "Enter your Book Author: ";
            cin >> author;
            cout << "Enter your Book ISBN: ";
            cin >> isbn;
            cout << "Enter No of Copies: ";
            cin >> copyCount;
            l.addBook(title, author, isbn, copyCount);
            break;

        case 2:
            int n;
            cout << "Enter number of books you want to add: ";
            cin >> n;
            for (int i = 0; i < n; i++) {
                cout << "------------------------------------------------------------------" << endl;
                cout << "Enter your Book Title: ";
                cin >> title;
                cout << "Enter your Book Author: ";
                cin >> author;
                cout << "Enter your Book ISBN: ";
                cin >> isbn;
                cout << "Enter No of Copies: ";
                cin >> copyCount;
                l.addBook(title, author, isbn, copyCount);
                cout << "------------------------------------------------------------------" << endl;
            }
            break;

        case 3:
            l.displayBooks();
            break;

        case 4:
            return;

        default:
            cout << "Invalid option" << endl;
            break;
        }
    }
}

void function_2insert(Library& l) {
    int op;
    string name, id;

    while (true) {
        cout << "1. Do You Want Single Member Entry" << endl;
        cout << "2. Do You Want Multiple Member Entry" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter Options: ";
        cin >> op;

        switch (op) {
        case 1:
            cout << "------------------------------------------------------------------" << endl;
            cout << "Enter your Member-Name: ";
            cin >> name;
            cout << "Enter your Member ID: ";
            cin >> id;
            l.addMember(name, id);
            cout << "------------------------------------------------------------------" << endl;
            break;

        case 2:
            int n;
            cout << "Enter number of members you want to add: ";
            cin >> n;
            for (int i = 0; i < n; i++) {
                cout << "------------------------------------------------------------------" << endl;
                cout << "Enter your Member-Name: ";
                cin >> name;
                cout << "Enter your Member ID: ";
                cin >> id;
                l.addMember(name, id);
                cout << "------------------------------------------------------------------" << endl;
            }
            break;

        case 3:
            l.displayMembers();
            return;

        default:
            cout << "Invalid option" << endl;
            break;
        }
    }
}

int main() {
    Library library;
    int options;

    while (true) {
        cout << "1. Book Management" << endl;
        cout << "2. Member Management" << endl;
        cout << "3. Borrow a Book" << endl;
        cout << "4. Return a Book" << endl;
        cout << "5. Search Book by Title" << endl;
        cout << "6. Search Book by Author" << endl;
        cout << "7. Remove a Book" << endl;
        cout << "8. Generate Report" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your Options: ";
        cin >> options;

        switch (options) {
        case 1:
            cout << "You are in Book Management" << endl;
            cout << "------------------------------------------------------------------" << endl;
            function_1insert(library);
            break;

        case 2:
            cout << "You are in Member Management" << endl;
            cout << "------------------------------------------------------------------" << endl;
            function_2insert(library);
            break;

        case 3: {
            string memberID, isbn;
            cout << "Enter Member ID: ";
            cin >> memberID;
            cout << "Enter Book ISBN: ";
            cin >> isbn;
            library.borrowBook(memberID, isbn);
        break;
        }
        case 4: {
            string isbn;
            cout << "Enter Book ISBN to Return: ";
            cin >> isbn;
            library.returnBook(isbn);
            break;
        }
        case 5:{
            
        }
        case 8:
            library.generateReport();
            break;
        case 9:
            return 0;
        default:
            cout << "Invalid option" << endl;
            break;
        }
    }

    return 0;
}
