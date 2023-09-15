#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct Node {
    int data;
    struct Node *next;
};

int n;                 //number of values
int m;                 //number of operations in each execution
int thread_count;       //number of threads

float m_member, m_insert, m_delete;

int num_member, num_insert, num_delete;

int count, count_m, count_i, count_d = 0; //to store the number of operations executed

struct Node* head = NULL;

pthread_mutex_t mutex;

void Insert(struct Node** head_pp, int value);

void Delete(struct Node** head_pp, int value);

int Member(struct Node* head_p, int value);

void *Thread_function();

int main(int argc, char *argv[]) {   
    pthread_t *thread_handlers;
    thread_handlers = malloc(thread_count * sizeof(pthread_t));

    clock_t start_time, end_time;
    double elapsed_time;

    srand(time(NULL));

    if (argc != 7) {
        fprintf(stderr,
                "Enter correct arguments n,m,threads,m_mem,m_inse,m_del\n");
        exit(0);
    }

    n = (int) strtol(argv[1], NULL, 10);
    m = (int) strtol(argv[2], NULL, 10);
    thread_count = strtol(argv[3], NULL, 10);
    m_member = (float) atof(argv[4]);
    m_insert = (float) atof(argv[5]);
    m_delete = (float) atof(argv[6]);
    

    num_member =(int) (m_member * m);
    num_insert = (int) (m_insert * m);
    num_delete = m-num_member-num_insert;
    // Populate the linked list with n random, unique values
    for (int i = 0; i < n; i++) {
        int value;
        do {
            value = rand() % ((1 << 16) - 1); // Generate a random value between 0 and 2^16 - 1
        } while (Member(head, value)); // Ensure it's unique
        Insert(&head, value);
    }
    
    start_time = clock();

    // Initializing the mutex
    pthread_mutex_init(&mutex, NULL);

    // Thread Creation
  

    for (int i=0;i < thread_count;i++) {
        pthread_create(&thread_handlers[i], NULL, (void *) Thread_function, NULL);
       
    }

    // Thread Join
   
    for (int i=0;i < thread_count;i++) {
        pthread_join(thread_handlers[i], NULL);
    }

    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Elapsed time : %f\n", elapsed_time);

    

    FILE *fpt;

    // Open the file pointer in write mode.
    fpt = fopen("linked_list_with_one_mutex_case_1_thread4.csv", "a");

    // Write the data to the file pointer, separated by commas.
    fprintf(fpt,"%f\n", elapsed_time);
    // Close the file pointer.
    fclose(fpt);

    pthread_mutex_destroy(&mutex);
    free(thread_handlers);

    return 0;
}

void *Thread_function() {
    while (count < m) {
        if (count_m < num_member){
            pthread_mutex_lock(&mutex);
            Member(head, rand() % (1 << 16));
            count++;
            count_m++;
            pthread_mutex_unlock(&mutex);
        } else if (count_i < num_insert ) {
            pthread_mutex_lock(&mutex);
            Insert(&head, rand() % (1 << 16));
            count++;
            count_i++;
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_lock(&mutex);
            Delete(&head, rand() % (1 << 16));
            count++;
            pthread_mutex_unlock(&mutex);
        }   
        // printf("Count : %d\n", count) ;    
    }
    return NULL;
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
    struct Node* current = *head;

    while (current != NULL) {
        if (current->data == value) {
            return;
        }
        current = current->next;
    }

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