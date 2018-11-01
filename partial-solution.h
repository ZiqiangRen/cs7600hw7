#include <pthread.h>

struct sem_t {
  int count;
  pthread_mutex_t mutex;
};



int sem_init(struct sem_t *sem, int ignore, int init);


int sem_post(struct sem_t *sem);
    
void block(struct sem_t *sem); // Used by sem_wait()

int sem_wait(struct sem_t *sem);
