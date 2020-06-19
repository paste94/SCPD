//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <iostream>


void *producer(void *);
void *consumer(void *);


using namespace std;


// This is the two-locks Michael and Scott queue. PODC 1996.

template <typename T> class  queue {
private:
  
  struct node {
    T value;
    struct node * next;
  };

  struct node *head;
  struct node *tail;
  pthread_mutex_t h_mutex, t_mutex;

public:
  queue() {
    pthread_mutex_init(&h_mutex,NULL);
    pthread_mutex_init(&t_mutex,NULL);
    struct node * n = new struct node();
    n->next = NULL;
    head = tail = n;	
  };
    
  bool pop( T &e) {
    struct node * n;
    pthread_mutex_lock(&h_mutex);
    n = head;
    struct node * new_head = n->next; 
    if (new_head == NULL) {
      pthread_mutex_unlock(&h_mutex);
      return false;
    }
    e = new_head->value;
    head = new_head;
    pthread_mutex_unlock(&h_mutex);
    delete(n);
    return true;
  }
 
  void push(const T & elem) {
    struct node * n = new struct node();
    n->value = elem;
    n->next = NULL;
    pthread_mutex_lock(&t_mutex);
    tail->next = n;
    tail = n;
    pthread_mutex_unlock(&t_mutex);
  }

};



int main()
{
  pthread_t thread1, thread2, thread3, thread4;
  queue<int> * q = new queue<int>(); 
   

  pthread_create( &thread1, NULL,  (void* (*)(void*)) &producer, (void *) q);
  pthread_create( &thread2, NULL, ( void* (*)(void*)) &consumer, (void *) q);
  pthread_create( &thread3, NULL,  (void* (*)(void*)) &producer, (void *) q);
  pthread_create( &thread4, NULL,  (void* (*)(void*)) &producer, (void *) q);
  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);
  pthread_join( thread3, NULL);
  pthread_join( thread4, NULL);
  return(0);
}



void *producer(void * arg) {
  queue<int> * q = (queue<int> *) arg;
  int count = 0;
  for(;;) {
    //sleep(5);
    q->push(count++);
  }
}

void *consumer(void * arg) {
  queue<int> * q = (queue<int> *) arg;
  for(;;) {
    //sleep(2);
    int value;
    if (q->pop(value)) {
      cout << value << "\n";
    } else {
      cout << "empty\n";
    }
  }
}

// esercizio: FIFO(k)
