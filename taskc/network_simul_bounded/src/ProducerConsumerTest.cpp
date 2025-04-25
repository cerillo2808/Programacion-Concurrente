// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "AssemblerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"

const char* const usage =
  "Usage: netsim packages consumers prod_delay disp_delay cons_delay\n"
  "\n"
  "  packages    number of packages to be produced\n"
  "  consumers   number of consumer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "  loss_prob   probability to loss packages [0, 100]\n"
  "\n"
  "Delays are in milliseconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  for (ProducerTest* producer : this->producers) {
    delete producer;
  }
  delete this->assembler;
  delete this->dispatcher;
  for (ConsumerTest* consumer : this->consumers) {
    delete consumer;
  }
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in this object's attributes
  if (int error = this->analyzeArguments(argc, argv)) {
    return error;
  }
  // Create objects for the simulation
  this->createThreads();
  // Communicate simulation objects
  this->connectQueues();
  // Start the simulation
  this->startThreads();
  // Wait for objects to finish the simulation
  this->joinThreads();
  // Simulation finished
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 7 + 1 arguments are mandatory
  if (argc != 8) {
    std::cout << usage;
    return EXIT_FAILURE;
  }
  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  this->remainingPackageToProduce = this->packageCount;
  this->producerCount = std::strtoull(argv[index++], nullptr, 10);
  this->consumerCount = std::strtoull(argv[index++], nullptr, 10);
  this->productorDelay = std::atoi(argv[index++]);
  this->dispatcherDelay = std::atoi(argv[index++]);
  this->consumerDelay = std::atoi(argv[index++]);
  this->packageLossProbability = std::atof(argv[index++]);
  // TODO(any): Validate that given arguments are fine
  return EXIT_SUCCESS;
}

void ProducerConsumerTest::createThreads() {
  this->producers.resize(this->producerCount);
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index] = new ProducerTest(this->packageCount
      , this->productorDelay
      , this->consumerCount
      , index
      , this->producerCount
      , this->remainingPackageToProduce
      , this->canAccessRemainingPackageToProduce);
    assert(this->producers[index]);
  }
  this->dispatcher = new DispatcherTest(this->dispatcherDelay);
  this->dispatcher->createOwnQueue();
  // Create each consumer
  this->consumers.resize(this->consumerCount);
  for (size_t index = 0; index < this->consumerCount; ++index) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    this->consumers[index]->createOwnQueue();
  }
  this->assembler = new AssemblerTest(this->consumerDelay,
      this->packageLossProbability, this->consumerCount);
  this->assembler->createOwnQueue();
}

void ProducerConsumerTest::connectQueues() {
  // Producer push network messages to the dispatcher queue
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index]->setProducingQueue(
        this->assembler->getConsumingQueue());
  }
  // Dispatcher delivers to each consumer, and they should be registered
  for (size_t index = 0; index < this->consumerCount; ++index) {
    this->dispatcher->registerRedirect(index + 1
      , this->consumers[index]->getConsumingQueue());
  }
  // Assembler returns packages that were not lost to the dispatcher
  this->assembler->setProducingQueue(this->dispatcher->getConsumingQueue());
}

void ProducerConsumerTest::startThreads() {
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index]->startThread();
  }
  this->dispatcher->startThread();
  for (size_t index = 0; index < this->consumerCount; ++index) {
    this->consumers[index]->startThread();
  }
  this->assembler->startThread();
}

void ProducerConsumerTest::joinThreads() {
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index]->waitToFinish();
  }
  this->dispatcher->waitToFinish();
  for (size_t index = 0; index < this->consumerCount; ++index) {
    this->consumers[index]->waitToFinish();
  }
  this->assembler->waitToFinish();
}
