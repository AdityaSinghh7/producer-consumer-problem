# Multithreaded Producer-Consumer Simulation

## Overview
This project provides a multithreaded C implementation to simulate the classic producer-consumer problem using POSIX threads, mutexes, and condition variables. The program dynamically creates multiple producer and consumer threads based on command line inputs to simulate concurrent production and consumption of items using a shared buffer.

## Project File
- `producer-consumer.c`: This is the main file that contains all the code to setup, execute, and manage the producer-consumer simulation.

## How It Works
Producers generate items and place them in a shared buffer, while consumers remove items from this buffer. The synchronization between producers and consumers ensures that producers do not produce items when the buffer is full, and consumers do not consume when the buffer is empty.

## Compilation and Execution
To compile and run this program, you will need a C compiler that supports POSIX threads. Use the following command to compile:
```gcc producer-consumer.c -o producer-consumer -lpthread```

To run the program, you need to provide five command-line arguments:
```./producer-consumer <numOfProducers> <numOfConsumers> <numOfItemsForEachProducer> <bufferSize> <delayArg>```

Where:
<numOfProducers> is the number of producer threads.
<numOfConsumers> is the number of consumer threads.
<numOfItemsForEachProducer> is the number of items each producer will produce.
<bufferSize> is the size of the shared buffer (maximum is 10).
<delayArg> if set to 1, producers will delay after each production; if 0, consumers will delay after each consumption.

## Example
To start a simulation with 3 producers, 2 consumers, each producer producing 5 items, a buffer size of 10, and a delay for producers:
```./producer-consumer 3 2 5 10 1```

## Constraints
1. Maximum buffer size (bufferSize) is defined as 10.
2. Maximum number of threads (either producers or consumers) is 16.
3. The number of consumers should not exceed the total number of items produced by all producers.

## Dependencies
1. POSIX Threads Library for thread management.
2. Standard C libraries.
