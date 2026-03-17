/*
 * ============================================================
 *  LIBRARY MANAGEMENT SYSTEM
 *  Technical University of Mombasa
 *  CSE 4101 - Introduction to Programming
 *  Module: Library (4.6)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS    100
#define MAX_BORROWS  200
#define TITLE_LEN     80
#define AUTHOR_LEN    60
#define ID_LEN        10
#define NAME_LEN      60

/* ── Data Structures ─────────────────────────────────────── */

typedef struct {
    char   id[ID_LEN];
    char   title[TITLE_LEN];
    char   author[AUTHOR_LEN];
    int    year;
    int    totalCopies;
    int    availableCopies;
} Book;

typedef struct {
    char borrowId[ID_LEN];
    char bookId[ID_LEN];
    char studentName[NAME_LEN];
    char studentId[ID_LEN];
    char borrowDate[20];
    char returnDate[20];
    int  returned;
} BorrowRecord;

/* ── Global State ────────────────────────────────────────── */

Book         library[MAX_BOOKS];
BorrowRecord borrows[MAX_BORROWS];
int          bookCount   = 0;
int          borrowCount = 0;

/* ── Utility Helpers ─────────────────────────────────────── */

void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fflush(stdout);
    int c;
    while ((c = getchar()) == '\n' || c == '\r');
    ungetc(c, stdin);
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
    }
}

int findBook(const char *id) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(library[i].id, id) == 0) return i;
    }
    return -1;
}

void generateBookId(char *out)   { sprintf(out, "LIB%03d", bookCount + 1); }
void generateBorrowId(char *out) { sprintf(out, "BRW%03d", borrowCount + 1); }
void printLine() { printf("--------------------------------------------------\n"); }
