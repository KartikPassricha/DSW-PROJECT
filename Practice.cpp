#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <vector>

using namespace std;

void printLine(char ch, int length) {
    for (int i = 0; i < length; i++) {
        cout << ch;
    }
    cout << endl;
}

class Book {
public:
    int ISBN;
    string title;
    string author;
    int year;
    string category;
    int totalCopies;
    int availableCopies;

    Book(int isbn, string t, string a, int y, string c, int copies)
        : ISBN(isbn), title(t), author(a), year(y), category(c), totalCopies(copies), availableCopies(copies) {}

    void display() {
        printLine('-', 70);
        cout << "| ISBN: " << ISBN << "\t| Title: " << title
             << "\t| Author: " << author
             << "\t| Year: " << year
             << "\t| Category: " << category
             << "\t| Available Copies: " << availableCopies
             << "/" << totalCopies << "\t|";
        printLine('-', 70);
    }

    bool isAvailable() const {
        return availableCopies > 0;
    }

    bool borrow() {
        if (availableCopies > 0) {
            availableCopies--;
            return true;
        }
        return false;
    }

    void returnBook() {
        availableCopies++;
    }
};

class User {
public:
    int userID;
    string name;
    stack<Book*> borrowHistory;

    User(int id, string n) : userID(id), name(n) {}

    void displayBorrowHistory() {
        printLine('=', 70);
        cout << "Borrowing History for User: " << name << endl;
        printLine('=', 70);

        if (borrowHistory.empty()) {
            cout << "No borrowing history for user " << name << ".\n";
            printLine('=', 70);
            return;
        }

        stack<Book*> temp = borrowHistory;
        while (!temp.empty()) {
            Book* book = temp.top();
            book->display();
            temp.pop();
        }
    }
};

class BorrowRequest {
public:
    int userID;
    int ISBN;
    string borrowDate;
    string dueDate;

    BorrowRequest(int uID, int isbn, string bDate, string dDate)
        : userID(uID), ISBN(isbn), borrowDate(bDate), dueDate(dDate) {}
};

struct CompareDueDate {
    bool operator()(BorrowRequest* a, BorrowRequest* b) {
        return a->dueDate > b->dueDate;
    }
};

class Library {
public:
    map<int, Book*> booksByISBN;
    map<string, vector<Book*>> booksByTitle;
    map<string, vector<Book*>> booksByAuthor;
    map<int, vector<Book*>> booksByYear;
    map<string, vector<Book*>> booksByCategory;
    map<int, User*> users;
    priority_queue<BorrowRequest*, vector<BorrowRequest*>, CompareDueDate> overdueBooks;

    void addBook() {
        int isbn, year, copies;
        string title, author, category;

        cout << "\n=== Add Book ===\n";
        cout << "Enter ISBN: ";
        cin >> isbn;
        cout << "Enter title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter year: ";
        cin >> year;
        cout << "Enter category: ";
        cin.ignore();
        getline(cin, category);
        cout << "Enter number of copies: ";
        cin >> copies;

        Book* book = new Book(isbn, title, author, year, category, copies);
        booksByISBN[isbn] = book;
        booksByTitle[title].push_back(book);
        booksByAuthor[author].push_back(book);
        booksByYear[year].push_back(book);
        booksByCategory[category].push_back(book);

        cout << "Book added successfully.\n";
    }

    void addUser() {
        int userID;
        string name;

        cout << "\n=== Add User ===\n";
        cout << "Enter user ID: ";
        cin >> userID;
        cout << "Enter user name: ";
        cin.ignore();
        getline(cin, name);

        users[userID] = new User(userID, name);
        cout << "User added successfully.\n";
    }

