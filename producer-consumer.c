#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_NUMBER_THREADS 16
#define MAX_BUFFER_SIZE 10

int numOfProducers;
int numofConsumers;
int numOfItemsforEachProducer;
int myBufferSize;
int delayArg;

volatile int totalNumberOfItemsProduced = 0;
volatile int totalNumberOfItemsConsumed = 0;
int numberOfItemsConsumedPerConsumer;

int *sharedGlobalBuffer;
// volatile int count = 0;
volatile int in = 0, out = 0;
pthread_mutex_t mutex;
pthread_cond_t empty, full;

void *producer(void *args) {
  int myThreadID = *(int *)args;
  free(args);

  for (int i = 0; i < numOfItemsforEachProducer; i++) {
    pthread_mutex_lock(&mutex);
    while (in == out &&
           totalNumberOfItemsProduced - totalNumberOfItemsConsumed != 0) {
      pthread_cond_wait(&empty, &mutex);
    }
    int item = totalNumberOfItemsProduced++;
    sharedGlobalBuffer[in] = item;
    in = (in + 1) % myBufferSize;

    printf("producer_%d produced item %d\n", myThreadID, item);
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);
    if (delayArg == 1) {
      usleep(500000);
    }
  }
  pthread_exit(NULL);
  return NULL;
}

void *consumer(void *args) {
  int myThreadID = *(int *)args;
  free(args);

  for (int i = 0; i < numberOfItemsConsumedPerConsumer; i++) {
    pthread_mutex_lock(&mutex);

    while (in == out &&
           totalNumberOfItemsProduced - totalNumberOfItemsConsumed == 0) {
      pthread_cond_wait(&full, &mutex);
    }

    int item = sharedGlobalBuffer[out];
    out = (out + 1) % myBufferSize;
    printf("consumer_%d consumed item %d\n", myThreadID, item);
    totalNumberOfItemsConsumed++;
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);

    if (delayArg == 0) {
      usleep(500000);
    }
  }
  pthread_exit(NULL);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 6) {
    printf("Invalid number of command-line arguments\n");
    return 1;
  }

  numOfProducers = atoi(argv[1]);
  numofConsumers = atoi(argv[2]);
  numOfItemsforEachProducer = atoi(argv[3]);
  myBufferSize = atoi(argv[4]);
  delayArg = atoi(argv[5]);

  if (myBufferSize > MAX_BUFFER_SIZE || numofConsumers > MAX_NUMBER_THREADS ||
      numOfProducers > MAX_NUMBER_THREADS ||
      numofConsumers > numOfProducers * numOfItemsforEachProducer) {
    printf("Arguments exceed maximum boundaries\n");
    return 1;
  }

  // totalNumberOfItemsProduced = numOfProducers * numOfItemsforEachProducer;
  sharedGlobalBuffer = (int *)malloc(myBufferSize * sizeof(int));
  numberOfItemsConsumedPerConsumer =
      (numOfProducers * numOfItemsforEachProducer) / numofConsumers;

  if (sharedGlobalBuffer == NULL) {
    printf("Memory allocation failed!");
    return 1;
  }

  pthread_t producerThreads[numOfProducers];
  pthread_t consumerThreads[numofConsumers];
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&full, NULL);
  pthread_cond_init(&empty, NULL);
  // int *producerThreadIDs[numOfProducers];
  // int *consumerThreadIDs[numofConsumers];

  for (int i = 0; i < numOfProducers; i++) {
    int *producerThreadID = malloc(sizeof(int));
    *producerThreadID = i;
    pthread_create(&producerThreads[i], NULL, producer, producerThreadID);
  }

  for (int i = 0; i < numofConsumers; i++) {
    int *consumerThreadID = malloc(sizeof(int));
    *consumerThreadID = i;
    pthread_create(&consumerThreads[i], NULL, consumer, consumerThreadID);
  }

  for (int i = 0; i < numOfProducers; i++) {
    pthread_join(producerThreads[i], NULL);
  }

  for (int i = 0; i < numofConsumers; i++) {
    pthread_join(consumerThreads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&empty);
  pthread_cond_destroy(&full);
  free(sharedGlobalBuffer);

  return 0;
}