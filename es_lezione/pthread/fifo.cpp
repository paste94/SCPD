//#include <stdio.h>
//#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <unistd.h>


void *functionCount1(void *);
void *functionCount2(void *);


using namespace std;


// This is the Lamport's FIFO, asynchrony is n-1. It does not requires the lock under sequential consistency.
template <typename T> class  fifo_buffer {
private:
  pthread_mutex_t fifo_mutex;
  pthread_cond_t  fifo_cond;
  int size;
  int head,tail;
  bool full; // useless, only to simplify the code
  bool empty; // useless, only to simplify the code
  vector< T > buf;
public:
  fifo_buffer(int n): size(n),head(0),tail(0),full(false),empty(true),buf(n) {
	pthread_mutex_init(&fifo_mutex,NULL);
	pthread_cond_init(&fifo_cond,NULL);
  };
  
  T pop() {
	T e;
	pthread_mutex_lock(&fifo_mutex);
	while (head==tail) {
	  // FIFO empty
	  cout << "FIFO empty\n";
	  empty = true;
	  pthread_cond_wait( &fifo_cond, &fifo_mutex );
	}
	cout << "FIFO pop " << head << "\n";
	e = buf[tail];
	if (full) {
	  // FIFO previously full
	  full = false;
	  pthread_cond_broadcast( &fifo_cond);
	}
	tail = ++tail%size;
	pthread_mutex_unlock(&fifo_mutex);
	return e;
  }

  void push(const T & elem) {
	pthread_mutex_lock(&fifo_mutex);
	int next = (1+head)%size;
	while ( next==tail ) {
	  // FIFO full
	  cout << "FIFO full\n";
	  full = true;
	  pthread_cond_wait( &fifo_cond, &fifo_mutex );
	}
	cout << "FIFO push " << head << "\n";
	buf[head] = elem;
	if (empty) {
	  // FIFO previously empty
	  empty = false;
	  pthread_cond_broadcast( &fifo_cond);
	}
	head = next;
	pthread_mutex_unlock(&fifo_mutex);
  }
};



int main()
{
  pthread_t thread1a, thread1b, thread1c, thread2a, thread2b;
   fifo_buffer<int> * b = new fifo_buffer<int>(5); // asynchrony is n-1
   

   pthread_create( &thread1a, NULL,  (void* (*)(void*)) &functionCount1, (void *) b);
   pthread_create( &thread1b, NULL,  (void* (*)(void*)) &functionCount1, (void *) b);
   pthread_create( &thread1c, NULL,  (void* (*)(void*)) &functionCount1, (void *) b);
   pthread_create( &thread2a, NULL, ( void* (*)(void*)) &functionCount2, (void *) b);
   pthread_create( &thread2b, NULL, ( void* (*)(void*)) &functionCount2, (void *) b);
   pthread_join( thread1a, NULL);
   pthread_join( thread1b, NULL);
   pthread_join( thread1c, NULL);
   pthread_join( thread2a, NULL);
   pthread_join( thread2b, NULL);

   exit(0);
}



void *functionCount1(void * arg) {
  fifo_buffer<int> * b = (fifo_buffer<int> *) arg;
  int count = 0;
  for(;;) {
    sleep(1);
    b->push(count++);
  }
}

void *functionCount2(void * arg) {
  fifo_buffer<int> * b = (fifo_buffer<int> *) arg;
  for(;;) {
    sleep(2);
    cout << b->pop() << "\n";
  }
}

// esercizio: FIFO(k)
