#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "library.dat"
#define MAX_TITLE 100
#define MAX_AUTHOR 50

// Book structure
typedef struct
{
    int bookID;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int quantity; // Total copies available
    int issued;   // Number of copies currently borrowed
} Book;

// Function prototypes
void addBook();
void displayBooks();
void searchBook();
void updateBook();
void deleteBook();
void borrowBook();
void returnBook();
void generateReport();
int loadBooks(Book books[], int max);
void saveBooks(Book books[], int count);
int findBookByID(Book books[], int count, int id);
int findBookByTitle(Book books[], int count, char *title);

// Main menu
int main()
{
    int choice;
    while (1)
    {
        printf("\n=== LIBRARY MANAGEMENT SYSTEM ===\n");
        printf("1. Add New Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Update Book Details\n");
        printf("5. Delete Book\n");
        printf("6. Borrow Book\n");
        printf("7. Return Book\n");
        printf("8. Generate Report\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            searchBook();
            break;
        case 4:
            updateBook();
            break;
        case 5:
            deleteBook();
            break;
        case 6:
            borrowBook();
            break;
        case 7:
            returnBook();
            break;
        case 8:
            generateReport();
            break;
        case 9:
            printf("Thank you for using the Library Management System!\n");
            return 0;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Load all books from binary file into array
int loadBooks(Book books[], int max)
{
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL)
    {
        return 0; // No file or empty
    }
    int count = 0;
    while (count < max && fread(&books[count], sizeof(Book), 1, file) == 1)
    {
        count++;
    }
    fclose(file);
    return count;
}

// Save all books from array to binary file
void saveBooks(Book books[], int count)
{
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL)
    {
        printf("Error: Unable to open file for writing!\n");
        return;
    }
    fwrite(books, sizeof(Book), count, file);
    fclose(file);
}

// Find book index by ID
int findBookByID(Book books[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (books[i].bookID == id)
        {
            return i;
        }
    }
    return -1;
}

// Find book index by Title (partial match)
int findBookByTitle(Book books[], int count, char *title)
{
    for (int i = 0; i < count; i++)
    {
        if (strstr(books[i].title, title) != NULL)
        {
            return i;
        }
    }
    return -1;
}

// Add a new book
void addBook()
{
    Book books[500];
    int count = loadBooks(books, 500);

    Book newBook;
    printf("\nEnter Book ID: ");
    scanf("%d", &newBook.bookID);
    getchar();

    if (findBookByID(books, count, newBook.bookID) != -1)
    {
        printf("Error: Book ID already exists!\n");
        return;
    }

    printf("Enter Title: ");
    fgets(newBook.title, MAX_TITLE, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove newline

    printf("Enter Author: ");
    fgets(newBook.author, MAX_AUTHOR, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    printf("Enter Quantity: ");
    scanf("%d", &newBook.quantity);
    newBook.issued = 0;

    if (newBook.quantity <= 0)
    {
        printf("Quantity must be positive!\n");
        return;
    }

    books[count] = newBook;
    saveBooks(books, count + 1);
    printf("Book added successfully!\n");
}

// Display all books
void displayBooks()
{
    Book books[500];
    int count = loadBooks(books, 500);
    if (count == 0)
    {
        printf("No books in the library.\n");
        return;
    }
    printf("\n%-5s %-30s %-25s %-10s %-10s\n", "ID", "Title", "Author", "Quantity", "Issued");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-5d %-30s %-25s %-10d %-10d\n",
                books[i].bookID, books[i].title, books[i].author,
                books[i].quantity, books[i].issued);
    }
}

// Search for a book
void searchBook()
{
    Book books[500];
    int count = loadBooks(books, 500);
    if (count == 0)
    {
        printf("No books available.\n");
        return;
    }

    int choice, id, idx;
    char title[MAX_TITLE];
    printf("\nSearch by:\n1. Book ID\n2. Title\nChoice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1)
    {
        printf("Enter Book ID: ");
        scanf("%d", &id);
        idx = findBookByID(books, count, id);
    }
    else
    {
        printf("Enter Title (partial): ");
        fgets(title, MAX_TITLE, stdin);
        title[strcspn(title, "\n")] = 0;
        idx = findBookByTitle(books, count, title);
    }

    if (idx == -1)
    {
        printf("Book not found!\n");
    }
    else
    {
        printf("\nBook Found:\n");
        printf("ID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\nIssued: %d\n",
                books[idx].bookID, books[idx].title, books[idx].author,
                books[idx].quantity, books[idx].issued);
    }
}

// Update book details
void updateBook()
{
    Book books[500];
    int count = loadBooks(books, 500);
    if (count == 0)
    {
        printf("No books to update.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    idx = findBookByID(books, count, id);

    if (idx == -1)
    {
        printf("Book not found!\n");
        return;
    }

    printf("Enter new Title (or press Enter to keep): ");
    getchar();
    char temp[MAX_TITLE];
    fgets(temp, MAX_TITLE, stdin);
    if (temp[0] != '\n')
    {
        temp[strcspn(temp, "\n")] = 0;
        strcpy(books[idx].title, temp);
    }

    printf("Enter new Author (or press Enter to keep): ");
    fgets(temp, MAX_TITLE, stdin);
    if (temp[0] != '\n')
    {
        temp[strcspn(temp, "\n")] = 0;
        strcpy(books[idx].author, temp);
    }

    printf("Enter new Quantity (or -1 to keep): ");
    int qty;
    scanf("%d", &qty);
    if (qty != -1)
    {
        if (qty < books[idx].issued)
        {
            printf("Cannot set quantity less than issued books!\n");
            return;
        }
        books[idx].quantity = qty;
    }

    saveBooks(books, count);
    printf("Book updated successfully!\n");
}

// Delete a book
void deleteBook()
{
    Book books[500];
    int count = loadBooks(books, 500);
    if (count == 0)
    {
        printf("No books to delete.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    idx = findBookByID(books, count, id);

    if (idx == -1)
    {
        printf("Book not found!\n");
        return;
    }

    // Shift remaining books
    for (int i = idx; i < count - 1; i++)
    {
        books[i] = books[i + 1];
    }
    saveBooks(books, count - 1);
    printf("Book deleted successfully!\n");
}

// Borrow a book
void borrowBook()
{
    Book books[500];
    int count = loadBooks(books, 500);
    if (count == 0)
    {
        printf("No books available.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to borrow: ");
    scanf("%d", &id);
    idx = findBookByID(books, count, id);

    if (idx == -1)
    {
        printf("Book not found!\n");
        return;
    }

    if (books[idx].quantity - books[idx].issued <= 0)
    {
        printf("Sorry, no copies available for borrowing!\n");
        return;
    }

    books[idx].issued++;
    saveBooks(books, count);
    printf("Book borrowed successfully! (Remaining copies: %d)\n",
            books[idx].quantity - books[idx].issued);
}

// Return a book
void returnBook()
{
    Book books[500];
    int count = loadBooks(books, 500);
    if (count == 0)
    {
        printf("No books in library.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);
    idx = findBookByID(books, count, id);

    if (idx == -1)
    {
        printf("Book not found!\n");
        return;
    }

    if (books[idx].issued <= 0)
    {
        printf("No copies of this book are currently issued!\n");
        return;
    }

    books[idx].issued--;
    saveBooks(books, count);
    printf("Book returned successfully!\n");
}

// Generate reports
void generateReport()
{
    Book books[500];
    int count = loadBooks(books, 500);
    if (count == 0)
    {
        printf("No books in the library.\n");
        return;
    }

    int totalBooks = 0, totalIssued = 0, totalAvailable = 0;

    printf("\n=== LIBRARY REPORT ===\n");
    printf("%-5s %-30s %-10s %-10s\n", "ID", "Title", "Available", "Issued");
    printf("---------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        int available = books[i].quantity - books[i].issued;
        printf("%-5d %-30s %-10d %-10d\n",
                books[i].bookID, books[i].title, available, books[i].issued);

        totalBooks += books[i].quantity;
        totalIssued += books[i].issued;
        totalAvailable += available;
    }

    printf("\nTotal Books in Library : %d\n", totalBooks);
    printf("Total Available        : %d\n", totalAvailable);
    printf("Total Borrowed         : %d\n", totalIssued);

    // List of borrowed books
    printf("\n=== CURRENTLY BORROWED BOOKS ===\n");
    int borrowedCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (books[i].issued > 0)
        {
            printf("ID: %d | Title: %s | Borrowed: %d\n",
                    books[i].bookID, books[i].title, books[i].issued);
            borrowedCount++;
        }
    }
    if (borrowedCount == 0)
    {
        printf("No books are currently borrowed.\n");
    }
}