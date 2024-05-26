//25/05
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<windows.h>

#define MAX_NAGAD_LENGTH MAX_NID_LENGTH
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define MAX_MAPS_LINK_LENGTH 200
#define ADMIN_ROLE 1
#define TENANT_ROLE 2
#define OWNER_ROLE 3
#define MAX_PROPERTIES 100
#define MAX_COMPLAINTS 100
#define MAX_NOTIFICATION_LENGTH 200
#define MAX_NOTIFICATIONS 100
#define MAX_NOTIFICATION_LENGTH 200
#define MAX_NID_LENGTH 20
#define MAX_EDUCATION_LENGTH 50
#define MAX_STATUS_LENGTH 5
#define MAX_BKASH_LENGTH 20
// Structs
typedef struct User
{
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    int role;
    char bkashNumber[MAX_BKASH_LENGTH]; // Add bkash number field
    char nagadNumber[MAX_NAGAD_LENGTH]; // Add nagad number field
    struct Notification *notifications;
    struct User *next;
} User;

typedef struct Flat
{
    int flatNumber;
    float rentalPrice;
    bool isRented;
    struct Flat *next;
} Flat;

typedef struct Property
{
    int id;
    char ownerName[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    int numberOfFlats;
    Flat *flats;
    bool isApproved;
    bool isRented;
    char rentedBy[MAX_NAME_LENGTH];
    float rentalAmount;
    int paymentMethod; // Add payment method field (1. bkash, 2. nagad)
    char paymentAccount[MAX_NAME_LENGTH]; // Add payment account field
    char googleMapsLink[MAX_MAPS_LINK_LENGTH];
    char tenantNID[MAX_NID_LENGTH];
    char tenantEducation[MAX_EDUCATION_LENGTH];
    char tenantStatus[MAX_STATUS_LENGTH];
    char tenantBkash[MAX_BKASH_LENGTH];
    char tenantNagad[MAX_NAGAD_LENGTH];
    struct Property *next;
} Property;




typedef struct Complaint
{
    int propertyId;
    char complaint[MAX_ADDRESS_LENGTH];
    struct Complaint *next;
} Complaint;

typedef struct Notification
{
    char message[MAX_NOTIFICATION_LENGTH];
    struct Notification *next;
} Notification;

// Global variables
User *users = NULL;
User *currentUser = NULL;
Property *properties = NULL;
Complaint *complaints = NULL;
Notification *notifications = NULL;
int propertyCount = 0;
int complaintCount = 0;
int notificationCount = 0;

// Function prototypes
void setupUsers();
void loadUsers();
void saveUsers();
void loadProperties();
void saveProperties();
void loadComplaints();
void saveComplaints();
void loadNotifications();
void saveNotifications();
void displayHeadline();
void displayJoinCommunity();
void signUp();
void logIn();
void adminPanel();
void viewMyProperties();
void ownerPanel();
void tenantPanel();
void displayPropertyDetails(Property *property);
void displayFlatDetails(Flat *flat);
void addProperty();
void listProperties();
void rateProperty();
void complainAboutProperty();
void approveProperties();
void deleteProperty();
void displayNewsFeed();
void addNotification(const char *message);
void displayNotifications();
int validateMapLink(const char *link);
int getRentedPropertyId(const char *username);
void viewComplaints();
void viewNotifications();
void viewRentedPropertyDetails();
bool propertyIsRentedByTenant(const Property *property, const char *username);
void viewAvailableProperties();
void rentProperty();



int main()
{
    SetConsoleTitle("Real Estate Management System");
    system("color 3F");
    
    setupUsers();
    loadProperties();
    loadComplaints();
    loadNotifications();

    while (1)
    {
        displayHeadline();
        displayJoinCommunity();
        int option;
        printf("\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\tChoose an option...>");
        scanf("%d", &option);
        getchar(); // Consume newline

        switch (option)
        {
        case 1:
            signUp();
            break;
        case 2:
            logIn();
            break;
        default:
            printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\tInvalid option. Please choose a valid option.\n");
            continue; // Continue to prompt for input
        }

        if (currentUser != NULL)
        {
            if (currentUser->role == ADMIN_ROLE)
            {
                adminPanel();
            }
            else if (currentUser->role == OWNER_ROLE)
            {
                ownerPanel();
            }
            else if (currentUser->role == TENANT_ROLE)
            {
                tenantPanel();
            }
        }

        saveProperties();
        saveComplaints();
        saveNotifications();
        saveUsers();

        printf("Press any key to continue...\n");
        getchar(); // Wait for user to press any key
    }

    return 0;
}



void setupUsers()
{
    // Initialize users
    // For simplicity, let's hardcode some users
    // Admin user
    User *admin = malloc(sizeof(User));
    strcpy(admin->username, "admin");
    strcpy(admin->password, "admin123");
    admin->role = ADMIN_ROLE;
    admin->notifications = NULL;
    admin->next = NULL;

    // Tenant user
    User *tenant = malloc(sizeof(User));
    strcpy(tenant->username, "tenant");
    strcpy(tenant->password, "tenant123");
    tenant->role = TENANT_ROLE;
    tenant->notifications = NULL;
    tenant->next = NULL;

    // Owner user
    User *owner = malloc(sizeof(User));
    strcpy(owner->username, "owner");
    strcpy(owner->password, "owner123");
    owner->role = OWNER_ROLE;
    owner->notifications = NULL;
    owner->next = NULL;

    users = admin;
    admin->next = tenant;
    tenant->next = owner;
}

void loadUsers()
{
    // Load user data from file
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    while (fscanf(file, "%s %s %d", users->username, users->password, &users->role) == 3)
    {
        User *newUser = malloc(sizeof(User));
        strcpy(newUser->username, users->username);
        strcpy(newUser->password, users->password);
        newUser->role = users->role;
        newUser->notifications = NULL;
        newUser->next = users->next;
        users->next = newUser;
    }

    fclose(file);
}

void saveUsers()
{
    // Save user data to file
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    User *temp = users;
    while (temp != NULL)
    {
        fprintf(file, "%s %s %d\n", temp->username, temp->password, temp->role);
        temp = temp->next;
    }

    fclose(file);
}

void loadProperties()
{
    // Load property data from file
    FILE *file = fopen("properties.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    while (fscanf(file, "%d %s %s %d %d %s", &properties->id, properties->ownerName, properties->address,
                  &properties->numberOfFlats, (int*)&properties->isApproved, properties->googleMapsLink) == 6)
    {
        Property *newProperty = malloc(sizeof(Property));
        newProperty->id = properties->id;
        strcpy(newProperty->ownerName, properties->ownerName);
        strcpy(newProperty->address, properties->address);
        newProperty->numberOfFlats = properties->numberOfFlats;
        newProperty->isApproved = properties->isApproved;
        strcpy(newProperty->googleMapsLink, properties->googleMapsLink);
        newProperty->next = properties->next;
        properties->next = newProperty;
    }

    fclose(file);
}

