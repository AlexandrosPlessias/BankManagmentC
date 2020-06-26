#include <stdio.h>       //We need the library Standard Input and Output (for the use of basic functions such as printf & scanf).
#include <stdlib.h>     //We need on the Standard General Utilities Library (for use malloc, free and filefunctions).
#include <string.h>     //We need this library to manage the strings( copy, compare....)
#include <time.h>       //We need the library time.h to generate the current time


typedef struct client   //Main struct were all the data of each client are saved on this type
{
char firstName[50];
char lastName[50];
char birthday[13];
char accNum[16];
char telNum[15];
char email[50];
char entryDate[14];
char entryTime[6];
char bankOp[15];
struct client *next;
} clientData;

clientData *head = NULL; // global head pointer
clientData *tail = NULL; // global head pointer

void newClient(char str[], int len);    //function that read from a new Clients the data and call the necesary functions to do checks
int isValidName(char name[]);       //check if the name is valid
int isValidDate(char date[]);       //check if the date is valid
int isValidAccountNum(char num[]);  //check if the account number is valid
int isValidPhone(char num[]);   //check if the phone is valid
int isValidMail(char mail[]);   //check if the mail is valid
int isValidBankOp(char bankop[]);   //check if the bank operation is valid
void insertClient(char fName[], char sName[],char date[], char accNum[], char phone[], char mail[],char cdate[], char ctime[], char bankop[]); //save client's data after all input checks
void serveClient();     //remove client from the head of the queue
void removeClient(char str[], int len);     //removes a client form the queue
void searchClient(char str[], int len);     //search clients with a keyword
void printAllClients();     //print clients from the old to the new
void printAllClientsRev(clientData *head);      //print the clients from the new to the old
void printOrder(char str[], int len);       //print the clients that they have the same bank operation
void saveToFile(char str[], int len);       //save the data in a file (backup)
void loadFromFile(char str[], int len);     //load the data from a file
void TextArt();     //welcome text
void Menu();       //menu that tells the user what to type
void free_list();

/*--------------------Start of main--------------------------------------- */
int main(){

    char str[500];  //variables declaration
    int len;
    TextArt();  //call of the function TextArt
    Menu();     //call of the function Menu

    while(1){       //endless loop, the loop ends only when user give the command "exit"
        printf("\n$> ");
        gets(str);      //read the string that user gives as command
        len=strlen(str);    //for most of the commands we need the lenghth of the string

        if(!strncmp("newClient ",str,10))   //depending from users input to take the right command, it enters on a loop and call the right function.
            newClient(str,len);

        if(!strcmp("serveClient",str))
            serveClient();

        if(!strncmp("removeClient ",str,13))
            removeClient(str, len);

        if(!strncmp("searchClient ",str,13))
            searchClient(str, len);

        if(!strncmp("backup ",str,7))
            saveToFile(str,len);

        if(!strcmp("printAll",str))
            printAllClients();

        if(!strcmp("printAllRev",str)){
            printf( "\n   _____ _ _            _         _____        _ ");
            printf( "\n  / ____| (_)          | |       |  __ \\      | |       ");
            printf( "\n | |    | |_  ___ _ __ | |_ ___  | |  | | __ _| |_ __ _ ");
            printf( "\n | |    | | |/ _ \\ '_ \\| __/ __| | |  | |/ _` | __/ _` |");
            printf( "\n | |____| | |  __/ | | | |_\\__ \\ | |__| | (_| | || (_| |");
            printf( "\n  \\_____|_|_|\\___|_| |_|\\__|___/ |_____/ \\__,_|\\__\\__,_|");
            printf("\n|%-16s|%-18s|%-13s|%-16s|%-15s|%-18s|%-10s|%-10s|%-15s|","First Name","Surnmame","Birthday","Account Num.","Telephone Num.","e-mail","Entry Date", "Entry Time","Bank Operetion");
            printf("\n---------------------------------------------------------------------------------------------------------------------------------------------\n");
            printAllClientsRev(head);
        }

        if(!strncmp("print ",str,6))
            printOrder(str,len);

        if(!strncmp("restore ",str,8))
            loadFromFile(str,len);

        if(!strcmp("exit",str)){
            free_list();
        return 0;
        }
    }
return 0;
}

