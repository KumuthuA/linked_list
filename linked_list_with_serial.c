#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


struct Node {
    int data;
    struct Node* next;
};

// Function prototypes
int Member(struct Node* head, int value);
void Insert(struct Node** head, int value);
void Delete(struct Node** head, int value);
void PrintList(struct Node* head);

int n = 0;
int m = 0;
float m_insert_frac = 0.0, m_delete_frac = 0.0, m_member_frac = 0.0;
int m_insert = 0, m_delete = 0, m_member = 0;
struct Node *head = NULL;


int main(int argc, char *argv[]) {
    srand(time(NULL)); // Seed the random number generator with current time
     if (argc != 6) {
        printf("Enter correct arguments n,m,threads,m_mem,m_inse,m_del");
        exit(0);
    }
    // Setting the input values of n,m and thread count
    n = (int) strtol(argv[1], (char **) NULL, 10);
    m = (int) strtol(argv[2], (char **) NULL, 10);

    // Setting the input values of operation fraction values
    m_member_frac = (float) atof(argv[3]);
    m_insert_frac = (float) atof(argv[4]);
    m_delete_frac = (float) atof(argv[5]);

    int numMember = m_member_frac * m;
    int numInsert = m_insert_frac * m;
    int numDelete = m_delete_frac * m; 
    clock_t start_time, end_time;
    double elapsed_time;
   
    struct Node* head = NULL; // Initialize an empty linked list

    // Populate the linked list with n random, unique values
    for (int i = 0; i < n; i++) {
        int new_value;
        do {
            new_value = rand() % ((1 << 16) - 1); 
        } while (Member(head, new_value)); 
        Insert(&head, new_value);
    }

    start_time = clock();

    for (int i = 0; i < numMember; i++) {
        // Generate and perform Member operation
        int new_value = rand() % (1 << 16);
        Member(head, new_value);
        m--;
    }

    for (int i = 0; i < numInsert; i++) {
        // Generate and perform Insert operation
        int new_value = rand() % (1 << 16);
        Insert(&head, new_value);
        m--;
    }

    for (int i = 0; i < numDelete; i++) {
        // Generate and perform Delete operation
        int new_value = rand() % (1 << 16);
        Delete(&head, new_value);
        m--;
    }

    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Linked List with serial : %.6f\n", elapsed_time); 
    FILE *fpt;

    // Open the file pointer in write mode.
    fpt = fopen("linked_list_with_serial_case_3.csv", "a");

    // Write the data to the file pointer, separated by commas.
    fprintf(fpt,"%f\n", elapsed_time);
    // Close the file pointer.
    fclose(fpt);
    
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }   
    return 0;   
}
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
