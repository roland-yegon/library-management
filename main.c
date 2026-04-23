#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_BORROWS 200
#define TITLE_LEN 80
#define AUTHOR_LEN 60
#define ID_LEN 10
#define NAME_LEN 60

/* ── Data Structures ─────────────────────────────────────── */

typedef struct
{
    char id[ID_LEN];
    char title[TITLE_LEN];
    char author[AUTHOR_LEN];
    int year;
    int totalCopies;
    int availableCopies;
} Book;

typedef struct
{
    char borrowId[ID_LEN];
    char bookId[ID_LEN];
    char studentName[NAME_LEN];
    char studentId[ID_LEN];
    char borrowDate[20];
    char returnDate[20];
    int returned;
} BorrowRecord;

/* ── Global State ────────────────────────────────────────── */

Book library[MAX_BOOKS];
BorrowRecord borrows[MAX_BORROWS];
int bookCount = 0;
int borrowCount = 0;

/* ── Utility Helpers ─────────────────────────────────────── */

void readLine(const char *prompt, char *buffer, int size)
{
    printf("%s", prompt);
    fflush(stdout);
    int c;
    while ((c = getchar()) == '\n' || c == '\r')
        ;
    ungetc(c, stdin);
    if (fgets(buffer, size, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\r\n")] = '\0';
    }
}

int findBook(const char *id)
{
    for (int i = 0; i < bookCount; i++)
    {
        if (strcmp(library[i].id, id) == 0)
            return i;
    }
    return -1;
}

void generateBookId(char *out) { sprintf(out, "LIB%03d", bookCount + 1); }
void generateBorrowId(char *out) { sprintf(out, "BRW%03d", borrowCount + 1); }
void printLine() { printf("--------------------------------------------------\n"); }

/* ── 1. Add Book ─────────────────────────────────────────── */

void addBook()
{
    if (bookCount >= MAX_BOOKS)
    {
        printf("Library catalogue is full.\n");
        return;
    }
    Book b;
    generateBookId(b.id);
    readLine("Enter book title  : ", b.title, TITLE_LEN);
    readLine("Enter author name : ", b.author, AUTHOR_LEN);
    printf("Enter publication year : ");
    scanf("%d", &b.year);
    printf("Enter number of copies : ");
    scanf("%d", &b.totalCopies);
    b.availableCopies = b.totalCopies;
    library[bookCount++] = b;
    printf("\nBook added successfully! Assigned ID: %s\n", b.id);
}

/* ── 2. Update Book ──────────────────────────────────────── */

void updateBook()
{
    char id[ID_LEN];
    readLine("Enter Book ID to update: ", id, ID_LEN);
    int idx = findBook(id);
    if (idx == -1)
    {
        printf("Book ID '%s' not found.\n", id);
        return;
    }

    printf("\nCurrent: [%s] %s by %s (%d) | Copies: %d/%d\n",
            library[idx].id, library[idx].title, library[idx].author,
            library[idx].year, library[idx].availableCopies, library[idx].totalCopies);
    printf("Enter new values (press Enter to keep current):\n");

    char buf[TITLE_LEN];
    readLine("New title  : ", buf, TITLE_LEN);
    if (strlen(buf) > 0)
        strcpy(library[idx].title, buf);
    readLine("New author : ", buf, AUTHOR_LEN);
    if (strlen(buf) > 0)
        strcpy(library[idx].author, buf);
    printf("New year (0 to keep %d): ", library[idx].year);
    int yr;
    scanf("%d", &yr);
    if (yr != 0)
        library[idx].year = yr;
    printf("New total copies (0 to keep %d): ", library[idx].totalCopies);
    int cp;
    scanf("%d", &cp);
    if (cp != 0)
    {
        int diff = cp - library[idx].totalCopies;
        library[idx].totalCopies = cp;
        library[idx].availableCopies += diff;
        if (library[idx].availableCopies < 0)
            library[idx].availableCopies = 0;
    }
    printf("Book updated successfully.\n");
}

/* ── 3. Borrow Book ──────────────────────────────────────── */

void borrowBook()
{
    if (borrowCount >= MAX_BORROWS)
    {
        printf("Borrow records full.\n");
        return;
    }
    char id[ID_LEN];
    readLine("Enter Book ID to borrow: ", id, ID_LEN);
    int idx = findBook(id);
    if (idx == -1)
    {
        printf("Book ID '%s' not found.\n", id);
        return;
    }
    if (library[idx].availableCopies <= 0)
    {
        printf("No copies of '%s' currently available.\n", library[idx].title);
        return;
    }
    BorrowRecord rec;
    generateBorrowId(rec.borrowId);
    strcpy(rec.bookId, id);
    readLine("Student Name : ", rec.studentName, NAME_LEN);
    readLine("Student ID   : ", rec.studentId, ID_LEN);
    readLine("Borrow Date (DD/MM/YYYY): ", rec.borrowDate, 20);
    strcpy(rec.returnDate, "");
    rec.returned = 0;
    library[idx].availableCopies--;
    borrows[borrowCount++] = rec;
    printf("\nBook '%s' borrowed! Record ID: %s | Copies left: %d\n",
            library[idx].title, rec.borrowId, library[idx].availableCopies);
}

/* ── 4. Return Book ──────────────────────────────────────── */

void returnBook()
{
    char bid[ID_LEN];
    readLine("Enter Borrow Record ID: ", bid, ID_LEN);
    int found = -1;
    for (int i = 0; i < borrowCount; i++)
    {
        if (strcmp(borrows[i].borrowId, bid) == 0 && borrows[i].returned == 0)
        {
            found = i;
            break;
        }
    }
    if (found == -1)
    {
        printf("No active record found for ID '%s'.\n", bid);
        return;
    }
    readLine("Return Date (DD/MM/YYYY): ", borrows[found].returnDate, 20);
    borrows[found].returned = 1;
    int idx = findBook(borrows[found].bookId);
    if (idx != -1)
        library[idx].availableCopies++;
    printf("\nBook returned successfully. Thank you, %s!\n", borrows[found].studentName);
}

/* ── 5. Display All Books ────────────────────────────────── */

void displayAllBooks()
{
    printLine();
    printf("  ALL BOOKS IN CATALOGUE\n");
    printLine();
    if (bookCount == 0)
    {
        printf("  No books in the catalogue.\n");
    }
    else
    {
        for (int i = 0; i < bookCount; i++)
        {
            printf("  [%s] %s\n       Author : %s\n       Year   : %d\n       Copies : %d available / %d total\n\n",
                    library[i].id, library[i].title, library[i].author,
                    library[i].year, library[i].availableCopies, library[i].totalCopies);
        }
    }
    printLine();
    printf("  Total titles: %d\n", bookCount);
    printLine();
}

/* ── 6. Report: Available Books ──────────────────────────── */

void reportAvailableBooks()
{
    printLine();
    printf("  REPORT: AVAILABLE BOOKS\n");
    printLine();
    int count = 0;
    for (int i = 0; i < bookCount; i++)
    {
        if (library[i].availableCopies > 0)
        {
            printf("  %-8s | %-35s | %-20s | %d | Copies: %d/%d\n",
                    library[i].id, library[i].title, library[i].author,
                    library[i].year, library[i].availableCopies, library[i].totalCopies);
            count++;
        }
    }
    if (count == 0)
        printf("  No books currently available.\n");
    printLine();
    printf("  Total available titles: %d\n", count);
    printLine();
}

/* ── 7. Report: Borrowed Books ───────────────────────────── */

void reportBorrowedBooks()
{
    printLine();
    printf("  REPORT: BORROWED BOOKS\n");
    printLine();
    int count = 0;
    for (int i = 0; i < borrowCount; i++)
    {
        if (borrows[i].returned == 0)
        {
            int idx = findBook(borrows[i].bookId);
            char *title = (idx != -1) ? library[idx].title : "(unknown)";
            printf("  %-8s | %-25s | %-12s | %-25s | %s\n",
                    borrows[i].borrowId, borrows[i].studentName,
                    borrows[i].studentId, title, borrows[i].borrowDate);
            count++;
        }
    }
    if (count == 0)
        printf("  No books currently on loan.\n");
    printLine();
    printf("  Total on loan: %d\n", count);
    printLine();
}

/* ── Main Menu ───────────────────────────────────────────── */

int main()
{
    int choice;
    do
    {
        printf("\n==================================================\n");
        printf("   LIBRARY MANAGEMENT SYSTEM - TU MOMBASA\n");
        printf("==================================================\n");
        printf("  1. Add Book\n");
        printf("  2. Update Book\n");
        printf("  3. Borrow Book\n");
        printf("  4. Return Book\n");
        printf("  5. Display All Books\n");
        printf("  6. Report: Available Books\n");
        printf("  7. Report: Borrowed Books\n");
        printf("  0. Exit\n");
        printf("--------------------------------------------------\n");
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            updateBook();
            break;
        case 3:
            borrowBook();
            break;
        case 4:
            returnBook();
            break;
        case 5:
            displayAllBooks();
            break;
        case 6:
            reportAvailableBooks();
            break;
        case 7:
            reportBorrowedBooks();
            break;
        case 0:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