void saveProperties()
{
    // Save property data to file
    FILE *file = fopen("properties.txt", "w");
    if (file == NULL)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    Property *temp = properties;
    while (temp != NULL)
    {
        fprintf(file, "%d %s %s %d %d %s\n", temp->id, temp->ownerName, temp->address, temp->numberOfFlats,
                temp->isApproved, temp->googleMapsLink);
        temp = temp->next;
    }

    fclose(file);
}

void loadComplaints()
{
    // Load complaint data from file
    FILE *file = fopen("complaints.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    while (fscanf(file, "%d %s", &complaints->propertyId, complaints->complaint) == 2)
    {
        Complaint *newComplaint = malloc(sizeof(Complaint));
        newComplaint->propertyId = complaints->propertyId;
        strcpy(newComplaint->complaint, complaints->complaint);
        newComplaint->next = complaints->next;
        complaints->next = newComplaint;
    }

    fclose(file);
}

void saveComplaints()
{
    // Save complaint data to file
    FILE *file = fopen("complaints.txt", "w");
    if (file == NULL)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    Complaint *temp = complaints;
    while (temp != NULL)
    {
        fprintf(file, "%d %s\n", temp->propertyId, temp->complaint);
        temp = temp->next;
    }

    fclose(file);
}

void loadNotifications()
{
    // Load notification data from file
    FILE *file = fopen("notifications.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    while (fgets(notifications->message, MAX_NOTIFICATION_LENGTH, file) != NULL)
    {
        Notification *newNotification = malloc(sizeof(Notification));
        strcpy(newNotification->message, notifications->message);
        newNotification->next = notifications->next;
        notifications->next = newNotification;
    }

    fclose(file);
}

void saveNotifications()
{
    // Save notification data to file
    FILE *file = fopen("notifications.txt", "w");
    if (file == NULL)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    Notification *temp = notifications;
    while (temp != NULL)
    {
        fprintf(file, "%s\n", temp->message);
        temp = temp->next;
    }

    fclose(file);
}

void displayHeadline()
{
    system("cls");
    printf("\n\n\n\n\t\t\t\t\tWelcome to Real Estate Management System\n");
    printf("\t\t\t\t\t\tWE ENSURE YOUR COMFORT\n");
}

void displayJoinCommunity()
{
    printf("\n\n\n\n");
    printf("\t\t\t\t\t\t Sign up\t Log in");
    printf("\n\t\t\t\t\t\t~~~~~~~~\t~~~~~~~~");
    printf("\n\t\t\t\t\t\t   1  \t\t   2");
    printf("\n\t\t\t\t\t\t~~~~~~~~\t~~~~~~~~");
}

void signUp()
{
    system("cls");
    printf("\n\n\t\t\t\t\t\t\tSign Up\n");
    printf("\t\t\t\t\t\t\t~~~~~~~\n\n");
    printf("\t\t\t\t\tEnter your username: ");
    char username[MAX_NAME_LENGTH];
    fgets(username, MAX_NAME_LENGTH, stdin);
    strtok(username, "\n"); // Remove newline character

    printf("\t\t\t\t\tEnter your password: ");
    char password[MAX_NAME_LENGTH];
    fgets(password, MAX_NAME_LENGTH, stdin);
    strtok(password, "\n"); // Remove newline character

    printf("\t\t\t\t\tSelect your role: \n");
    printf("\t\t\t\t\t\t\t 1. Admin.\n");
    printf("\t\t\t\t\t\t\t 2. Tenant.\n");
    printf("\t\t\t\t\t\t\t 3. Owner.\n");
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\t\t\t\t\t\tYour Selection...>");
    int role;
    scanf("%d", &role);
    getchar(); // Consume newline

    User *newUser = malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->role = role;
    newUser->notifications = NULL;
    newUser->next = users;
    users = newUser;
    currentUser = newUser;

    // If the user is an owner, prompt for bkash and nagad numbers
    if (role == OWNER_ROLE)
    {

        printf("Enter your bkash number: ");
        fgets(newUser->bkashNumber, MAX_BKASH_LENGTH, stdin);
        strtok(newUser->bkashNumber, "\n"); // Remove newline character

        printf("Enter your nagad number: ");
        fgets(newUser->nagadNumber, MAX_NAGAD_LENGTH, stdin);
        strtok(newUser->nagadNumber, "\n"); // Remove newline character
    }
}

void logIn()
{
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    bool validCredentials = false;

    while (!validCredentials)
    {
        printf("Enter your username: ");
        scanf("%s", username);
        printf("Enter your password: ");
        scanf("%s", password);
        getchar(); // Consume newline

        // Check if the username and password are valid
        User *temp = users;
        while (temp != NULL)
        {
            if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
            {
                validCredentials = true;
                currentUser = temp;
                printf("Login successful. Welcome, %s!\n", username);
                break;
            }
            temp = temp->next;
        }

        if (!validCredentials)
        {
            printf("Invalid username or password. Please try again.\n");
        }
    }
}


void adminPanel()
{

    system("cls");
    //printf("\n\n\t\t\t\t\t\t\tAdmin Panel\n");
    //printf("\t\t\t\t\t\t\t~~~~~~~~~~~\n\n");

    int choice;
    do
    {
        printf("\n\n\t\t\t\t\t\t\tAdmin Panel\n");
        printf("\t\t\t\t\t\t\t~~~~~~~~~~~\n\n");
        printf("\n Admin Options\n\n");
        printf(" 1. View Properties\n");
        printf(" 2. Approve Property\n");
        printf(" 3. View Complaints\n");
        printf(" 4. View Notifications\n");
        printf(" 5. Exit Admin Panel\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\tChoose an option...>");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            listProperties();
            system("cls");
            break;
        case 2:
            approveProperties();
            system("cls");
            break;
        case 3:
            // Implement viewing complaints
            break;
        case 4:
            displayNotifications();
            system("cls");
            break;
        case 5:
            printf("Exiting Admin Panel.\n");
            break;
        default:
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\t\t\t\t\t Invalid choice. Please try again...\n");
            sleep(2);
            system("cls");
            break;
        }
    }
    while (choice != 5);
}


void ownerPanel()
{
    system("cls");
    printf("\n\n\t\t\t\t\t\t\tOwner Panel\n");
    printf("\n\n\t\t\t\t\t\t\t~~~~~~~~~~~\n");
    int choice;

    do
    {
        printf("\n Owner Options:\n");
        printf(" 1. View My Properties\n");
        printf(" 2. Add Property\n");
        printf(" 3. Delete Property\n");
        printf(" 4. View Complaints\n");
        printf(" 5. View Notifications\n");
        printf(" 6. Rent Out Property\n"); // Add the option to rent out a property
        printf(" 7. Exit Owner Panel\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\tChoose an option...>");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            viewMyProperties();
            break;
        case 2:
            addProperty();
            break;
        case 3:
            deleteProperty();
            break;
        case 4:
            viewComplaints();
            break;
        case 5:
            viewNotifications();
            break;
        case 6:
            rentProperty(); // Call the function to rent out a property
            break;
        case 7:
            printf("Exiting Owner Panel.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }
    while (choice != 7);
}
void rentProperty()
{
    printf("Rent Property\n");

    // Get property ID to rent
    int propertyId;
    printf("Enter the ID of the property to rent: ");
    scanf("%d", &propertyId);
    fflush(stdin); // Clear input buffer

    // Find the property with the given ID
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (currentProperty->id == propertyId)
        {
            // Check if the property is already rented
            if (currentProperty->isRented)
            {
                printf("This property is already rented out.\n");
                return;
            }

            // Get tenant details
            printf("Enter your NID number: ");
            fgets(currentProperty->tenantNID, MAX_NID_LENGTH, stdin);
            strtok(currentProperty->tenantNID, "\n"); // Remove newline character

            printf("Enter your education qualification: ");
            fgets(currentProperty->tenantEducation, MAX_EDUCATION_LENGTH, stdin);
            strtok(currentProperty->tenantEducation, "\n"); // Remove newline character

            printf("Are you a bachelor? (yes/no): ");
            fgets(currentProperty->tenantStatus, MAX_STATUS_LENGTH, stdin);
            strtok(currentProperty->tenantStatus, "\n"); // Remove newline character

            // Prompt for payment details
            printf("Enter your bkash number: ");
            fgets(currentProperty->tenantBkash, MAX_BKASH_LENGTH, stdin);
            strtok(currentProperty->tenantBkash, "\n"); // Remove newline character

            printf("Enter the rental amount: ");
            scanf("%f", &currentProperty->rentalAmount);
            fflush(stdin); // Clear input buffer

            // Mark the property as rented
            currentProperty->isRented = true;

            // Notify the owner that their property is rented
            char notificationMessage[MAX_NOTIFICATION_LENGTH];
            snprintf(notificationMessage, MAX_NOTIFICATION_LENGTH, "Your property (ID: %d) has been rented out to %s.", currentProperty->id, currentProperty->tenantNID);
            addNotification(notificationMessage);

            printf("Property rented successfully.\n");
            return;
        }
        currentProperty = currentProperty->next;
    }

    // If the property with the given ID is not found
    printf("Property not found.\n");
}

void viewAvailableProperties()
{
    printf("\n\n");
    printf("\t\t\t\t\t\t\tAvailable Properties\n");
    printf("\t\t\t\t\t\t\t~~~~~~~~~~~~~~~~~~~~\n");

    // Display details of available properties
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (!currentProperty->isRented)
        {
            printf("Property ID: %d\n", currentProperty->id);
            printf("Owner: %s\n", currentProperty->ownerName);
            printf("Address: %s\n", currentProperty->address);
            printf("Number of Flats: %d\n", currentProperty->numberOfFlats);
            printf("Google Maps Link: %s\n", currentProperty->googleMapsLink);
            printf("\n");
        }
        currentProperty = currentProperty->next;
    }

    // Rent a property
    rentProperty();
}


void tenantPanel()
{
    system("cls");
    //intf("Tenant Panel\n");

    int option;

    do
    {
        printf("\n\n");
        printf("\n\n\t\t\t\t\t\t\tTenant Panel\n");
        printf("\t\t\t\t\t\t\t~~~~~~~~~~~~\n");
        printf("\n Choose an option:\n\n");
        printf(" 1. View available properties\n");
        printf(" 2. View my rented property details\n");
        printf(" 3. View notifications\n");
        printf(" 4. Complain about a property\n");
        printf(" 5. Rate a property\n");
        printf(" 6. Rent a property\n");
        printf(" 7. Exit\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\tChoose an option...>");

        scanf("%d", &option);
        getchar(); // Consume newline

        switch (option)
        {
        case 1:
            viewAvailableProperties();
            system("cls");
            break;
        case 2:
            viewRentedPropertyDetails();
            break;
        case 3:
            displayNotifications();
            break;
        case 4:
            complainAboutProperty();
            break;
        case 5:
            rateProperty();
            break;
        case 6:
            rentProperty();
            break;
        case 7:
            printf("Exiting tenant panel.\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    }
    while (option != 7);
}


void displayPropertyDetails(Property *property)
{
    printf("Property ID: %d\n", property->id);
    printf("Owner: %s\n", property->ownerName);
    printf("Address: %s\n", property->address);
    printf("Number of flats: %d\n", property->numberOfFlats);
    printf("Approved: %s\n", property->isApproved ? "Yes" : "No");
    printf("Google Maps Link: %s\n", property->googleMapsLink);
}

void displayFlatDetails(Flat *flat)
{
    printf("Flat Number: %d\n", flat->flatNumber);
    printf("Rental Price: %.2f\n", flat->rentalPrice);
    printf("Status: %s\n", flat->isRented ? "Rented" : "Available");
}

void addProperty()
{
    system("cls");
    printf("\n\n");
    printf("\n\n\t\t\t\t\t\t\tAdd Property\n");
    printf("\t\t\t\t\t\t\t~~~~~~~~~~~~\n");

    // Check if the maximum number of properties has been reached
    if (propertyCount >= MAX_PROPERTIES)
    {
        printf("Maximum number of properties reached. Unable to add more.\n");
        return;
    }

    // Allocate memory for a new property
    Property *newProperty = malloc(sizeof(Property));
    if (newProperty == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    // Get property details from the user
    printf("\n\n\t\t\t\t\tEnter property ID: ");
    if (scanf("%d", &newProperty->id) != 1)
    {
        printf("\t\t\t\t\tInvalid input for property ID...\n");
        free(newProperty); // Free allocated memory
        getchar(); // Consume newline
        return;
    }
    getchar(); // Consume newline

    printf("\t\t\t\t\tEnter owner's name: ");
    fgets(newProperty->ownerName, MAX_NAME_LENGTH, stdin);
    strtok(newProperty->ownerName, "\n"); // Remove newline character

    printf("\t\t\t\t\tEnter property address: ");
    fgets(newProperty->address, MAX_ADDRESS_LENGTH, stdin);
    strtok(newProperty->address, "\n"); // Remove newline character

    printf("\t\t\t\t\tEnter number of flats: ");
    if (scanf("%d", &newProperty->numberOfFlats) != 1)
    {
        printf("\t\t\t\t\tInvalid input for number of flats...\n");
        free(newProperty); // Free allocated memory
        getchar(); // Consume newline
        return;
    }
    getchar(); // Consume newline

    // Loop until a valid Google Maps link is provided
    while (1)
    {
        printf("\t\t\t\t\tEnter Google Maps link: ");
        fgets(newProperty->googleMapsLink, MAX_MAPS_LINK_LENGTH, stdin);
        strtok(newProperty->googleMapsLink, "\n"); // Remove newline character

        // Validate Google Maps link
        if (validateMapLink(newProperty->googleMapsLink))
        {
            // Valid link, exit the loop
            break;
        }
        else
        {
            // Invalid link, prompt the user to enter again
            printf("\t\t\t\t\tInvalid Google Maps link format. Please try again.\n");
        }
    }

    // Get rental details
    printf("\t\t\t\t\tEnter the rental amount: ");
    scanf("%f", &newProperty->rentalAmount);
    getchar(); // Consume newline

    // Set initial values
    newProperty->flats = NULL;
    newProperty->isApproved = false;
    newProperty->next = properties;
    properties = newProperty;

    // Increment property count
    propertyCount++;

    printf("\t\t\t\t\tProperty added successfully...\n");
}

void listProperties()
{
    system("cls");
    printf("\n\n");
    printf("\t\t\t\t\t\t\tList Properties\n");
    printf("\t\t\t\t\t\t\t~~~~~~~~~~~~~~~\n");

    // Check if there are properties to list
    if (properties == NULL)
    {
        printf("\n\n\n No properties available...\n");
        sleep(3);
        return;
    }

    Property *currentProperty = properties;
    int propertyIndex = 1;

    // Traverse the linked list of properties and display each property's details
    while (currentProperty != NULL)
    {
        printf("Property %d:\n", propertyIndex);
        printf("ID: %d\n", currentProperty->id);
        printf("Owner: %s\n", currentProperty->ownerName);
        printf("Address: %s\n", currentProperty->address);
        printf("Number of Flats: %d\n", currentProperty->numberOfFlats);
        printf("Approved: %s\n", currentProperty->isApproved ? "Yes" : "No");
        printf("Google Maps Link: %s\n", currentProperty->googleMapsLink);

        // Move to the next property
        currentProperty = currentProperty->next;
        propertyIndex++;
    }
}

void rateProperty()
{
    printf("Rate Property\n");

    // Check if there are properties to rate
    if (properties == NULL)
    {
        printf("No properties available to rate.\n");
        return;
    }

    int propertyId;
    float rating;

    // Prompt user to enter the property ID to rate
    printf("Enter the ID of the property you want to rate: ");
    scanf("%d", &propertyId);

    // Search for the property with the given ID
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (currentProperty->id == propertyId)
        {
            // Property found, prompt user to enter the rating
            printf("Enter your rating for this property (out of 5): ");
            scanf("%f", &rating);

            // Validate the rating (assume it must be between 0 and 5)
            if (rating < 0 || rating > 5)
            {
                printf("Invalid rating. Please enter a value between 0 and 5.\n");
                return;
            }

            // Rating is valid, update the property's rating
            // Here, you can implement the logic to update the property's rating
            printf("Thank you for rating this property!\n");
            return;
        }
        currentProperty = currentProperty->next;
    }

    // If the property with the given ID is not found
    printf("Property with ID %d not found.\n", propertyId);
}


void complainAboutProperty()
{
    printf("Complain About Property\n");

    // Check if there are properties to complain about
    if (properties == NULL)
    {
        printf("No properties available to complain about.\n");
        return;
    }

    int propertyId;
    char complaint[MAX_ADDRESS_LENGTH];

    // Prompt user to enter the property ID to complain about
    printf("Enter the ID of the property you want to complain about: ");
    scanf("%d", &propertyId);

    // Search for the property with the given ID
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (currentProperty->id == propertyId)
        {
            // Property found, prompt user to enter the complaint
            printf("Enter your complaint about this property: ");
            getchar(); // Consume newline character left in buffer
            fgets(complaint, MAX_ADDRESS_LENGTH, stdin);
            strtok(complaint, "\n"); // Remove newline character

            // Validate the complaint (optional)
            // Here, you can implement additional validation if needed

            // Add the complaint to the list of complaints
            Complaint *newComplaint = malloc(sizeof(Complaint));
            newComplaint->propertyId = propertyId;
            strcpy(newComplaint->complaint, complaint);
            newComplaint->next = complaints;
            complaints = newComplaint;

            printf("Complaint submitted successfully!\n");
            return;
        }
        currentProperty = currentProperty->next;
    }

    // If the property with the given ID is not found
    printf("Property with ID %d not found.\n", propertyId);
}

void approveProperties()
{
    system("cls");
    printf("\n\n");
    printf("\t\t\t\t\t\t\tApprove Properties\n");
    printf("\t\t\t\t\t\t\t~~~~~~~~~~~~~~~~~~\n");

    // Check if there are properties to approve
    if (properties == NULL)
    {
        printf("\n\n\n No properties available to approve...\n");
        sleep(3);
        return;
    }

    // Display the list of properties awaiting approval
    printf("Properties awaiting approval:\n");
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (!currentProperty->isApproved)
        {
            printf("Property ID: %d\n", currentProperty->id);
            printf("Owner: %s\n", currentProperty->ownerName);
            printf("Address: %s\n", currentProperty->address);
            printf("Google Maps Link: %s\n", currentProperty->googleMapsLink);
            printf("\n");
        }
        currentProperty = currentProperty->next;
    }

    // Prompt user to enter the ID of the property to approve
    int propertyId;
    printf("Enter the ID of the property to approve: ");
    scanf("%d", &propertyId);

    // Search for the property with the given ID
    currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (currentProperty->id == propertyId)
        {
            // Approve the property
            currentProperty->isApproved = 1;
            printf("Property with ID %d approved successfully!\n", propertyId);
            return;
        }
        currentProperty = currentProperty->next;
    }

    // If the property with the given ID is not found
    printf("Property with ID %d not found.\n", propertyId);
}


void deleteProperty()
{
    printf("Delete Property\n");

    // Check if there are properties to delete
    if (properties == NULL)
    {
        printf("No properties available to delete.\n");
        return;
    }

    // Display the list of properties
    printf("List of properties:\n");
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        printf("Property ID: %d\n", currentProperty->id);
        printf("Owner: %s\n", currentProperty->ownerName);
        printf("Address: %s\n", currentProperty->address);
        printf("Google Maps Link: %s\n", currentProperty->googleMapsLink);
        printf("\n");
        currentProperty = currentProperty->next;
    }

    // Prompt user to enter the ID of the property to delete
    int propertyId;
    printf("Enter the ID of the property to delete: ");
    scanf("%d", &propertyId);

    // Search for the property with the given ID
    Property *prevProperty = NULL;
    currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (currentProperty->id == propertyId)
        {
            // Remove the property from the list
            if (prevProperty == NULL)
            {
                properties = currentProperty->next;
            }
            else
            {
                prevProperty->next = currentProperty->next;
            }
            free(currentProperty);
            printf("Property with ID %d deleted successfully!\n", propertyId);
            return;
        }
        prevProperty = currentProperty;
        currentProperty = currentProperty->next;
    }

    // If the property with the given ID is not found
    printf("Property with ID %d not found.\n", propertyId);
}


