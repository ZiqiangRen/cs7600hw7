// For debugging in GDB with threads: methodology 1
//   (gdb) info threads
//   (gdb) thread 2
//   (gdb) # and so on for other threads
//   (gdb) where # each thread has its own stack
//   (gdb) frame 2  # to go to call frame 2

// For debugging in GDB: methodology 2
//   (gdb) break consumer
//   (gdb) run
//   (gdb) print buf
//   (gdb) next
//   (gdb) print buf  # and so on
#include "partial-solution.h"
#include <stdio.h>
#include <pthread.h> // Needed for pthread_mutex_lock(), etc.
#define MAX 64
int buffer[MAX];
int fill = 0;
int use = 0;

typedef struct _sem_t_ {
  int count;
  pthread_mutex_t mutex;
}sem_t;

sem_t sem_producer;  // Should count number of empty slots available
sem_t sem_consumer;  // Should count number of items in the buffer
pthread_mutex_t mut_buf = PTHREAD_MUTEX_INITIALIZER;  // Lock for anybody touching buf


void push_buf(int value){
    buffer[fill] = value;
    fill = (fill+1)%MAX;
    }

int take_from_buf(){
    int tmp = buffer[use];
    use = (use+1)%MAX;
    return tmp;
    }

void *producer(void *arg) {
  int work_item = 1;
  while (1) {
    sleep( rand() % 5 );
    sem_wait(&sem_producer);  // Wait for empty slots
    pthread_mutex_lock(&mut_buf);
      push_buf(work_item++);  // inside critical section with mut_buf lock
    pthread_mutex_unlock(&mut_buf);
    sem_post(&sem_consumer);  // Tell the consumer there's a new work item
  }
}

// Exactly the same, but the inverse:
void *consumer(void *arg) {
  while (1) {
    int work_item;
    sleep( rand() % 5 );
    sem_wait(&sem_consumer);
    pthread_mutex_lock(&mut_buf);
      work_item = take_from_buf();
    pthread_mutex_unlock(&mut_buf);
    sem_post(&sem_producer);

    printf("%d ", work_item);
    fflush(stdout);  // Force printing now; don't wait for the newline
  }
}


int main() {
 pthread_t p, c;
 pthread_t p1, p2, c1, c2;
 sem_init(&sem_producer, 0, MAX);
 sem_init(&sem_consumer, 0, 0);
 pthread_create(&p, NULL, producer, NULL);
 pthread_create(&c, NULL, consumer, NULL);
 pthread_create(&p1, NULL, producer, NULL);
 pthread_create(&c1, NULL, consumer, NULL);
 pthread_create(&p2, NULL, producer, NULL);
 pthread_create(&c2, NULL, consumer, NULL);

 // ... uses pthread_create to start producer and consumer
 // WARNING:  the primary thread runs main().  When main exits, the primary
 //             thread exits
 while (1);  // Don't let the primary thread exit
}