/*----------------------Functions-----------------------------------------*/
void newClient(char str[], int len){
    char fName[50], sName[50],date[50], accNum[50], phone[50], mail[50],cdate[14],ctime[7], bankop[50];
    int pos[6]={0,0,0,0,0,0};
    int i,j=0,flag=0;

    for(i=0;i<len;i++){     //save comma's potitions
        if (str[i]==','){
            pos[j]=i;
            j+=1;
        }
    }

    if(j!=6){   //if commas are wrong it returns
        printf("\nPlease enter the necessary data correctly!");
        return;
    }

    memcpy(fName,&str[10],pos[0]-10);       //isValid functions call, thos functions return integer 0if something is wrong or 1 for correct input
    fName[pos[0]-10]='\0';
    flag+=isValidName(fName);

    memcpy(sName,&str[pos[0]+2],pos[1]-pos[0]-2);
    sName[pos[1]-pos[0]-2]='\0';
    flag+=isValidName(sName);

    memcpy(date,&str[pos[1]+2],pos[2]-pos[1]-2);
    date[pos[2]-pos[1]-2]='\0';
    flag+=isValidDate(date);

    memcpy(accNum,&str[pos[2]+2],pos[3]-pos[2]-2);
    accNum[pos[3]-pos[2]-2]='\0';
    flag+=isValidAccountNum(accNum);

    memcpy(phone,&str[pos[3]+2],pos[4]-pos[3]-2);
    phone[pos[4]-pos[3]-2]='\0';
    flag+=isValidPhone(phone);

    memcpy(mail,&str[pos[4]+2],pos[5]-pos[4]-2);
    mail[pos[5]-pos[4]-2]='\0';
    flag+=isValidMail(mail);

    memcpy(bankop,&str[pos[5]+2],len-pos[5]-2);
    bankop[len-pos[5]-2]='\0';
    flag+=isValidBankOp(bankop);

    if (flag!=7){       //if flag=7 the loop ends means all inputs are correct
        printf("\nPlease enter the necessary data correctly!");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);     //struct from time.h library, generate the current time and date

    cdate[0]=((t->tm_mday)/10)+'0';     //convert from integer to string
    cdate[1]=((t->tm_mday)%10)+'0';
    cdate[2]=cdate[5]='/';
    cdate[3]=((t->tm_mon+1)/10)+'0';
    cdate[4]=((t->tm_mon+1)%10)+'0';

    cdate[6]=((t->tm_year+1900)/1000)+'0';
    cdate[7]=(((t->tm_year+1900)%1000)/100)+'0';
    cdate[8]=((((t->tm_year+1900)%1000)%100)/10)+'0';
    cdate[9]=((((t->tm_year+1900)%1000)%100)%10)+'0';
    cdate[10]='\0';

    ctime[0]=((t->tm_hour)/10)+'0';
    ctime[1]=((t->tm_hour)%10)+'0';
    ctime[2]=':';
    ctime[3]=((t->tm_min)/10)+'0';
    ctime[4]=((t->tm_min)%10)+'0';
    ctime[5]='\0';

    insertClient(fName,sName,date,accNum,phone,mail,cdate,ctime,bankop);    //inserClient function

    return;
}

int isValidName(char name[]){   //function that checks if 1st letter is upper and the others are lower
    int i,len;
    len=strlen(name);
    if((name[0]<'A') || (name[0]>'Z'))
        return 0;
    for(i=1;i<len-1;i++)
        if ((name[i]<'a') || (name[i]>'z'))
            return 0;
    return 1;
}

int isValidDate(char date[]){   //function that check if the date and the monts are correct (max 31days,  12 months) and correct /.
    int y,m,d;
    if (date[10]!='\0')
        return 0;
    if ((date[2]!='/') || (date[5]!='/'))
        return 0;
    d=(date[0]-'0')*10 + (date[1]-'0');
    if ((d<1) || (d>31))
        return 0;
    m=(date[3]-'0')*10+(date[4]-'0');
    if ((m<1) || (m>12))
        return 0;
    y=(date[6]-'0')*1000+(date[7]-'0')*100+(date[8]-'0')*10+(date[9]-'0');
    if ((y<1900) || (y>1995))
        return 0;
    return 1;
}

int isValidAccountNum(char num[]){  //function that check if the account number is only numbers and this form xxxx-xxxxxx-xxx
    int i;
    if(num[15]!='\0')
        return 0;
    if ((num[4]!='-') || (num[11]!='-'))
        return 0;
    for(i=0;i<15;i++){
        if ((i==4) || (i==11))
            i+=1;
        if ((num[i]<'0') || (num[i]>'9'))
            return 0;
    }
    return 1;
}

