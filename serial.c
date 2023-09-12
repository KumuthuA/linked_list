#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct Node {
    int data;
    struct Node* next;
};

int Member(struct Node* head, int value) {
    struct Node* current = head;
    while (current != NULL) {
        if (current->data == value) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}

void Insert(struct Node** head, int value) {
    // Check if the value is already in the linked list
    struct Node* current = *head;
    while (current != NULL) {
        if (current->data == value) {
            return;
        }
        current = current->next;
    }

    // Allocate memory for a new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = *head;
    *head = newNode;
}

void Delete(struct Node** head, int value) {
    struct Node* current = *head;
    struct Node* prev = NULL;

    while (current != NULL) {
        if (current->data == value) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void PrintList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    srand(time(NULL)); // Seed the random number generator with current time


    int n, numOperations;
    double mMember, mInsert, mDelete;

    printf("Enter the number of initial unique values (n): ");
    scanf("%d", &n);

    printf("Enter the total number of operations (m): ");
    scanf("%d", &numOperations);

    printf("Enter the fraction of Member operations: ");
    scanf("%lf", &mMember);

    printf("Enter the fraction of Insert operations: ");
    scanf("%lf", &mInsert);

    printf("Enter the fraction of Delete operations: ");
    scanf("%lf", &mDelete);
    
    int numMember = mMember * numOperations;
    int numInsert = mInsert * numOperations;
    int numDelete = mDelete * numOperations; 

    printf("No. of member operations : %d\n", numMember); 
    printf("No. of insert operations : %d\n", numInsert); 
    printf("No. of delete operations : %d\n", numDelete); 

    int sample_size = 385;

    clock_t start_time, end_time;
    double total_time = 0.0;
    double squared_total_time = 0.0;
    double elapsed_times[sample_size]; // Array to store elapsed times

    for (int iteration = 0; iteration < sample_size; iteration++) {

        struct Node* head = NULL; // Initialize an empty linked list

        // Populate the linked list with n random, unique values
        for (int i = 0; i < n; i++) {
            int value;
            do {
                value = rand() % ((1 << 16) - 1); 
            } while (Member(head, value)); 
            Insert(&head, value);
        }

        start_time = clock();

        for (int i = 0; i < numMember; i++) {
            // Generate and perform Member operation
            int target = rand() % (1 << 16);
            Member(head, target);
            numOperations--;
        }

        for (int i = 0; i < numInsert; i++) {
            // Generate and perform Insert operation
            int newValue = rand() % (1 << 16);
            Insert(&head, newValue);
            numOperations--;
        }

        for (int i = 0; i < numDelete; i++) {
            // Generate and perform Delete operation
            int target = rand() % (1 << 16);
            Delete(&head, target);
            numOperations--;
        }

        end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        elapsed_times[iteration] = elapsed_time;
        total_time += elapsed_time;

        printf("Iteration : %d, Elapsed time : %lf seconds\n", iteration, elapsed_time);

        while (head != NULL) {
            struct Node* temp = head;
            head = head->next;
            free(temp);
        }
    }
        
    // Calculate the mean time
    double mean_time = total_time / sample_size;
    
    // Calculate the standard deviation
    double variance = 0;
    for (int i = 0; i < sample_size; i++) {
        variance += pow(elapsed_times[i] - mean_time, 2);
    }
    double std_deviation = sqrt(variance / sample_size);

    printf("Mean Time: %lf seconds\n", mean_time);
    printf("Standard Deviation of Time: %lf seconds\n", std_deviation);    
}