// A program for a library management system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BOOKS 100

typedef struct
{
    char title[50];
    char author[50];
    int year;
} Book;

Book library[MAX_BOOKS];
int bookCount = 0;

void addBook()
{
    if (bookCount < MAX_BOOKS)
    {
        printf("Enter book title: ");
        scanf("%s", library[bookCount].title);
        printf("Enter book author: ");
        scanf("%s", library[bookCount].author);
        printf("Enter publication year: ");
        scanf("%d", &library[bookCount].year);
        bookCount++;
        printf("Book added successfully!\n");
    }
    else
    {
        printf("Library is full. Cannot add more books.\n");
    }
}

void displayBooks()
{
    if (bookCount == 0)
    {
        printf("No books in the library.\n");
    }
    else
    {
        printf("Books in the library:\n");
        for (int i = 0; i < bookCount; i++)
        {
            printf("%d. %s by %s (%d)\n", i + 1, library[i].title, library[i].author, library[i].year);
        }
    }
}

int main()
{
    int choice;
    do
    {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}