void displayNewsFeed()
{
    printf("Display News Feed\n");

    // Check if there are notifications to display
    if (notifications == NULL)
    {
        printf("No notifications available.\n");
        return;
    }

    // Display the notifications
    printf("News Feed:\n");
    Notification *currentNotification = notifications;
    while (currentNotification != NULL)
    {
        printf("%s\n", currentNotification->message);
        currentNotification = currentNotification->next;
    }
}


void addNotification(const char *message)
{
    printf("Add Notification\n");

    // Add the notification to the list
    Notification *newNotification = malloc(sizeof(Notification));
    if (newNotification == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    strcpy(newNotification->message, message);
    newNotification->next = notifications;
    notifications = newNotification;

    // If the notification is about renting a property, send notification to the property owner
    if (strstr(message, "has been rented out to") != NULL)
    {
        char ownerName[MAX_NAME_LENGTH];
        sscanf(message, "Your property (ID: %*d) has been rented out to %s.", ownerName);
        printf("Sending notification to the owner (%s)...\n", ownerName);
        // You can implement the code to send a notification to the owner here
        printf("Notification sent to the owner (%s) successfully.\n", ownerName);
    }
}


void displayNotifications()
{
    printf("\n\n");
    printf("\n\n\t\t\t\t\t\t\tDisplay Notifications\n");
    printf("\t\t\t\t\t\t\t~~~~~~~~~~~~~~~~~~~~~\n");
    Notification *currentNotification = notifications;

    if (currentNotification == NULL)
    {
        printf("\n\n");
        printf("  No notifications to display...\n");
        Sleep(3);
        return;
    }

    printf("Notifications:\n");
    while (currentNotification != NULL)
    {
        printf("- %s\n", currentNotification->message);
        currentNotification = currentNotification->next;
    }
}

//#include <stdio.h>
//#include <string.h>
//#include <stdbool.h>

// Function to validate a map link
int validateMapLink(const char *link)
{
    printf("Validate Map Link\n");

    // List of expected prefixes for valid map links
    const char *prefixes[] =
    {
        "https://maps.google.com",
        "https://maps.app.goo.gl",
        "https://www.google.com/maps/place"
    };

    // Check if the link starts with any of the expected prefixes
    int numPrefixes = sizeof(prefixes) / sizeof(prefixes[0]);
    for (int i = 0; i < numPrefixes; ++i)
    {
        if (strncmp(link, prefixes[i], strlen(prefixes[i])) == 0)
        {
            return 1; // Return 1 to indicate success
        }
    }

    // If the link doesn't match any expected prefix, it's invalid
    printf("Invalid map link: Link must start with one of the following prefixes:\n");
    for (int i = 0; i < numPrefixes; ++i)
    {
        printf("- %s\n", prefixes[i]);
    }

    return 0; // Return 0 to indicate failure
}

// Example usage of validateMapLink function
void exampleUsage()
{
    // Test the function with example links
    const char *links[] =
    {
        "https://maps.app.goo.gl/fzQYLRDjnCN45hry5",
        "https://www.google.com/maps/place/%E0%A6%AC%E0%A6%BE%E0%A6%87%E0%A6%A4%E0%A7%81%E0%A6%B2+%E0%A6%B0%E0%A6%BF%E0%A6%A6%E0%A6%BE/@23.8690116,90.3188011,18z/data=!4m9!1m2!2m1!1sDaffodil+International+University!3m5!1s0x3755c3ceb416c39d:0xd9be71d33b324fae!8m2!3d23.8690116!4d90.3203942!16s%2Fg%2F11kb1vdd1q?entry=ttu",
        "https://example.com"
    };

    int numLinks = sizeof(links) / sizeof(links[0]);
    for (int i = 0; i < numLinks; ++i)
    {
        printf("Link %d validation result: %s\n", i+1, validateMapLink(links[i]) ? "Valid" : "Invalid");
    }
}

// No int main in this version of the code

// You can call exampleUsage() from wherever you need in your project
void viewMyProperties()
{
    printf("My Properties\n");

    // Check if there are properties to display
    if (properties == NULL)
    {
        printf("You don't have any properties.\n");
        return;
    }

    // Traverse the list of properties and display properties owned by the current owner
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (strcmp(currentProperty->ownerName, currentUser->username) == 0)
        {
            printf("Property ID: %d\n", currentProperty->id);
            printf("Owner: %s\n", currentProperty->ownerName);
            printf("Address: %s\n", currentProperty->address);
            printf("Number of Flats: %d\n", currentProperty->numberOfFlats);
            printf("Approved: %s\n", currentProperty->isApproved ? "Yes" : "No");
            printf("Google Maps Link: %s\n", currentProperty->googleMapsLink);
            printf("\n");
        }
        currentProperty = currentProperty->next;
    }
}



