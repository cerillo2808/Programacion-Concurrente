// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#ifndef PRODUCERTEST_HPP
#define PRODUCERTEST_HPP

#include <mutex>

#include "NetworkMessage.hpp"
#include "Producer.hpp"

/**
 * @brief A productor class example
 * Produces network messages and push them to the queue
 */
class ProducerTest : public Producer<NetworkMessage> {
  DISABLE_COPY(ProducerTest);

 protected:
  /// Number of packages to be produced
  size_t packageCount = 0;
  /// Delay of producer to create a package, negative for max random
  int productorDelay = 0;
  /// Number of consumer threads
  size_t consumerCount = 0;
  /// Number of producer that I am starting from 0
  size_t producerNumber = 0;
  /// Number of producer threads
  size_t producerCount = 0;
  /// Shared counter for producers
  long long& remainingPackageToProduce;
  /// Protects the counter of remaining packages to be produced
  std::mutex& canAccessRemainingPackageToProduce;

 public:
  /// Constructor
  ProducerTest(size_t packageCount, int productorDelay, size_t consumerCount,
    const size_t producerNumber, const size_t producerCount,
    long long& remainingPackageToProduce,
    std::mutex& canAccessRemainingPackageToProduce);
  /// Do the message production in its own execution thread
  int run() override;
  /// Creates a simulation message to be sent by the network
  NetworkMessage createMessage(size_t index) const;
};

#endif  // PRODUCERTEST_HPP
