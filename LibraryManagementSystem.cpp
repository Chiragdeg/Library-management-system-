#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

// Book class
class Book {
public:
    int bookId;
    string title;
    string author;
    string isbn;
    bool isAvailable;
    int totalCopies;
    int availableCopies;

    Book() : bookId(0), isAvailable(true), totalCopies(0), availableCopies(0) {}
    
    Book(int id, string t, string a, string isbn_val, int copies)
        : bookId(id), title(t), author(a), isbn(isbn_val), 
          totalCopies(copies), availableCopies(copies), isAvailable(true) {}

    void display() const {
        cout << setw(8) << bookId 
             << setw(25) << title.substr(0, 24)
             << setw(20) << author.substr(0, 19)
             << setw(15) << isbn 
             << setw(12) << (isAvailable ? "Available" : "Not Avail")
             << setw(10) << availableCopies << "/" << totalCopies << endl;
    }
};

// Member class
class Member {
public:
    int memberId;
    string name;
    string email;
    string phone;
    int borrowedBooks;

    Member() : memberId(0), borrowedBooks(0) {}
    
    Member(int id, string n, string e, string p)
        : memberId(id), name(n), email(e), phone(p), borrowedBooks(0) {}

    void display() const {
        cout << setw(10) << memberId 
             << setw(20) << name 
             << setw(25) << email 
             << setw(15) << phone 
             << setw(15) << borrowedBooks << endl;
    }
};

// Borrow Record class
class BorrowRecord {
public:
    int recordId;
    int memberId;
    int bookId;
    string borrowDate;
    string returnDate;
    bool isReturned;

    BorrowRecord() : recordId(0), memberId(0), bookId(0), isReturned(false) {}
    
    BorrowRecord(int rid, int mid, int bid, string bdate)
        : recordId(rid), memberId(mid), bookId(bid), 
          borrowDate(bdate), isReturned(false) {}

    void display() const {
        cout << setw(10) << recordId 
             << setw(12) << memberId 
             << setw(10) << bookId 
             << setw(15) << borrowDate 
             << setw(15) << (isReturned ? returnDate : "Not Returned")
             << setw(12) << (isReturned ? "Returned" : "Active") << endl;
    }
};

// Library Management System class
class LibraryManagementSystem {
private:
    vector<Book> books;
    vector<Member> members;
    vector<BorrowRecord> borrowRecords;
    
    const string bookFile = "books_library.txt";
    const string memberFile = "members_library.txt";
    const string borrowFile = "borrow_records.txt";

public:
    LibraryManagementSystem() {
        loadAllData();
    }

    // ===================== FILE OPERATIONS =====================

    void loadAllData() {
        loadBooksFromFile();
        loadMembersFromFile();
        loadBorrowRecordsFromFile();
    }