int isValidPhone(char num[]){ //function that check if the phone number is only numer and this form xxx-xxxxxx or xxxx-xxxxx
    int i,hyphen=0,pos=0;
     if(num[11]!='\0')
        return 0;
     for(i=0;i<11;i++){
        if (num[i]=='-'){
            hyphen+=1;
            pos=i;
        }
     }
     if (hyphen !=1)
        return 0;
     if ((pos!=3) && (pos!=4))
        return 0;
    for(i=0;i<11;i++){
        if (i==pos)
            i+=1;
        if ((num[i]<'0') && (num[i]>'9'))
            return 0;
    }
     return 1;
}

int isValidMail(char mail[]){ //function that check if the e mail input is correct meaining, that has @ character and letter or numbers only
    int i,len,c=0,bAt=0,aAt=0,posAt=0;
    len=strlen(mail);


    for (i=0;i<len;i++){
        if(mail[i]=='@'){
            c+=1;
            posAt=i;
        }
    }
    if (c!=1)
        return 0;
    if((mail[len-1]<'A') || ((mail[len-1]>'Z') && (mail[len-1]<'a')) || (mail[len-1]>'z'))
        return 0;
    if((mail[len-2]<'A') || ((mail[len-2]>'Z') && (mail[len-2]<'a')) || (mail[len-2]>'z'))
        return 0;
    if(mail[len-3]!='.')
        return 0;
    for (i=len-4;i!=posAt;i--){
        if((mail[i]<'0') || ((mail[i]>'9') && (mail[i]<'A')) || ((mail[i]>'Z') && (mail[i]<'a')) || (mail[i]>'z'))
            return 0;
        else
            aAt+=1;
    }
    for (i=0;i!=posAt;i++){
        if((mail[i]<'0') || ((mail[i]>'9') && (mail[i]<'A')) || ((mail[i]>'Z') && (mail[i]<'a')) || (mail[i]>'z'))
            return 0;
        else
            bAt+=1;
    }
    if ((bAt<4) || (aAt<2))
        return 0;
    return 1;
}

int isValidBankOp(char bankop[]){//function that check if there are only correct bank operations
    char analipsi[]="withdrawal", katathesi[]="deposit", enimBibliariou[]="account update", danio[]="loan issue", ekdosiKartas[]="card issue";
    if (!strcmp(bankop,analipsi))
        return 1;
    if (!strcmp(bankop,katathesi))
        return 1;
    if (!strcmp(bankop,enimBibliariou))
        return 1;
    if (!strcmp(bankop,danio))
        return 1;
    if (!strcmp(bankop,ekdosiKartas))
        return 1;
    return 0;
}

void insertClient(char fName[], char sName[],char date[], char accNum[], char phone[], char mail[],char cdate[], char ctime[], char bankop[]){ //this function create add the new client in queue
    clientData *nClient;
    char ch=0;

        if (tail!=NULL &&((!strcmp(tail->firstName,fName))&&(!strcmp(tail->lastName,sName))&&(!strcmp(tail->birthday,date)))){  //if the new client already exists there is a message that ask him if he want to update the data
                do{
                    printf("\nYour data(Name & Birthday) already exist.\nDo you want update your record with the newone(y/n)?:");
                    scanf ("%c",&ch);
                }while ((ch != 'y') && (ch != 'n'));
                if(ch=='n')
                    return;
               else
                    nClient=tail;
        }
        if (ch==0)      //memory allocation for the new client in queue
            nClient = (clientData*) malloc( sizeof( clientData ));


        if ( nClient != NULL ) {     //if space available (after the input check from the last function) store the data of the client in struct
                strncpy(nClient->firstName,fName, sizeof(nClient->firstName)-1);
                nClient->firstName[sizeof (nClient->firstName) - 1] = '\0';
                strncpy(nClient->lastName,sName,sizeof(nClient->lastName)- 1);
                nClient->lastName[sizeof (nClient->lastName) - 1] = '\0';
                strncpy(nClient->birthday,date,sizeof(nClient->birthday)- 1);
                nClient->birthday[sizeof (nClient->birthday) - 1] = '\0';
                strncpy(nClient->accNum,accNum,sizeof(nClient->accNum)-1);
                nClient->accNum[sizeof (nClient->accNum) - 1] = '\0';
                strncpy(nClient->telNum,phone,sizeof(nClient->telNum)-1);
                nClient->telNum[sizeof (nClient->telNum) - 1] = '\0';
                strncpy(nClient->email,mail,sizeof(nClient->email)-1 );
                nClient->email[sizeof (nClient->email) - 1] = '\0';
                strncpy(nClient->entryDate,cdate,sizeof(nClient->entryDate)-1 );
                nClient->entryDate[sizeof (nClient->entryDate) - 1] = '\0';
                strncpy(nClient->entryTime,ctime,sizeof(nClient->entryTime)-1 );
                nClient->entryTime[sizeof (nClient->entryTime) - 1] = '\0';
                strncpy(nClient->bankOp,bankop,sizeof(nClient->bankOp)-1);
                nClient->bankOp[sizeof (nClient->bankOp) - 1] = '\0';

                nClient->next = NULL;

                if ( head == NULL ) {
                    head=tail=nClient;
                }
                else {
                    tail->next = nClient;
                    tail=nClient;
                }
                printf("\n Succesfully Client add in queue!");
            }
   else
      printf("No memory available.\n" );

nClient->next = NULL;
}

