#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 100
#define MAX_BORROWS 200
#define ID_LEN 10
#define TITLE_LEN 100
#define AUTHOR_LEN 100

#define BOOK_FILE "books.dat"
#define BORROW_FILE "borrows.dat"

/* ---------------- STRUCTS ---------------- */

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
    int returned;
} BorrowRecord;

/* ---------------- GLOBAL DATA ---------------- */

Book library[MAX_BOOKS];
BorrowRecord borrows[MAX_BORROWS];

int bookCount = 0;
int borrowCount = 0;

/* ---------------- INPUT HANDLING ---------------- */

void getInput(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

/* ---------------- FILE HANDLING ---------------- */

void loadData() {
    FILE *f1 = fopen(BOOK_FILE, "rb");
    FILE *f2 = fopen(BORROW_FILE, "rb");

    if (f1) {
        fread(&bookCount, sizeof(int), 1, f1);
        fread(library, sizeof(Book), bookCount, f1);
        fclose(f1);
    }

    if (f2) {
        fread(&borrowCount, sizeof(int), 1, f2);
        fread(borrows, sizeof(BorrowRecord), borrowCount, f2);
        fclose(f2);
    }
}

void saveData() {
    FILE *f1 = fopen(BOOK_FILE, "wb");
    FILE *f2 = fopen(BORROW_FILE, "wb");

    if (f1) {
        fwrite(&bookCount, sizeof(int), 1, f1);
        fwrite(library, sizeof(Book), bookCount, f1);
        fclose(f1);
    }

    if (f2) {
        fwrite(&borrowCount, sizeof(int), 1, f2);
        fwrite(borrows, sizeof(BorrowRecord), borrowCount, f2);
        fclose(f2);
    }
}

/* ---------------- UTILITIES ---------------- */

int findBook(char id[]) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(library[i].id, id) == 0)
            return i;
    }
    return -1;
}

void createBookId(char id[]) {
    sprintf(id, "B%03d", bookCount + 1);
}

void createBorrowId(char id[]) {
    sprintf(id, "R%03d", borrowCount + 1);
}

/* ---------------- CORE FEATURES ---------------- */

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library full\n");
        return;
    }

    Book b;
    createBookId(b.id);

    printf("Title: ");
    getInput(b.title, TITLE_LEN);

    printf("Author: ");
    getInput(b.author, AUTHOR_LEN);

    printf("Year: ");
    scanf("%d", &b.year);

    printf("Total copies: ");
    scanf("%d", &b.totalCopies);
    getchar();

    b.availableCopies = b.totalCopies;

    library[bookCount++] = b;

    printf("Book added (ID: %s)\n", b.id);
}

void viewBooks() {
    if (bookCount == 0) {
        printf("No books available\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        printf("\nID: %s\nTitle: %s\nAuthor: %s\nYear: %d\nAvailable: %d/%d\n",
                library[i].id,
                library[i].title,
                library[i].author,
                library[i].year,
                library[i].availableCopies,
                library[i].totalCopies);
    }
}

/* FIXED STATE LOGIC */
void updateBook() {
    char id[ID_LEN];
    int index;

    printf("Book ID: ");
    getInput(id, ID_LEN);

    index = findBook(id);

    if (index == -1) {
        printf("Not found\n");
        return;
    }

    printf("New title: ");
    getInput(library[index].title, TITLE_LEN);

    printf("New author: ");
    getInput(library[index].author, AUTHOR_LEN);

    printf("New year: ");
    scanf("%d", &library[index].year);

    printf("New total copies: ");
    int newTotal;
    scanf("%d", &newTotal);
    getchar();

    int borrowed = library[index].totalCopies - library[index].availableCopies;

    library[index].totalCopies = newTotal;
    library[index].availableCopies = newTotal - borrowed;

    if (library[index].availableCopies < 0)
        library[index].availableCopies = 0;

    printf("Updated successfully\n");
}

void searchBook() {
    char title[TITLE_LEN];
    int found = 0;

    printf("Search title: ");
    getInput(title, TITLE_LEN);

    for (int i = 0; i < bookCount; i++) {
        if (strstr(library[i].title, title)) {
            printf("ID:%s | %s | %s\n",
                    library[i].id,
                    library[i].title,
                    library[i].author);
            found = 1;
        }
    }

    if (!found)
        printf("No match found\n");
}

void borrowBook() {
    if (borrowCount >= MAX_BORROWS) {
        printf("Borrow storage full\n");
        return;
    }

    char id[ID_LEN];
    printf("Book ID: ");
    getInput(id, ID_LEN);

    int index = findBook(id);

    if (index == -1) {
        printf("Not found\n");
        return;
    }

    if (library[index].availableCopies <= 0) {
        printf("No copies available\n");
        return;
    }

    BorrowRecord b;
    createBorrowId(b.borrowId);

    strcpy(b.bookId, id);
    b.returned = 0;

    borrows[borrowCount++] = b;
    library[index].availableCopies--;

    printf("Borrowed. ID: %s\n", b.borrowId);
}

void returnBook() {
    char id[ID_LEN];

    printf("Borrow ID: ");
    getInput(id, ID_LEN);

    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].borrowId, id) == 0 && borrows[i].returned == 0) {

            int bookIndex = findBook(borrows[i].bookId);

            if (bookIndex != -1) {
                library[bookIndex].availableCopies++;
            }

            borrows[i].returned = 1;

            printf("Returned successfully\n");
            return;
        }
    }

    printf("Invalid borrow ID\n");
}

/* DELETE BOOK (SHIFT ARRAY SAFELY) */
void deleteBook() {
    char id[ID_LEN];
    printf("Book ID to delete: ");
    getInput(id, ID_LEN);

    int index = findBook(id);

    if (index == -1) {
        printf("Not found\n");
        return;
    }

    for (int i = index; i < bookCount - 1; i++) {
        library[i] = library[i + 1];
    }

    bookCount--;

    printf("Book deleted\n");
}

/* ---------------- MENU ---------------- */

void menu() {
    printf("\n===== LIBRARY SYSTEM =====\n");
    printf("1. Add Book\n");
    printf("2. View Books\n");
    printf("3. Update Book\n");
    printf("4. Search Book\n");
    printf("5. Borrow Book\n");
    printf("6. Return Book\n");
    printf("7. Delete Book\n");
    printf("8. Save & Exit\n");
    printf("Choice: ");
}

/* ---------------- MAIN ---------------- */

int main() {
    int choice;

    loadData();

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
            case 7: deleteBook(); break;
            case 8:
                saveData();
                printf("Saved. Exiting...\n");
                return 0;
            default:
                printf("Invalid option\n");
        }
    }
}