    void loadBooksFromFile() {
        ifstream file(bookFile);
        if (!file.is_open()) return;

        books.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            int id, total, available;
            string title, author, isbn;
            bool avail;
            size_t pos = 0;

            pos = line.find("|");
            id = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find("|");
            title = line.substr(0, pos);
            line = line.substr(pos + 1);

            pos = line.find("|");
            author = line.substr(0, pos);
            line = line.substr(pos + 1);

            pos = line.find("|");
            isbn = line.substr(0, pos);
            line = line.substr(pos + 1);

            pos = line.find("|");
            total = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find("|");
            available = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            avail = stoi(line) == 1;

            books.push_back(Book(id, title, author, isbn, total));
            books.back().availableCopies = available;
            books.back().isAvailable = avail;
        }
        file.close();
    }

    void loadMembersFromFile() {
        ifstream file(memberFile);
        if (!file.is_open()) return;

        members.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            int id, borrowed;
            string name, email, phone;
            size_t pos = 0;

            pos = line.find("|");
            id = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find("|");
            name = line.substr(0, pos);
            line = line.substr(pos + 1);

            pos = line.find("|");
            email = line.substr(0, pos);
            line = line.substr(pos + 1);

            pos = line.find("|");
            phone = line.substr(0, pos);
            line = line.substr(pos + 1);

            borrowed = stoi(line);

            members.push_back(Member(id, name, email, phone));
            members.back().borrowedBooks = borrowed;
        }
        file.close();
    }

    void loadBorrowRecordsFromFile() {
        ifstream file(borrowFile);
        if (!file.is_open()) return;

        borrowRecords.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            int rid, mid, bid;
            string bdate, rdate;
            bool returned;
            size_t pos = 0;

            pos = line.find("|");
            rid = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find("|");
            mid = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find("|");
            bid = stoi(line.substr(0, pos));
            line = line.substr(pos + 1);

            pos = line.find("|");
            bdate = line.substr(0, pos);
            line = line.substr(pos + 1);

            pos = line.find("|");
            rdate = line.substr(0, pos);
            line = line.substr(pos + 1);

            returned = stoi(line) == 1;

            BorrowRecord rec(rid, mid, bid, bdate);
            rec.returnDate = rdate;
            rec.isReturned = returned;
            borrowRecords.push_back(rec);
        }
        file.close();
    }

    void saveBooksToFile() {
        ofstream file(bookFile);
        if (!file.is_open()) return;

        for (const auto& book : books) {
            file << book.bookId << "|"
                 << book.title << "|"
                 << book.author << "|"
                 << book.isbn << "|"
                 << book.totalCopies << "|"
                 << book.availableCopies << "|"
                 << (book.isAvailable ? 1 : 0) << "\n";
        }
        file.close();
    }

    void saveMembersToFile() {
        ofstream file(memberFile);
        if (!file.is_open()) return;

        for (const auto& member : members) {
            file << member.memberId << "|"
                 << member.name << "|"
                 << member.email << "|"
                 << member.phone << "|"
                 << member.borrowedBooks << "\n";
        }
        file.close();
    }

    void saveBorrowRecordsToFile() {
        ofstream file(borrowFile);
        if (!file.is_open()) return;

        for (const auto& record : borrowRecords) {
            file << record.recordId << "|"
                 << record.memberId << "|"
                 << record.bookId << "|"
                 << record.borrowDate << "|"
                 << record.returnDate << "|"
                 << (record.isReturned ? 1 : 0) << "\n";
        }
        file.close();
    }

    // ===================== BOOK OPERATIONS =====================

    void addBook() {
        cout << "\n--- Naya Book Add Karo ---\n";
        int id, copies;
        string title, author, isbn;

        cout << "Book ID enter karo: ";
        cin >> id;
        cin.ignore();

        for (const auto& b : books) {
            if (b.bookId == id) {
                cout << "ERROR: Ye book ID pehle se exist karta h!\n";
                return;
            }
        }

        cout << "Title enter karo: ";
        getline(cin, title);

        cout << "Author enter karo: ";
        getline(cin, author);

        cout << "ISBN enter karo: ";
        getline(cin, isbn);

        cout << "Total copies enter karo: ";
        cin >> copies;
        cin.ignore();

        books.push_back(Book(id, title, author, isbn, copies));
        saveBooksToFile();
        cout << "✓ Book successfully add ho gaya!\n";
    }

    void displayAllBooks() const {
        cout << "\n";
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";
        cout << setw(8) << "ID" 
             << setw(25) << "Title" 
             << setw(20) << "Author" 
             << setw(15) << "ISBN" 
             << setw(12) << "Status"
             << setw(10) << "Copies" << endl;
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";

        if (books.empty()) {
            cout << "Koi book nahi h abhi!\n";
        } else {
            for (const auto& book : books) {
                book.display();
            }
        }
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";
    }

    void searchBookByTitle() {
        cout << "\n--- Book Ko Title Se Search Karo ---\n";
        string title;
        cout << "Title enter karo: ";
        cin.ignore();
        getline(cin, title);

        bool found = false;
        for (const auto& book : books) {
            if (book.title.find(title) != string::npos) {
                cout << "\nBook Found!\n";
                cout << "Book ID: " << book.bookId << endl;
                cout << "Title: " << book.title << endl;
                cout << "Author: " << book.author << endl;
                cout << "ISBN: " << book.isbn << endl;
                cout << "Available Copies: " << book.availableCopies << "/" << book.totalCopies << endl;
                found = true;
                break;
            }
        }
        if (!found) cout << "ERROR: Ye title nahi mila!\n";
    }

    void searchBookByAuthor() {
        cout << "\n--- Book Ko Author Se Search Karo ---\n";
        string author;
        cout << "Author enter karo: ";
        cin.ignore();
        getline(cin, author);

        bool found = false;
        cout << "\nBooks by " << author << ":\n";
        cout << "─────────────────────────────────────────\n";
        for (const auto& book : books) {
            if (book.author.find(author) != string::npos) {
                cout << "ID: " << book.bookId << " | Title: " << book.title 
                     << " | Available: " << book.availableCopies << endl;
                found = true;
            }
        }
        if (!found) cout << "ERROR: Ye author nahi mila!\n";
    }

    // ===================== MEMBER OPERATIONS =====================

    void addMember() {
        cout << "\n--- Naya Member Register Karo ---\n";
        int id;
        string name, email, phone;

        cout << "Member ID enter karo: ";
        cin >> id;
        cin.ignore();

        for (const auto& m : members) {
            if (m.memberId == id) {
                cout << "ERROR: Ye member ID pehle se exist karta h!\n";
                return;
            }
        }

        cout << "Name enter karo: ";
        getline(cin, name);

        cout << "Email enter karo: ";
        getline(cin, email);

        cout << "Phone enter karo: ";
        getline(cin, phone);

        members.push_back(Member(id, name, email, phone));
        saveMembersToFile();
        cout << "✓ Member successfully register ho gaya!\n";
    }

    void displayAllMembers() const {
        cout << "\n";
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";
        cout << setw(10) << "Member ID" 
             << setw(20) << "Name" 
             << setw(25) << "Email" 
             << setw(15) << "Phone" 
             << setw(15) << "Books Borrowed" << endl;
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";

        if (members.empty()) {
            cout << "Koi member register nahi h!\n";
        } else {
            for (const auto& member : members) {
                member.display();
            }
        }
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";
    }

    // ===================== ISSUE & RETURN OPERATIONS =====================

    void issueBook() {
        cout << "\n--- Book Issue Karo ---\n";
        int memberId, bookId;
        string borrowDate;

        cout << "Member ID enter karo: ";
        cin >> memberId;

        cout << "Book ID enter karo: ";
        cin >> bookId;
        cin.ignore();

        // Check member exists
        Member* member = nullptr;
        for (auto& m : members) {
            if (m.memberId == memberId) {
                member = &m;
                break;
            }
        }
        if (!member) {
            cout << "ERROR: Member nahi mila!\n";
            return;
        }

        // Check book exists and available
        Book* book = nullptr;
        for (auto& b : books) {
            if (b.bookId == bookId) {
                book = &b;
                break;
            }
        }
        if (!book) {
            cout << "ERROR: Book nahi mila!\n";
            return;
        }

        if (book->availableCopies <= 0) {
            cout << "ERROR: Book available nahi h!\n";
            return;
        }

        cout << "Borrow date enter karo (DD/MM/YYYY): ";
        getline(cin, borrowDate);

        // Create borrow record
        int recordId = borrowRecords.empty() ? 1 : borrowRecords.back().recordId + 1;
        borrowRecords.push_back(BorrowRecord(recordId, memberId, bookId, borrowDate));

        // Update book availability
        book->availableCopies--;
        if (book->availableCopies == 0) {
            book->isAvailable = false;
        }

        // Update member borrowed count
        member->borrowedBooks++;

        saveBooksToFile();
        saveMembersToFile();
        saveBorrowRecordsToFile();
        cout << "✓ Book successfully issue ho gaya!\n";
    }

    void returnBook() {
        cout << "\n--- Book Return Karo ---\n";
        int recordId;
        string returnDate;

        cout << "Borrow Record ID enter karo: ";
        cin >> recordId;
        cin.ignore();

        BorrowRecord* record = nullptr;
        for (auto& r : borrowRecords) {
            if (r.recordId == recordId && !r.isReturned) {
                record = &r;
                break;
            }
        }

        if (!record) {
            cout << "ERROR: Record nahi mila ya pehle se returned h!\n";
            return;
        }

        cout << "Return date enter karo (DD/MM/YYYY): ";
        getline(cin, returnDate);

        // Update record
        record->isReturned = true;
        record->returnDate = returnDate;

        // Update book availability
        for (auto& b : books) {
            if (b.bookId == record->bookId) {
                b.availableCopies++;
                b.isAvailable = true;
                break;
            }
        }

        // Update member
        for (auto& m : members) {
            if (m.memberId == record->memberId) {
                m.borrowedBooks--;
                break;
            }
        }

        saveBooksToFile();
        saveMembersToFile();
        saveBorrowRecordsToFile();
        cout << "✓ Book successfully return ho gaya!\n";
    }

    void displayBorrowRecords() const {
        cout << "\n";
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";
        cout << setw(10) << "Record ID" 
             << setw(12) << "Member ID" 
             << setw(10) << "Book ID" 
             << setw(15) << "Borrow Date" 
             << setw(15) << "Return Date"
             << setw(12) << "Status" << endl;
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";

        if (borrowRecords.empty()) {
            cout << "Koi record nahi h!\n";
        } else {
            for (const auto& record : borrowRecords) {
                record.display();
            }
        }
        cout << "═══════════════════════════════════════════════════════════════════════════════════\n";
    }

    // ===================== MENU =====================

    void displayMenu() {
        while (true) {
            cout << "\n╔══════════════════════════════════════════════╗\n";
            cout << "║   LIBRARY MANAGEMENT SYSTEM                  ║\n";
            cout << "╠══════════════════════════════════════════════╣\n";
            cout << "║  BOOKS:                                      ║\n";
            cout << "║  1. Book Add Karo                            ║\n";
            cout << "║  2. Sab Books Display Karo                   ║\n";
            cout << "║  3. Book Search By Title                     ║\n";
            cout << "║  4. Book Search By Author                    ║\n";
            cout << "║                                              ║\n";
            cout << "║  MEMBERS:                                    ║\n";
            cout << "║  5. Member Register Karo                     ║\n";
            cout << "║  6. Sab Members Display Karo                 ║\n";
            cout << "║                                              ║\n";
            cout << "║  ISSUE & RETURN:                             ║\n";
            cout << "║  7. Book Issue Karo                          ║\n";
            cout << "║  8. Book Return Karo                         ║\n";
            cout << "║  9. Borrow Records Display Karo              ║\n";
            cout << "║  10. Exit                                    ║\n";
            cout << "╚══════════════════════════════════════════════╝\n";

            cout << "Choose option (1-10): ";
            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    addBook();
                    break;
                case 2:
                    displayAllBooks();
                    break;
                case 3:
                    searchBookByTitle();
                    break;
                case 4:
                    searchBookByAuthor();
                    break;
                case 5:
                    addMember();
                    break;
                case 6:
                    displayAllMembers();
                    break;
                case 7:
                    issueBook();
                    break;
                case 8:
                    returnBook();
                    break;
                case 9:
                    displayBorrowRecords();
                    break;
                case 10:
                    cout << "Thank you! Program band ho gaya. Bye bye! 👋\n";
                    return;
                default:
                    cout << "ERROR: Wrong choice! 1-10 ke beech select karo!\n";
            }
        }
    }
};

// Main function
int main() {
    cout << "\n╔══════════════════════════════════════════════╗\n";
    cout << "║  WELCOME TO LIBRARY MANAGEMENT SYSTEM        ║\n";
    cout << "║  Internship Project                          ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";

    LibraryManagementSystem library;
    library.displayMenu();

    return 0;
}
