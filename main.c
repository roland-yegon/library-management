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
