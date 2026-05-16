// Simple Banking System with Authentication
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Account {
    int id;
    char user[20];
    char pass[20];
    char first[20];
    char last[20];
    double balance;
};

void menu();
void registerUser(FILE *fp);
int login(FILE *fp, struct Account *acc);
void deposit(FILE *fp, struct Account acc);
void withdraw(FILE *fp, struct Account acc);
void viewAccount(struct Account acc);
void updatePassword(FILE *fp, struct Account acc);

int main() {
    FILE *fp;
    int choice;
    struct Account current;

    fp = fopen("bank.dat", "rb+");
    if (!fp) fp = fopen("bank.dat", "wb+");

    do {
        menu();
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                registerUser(fp);
                break;

            case 2:
                if(login(fp, &current)) {
                    int opt;
                    do {
                        printf("\n--- USER PANEL ---\n");
                        printf("1.View Account\n2.Deposit\n3.Withdraw\n4.Change Password\n5.Logout\nChoice: ");
                        scanf("%d", &opt);

                        switch(opt) {
                            case 1: viewAccount(current); break;
                            case 2: deposit(fp, current); break;
                            case 3: withdraw(fp, current); break;
                            case 4: updatePassword(fp, current); break;
                        }

                        rewind(fp);
                        while(fread(&current,sizeof(struct Account),1,fp))
                            if(current.id == current.id) break;

                    } while(opt != 5);
                }
                break;

            case 3:
                printf("Program Closed.\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while(choice != 3);

    fclose(fp);
    return 0;
}

void menu() {
    printf("\n===== BANK SYSTEM =====\n");
    printf("1.Register\n");
    printf("2.Login\n");
    printf("3.Exit\n");
    printf("Enter Choice: ");
}

void registerUser(FILE *fp) {
    struct Account acc;
    int exists = 0;

    printf("Enter ID: ");
    scanf("%d", &acc.id);

    rewind(fp);
    while(fread(&acc, sizeof(struct Account), 1, fp)) {
        if(acc.id == acc.id) exists = 1;
    }

    if(exists) {
        printf("Account already exists!\n");
        return;
    }

    printf("Username: ");
    scanf("%s", acc.user);

    printf("Password: ");
    scanf("%s", acc.pass);

    printf("First Name: ");
    scanf("%s", acc.first);

    printf("Last Name: ");
    scanf("%s", acc.last);

    printf("Initial Balance: ");
    scanf("%lf", &acc.balance);

    fseek(fp, 0, SEEK_END);
    fwrite(&acc, sizeof(struct Account), 1, fp);

    printf("Account Created Successfully!\n");
}

int login(FILE *fp, struct Account *acc) {
    char u[20], p[20];
    struct Account temp;

    printf("Username: ");
    scanf("%s", u);

    printf("Password: ");
    scanf("%s", p);

    rewind(fp);

    while(fread(&temp, sizeof(struct Account), 1, fp)) {
        if(strcmp(temp.user, u) == 0 &&
           strcmp(temp.pass, p) == 0) {
            *acc = temp;
            printf("Login Successful!\n");
            return 1;
        }
    }

    printf("Invalid Login!\n");
    return 0;
}

void viewAccount(struct Account acc) {
    printf("\n--- ACCOUNT DETAILS ---\n");
    printf("ID: %d\n", acc.id);
    printf("Name: %s %s\n", acc.first, acc.last);
    printf("Username: %s\n", acc.user);
    printf("Balance: %.2lf\n", acc.balance);
}

void deposit(FILE *fp, struct Account acc) {
    double amt;
    struct Account temp;

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amt);

    rewind(fp);

    while(fread(&temp,sizeof(struct Account),1,fp)) {
        if(temp.id == acc.id) {
            temp.balance += amt;
            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&temp,sizeof(struct Account),1,fp);
            printf("Deposit Successful!\n");
            break;
        }
    }
}

void withdraw(FILE *fp, struct Account acc) {
    double amt;
    struct Account temp;

    printf("Enter Withdraw Amount: ");
    scanf("%lf", &amt);

    rewind(fp);

    while(fread(&temp,sizeof(struct Account),1,fp)) {
        if(temp.id == acc.id) {

            if(temp.balance < amt) {
                printf("Insufficient Balance!\n");
                return;
            }

            temp.balance -= amt;

            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&temp,sizeof(struct Account),1,fp);

            printf("Withdrawal Successful!\n");
            break;
        }
    }
}

void updatePassword(FILE *fp, struct Account acc) {
    char newPass[20];
    struct Account temp;

    printf("Enter New Password: ");
    scanf("%s", newPass);

    rewind(fp);

    while(fread(&temp,sizeof(struct Account),1,fp)) {
        if(temp.id == acc.id) {
            strcpy(temp.pass, newPass);

            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&temp,sizeof(struct Account),1,fp);

            printf("Password Updated!\n");
            break;
        }
    }
}