void viewComplaints()
{
    printf("Complaints\n");

    // Check if there are complaints to display
    if (complaints == NULL)
    {
        printf("No complaints to display.\n");
        return;
    }

    // Traverse the list of complaints and display complaints related to properties owned by the current owner
    Complaint *currentComplaint = complaints;
    while (currentComplaint != NULL)
    {
        // Find the property associated with the complaint
        Property *currentProperty = properties;
        while (currentProperty != NULL)
        {
            if (currentProperty->id == currentComplaint->propertyId &&
                    strcmp(currentProperty->ownerName, currentUser->username) == 0)
            {
                // Print the complaint
                printf("Property ID: %d\n", currentProperty->id);
                printf("Complaint: %s\n", currentComplaint->complaint);
                printf("\n");
                break; // Move to the next complaint
            }
            currentProperty = currentProperty->next;
        }
        currentComplaint = currentComplaint->next;
    }

    printf("End of complaints.\n");
}


void viewNotifications()
{
    printf("Notifications\n");

    // Check if there are notifications to display
    if (notifications == NULL)
    {
        printf("No notifications to display.\n");
        return;
    }

    // Traverse the list of notifications and display notifications relevant to the current owner
    Notification *currentNotification = notifications;
    while (currentNotification != NULL)
    {
        // Check if the notification is relevant to the current owner
        // You can define relevance based on specific criteria like property ownership, etc.
        // For demonstration purposes, let's assume all notifications are relevant to the current owner
        printf("- %s\n", currentNotification->message);

        // Move to the next notification
        currentNotification = currentNotification->next;
    }

    printf("End of notifications.\n");
}