void serveClient(){ //serve the first client and remove him from the head of the queue
    clientData *temp = head;// store the old value of head pointer
    if(head != NULL)
    {
       head = head ->next; // Change head pointer to point to next node
       free(temp); // delete memory allocated for the previous head node
    }
    printf("\n\nThe first Client has been served.\n");
    //return head;
}

void removeClient(char str[], int len){ //remove a client from anywere in the queue
   clientData *pClient, *remClient,*temp=head;
   char fName[50], sName[50];
   int i,pos,count=0;
   char ch,c;

   for(i=14;i<len;i++){
    if (str[i]==' ')
        pos=i;
   }
    memcpy(fName,&str[13],pos-13);
    fName[pos-13]='\0';
    memcpy(sName,&str[pos+1],len-pos);
    sName[len-pos]='\0';

    if ( temp == NULL ){
        printf( "\n\nList is empty." );
        return;
    }

    while(temp!=NULL){
        if(!strcmp(temp->firstName,fName) && (!strcmp(temp->lastName,sName)))
            count+=1;
        temp = temp->next;
    }

    if(count==0){
        printf("\n\nThere is no Client with name: %s %s doesn't exist.",fName,sName);
        return;
    }
    else
        count=0;

    printf( "\n\nThe list is:\n\n" );   //print message if there are more clients with the same name and ask user who want to remove
    printf("|%-16s|%-18s|%-13s|%-16s|%-15s|%-18s|%-10s|%-10s|%-15s|","First Name","Surnmame","Birthday","Account Num.","Telephone Num.","e-mail","Entry Date", "Entry Time","Bank Operetion");
    printf("\n---------------------------------------------------------------------------------------------------------------------------------------------\n");
    temp=head;
    while (temp != NULL ) {
            if ((!strcmp(temp->firstName,fName) && (!strcmp(temp->lastName,sName)))){
                count+=1;
                printf("%d.",count);
                printf("%-16s|",temp->firstName);
                printf("%-18s|",temp->lastName);
                printf("%-13s|",temp->birthday);
                printf("%-16s|",temp->accNum);
                printf("%-15s|",temp->telNum);
                printf("%-18s|",temp->email);
                printf("%-10s|",temp->entryDate);
                printf("%-10s|",temp->entryTime);
                printf("%-15s|\n",temp->bankOp);
            }
            temp = temp->next;
    }
    printf("Enter the number of the client you want to delete:");
    scanf("%c",&c);

    remClient=pClient=head;
    count=0;
    while(remClient!=NULL){
        if((!strcmp(remClient->firstName,fName) && (!strcmp(remClient->lastName,sName))))
            count+=1;
        if((!strcmp(remClient->firstName,fName) && (!strcmp(remClient->lastName,sName)))&&((c-'0')==count)){
                if(remClient==head)
                    head=remClient->next;
                else{
                    pClient->next=remClient->next;
                    if(remClient==tail)
                        tail=pClient;
                }
        free(remClient); //remove of a client
        printf("\n\nThe Client: %s %s has been removed.",fName,sName);
        return;
        }
        pClient=remClient;
        remClient=remClient->next;
    }
}

