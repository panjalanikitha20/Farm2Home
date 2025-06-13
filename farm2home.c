#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product
{
    char name[50];
    float price;
    int quantity; // Quantity available
    struct Product *next;
} Product;

typedef struct User
{
    char username[50];
    char password[50];
    char role[10];     // "farmer" or "customer"
    Product *products; // Only for farmers
    struct User *next;
} User;

User *users = NULL;
User *currentUser = NULL;

// Function declarations
User *createAccount();
User *login();
void listProduct(User *user);
void browseProducts();
void placeOrder();
void giveFeedback();
User *findUserByName(const char *username);

User *findUserByName(const char *username)
{
    User *temp = users;
    while (temp)
    {
        if (strcmp(temp->username, username) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

User *createAccount()
{
    User *newUser = (User *)malloc(sizeof(User));
    newUser->products = NULL;
    newUser->next = users;

    printf("Enter username: ");
    scanf("%s", newUser->username);
    printf("Enter password: ");
    scanf("%s", newUser->password);
    printf("Enter role (farmer/customer): ");
    scanf("%s", newUser->role);

    users = newUser;

    printf("Account created successfully for %s!\n", newUser->username);
    return newUser;
}

User *login()
{
    char uname[50], pass[50];
    printf("Enter username: ");
    scanf("%s", uname);
    printf("Enter password: ");
    scanf("%s", pass);

    User *temp = users;
    while (temp != NULL)
    {
        if (strcmp(temp->username, uname) == 0 && strcmp(temp->password, pass) == 0)
        {
            printf("Login successful as %s (%s)\n", temp->username, temp->role);
            return temp;
        }
        temp = temp->next;
    }
    printf("Login failed.\n");
    return NULL;
}

void listProduct(User *user)
{
    if (strcmp(user->role, "farmer") != 0)
    {
        printf("Only farmers can list products.\n");
        return;
    }

    Product *newProd = (Product *)malloc(sizeof(Product));
    printf("Enter product name: ");
    scanf("%s", newProd->name);
    printf("Enter price (per kg): ");
    scanf("%f", &newProd->price);
    printf("Enter available quantity (kg): ");
    scanf("%d", &newProd->quantity);
    newProd->next = user->products;
    user->products = newProd;
    printf("Product listed successfully!\n");
}

void browseProducts()
{
    printf("Available Products:\n");
    User *temp = users;
    while (temp)
    {
        if (strcmp(temp->role, "farmer") == 0)
        {
            Product *p = temp->products;
            while (p)
            {
                if (p->quantity > 0)
                    printf("Farmer: %s | Product: %s | Price: ₹%.2f | Qty Available: %d kg\n",
                           temp->username, p->name, p->price, p->quantity);
                p = p->next;
            }
        }
        temp = temp->next;
    }
}

void placeOrder()
{
    if (strcmp(currentUser->role, "customer") != 0)
    {
        printf("Only customers can place orders.\n");
        return;
    }

    char farmerName[50], productName[50];
    int orderQty;

    printf("Enter farmer's username: ");
    scanf("%s", farmerName);
    User *farmer = findUserByName(farmerName);
    if (!farmer || strcmp(farmer->role, "farmer") != 0)
    {
        printf("Farmer not found.\n");
        return;
    }

    printf("Enter product name to order: ");
    scanf("%s", productName);
    printf("Enter quantity to order (kg): ");
    scanf("%d", &orderQty);

    Product *p = farmer->products;
    while (p)
    {
        if (strcmp(p->name, productName) == 0)
        {
            if (p->quantity >= orderQty)
            {
                float total = orderQty * p->price;
                p->quantity -= orderQty;
                printf("Order confirmed!\nProduct: %s\nFarmer: %s\nQuantity: %d kg\nTotal Bill: ₹%.2f\n",
                       p->name, farmer->username, orderQty, total);
                return;
            }
            else
            {
                printf("Only %d kg available. Please reduce quantity.\n", p->quantity);
                return;
            }
        }
        p = p->next;
    }

    printf("Product not found under this farmer.\n");
}

void giveFeedback()
{
    char feedback[200];
    printf("Enter your feedback: ");
    getchar(); // clear newline
    fgets(feedback, sizeof(feedback), stdin);
    FILE *fb = fopen("feedback.txt", "a");
    fprintf(fb, "%s: %s\n", currentUser->username, feedback);
    fclose(fb);
    printf("Thank you for your feedback!\n");
}

int main()
{
    int choice;

    while (1)
    {
        printf("\n=== FARM2HOME DIGITAL MARKETPLACE ===\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        if (currentUser != NULL)
        {
            if (strcmp(currentUser->role, "farmer") == 0)
                printf("3. List Product\n");
            if (strcmp(currentUser->role, "customer") == 0)
            {
                printf("4. Browse Products\n");
                printf("5. Place Order\n");
            }
            printf("6. Give Feedback\n");
            printf("7. Logout\n");
        }
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (!currentUser)
                currentUser = createAccount();
            else
                printf("Already logged in as %s\n", currentUser->username);
            break;
        case 2:
            if (!currentUser)
                currentUser = login();
            else
                printf("Already logged in as %s\n", currentUser->username);
            break;
        case 3:
            if (currentUser && strcmp(currentUser->role, "farmer") == 0)
                listProduct(currentUser);
            else
                printf("Invalid choice.\n");
            break;
        case 4:
            if (currentUser && strcmp(currentUser->role, "customer") == 0)
                browseProducts();
            else
                printf("Invalid choice.\n");
            break;
        case 5:
            if (currentUser && strcmp(currentUser->role, "customer") == 0)
                placeOrder();
            else
                printf("Invalid choice.\n");
            break;
        case 6:
            if (currentUser)
                giveFeedback();
            else
                printf("Login first.\n");
            break;
        case 7:
            if (currentUser)
            {
                printf("Logged out from %s.\n", currentUser->username);
                currentUser = NULL;
            }
            else
            {
                printf("No user is currently logged in.\n");
            }
            break;
        case 8:
            printf("Thank you for using Farm2Home!\n");
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
