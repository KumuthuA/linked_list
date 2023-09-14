
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>


int n = 0;
int m = 0;
int num_thread = 0;
float m_insert_frac = 0.0, m_delete_frac = 0.0, m_member_frac = 0.0;
int m_insert = 0, m_delete = 0, m_member = 0;
struct Node *head = NULL;
pthread_rwlock_t rwlock;

//Node
struct Node {
    int data;
    struct Node *next;
};

void Insert(struct Node **head, int value);

void Delete(struct Node **head, int value);

int Member(struct Node *head, int value);

double CalcTime(struct timeval time_begin, struct timeval time_end);

void *Thread_function(void *id);

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printf("Enter correct arguments n,m,threads,m_mem,m_inse,m_del");
        exit(0);
    }
    // Setting the input values of n,m and thread count
    n = (int) strtol(argv[1], (char **) NULL, 10);
    m = (int) strtol(argv[2], (char **) NULL, 10);
    num_thread = (int) strtol(argv[3], (char **) NULL, 10);

    // Setting the input values of operation fraction values
    m_member_frac = (float) atof(argv[4]);
    m_insert_frac = (float) atof(argv[5]);
    m_delete_frac = (float) atof(argv[6]);
    pthread_t *thread_handlers;
    thread_handlers = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);
    clock_t start_time, end_time;
    double elapsed_time;

    int *thread_id;
    thread_id = (int *)malloc(sizeof(int) * num_thread);

    m_insert = (int) (m_insert_frac * m);
    m_delete = (int) (m_delete_frac * m);
    m_member = (int) (m_member_frac * m);

    // Linked List Generation 
    for(int i=0;i<n;i++) {
        Insert(&head,rand() % 65535);
    }

    pthread_rwlock_init(&rwlock, NULL);
    start_time = clock();
    // Thread Creation
    for (int i=0;i < num_thread;i++) {
        thread_id[i] = i;
        pthread_create(&thread_handlers[i], NULL, (void *) Thread_function, (void *) &thread_id[i]);   
    }
    // Thread Join
   for (int i=0;i < num_thread;i++) {
        pthread_join(thread_handlers[i], NULL);
    }
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    pthread_rwlock_destroy(&rwlock);

    printf("Linked List with read write locks Time Spent : %.6f\n", elapsed_time);
    FILE *fpt;

    // Open the file pointer in write mode.
    fpt = fopen("linked_list_with_read_write_lock_case_3_thread8.csv", "a");

    // Write the data to the file pointer, separated by commas.
    fprintf(fpt,"%f\n", elapsed_time);
    // Close the file pointer.
    fclose(fpt);
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
    //printf("delete");
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
void *Thread_function(void *thread_id) {
    int local_m = 0;int local_m_insert = 0;int local_m_delete = 0;int local_m_member = 0;

    int id = *(int *)thread_id;

    // Generate local no of insertion operations without loss
    if (m_insert % num_thread == 0 || m_insert % num_thread <= id) {
        local_m_insert = m_insert / num_thread;
    }
    else if (m_insert % num_thread > id) {
        local_m_insert = m_insert / num_thread + 1;
    }
    if (m_member % num_thread == 0 || m_member % num_thread <= id) {
        local_m_member = m_member / num_thread;
    }
    else if (m_member % num_thread> id) {
        local_m_member = m_member /num_thread + 1;
    }

    if (m_delete % num_thread == 0 || m_delete % num_thread <= id) {
        local_m_delete = m_delete / num_thread;
    }
    else if (m_delete % num_thread > id) {
        local_m_delete = m_delete / num_thread + 1;
    }

    local_m = local_m_insert + local_m_delete + local_m_member;

    int tot_operations = 0;
    int tot_member_operations = 0;
    int tot_insert_operations = 0;
    int tot_delete_operations = 0;

    int i = 0;
    while (tot_operations < local_m) {
        int random_value = rand() % 65535;
        // Member operation
        if (tot_member_operations < local_m_member) {
            pthread_rwlock_rdlock(&rwlock);
            Member(head,random_value);
            pthread_rwlock_unlock(&rwlock);
            tot_member_operations++;
            }
        // Insert Operation
        else if (tot_insert_operations < local_m_insert) {
            pthread_rwlock_wrlock(&rwlock);
            Insert(&head,random_value);
            pthread_rwlock_unlock(&rwlock);
            tot_insert_operations++;
        }
        // Delete Operation
        else if (tot_delete_operations < local_m_delete){
            pthread_rwlock_wrlock(&rwlock);
            Delete(&head,random_value);
            pthread_rwlock_unlock(&rwlock);
            tot_delete_operations++;
        } 
        tot_operations = tot_insert_operations + tot_member_operations + tot_delete_operations;
        i++;
    }
    return NULL;
}