void searchClient(char str[], int len){ //search a client from a keyword of the surname
    clientData *temp=head;
    char partSur[50];

    memcpy(partSur,&str[13],len-13);
    partSur[len-13]='\0';

    if ( temp == NULL )
        printf( "\n\nList is empty." );
    else {
        printf( "\n\nThe list content Surname with -%s is:\n\n", partSur );
        printf("|%-16s|%-18s|%-13s|%-16s|%-15s|%-18s|%-10s|%-10s|%-15s|","First Name","Surnmame","Birthday","Account Num.","Telephone Num.","e-mail","Entry Date", "Entry Time","Bank Operetion");
        printf("\n---------------------------------------------------------------------------------------------------------------------------------------------");

        while ( temp != NULL ) {
                if(strstr(temp->lastName,partSur)){
                    printf("\n|%-16s|",temp->firstName);
                    printf("%-18s|",temp->lastName);
                    printf("%-13s|",temp->birthday);
                    printf("%-16s|",temp->accNum);
                    printf("%-15s|",temp->telNum);
                    printf("%-18s|",temp->email);
                    printf("%-10s|",temp->entryDate);
                    printf("%-10s|",temp->entryTime);
                    printf("%-15s|\n",temp->bankOp);
                }
                temp = temp->next;
        }
    }

}

void printAllClients(){ //print all data in the queue
    clientData *temp=head;
    if ( temp == NULL )
        printf( "\n\nList is empty." );
    else {
        printf( "\n   _____ _ _            _         _____        _ ");
        printf( "\n  / ____| (_)          | |       |  __ \\      | |       ");
        printf( "\n | |    | |_  ___ _ __ | |_ ___  | |  | | __ _| |_ __ _ ");
        printf( "\n | |    | | |/ _ \\ '_ \\| __/ __| | |  | |/ _` | __/ _` |");
        printf( "\n | |____| | |  __/ | | | |_\\__ \\ | |__| | (_| | || (_| |");
        printf( "\n  \\_____|_|_|\\___|_| |_|\\__|___/ |_____/ \\__,_|\\__\\__,_|");
        printf("\n|%-16s|%-18s|%-13s|%-16s|%-15s|%-18s|%-10s|%-10s|%-15s|","First Name","Surnmame","Birthday","Account Num.","Telephone Num.","e-mail","Entry Date", "Entry Time","Bank Operetion");
        printf("\n---------------------------------------------------------------------------------------------------------------------------------------------\n");

      while ( temp != NULL ) {
            printf("|%-16s|",temp->firstName);
            printf("%-18s|",temp->lastName);
            printf("%-13s|",temp->birthday);
            printf("%-16s|",temp->accNum);
            printf("%-15s|",temp->telNum);
            printf("%-18s|",temp->email);
            printf("%-10s|",temp->entryDate);
            printf("%-10s|",temp->entryTime);
            printf("%-15s|\n",temp->bankOp);
            temp = temp->next; //edw skaei //
      }
    }
}

void printAllClientsRev(clientData *head){
    clientData *temp=head;

    if(temp == NULL)
        return;

    printAllClientsRev(temp->next);

    printf("|%-16s|",temp->firstName);
    printf("%-18s|",temp->lastName);
    printf("%-13s|",temp->birthday);
    printf("%-16s|",temp->accNum);
    printf("%-15s|",temp->telNum);
    printf("%-18s|",temp->email);
    printf("%-10s|",temp->entryDate);
    printf("%-10s|",temp->entryTime);
    printf("%-15s|\n",temp->bankOp);
}

void printOrder(char str[], int len){
    clientData *temp=head;
    char bankOrder[20];

    memcpy(bankOrder,&str[6],len-6);
    bankOrder[len-6]='\0';
    if ( temp == NULL )
        printf( "\n\nList is empty." );
    else {
        printf( "\n   _____ _ _            _         _____        _ ");
        printf( "\n  / ____| (_)          | |       |  __ \\      | |       ");
        printf( "\n | |    | |_  ___ _ __ | |_ ___  | |  | | __ _| |_ __ _ ");
        printf( "\n | |    | | |/ _ \\ '_ \\| __/ __| | |  | |/ _` | __/ _` |");
        printf( "\n | |____| | |  __/ | | | |_\\__ \\ | |__| | (_| | || (_| |");
        printf( "\n  \\_____|_|_|\\___|_| |_|\\__|___/ |_____/ \\__,_|\\__\\__,_|");
        printf( "\n\nThe list content only %s is:\n\n", bankOrder );
        printf("|%-16s|%-18s|%-13s|%-16s|%-15s|%-18s|%-10s|%-10s|%-15s|","First Name","Surnmame","Birthday","Account Num.","Telephone Num.","e-mail","Entry Date", "Entry Time","Bank Operetion");
        printf("\n---------------------------------------------------------------------------------------------------------------------------------------------\n");

        while ( temp != NULL ) {
                if(!strcmp(temp->bankOp,bankOrder)){
                    printf("|%-16s|",temp->firstName);
                    printf("%-18s|",temp->lastName);
                    printf("%-13s|",temp->birthday);
                    printf("%-16s|",temp->accNum);
                    printf("%-15s|",temp->telNum);
                    printf("%-18s|",temp->email);
                    printf("%-10s|",temp->entryDate);
                    printf("%-10s|",temp->entryTime);
                    printf("%-15s|\n",temp->bankOp);
                }
                temp = temp->next; //edw skaei //
        }
    }
}

