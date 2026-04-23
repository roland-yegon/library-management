#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 100
#define MAX_BORROWS 100
#define ID_LEN 10
#define TITLE_LEN 100
#define AUTHOR_LEN 100

typedef struct {
    char id[ID_LEN];
    char title[TITLE_LEN];
    char author[AUTHOR_LEN];
    int year;
    int totalCopies;
    int availableCopies;
} Book;

typedef struct {
    char borrowId[ID_LEN];
    char bookId[ID_LEN];
    int isReturned;
} BorrowRecord;

Book library[MAX_BOOKS];
BorrowRecord borrows[MAX_BORROWS];

int bookCount = 0;
int borrowCount = 0;

/* ---------------- UTILITIES ---------------- */

void readLine(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

int findBookIndex(char *id) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(library[i].id, id) == 0)
            return i;
    }
    return -1;
}

void generateBookId(char *id) {
    sprintf(id, "B%03d", bookCount + 1);
}

void generateBorrowId(char *id) {
    sprintf(id, "R%03d", borrowCount + 1);
}

/* ---------------- CORE FUNCTIONS ---------------- */

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library full!\n");
        return;
    }

    Book b;
    generateBookId(b.id);

    printf("Enter title: ");
    readLine(b.title, TITLE_LEN);

    printf("Enter author: ");
    readLine(b.author, AUTHOR_LEN);

    printf("Enter year: ");
    scanf("%d", &b.year);

    printf("Enter total copies: ");
    scanf("%d", &b.totalCopies);
    getchar();

    b.availableCopies = b.totalCopies;

    library[bookCount++] = b;

    printf("Book added successfully! ID: %s\n", b.id);
}

void viewBooks() {
    if (bookCount == 0) {
        printf("No books available.\n");
        return;
    }

    printf("\n--- Library Books ---\n");

    for (int i = 0; i < bookCount; i++) {
        printf("ID: %s | Title: %s | Author: %s | Year: %d | Available: %d/%d\n",
                library[i].id,
                library[i].title,
                library[i].author,
                library[i].year,
                library[i].availableCopies,
                library[i].totalCopies);
    }
}

void updateBook() {
    char id[ID_LEN];
    printf("Enter book ID to update: ");
    readLine(id, ID_LEN);

    int index = findBookIndex(id);

    if (index == -1) {
        printf("Book not found.\n");
        return;
    }

    printf("Enter new title: ");
    readLine(library[index].title, TITLE_LEN);

    printf("Enter new author: ");
    readLine(library[index].author, AUTHOR_LEN);

    printf("Enter new year: ");
    scanf("%d", &library[index].year);

    printf("Enter new total copies: ");
    scanf("%d", &library[index].totalCopies);
    getchar();

    library[index].availableCopies = library[index].totalCopies;

    printf("Book updated successfully.\n");
}

void searchBook() {
    char title[TITLE_LEN];
    printf("Enter title to search: ");
    readLine(title, TITLE_LEN);

    int found = 0;

    for (int i = 0; i < bookCount; i++) {
        if (strstr(library[i].title, title)) {
            printf("ID: %s | Title: %s | Author: %s\n",
                    library[i].id,
                    library[i].title,
                    library[i].author);
            found = 1;
        }
    }

    if (!found)
        printf("No matching books found.\n");
}

void borrowBook() {
    char id[ID_LEN];
    printf("Enter book ID to borrow: ");
    readLine(id, ID_LEN);

    int index = findBookIndex(id);

    if (index == -1) {
        printf("Book not found.\n");
        return;
    }

    if (library[index].availableCopies <= 0) {
        printf("No copies available.\n");
        return;
    }

    BorrowRecord br;
    generateBorrowId(br.borrowId);
    strcpy(br.bookId, id);
    br.isReturned = 0;

    borrows[borrowCount++] = br;
    library[index].availableCopies--;

    printf("Book borrowed successfully. Borrow ID: %s\n", br.borrowId);
}

void returnBook() {
    char borrowId[ID_LEN];
    printf("Enter borrow ID: ");
    readLine(borrowId, ID_LEN);

    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].borrowId, borrowId) == 0 && borrows[i].isReturned == 0) {

            int bookIndex = findBookIndex(borrows[i].bookId);

            if (bookIndex != -1) {
                library[bookIndex].availableCopies++;
            }

            borrows[i].isReturned = 1;

            printf("Book returned successfully.\n");
            return;
        }
    }

    printf("Invalid borrow ID or already returned.\n");
}

/* ---------------- MENU ---------------- */

void menu() {
    printf("\n====== LIBRARY SYSTEM ======\n");
    printf("1. Add Book\n");
    printf("2. View Books\n");
    printf("3. Update Book\n");
    printf("4. Search Book\n");
    printf("5. Borrow Book\n");
    printf("6. Return Book\n");
    printf("7. Exit\n");
    printf("Choose option: ");
}

/* ---------------- MAIN ---------------- */

int main() {
    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: updateBook(); break;
            case 4: searchBook(); break;
            case 5: borrowBook(); break;
            case 6: returnBook(); break;
            case 7:
                printf("Exiting system...\n");
                exit(0);
            default:
                printf("Invalid option.\n");
        }
    }

    return 0;
}