// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#ifndef PRODUCERCONSUMERTEST_HPP
#define PRODUCERCONSUMERTEST_HPP

#include <climits>
#include <cstddef>
#include <mutex>
#include <vector>

#include "common.hpp"

// Forward declarations
class AssemblerTest;
class ConsumerTest;
class DispatcherTest;
class ProducerTest;

/**
 * @brief Controller class that simulates the message passing between
 * producer and consumers through a dispatcher.
 */
class ProducerConsumerTest {
  DISABLE_COPY(ProducerConsumerTest);

 private:
  /// Number of packages to be produced
  size_t packageCount = 0;
  /// Number of producer threads
  size_t producerCount = 0;
  /// Number of consumer threads
  size_t consumerCount = 0;
  /// Delay of producer to create a package, negative for max random
  int productorDelay = 0;
  /// Delay of dispatcher to dispatch a package, negative for max random
  int dispatcherDelay = 0;
  /// Delay of consumer to consume a package, negative for max random
  int consumerDelay = 0;
  /// Probability of assembler to lose packages
  double packageLossProbability = 0.0;
  /// Default capacity of all queues
  unsigned queueCapacity = SEM_VALUE_MAX;

 private:
  /// Shared counter for producers
  long long remainingPackageToProduce = 0;
  /// Protects the counter of remaining packages to be produced
  std::mutex canAccessRemainingPackageToProduce;
  /// Producer of the simulated network messages
  std::vector<ProducerTest*> producers;
  /// A dispatcher of the of the simulated network messages
  DispatcherTest* dispatcher = nullptr;
  /// Consumers of the simulated network messages
  std::vector<ConsumerTest*> consumers;
  /// Assembler that can lose network packages
  AssemblerTest* assembler = nullptr;

 public:
  /// Constructor
  ProducerConsumerTest() = default;
  /// Destructor
  ~ProducerConsumerTest();
  /// Start the simulation
  int start(int argc, char* argv[]);

 private:
  /// Analyze the command line arguments
  int analyzeArguments(int argc, char* argv[]);
  /// Create network threads: producers, consumers, assemblers and dispatchers
  void createThreads();
  /// Intercommunicate threads using queues
  void connectQueues();
  /// Start the execution of threads
  void startThreads();
  /// Wait (join) the secondary threads
  void joinThreads();
};

#endif  // PRODUCERCONSUMERTEST_HPP