void saveToFile(char str[], int len){   //save to a file, backup
    clientData *temp=head;
    char nameFile[25];

    FILE *fp;
    memcpy(nameFile,&str[7],len-7);
    nameFile[len-7]='\0';

    fp=fopen(nameFile,"w");

    if (fp==NULL){
        printf("\nError: File can't be loaded.");
        exit(1);
    }

      while ( temp != NULL ) {
            fprintf(fp,"%-16s|",temp->firstName);
            fprintf(fp,"%-18s|",temp->lastName);
            fprintf(fp,"%-13s|",temp->birthday);
            fprintf(fp,"%-16s|",temp->accNum);
            fprintf(fp,"%-15s|",temp->telNum);
            fprintf(fp,"%-18s|",temp->email);
            fprintf(fp,"%-10s|",temp->entryDate);
            fprintf(fp,"%-10s|",temp->entryTime);
            fprintf(fp,"%-15s\n",temp->bankOp);
            temp = temp->next;
      }

    fclose(fp);
    return ;


}

void TextArt(){
    char ch;
    printf("\n");
printf("\n   $$\\          $$$$$$$\\   $$$$$$\\  $$\\   $$\\ $$\\   $$\\          $$\\  ");
printf("\n $$$$$$\\        $$  __$$\\ $$  __$$\\ $$$\\  $$ |$$ | $$  |       $$$$$$\\  ");
printf("\n$$  __$$\\       $$ |  $$ |$$ /  $$ |$$$$\\ $$ |$$ |$$  /       $$  __$$\\ ");
printf("\n$$ /  \\__|      $$$$$$$\\ |$$$$$$$$ |$$ $$\\$$ |$$$$$  /        $$ /  \\__|");
printf("\n\\$$$$$$\\        $$  __$$\\ $$  __$$ |$$ \\$$$$ |$$  $$<         \\$$$$$$\  ");
printf("\n \\___ $$\\       $$ |  $$ |$$ |  $$ |$$ |\\$$$ |$$ |\$$\\          \\___ $$\\ ");
printf("\n$$\\  \\$$ |      $$$$$$$  |$$ |  $$ |$$ | \\$$ |$$ | \$$\\        $$\\  \\$$ |");
printf("\n\\$$$$$$  |      \\_______/ \\__|  \\__|\\__|  \\__|\\__|  \\__|      \\$$$$$$  |");
printf("\n \\_$$  _/                                                      \\_$$  _/ ");
printf("\n   \\__/                                                          \\__/ ");
printf("\n");

printf("\n                    _._._                       _._._");
printf("\n                   _|   |_                     _|   |_");
printf("\n                   | ... |_._._._._._._._._._._| ... |");
printf("\n                   | ||| |  o NATIONAL BANK o  | ||| |");
printf("\n                   | ~~~ |  ~~~    ~~~    ~~~  | ~~~ |");
printf("\n              ())  |[-|-]| [-|-]  [-|-]  [-|-] |[-|-]|  ())");
printf("\n             (())) |     |---------------------|     | (()))");
printf("\n            (())())| ~~~ |  ~~~    ~~~    ~~~  | ~~~ |(())())");
printf("\n            (()))()|[-|-]|  :::   .-'-.   :::  |[-|-]|(()))()");
printf("\n            ()))(()|     | |~|~|  |_|_|  |~|~| |     |()))(()");
printf("\n               ||  |_____|_|_|_|__|_|_|__|_|_|_|_____|  ||");
printf("\n            ~ ~^^ @@@@@@@@@@@@@@/=======\@@@@@@@@@@@@@@ ^^~ ~");
printf("\n                 ^~^~            cst11068             ~^~^");
printf("\n");
printf("\n                  Application for Customer Service in Bank ");
do{
			printf("\n                       Do you want to continue, y/n? ");
			scanf("%s", &ch);
		}while ((ch != 'y') && (ch != 'n'));       //ask the use if he want to start the game or not!
		if (ch=='n')
            exit(0);
}

