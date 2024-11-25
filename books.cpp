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

void readbook(){
    cout<<"1. Letter to God"<<endl;
    cout<<"2. Thermodynamics"<<endl;
    int nb;
    cin>>nb;
    if (nb == 1)  {
        cout<<"\n Lencho was a dedicated farmer. He was expecting a decent harvest. However, to his grief, a hail storm came and destroyed his harvest completely. Lencho was very sad. However, he had a strong belief in God. He was certain that God would help him. Also, he was an extremely straightforward man. Although working for a long time on the farm, he knew writing. Thus, he composed a letter to God. In the letter, he asked God to send him one hundred pesos. At that point, he went to the post office and put his letter into the post box."<<endl;
        cout<<"\n The postman removed the letter from the letter-box. He read the address on it and laughed very much. Also, he rushed to the postmaster and demonstrated to him that strange letter. Moreover, the postmaster also laughed in the same way when he saw the address of God. However, on reading the letter, he got very serious. He lauded this man who had unquestioned faith in God and decided to help him in terms of money. He asked the employees of the post office to give charity. Moreover, he gave a part of-of his salary too. However, they were able to collect only a little more than 50 pesos as requested for by Lencho. The postmaster put the money in an envelope. It was addressed to Lencho."<<endl;
        cout<<"\n After Lencho had left the place, the postmaster and the employees read the letter. In it, Lencho had complained to God that he had received only seventy pesos. Also, he requested God to send him the rest of the money this time. However, he asked God not to send the money through the mail. He wrote that the post-office employees were a bunch of crooks and thus might have stolen the money."<<endl;
        cout<<"\n Conclusion of A letter to God"<<endl;
        cout<<"\n We learnt that Faith has the power to give us what we want, to fulfil our needs However, one must realise that humanity still prevails."<<endl;
    }

    else if(nb == 2) {
        cout<<"Thermodynamics is the branch of physics that studies heat, work, and temperature and their relation to energy, entropy, and the physical properties of matter and radiation. The behavior of these quantities is governed by the four laws of thermodynamics, which convey a quantitative description using measurable macroscopic physical quantities, but may be explained in terms of microscopic constituents by statistical mechanics. Thermodynamics plays a role in a wide variety of topics in science and engineering."

    "Historically, thermodynamics developed out of a desire to increase the efficiency of early steam engines, particularly through the work of French physicist Sadi Carnot (1824) who believed that engine efficiency was the key that could help France win the Napoleonic Wars.[1] Scots-Irish physicist Lord Kelvin was the first to formulate a concise definition of thermodynamics in 1854[2] which stated, Thermo-dynamics is the subject of the relation of heat to forces acting between contiguous parts of bodies, and the relation of heat to electrical agency. German physicist and mathematician Rudolf Clausius restated Carnot's principle known as the Carnot cycle and gave to the theory of heat a truer and sounder basis. His most important paper, On the Moving Force of Heat,[3] published in 1850, first stated the second law of thermodynamics. In 1865 he introduced the concept of entropy. In 1870 he introduced the virial theorem, which applied to heat.[4]"

    "The initial application of thermodynamics to mechanical heat engines was quickly extended to the study of chemical compounds and chemical reactions. Chemical thermodynamics studies the nature of the role of entropy in the process of chemical reactions and has provided the bulk of expansion and knowledge of the field. Other formulations of thermodynamics emerged. Statistical thermodynamics, or statistical mechanics, concerns itself with statistical predictions of the collective motion of particles from their microscopic behavior. In 1909, Constantin Carathéodory presented a purely mathematical approach in an axiomatic formulation, a description often referred to as geometrical thermodynamics."

    "Introduction"
    "A description of any thermodynamic system employs the four laws of thermodynamics that form an axiomatic basis. The first law specifies that energy can be transferred between physical systems as heat, as work, and with transfer of matter.[5] The second law defines the existence of a quantity called entropy, that describes the direction, thermodynamically, that a system can evolve and quantifies the state of order of a system and that can be used to quantify the useful work that can be extracted from the system.[6]"

    "In thermodynamics, interactions between large ensembles of objects are studied and categorized. Central to this are the concepts of the thermodynamic system and its surroundings. A system is composed of particles, whose average motions define its properties, and those properties are in turn related to one another through equations of state. Properties can be combined to express internal energy and thermodynamic potentials, which are useful for determining conditions for equilibrium and spontaneous processes."

    "With these tools, thermodynamics can be used to describe how systems respond to changes in their environment. This can be applied to a wide variety of topics in science and engineering, such as engines, phase transitions, chemical reactions, transport phenomena, and even black holes. The results of thermodynamics are essential for other fields of physics and for chemistry, chemical engineering, corrosion engineering, aerospace engineering, mechanical engineering, electrical engineering, cell biology, biomedical engineering, materials science, and economics, to name a few.[7][8]"

    "This article is focused mainly on classical thermodynamics which primarily studies systems in thermodynamic equilibrium. Non-equilibrium thermodynamics is often treated as an extension of the classical treatment, but statistical mechanics has brought many advances to that field."<<endl;
    }
}
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
        cout << "8. Read book\n";
        cout << "9. Exit\n";
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
                readbook();
                break;
            case 9:
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