    void borrowBook() {
        int userID;
        string borrowDate, dueDate;

        cout << "\n=== Borrow Book ===\n";
        cout << "Enter user ID: ";
        cin >> userID;
        if ( users.find(userID) == users.end()) {
            cout << "User  not found.\n";
            return;
        }

        cout << "Enter search criteria:\n";
        cout << "1. Search by ISBN\n";
        cout << "2. Search by Title\n";
        cout << "3. Search by Author\n";
        cout << "4. Search by Year\n";
        cout << "5. Search by Category\n";
        cout << "6. Search by Availability\n";
        cout << "Enter your choice: ";
        int searchChoice;
        cin >> searchChoice;

        Book* book = nullptr;
        switch (searchChoice) {
            case 1: {
                int ISBN;
                cout << "Enter ISBN: ";
                cin >> ISBN;
                book = searchByISBN(ISBN);
                break;
            }
            case 2: {
                string title;
                cout << "Enter title: ";
                cin.ignore();
                getline(cin, title);
                vector<Book*> results = searchByTitle(title);
                if (results.empty()) {
                    cout << "No books found with the title \"" << title << "\".\n";
                    return;
                }
                cout << "Select a book:\n";
                for (int i = 0; i < results.size(); i++) {
                    cout << i + 1 << ". ";
                    results[i]->display();
                }
                int selection;
                cin >> selection;
                book = results[selection - 1];
                break;
            }
            case 3: {
                string author;
                cout << "Enter author: ";
                cin.ignore();
                getline(cin, author);
                vector<Book*> results = searchByAuthor(author);
                if (results.empty()) {
                    cout << "No books found by author \"" << author << "\".\n";
                    return;
                }
                cout << "Select a book:\n";
                for (int i = 0; i < results.size(); i++) {
                    cout << i + 1 << ". ";
                    results[i]->display();
                }
                int selection;
                cin >> selection;
                book = results[selection - 1];
                break;
            }
            case 4: {
                int year;
                cout << "Enter year: ";
                cin >> year;
                vector<Book*> results = searchByYear(year);
                if (results.empty()) {
                    cout << "No books found published in the year " << year << ".\n";
                    return;
                }
                cout << "Select a book:\n";
                for (int i = 0; i < results.size(); i++) {
                    cout << i + 1 << ". ";
                    results[i]->display();
                }
                int selection;
                cin >> selection;
                book = results[selection - 1];
                break;
            }
            case 5: {
                string category;
                cout << "Enter category: ";
                cin.ignore();
                getline(cin, category);
                vector<Book*> results = searchByCategory(category);
                if (results.empty()) {
                    cout << "No books found in the category \"" << category << "\".\n";
                    return;
                }
                cout << "Select a book:\n";
                for (int i = 0; i < results.size(); i++) {
                    cout << i + 1 << ". ";
                    results[i]->display();
                }
                int selection;
                cin >> selection;
                book = results[selection - 1];
                break;
            }
            case 6: {
                vector<Book*> results = searchByAvailability();
                if (results.empty()) {
                    cout << "No books available.\n";
                    return;
                }
                cout << "Select a book:\n";
                for (int i = 0; i < results.size(); i++) {
                    cout << i + 1 << ". ";
                    results[i]->display();
                }
                int selection;
                cin >> selection;
                book = results[selection - 1];
                break;
            }
        }

        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }

        if (!book->isAvailable()) {
            cout << "Book is not available.\n";
            return;
        }

        cout << "Enter borrow date (YYYY-MM-DD): ";
        cin >> borrowDate;
        cout << "Enter due date (YYYY-MM-DD): ";
        cin >> dueDate;

        book->borrow();
        users[userID]->borrowHistory.push(book);
        overdueBooks.push(new BorrowRequest(userID, book->ISBN, borrowDate, dueDate));