void Menu(){
printf ("\nAt $> type the Manual Comand for Bank Queue Managment:\n");
printf("\nType \"newClient (Firstname, Surname, DD/MM/YYYY, Acount_Number, Phone_Number, E-mail, Bank Operation)\"Insert Client's Data in Queue*.");
printf("\nType \"serveClient\" Serve the first Client of Bank Queue.");
printf("\nType \"removeClient (Name Surname)\" Remove someone from the Queue.");
printf("\nType \"printAll\"Print all Clients are waiting, from the older to the new.");
printf("\nType \"printAllRev\"Print all the Clients reversed, from the new to the older.");
printf("\nType \"print (bank_operation)\"Print all the Clients which have a specific bank_operation.");
printf("\nType \"searchClient (keyword)\" Search for a Client who has the keyword is his first Name or Surname.");
printf("\nType \"backup (filename.txt)\" Backup the data.");
printf("\nType \"restore (filename.txt)\" Restore the data from a file.");
printf("\nType \"exit\"Terminate the program.");
printf("\n\n*When a newClient Entry to our Bank the Entry Time & Date inserted automatically.\n");
}

void loadFromFile(char str[], int len){ //restore the data from a file
    clientData *temp=head;
    char tTable[500] , nameFile[50];
    char fName[50], sName[50] ,date[50], accNum[50], phone[50], mail[50],cdate[14],ctime[7], bankop[50];
    int posk[8]={0},poss[9]={0};
    int i,j,k;

    memcpy(nameFile,&str[8],len-8);
    nameFile[len-8]='\0';

    FILE* fp;
    fp = fopen(nameFile,"r");
    if(fp==NULL){
        printf("Error: File can not be loaded\n");
        return;
    }
    else{
        free_list();
        while (1){
                if (fgets(tTable, sizeof(tTable), fp)==NULL)
                    break;
                fgets(tTable, sizeof(tTable), fp);
                j=0;
                k=0;
              // printf("%s",tTable);
                for(i=0;i<500;i++){
                    if (tTable[i]=='|'){
                        posk[j]=i;
                        j+=1;
                    }
                    if(i>0){
                        if ((tTable[i]==' ')&&((tTable[i-1]>'/')&&(tTable[i-1]<'z'))){
                            poss[k]=i;
                            k+=1;
                        }
                    }
                }
                memcpy(fName,&tTable[0],poss[0]);
                fName[poss[0]]='\0';


                memcpy(sName,&tTable[17],poss[1]-17);
                sName[poss[1]-17]='\0';


                memcpy(date,&tTable[posk[1]+1],poss[2]-posk[1]-1);
                date[poss[2]-posk[1]-1]='\0';


                memcpy(accNum,&tTable[posk[2]+1],poss[3]-posk[2]-1);
                accNum[poss[3]-posk[2]-1]='\0';


                memcpy(phone,&tTable[posk[3]+1],poss[4]-posk[3]-1);
                phone[poss[4]-posk[3]-1]='\0';


                memcpy(mail,&tTable[posk[4]+1],poss[5]-posk[4]-1);
                mail[poss[5]-posk[4]-1]='\0';


                memcpy(cdate,&tTable[posk[5]+1],posk[6]-posk[5]-1);
                cdate[posk[6]-posk[5]-1]='\0';


                memcpy(ctime,&tTable[posk[6]+1],poss[6]-posk[6]-1);
                ctime[poss[6]-posk[6]-1]='\0';


                memcpy(bankop,&tTable[posk[7]+1],poss[8]-posk[7]-1);
                bankop[poss[8]-posk[7]-1]='\0';


                insertClient(fName,sName,date,accNum,phone,mail,cdate,ctime,bankop);


                j=k=0;
        }
        fclose(fp);
    }




}

void free_list(){   //free memory
    clientData *temp;
    clientData *nClient;

    temp=head;
    while(temp!=NULL){
        nClient=temp->next;
        free(temp);
        temp=nClient;
    }
}

