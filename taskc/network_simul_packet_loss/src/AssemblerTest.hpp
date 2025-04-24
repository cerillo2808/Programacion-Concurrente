// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#ifndef ASSEMBLERTEST_HPP
#define ASSEMBLERTEST_HPP

#include "Assembler.hpp"
#include "NetworkMessage.hpp"

/**
 * @brief A Assembler class example
 */
class AssemblerTest : public Assembler<NetworkMessage, NetworkMessage> {
  DISABLE_COPY(AssemblerTest);

 protected:
  /// Delay of consumer to consume a package, negative for max random
  int consumerDelay = 0;
  /// Probability of assembler to lose packages
  double packageLossProbability = 0.0;
  /// Number of consumer threads
  size_t consumerCount = 0;
  /// Number of messages received
  size_t lostMessages = 0;

 public:
  /// Constructor
  explicit AssemblerTest(int consumerDelay, double packageLossProbability,
      size_t consumerCount);
  /// Consume the messages in its own execution thread
  int run() override;
  /// Override this method to process any data extracted from the queue
  void consume(NetworkMessage data) override;
};

#endif  // ASSEMBLERTEST_HPP
