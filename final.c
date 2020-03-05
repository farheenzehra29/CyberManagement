#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include<fcntl.h>

#define NUM_USERS 10

/// user information
typedef struct {
    int user_id;
    char name[20];
    char dob[11];
    char address[80];
}ST_USER;

/// track usage
typedef struct {
    int user_id;
    clock_t start_clock;
    clock_t end_clock;
    float print_charges;
    float scan_charges;
    float usage_charges;
}ST_USAGE;

ST_USAGE user_charges[NUM_USERS];

/**
* \brief This function displays the main menu
*/
void display_menu(void)
{
    printf("Menu \n");
    printf("1. Add new customer\n");
    printf("2. Allocate PC\n");
    printf("3. Deallocate PC\n");
    printf("4. List users\n");
    printf("5. Exit\n");
}

/**
*   prompt name and read name to user structure
*   prompt date of birth and read dob to user structure
*   prompt address and read address to user structure
*   Allocate new user id and update user structure
*   Append user structure to user information file
*/
void add_new_customer(void)
{
    ST_USER new_user;
    ST_USER last_user;
    FILE *fp_user_info;
    //long file_pos;

    printf("\nEnter name: ");
    scanf("%s", new_user.name);

    printf("\nEnter DOB: ");
    scanf("%s", new_user.dob);

    printf("\nEnter Address: ");
    scanf("%s", new_user.address);
    printf("\n");

    new_user.address[79] = '\0';
    new_user.name[19] = '\0';
    new_user.dob[10] = '\0';
    // open user information file
    fp_user_info = fopen("user_info.txt", "a+");

    // get the last user id and add 1 for the new user id
    if(fseek(fp_user_info,-(sizeof(ST_USER)),SEEK_END) == 0)
    {
        fread(&last_user,sizeof(ST_USER),1,fp_user_info);
        new_user.user_id = last_user.user_id+1;
    }
    else
    {
        new_user.user_id = 1;
    }

    fseek(fp_user_info,0,SEEK_END);
    // append new user to the file
    fwrite(&new_user,sizeof(ST_USER),1,fp_user_info);

    // close file
    fclose(fp_user_info);
}


/**
*   Prompt to enter user id
*   Search for the id in user_charges
*       If found prompt "already allocated"
*   Search for ID in the user info file
*       If found display user details, display status "allocation successful"
*           Add user id and start clock to user_charges
*       else display " not an existing customer"
*/
void allocate_pc(void)
{
    int user_id, i;
    unsigned char id_found = 0;

    printf("Enter user ID: ");
    scanf("%d", &user_id);

    for(i=0; i < NUM_USERS; i++)
    {
        if(user_charges[i].user_id == user_id)
        {
            id_found = 1;
            break;
        }
    }
    if(id_found)
    {
        printf("\nPC already allocated for this user\n");
    }
    else
    {
        FILE *fp_user_info;
        ST_USER next_user;

        // open user information file
        fp_user_info = fopen("user_info.txt", "a+");

        id_found = 0;
        while(fread(&next_user,sizeof(ST_USER),1,fp_user_info)>0)
        {
            if(next_user.user_id == user_id)
            {
                id_found = 1;
                break;
            }
        }

        if(id_found)
        {
            printf("\nName: %s", next_user.name);
            printf("\nDOB: %s", next_user.dob);
            printf("\nAddress: %s", next_user.address);

            id_found = 0;
            // Search for a blank location in user charges
            for(i=0; i < NUM_USERS; i++)
            {
                if(user_charges[i].user_id == 0)
                {
                    id_found = 1;
                    break;
                }
            }
            if(id_found)
            {
                user_charges[i].user_id = user_id;
                user_charges[i].start_clock = clock();
                printf("\n Allocation successful");
            }
            else
            {
                printf("\n No PC allocated. \n");
            }
        }
        else
        {
            printf("\nNot an existing customer\n");
        }
        fclose(fp_user_info);
    }

}

/**
*   Prompt "Enter user ID"
*   Search ID in user_charges
*       If found
*           Save end clock in user_charges
*           Prompt " Printing charges" and read charges
*           Prompt " Scan charges" and read charges
*           Calculate usage charges and display
*           Display total charges
*       else
*           prompt " PC not allotted for this ID"
*/
void deallocate_PC(void)
{
    int user_id, i;
    unsigned char id_found = 0;
    printf("Enter user ID: ");
    scanf("%d", &user_id);

    for(i=0; i < NUM_USERS; i++)
    {
        if(user_charges[i].user_id == user_id)
        {
            id_found = 1;
            break;
        }
    }

    if(id_found)
    {
        user_charges[i].end_clock = clock();

        printf("\nEnter Printer charges: ");
        scanf("%f",&user_charges[i].print_charges);

        printf("\nEnter Scan charges: ");
        scanf("%f",&user_charges[i].scan_charges);

        user_charges[i].usage_charges = (user_charges[i].end_clock-user_charges[i].start_clock)/(CLOCKS_PER_SEC * 60.0);
        printf("\nUsage charges: %0.2f", user_charges[i].usage_charges);

        printf("\n Total charges = %0.2f", (user_charges[i].print_charges + user_charges[i].scan_charges + user_charges[i].usage_charges));

        user_charges[i].user_id = 0;
        user_charges[i].end_clock = 0;
        user_charges[i].print_charges = 0;
        user_charges[i].scan_charges = 0;
        user_charges[i].start_clock = 0;
        user_charges[i].usage_charges = 0;
        printf("\n");
    }
    else
    {
        printf("\nPC not allocated for this user ID\n");
    }
}

void display_users(void)
{
        FILE *fp_user_info;
        ST_USER next_user;

        // open user information file
        fp_user_info = fopen("user_info.txt", "r");

        while(fread(&next_user,sizeof(ST_USER),1,fp_user_info)>0)
        {
            printf("\nID=%d, Name=%s, DOB=%s, Address=%s", next_user.user_id, next_user.name, next_user.dob, next_user.address);
        }
        fclose(fp_user_info);

}

/**
*   It displays main menu
*   Read option
*   while (read option != exit)
*       Branch on option
*           Option 1 : Add new customer
*           Option 2 : Allocate PC
*           Option 3 : Deallocate PC
*           Option 4 : Exit
*       Display main menu
*/
int main()
{
    int selected_option = 4, i;

    // reset all user_id in user_charges
    for(i=0; i < NUM_USERS; i++)
    {
        user_charges[i].user_id = 0;
    }

    display_menu();
    scanf("%d", &selected_option);
    while(selected_option != 5)
    {
        switch(selected_option)
        {
        case 1:
            add_new_customer();
            break;
        case 2:
            allocate_pc();
            break;
        case 3:
            deallocate_PC();
            break;
        case 4:
            display_users();
            break;
        }
        display_menu();
        scanf("%d", &selected_option);
    }
}