bool propertyIsRentedByTenant(const Property *property, const char *username)
{
    // Check if property and username are not NULL
    if (property != NULL && username != NULL)
    {
        // Check if the property is rented and the owner's name matches the provided username
        if (property->isRented && strcmp(property->ownerName, username) == 0)
        {
            return true; // Property is rented by the tenant
        }
    }
    return false; // Property is not rented by the tenant
}


int getRentedPropertyId(const char *username)
{
    // Iterate through the properties to find the one rented by the tenant
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        // Check if the property is rented and matches the tenant's username
        // You need to adjust this condition based on how you track property rental status
        if (propertyIsRentedByTenant(currentProperty, username))
        {
            return currentProperty->id; // Return the ID of the rented property
        }
        currentProperty = currentProperty->next;
    }
    // If no rented property is found for the given username, return -1 or another appropriate value
    return -1;
}

void viewRentedPropertyDetails()
{
    printf("Rented Property Details\n");

    // Check if the current user is a tenant
    if (currentUser == NULL || currentUser->role != TENANT_ROLE)
    {
        printf("Access denied. Only tenants can view rented property details.\n");
        return;
    }

    // Check if the current tenant has a rented property
    // For demonstration purposes, let's assume the tenant's rented property ID is stored in a variable named rentedPropertyId
    int rentedPropertyId = getRentedPropertyId(currentUser->username); // You need to implement this function to get the rented property ID for the current tenant

    // Search for the rented property in the properties list
    Property *currentProperty = properties;
    while (currentProperty != NULL)
    {
        if (currentProperty->id == rentedPropertyId)
        {
            // Display details of the rented property
            displayPropertyDetails(currentProperty);
            return;
        }
        currentProperty = currentProperty->next;
    }

    // If the rented property is not found
    printf("You currently do not have a rented property.\n");
}

