#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[100];
    char author[100];
    int quantity;
};

struct Member {
    int id;
    char name[100];
};

struct Issue {
    int bookID;
    int memberID;
};

void addBook() {
    FILE *fp = fopen("library.dat", "ab");
    struct Book b;

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar();
    printf("Enter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    strtok(b.title, "\n");
    printf("Enter Author: ");
    fgets(b.author, sizeof(b.author), stdin);
    strtok(b.author, "\n");
    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("✅ Book added successfully!\n");
}

void viewBooks() {
    FILE *fp = fopen("library.dat", "rb");
    struct Book b;
    printf("\n--- Books Available ---\n");
    while (fread(&b, sizeof(b), 1, fp)) {
        printf("ID: %d | Title: %s | Author: %s | Quantity: %d\n", b.id, b.title, b.author, b.quantity);
    }
    fclose(fp);
}

void registerMember() {
    FILE *fp = fopen("members.dat", "ab");
    struct Member m;

    printf("Enter Member ID: ");
    scanf("%d", &m.id);
    getchar();
    printf("Enter Member Name: ");
    fgets(m.name, sizeof(m.name), stdin);
    strtok(m.name, "\n");

    fwrite(&m, sizeof(m), 1, fp);
    fclose(fp);
    printf("✅ Member registered!\n");
}

int isBookAvailable(int bookID) {
    FILE *fp = fopen("library.dat", "rb+");
    struct Book b;
    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == bookID && b.quantity > 0) {
            fseek(fp, -sizeof(b), SEEK_CUR);
            b.quantity--;
            fwrite(&b, sizeof(b), 1, fp);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void returnBookQuantity(int bookID) {
    FILE *fp = fopen("library.dat", "rb+");
    struct Book b;
    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == bookID) {
            fseek(fp, -sizeof(b), SEEK_CUR);
            b.quantity++;
            fwrite(&b, sizeof(b), 1, fp);
            break;
        }
    }
    fclose(fp);
}

void issueBook() {
    struct Issue i;
    printf("Enter Book ID: ");
    scanf("%d", &i.bookID);
    printf("Enter Member ID: ");
    scanf("%d", &i.memberID);

    if (!isBookAvailable(i.bookID)) {
        printf("❌ Book not available!\n");
        return;
    }

    FILE *fp = fopen("issue.dat", "ab");
    fwrite(&i, sizeof(i), 1, fp);
    fclose(fp);
    printf("✅ Book issued successfully!\n");
}

void returnBook() {
    int bookID, memberID, found = 0;
    printf("Enter Book ID to return: ");
    scanf("%d", &bookID);
    printf("Enter Member ID: ");
    scanf("%d", &memberID);

    FILE *fp = fopen("issue.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Issue i;

    while (fread(&i, sizeof(i), 1, fp)) {
        if (i.bookID == bookID && i.memberID == memberID) {
            found = 1;
            returnBookQuantity(bookID);
        } else {
            fwrite(&i, sizeof(i), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);
    remove("issue.dat");
    rename("temp.dat", "issue.dat");

    if (found)
        printf("✅ Book returned successfully!\n");
    else
        printf("❌ Record not found.\n");
}

int login() {
    char user[20], pass[20];
    printf("Enter Username: ");
    scanf("%s", user);
    printf("Enter Password: ");
    scanf("%s", pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0) {
        return 1;
    }
    return 0;
}

int main() {
    int choice;

    printf("=== LIBRARY LOGIN ===\n");
    if (!login()) {
        printf("❌ Invalid credentials!\n");
        return 1;
    }

    while (1) {
        printf("\n===== Library Menu =====\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Register Member\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: registerMember(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 0: exit(0);
            default: printf("❌ Invalid choice\n");
        }
    }

    return 0;
}
