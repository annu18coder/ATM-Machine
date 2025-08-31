#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>


struct atmmachine{
    long int acc_no;
    int pin;
    char accholder_name[100];
    long int balance;
    char acc_type[100];
};


int login(struct atmmachine atm[], int total);
void check_balance(struct atmmachine atm[], int index);
int deposit_money(struct atmmachine atm[], int index);
int withdraw_money(struct atmmachine atm[], int index);
void change_pin(struct atmmachine atm[], int index);
void mini_statement(struct atmmachine atm[], int index);

void recTransaction(int acc_no, char type[], int amount, int balance){
    char filename[20];
    sprintf(filename, "%d.txt", acc_no);    // account wise file banegi

    FILE *fp = fopen(filename, "a");    // baar baar change hoga islie append mode
    if(fp == NULL){
        printf("Error opening file!\n");
        return;
    }
    // live date time lena

    time_t now;
    time(&now);
    char *dt = ctime(&now);
    dt[strlen(dt)-1] = '\0';    //new line remove krne ke lie
    fprintf(fp, "%s | %s | Amount: %d | Balance After: %d\n", dt, type, amount, balance);
    // dt means char type fixed date, type means deposit hai ya withdraw type ka
    fclose(fp);
}


int main(){
    int choice;
    int i;
    int n;

    struct atmmachine atm[50];

    printf("===================================================================");
    printf("\n\t\t\tWELCOME TO ATM SYSTEM");
    printf("\n===================================================================");

    printf("\nEnter the number of accounts you want to create: ");
    scanf("%d", &n);
    for(i=0;i<n;i++){
        printf("\nEnter Account Number %d: ",i+1);
        scanf("%ld", &atm[i].acc_no);
    
        printf("\nEnter PIN (4-digit): ");
        scanf("%d", &atm[i].pin);
        getchar();      // caused error in name fiels for clearing buffer
    
        printf("\nAccount Holder Name: ");
        fgets(atm[i].accholder_name, sizeof(atm[i].accholder_name), stdin);
        atm[i].accholder_name[strcspn(atm[i].accholder_name, "\n")] = 0;
        // getchar();
    
        printf("\nEnter Account Type: ");
        fgets(atm[i].acc_type, sizeof(atm[i].acc_type), stdin);
        atm[i].acc_type[strcspn(atm[i].acc_type, "\n")] = 0;
    
        printf("\nBalance: ");
        scanf("%ld", &atm[i].balance);    
    }

    int index = login(atm, n);      // login check
    if(index == -1){
        return 0;
    }

    while(1){
        printf("\n1. Check Balance: ");
        printf("\n2. Withdraw Money: ");
        printf("\n3. Deposit Money: ");
        printf("\n4. Change PIN: ");
        printf("\n5. Generate Mini statement: ");
        printf("\n6. Exit: ");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            check_balance(atm, index);
            break;
        case 2:
            withdraw_money(atm, index);
            break;

        case 3: 
            deposit_money(atm, index);
            break;

        case 4:
            change_pin(atm, index);
            break;

        case 5:
            mini_statement(atm, index);

        case 6:
            printf("Thankyou for choosing our ATM!");
            exit(0);
        }

    }
    return 0;
}

int login(struct atmmachine atm[], int total){      // isme total mtlb n hai kitne users ka data store karna hai
    int ent_pin, i;
    long int  acc_no;
    int attempts=0;
    
    while(attempts < 3){
        printf("\nEnter Account number: ");
        scanf("%ld", &acc_no);
        printf("\nEnter pin no: ");
        scanf("%d", &ent_pin);
        while(getchar()!='\n');

        for(i=0;i<total;i++){
            if((atm[i].pin == ent_pin)&&(atm[i].acc_no == acc_no )){
                printf("\nLOGIN SUCCESFULLY! WELCOME %s!!", atm[i].accholder_name);
                return i;       // means return index of loggend in account vo account ab accessible hai
            }
        }
        attempts++;
        printf("\nInvalid Account or PIN!! Attempts Left: %d", 3-attempts);

    }
    printf("\nAccount blocked due to multiple failed attempts.");
    return -1;      // means accout index access failed
}

void check_balance(struct atmmachine atm[], int index){     // easily use karne ke liye hamne use kia hai argument
    printf("\nYour current balance is: %ld",atm[index].balance);
}

int withdraw_money(struct atmmachine atm[], int index){    // hamne argument me sturcture and int index islie lia hai taki uski value ko ham update karte rahe professional way me
    long int withdraw;
    printf("\nEnter amount to withdraw: ");
    scanf("%ld", &withdraw);

    if(withdraw > atm[index].balance){
        printf("\nInsufficient Balance !");
    }
    else{
        atm[index].balance -= withdraw;
        printf("\nYour current balance is : %ld", atm[index].balance);
    }
       
    recTransaction(atm[index].acc_no, "Withdraw", withdraw, atm[index].balance);
    return atm[index].balance;                  // hamne balance ko retrun kar uski value ko update kia hai
}

int deposit_money(struct atmmachine atm[], int index){
    int deposit;
    printf("\nEnter the amount to deposit: ");
    scanf("%ld", &deposit);

    if(deposit<=0){
        printf("Invalid amount!");
    }
    else{
        atm[index].balance += deposit;
        printf("\nYour current balance is: %ld", atm[index].balance);
    }

    recTransaction(atm[index].acc_no, "Deposit", deposit, atm[index].balance);
    return atm[index].balance;
}

void change_pin(struct atmmachine atm[], int index){
    int new_pin;
    printf("\nEnter your old PIN: ");
    scanf("%d", &atm[index].pin);
    new_pin == atm[index].pin;
    printf("\nEnter your CURRENT PIN: ");
    scanf("%d", &new_pin);
    printf("\nYour Updated PIN is: %d", new_pin);
}

void mini_statement(struct atmmachine atm[], int index){
    char filename[20];
    sprintf(filename, "%d.txt", atm[index].acc_no);

    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        printf("No transactions found !!");
    }
    else{
        printf("\n =============== MINI STATEMENT for %s =============== \n", atm[index].accholder_name);
        char line[200];
        while(fgets(line, sizeof(line), fp)){
            printf("%s", line);
        }
    }
    fclose(fp);

    printf("\n ====================================================== \n");
}