        cout << "Book borrowed successfully.\n";
    }

    void displayBooks() {
        cout << "\n=== Display Books ===\n";
        cout << "1. Display all books\n";
        cout << "2. Display books by ISBN\n";
        cout << "3. Display books by Title\n";
        cout << "4. Display books by Author\n";
 cout << "5. Display books by Year\n";
        cout << "6. Display books by Category\n";
        cout << "7. Display available books\n";
        cout << "Enter your choice: ";
        int displayChoice;
        cin >> displayChoice;

        switch (displayChoice) {
            case 1: {
                for (auto& book : booksByISBN) {
                    book.second->display();
                }
                break;
            }
            case 2: {
                int ISBN;
                cout << "Enter ISBN: ";
                cin >> ISBN;
                Book* book = searchByISBN(ISBN);
                if (book != nullptr) {
                    book->display();
                } else {
                    cout << "Book not found.\n";
                }
                break;
            }
            case 3: {
                string title;
                cout << "Enter title: ";
                cin.ignore();
                getline(cin, title);
                vector<Book*> results = searchByTitle(title);
                if (!results.empty()) {
                    for (auto& book : results) {
                        book->display();
                    }
                } else {
                    cout << "No books found with the title \"" << title << "\".\n";
                }
                break;
            }
            case 4: {
                string author;
                cout << "Enter author: ";
                cin.ignore();
                getline(cin, author);
                vector<Book*> results = searchByAuthor(author);
                if (!results.empty()) {
                    for (auto& book : results) {
                        book->display();
                    }
                } else {
                    cout << "No books found by author \"" << author << "\".\n";
                }
                break;
            }
            case 5: {
                int year;
                cout << "Enter year: ";
                cin >> year;
                vector<Book*> results = searchByYear(year);
                if (!results.empty()) {
                    for (auto& book : results) {
                        book->display();
                    }
                } else {
                    cout << "No books found published in the year " << year << ".\n";
                }
                break;
            }
            case 6: {
                string category;
                cout << "Enter category: ";
                cin.ignore();
                getline(cin, category);
                vector<Book*> results = searchByCategory(category);
                if (!results.empty()) {
                    for (auto& book : results) {
                        book->display();
                    }
                } else {
                    cout << "No books found in the category \"" << category << "\".\n";
                }
                break;
            }
            case 7: {
                vector<Book*> results = searchByAvailability();
                if (!results.empty()) {
                    for (auto& book : results) {
                        book->display();
                    }
                } else {
                    cout << "No books available.\n";
                }
                break;
            }
        }
    }

    Book* searchByISBN(int ISBN) {
        if (booksByISBN.find(ISBN) != booksByISBN.end()) {
            return booksByISBN[ISBN];
        }
        return nullptr;
    }

    vector<Book*> searchByTitle(string title) {
        if (booksByTitle.find(title) != booksByTitle.end()) {
            return booksByTitle[title];
        }
        return {};
    }

    vector<Book*> searchByAuthor(string author) {
        if (booksByAuthor.find(author) != booksByAuthor.end()) {
            return booksByAuthor[author];
        }
        return {};
    }

    vector<Book*> searchByYear(int year) {
        if (booksByYear.find(year) != booksByYear.end()) {
            return booksByYear[year];
        }
        return {};
    }

    vector<Book*> searchByCategory(string category) {
        if (booksByCategory.find(category) != booksByCategory.end()) {
            return booksByCategory[category];
        }
        return {};
    }

    vector<Book*> searchByAvailability() {
        vector<Book*> availableBooks;
        for (auto& book : booksByISBN) {
            if (book.second->isAvailable()) {
                availableBooks.push_back(book.second);
            }
        }
        return availableBooks;
    }

    void displayUsers() {
        cout << "\n=== Display Users ===\n";
        for (auto& user : users) {
            cout << "User   ID: " << user.first << "\tName: " << user.second->name << endl;
        }
    }

    void displayBorrowHistory() {
        cout << "\n=== Display Borrow History ===\n";
        for (auto& user : users) {
            user.second->displayBorrowHistory();
        }
    }

    void displayOverdueBooks() {
        cout << "\n=== Display Overdue Books ===\n";
        while (!overdueBooks.empty()) {
            BorrowRequest* request = overdueBooks.top();
            Book* book = searchByISBN(request->ISBN);
            cout << "User   ID: " << request-> userID << "\tBook Title: " << book->title << "\tDue Date: " << request->dueDate << endl;
            overdueBooks.pop();
        }
    }
};

int main() {
    Library library;

    while (true) {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Add Book\n";
        cout << "2. Add User\n";
        cout << "3. Borrow Book\n";
        cout << "4. Display Books\n";
        cout << "5. Display Users\n";
        cout << "6. Display Borrow History\n";
        cout << "7. Display Overdue Books\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.addUser();
                break;
            case 3:
                library.borrowBook();
                break;
            case 4:
                library.displayBooks();
                break;
            case 5:
                library.displayUsers();
                break;
            case 6:
                library.displayBorrowHistory();
                break;
            case 7:
                library.displayOverdueBooks();
                break;
            case 8:
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}