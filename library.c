#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "library.dat"
#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_BOOKS 500

// Book structure
typedef struct
{
    int bookID;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int quantity;
    int issued;
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
void toLowerCase(char *str);
int isValidBookID(int id);

// ====================== MAIN ======================
int main()
{
    int choice;
    while (1)
    {
        printf("\n=== MOMBASA MODERN LIBRARY MANAGEMENT SYSTEM ===\n");
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
        getchar(); // consume newline

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
            printf("Thank you for using our Library Management System!\n");
            return 0;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Load books from binary file
int loadBooks(Book books[], int max)
{
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL)
    {
        return 0;
    }
    int count = 0;
    while (count < max && fread(&books[count], sizeof(Book), 1, file) == 1)
    {
        count++;
    }
    fclose(file);
    return count;
}

// Save books to binary file
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

// Find book by ID
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

// Convert string to lowercase for case-insensitive search
void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Validate Book ID (1000 to 999999)
int isValidBookID(int id)
{
    return (id >= 1000 && id <= 999999);
}

// ====================== ADD BOOK ======================
void addBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);

    Book newBook;

    printf("\nEnter Book ID (1000 - 999999): ");
    scanf("%d", &newBook.bookID);
    getchar();

    if (!isValidBookID(newBook.bookID))
    {
        printf("Error: Book ID must be between 1000 and 999999!\n");
        return;
    }

    if (findBookByID(books, count, newBook.bookID) != -1)
    {
        printf("Error: Book ID %d already exists!\n", newBook.bookID);
        return;
    }

    printf("Enter Title: ");
    fgets(newBook.title, MAX_TITLE, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;

    printf("Enter Author: ");
    fgets(newBook.author, MAX_AUTHOR, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    printf("Enter Quantity: ");
    scanf("%d", &newBook.quantity);
    newBook.issued = 0;

    if (newBook.quantity <= 0)
    {
        printf("Error: Quantity must be positive!\n");
        return;
    }

    books[count] = newBook;
    saveBooks(books, count + 1);
    printf("Book added successfully! (ID: %d)\n", newBook.bookID);
}

// ====================== DISPLAY BOOKS ======================
void displayBooks()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    if (count == 0)
    {
        printf("No books in the library.\n");
        return;
    }

    printf("\n%-6s %-35s %-25s %-10s %-10s\n", "ID", "Title", "Author", "Quantity", "Issued");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-6d %-35s %-25s %-10d %-10d\n",
                books[i].bookID, books[i].title, books[i].author,
                books[i].quantity, books[i].issued);
    }
}

// ====================== SEARCH BOOK ======================
void searchBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    if (count == 0)
    {
        printf("No books available in the library.\n");
        return;
    }

    int choice, id, found = 0;
    char searchTerm[MAX_TITLE];
    char tempTitle[MAX_TITLE];

    printf("\nSearch by:\n");
    printf("1. Book ID\n");
    printf("2. Title (case-insensitive + partial/keyword)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1)
    {
        printf("Enter Book ID: ");
        scanf("%d", &id);
        getchar();

        int idx = findBookByID(books, count, id);
        if (idx != -1)
        {
            printf("\nBook Found:\n");
            printf("ID       : %d\n", books[idx].bookID);
            printf("Title    : %s\n", books[idx].title);
            printf("Author   : %s\n", books[idx].author);
            printf("Quantity : %d\n", books[idx].quantity);
            printf("Issued   : %d\n", books[idx].issued);
            printf("Available: %d\n", books[idx].quantity - books[idx].issued);
        }
        else
        {
            printf("Book with ID %d not found!\n", id);
        }
        return;
    }

    // Title search - improved
    printf("Enter any word or part of the title: ");
    fgets(searchTerm, MAX_TITLE, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    if (strlen(searchTerm) == 0)
    {
        printf("Search term cannot be empty!\n");
        return;
    }

    toLowerCase(searchTerm);

    printf("\nSearch Results for \"%s\":\n", searchTerm);
    printf("%-6s %-35s %-25s %-10s %-10s\n", "ID", "Title", "Author", "Qty", "Issued");
    printf("---------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        strcpy(tempTitle, books[i].title);
        toLowerCase(tempTitle);

        if (strstr(tempTitle, searchTerm) != NULL)
        {
            printf("%-6d %-35s %-25s %-10d %-10d\n",
                    books[i].bookID,
                    books[i].title,
                    books[i].author,
                    books[i].quantity,
                    books[i].issued);
            found++;
        }
    }

    if (found == 0)
    {
        printf("No books found matching your search.\n");
    }
    else
    {
        printf("\n%d book(s) found.\n", found);
    }
}

// ====================== UPDATE BOOK ======================
void updateBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    if (count == 0)
    {
        printf("No books to update.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    getchar();

    idx = findBookByID(books, count, id);
    if (idx == -1)
    {
        printf("Book with ID %d not found!\n", id);
        return;
    }

    // Update ID (optional)
    printf("Current ID: %d\n", books[idx].bookID);
    printf("Enter new Book ID (1000-999999) or 0 to keep: ");
    int newID;
    scanf("%d", &newID);
    getchar();

    if (newID != 0)
    {
        if (!isValidBookID(newID))
        {
            printf("Error: New Book ID must be between 1000 and 999999!\n");
            return;
        }
        if (newID != books[idx].bookID && findBookByID(books, count, newID) != -1)
        {
            printf("Error: Book ID %d already exists!\n", newID);
            return;
        }
        books[idx].bookID = newID;
    }

    // Update Title
    printf("Enter new Title (or press Enter to keep): ");
    char temp[MAX_TITLE];
    fgets(temp, MAX_TITLE, stdin);
    if (temp[0] != '\n')
    {
        temp[strcspn(temp, "\n")] = 0;
        strcpy(books[idx].title, temp);
    }

    // Update Author
    printf("Enter new Author (or press Enter to keep): ");
    fgets(temp, MAX_TITLE, stdin);
    if (temp[0] != '\n')
    {
        temp[strcspn(temp, "\n")] = 0;
        strcpy(books[idx].author, temp);
    }

    // Update Quantity
    printf("Enter new Quantity (or -1 to keep): ");
    int qty;
    scanf("%d", &qty);
    if (qty != -1)
    {
        if (qty < books[idx].issued)
        {
            printf("Error: Cannot set quantity less than currently issued books!\n");
            return;
        }
        books[idx].quantity = qty;
    }

    saveBooks(books, count);
    printf("Book updated successfully!\n");
}

// ====================== DELETE BOOK ======================
void deleteBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    if (count == 0)
    {
        printf("No books to delete.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    getchar();

    idx = findBookByID(books, count, id);
    if (idx == -1)
    {
        printf("Book with ID %d not found!\n", id);
        return;
    }

    // Shift remaining books left
    for (int i = idx; i < count - 1; i++)
    {
        books[i] = books[i + 1];
    }

    saveBooks(books, count - 1);
    printf("Book deleted successfully!\n");
}

// ====================== BORROW BOOK ======================
void borrowBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    if (count == 0)
    {
        printf("No books available.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to borrow: ");
    scanf("%d", &id);
    getchar();

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
    printf("Book borrowed successfully! Remaining copies: %d\n",
            books[idx].quantity - books[idx].issued);
}

// ====================== RETURN BOOK ======================
void returnBook()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    if (count == 0)
    {
        printf("No books in library.\n");
        return;
    }

    int id, idx;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);
    getchar();

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

// ====================== GENERATE REPORT ======================
void generateReport()
{
    Book books[MAX_BOOKS];
    int count = loadBooks(books, MAX_BOOKS);
    if (count == 0)
    {
        printf("No books in the library.\n");
        return;
    }

    int totalQuantity = 0, totalIssued = 0;

    printf("\n=== LIBRARY REPORT ===\n");
    printf("%-6s %-35s %-10s %-10s\n", "ID", "Title", "Available", "Issued");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        int available = books[i].quantity - books[i].issued;
        printf("%-6d %-35s %-10d %-10d\n",
                books[i].bookID, books[i].title, available, books[i].issued);

        totalQuantity += books[i].quantity;
        totalIssued += books[i].issued;
    }

    printf("\nTotal Books in Library : %d\n", totalQuantity);
    printf("Currently Available    : %d\n", totalQuantity - totalIssued);
    printf("Currently Borrowed     : %d\n", totalIssued);

    // List borrowed books
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