// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#include "AssemblerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

AssemblerTest::AssemblerTest(int consumerDelay, double packageLossProbability,
    size_t consumerCount)
  : consumerDelay(consumerDelay)
  , packageLossProbability(packageLossProbability)
  , consumerCount(consumerCount) {
}

int AssemblerTest::run() {
  // Start the forever loop to consume all the messages that arrive
  this->consumeLoop();
  this->produce(NetworkMessage());

  // If the forever loop finished, no more messages will arrive
  // Print statistics
  Log::append(Log::INFO, "Assembler", std::to_string(this->lostMessages)
    + " messages lost");
  return EXIT_SUCCESS;
}

void AssemblerTest::consume(NetworkMessage data) {
  if (Util::random(0.0, 100.0) < this->packageLossProbability) {
    ++this->lostMessages;
  } else {
    // data.target = Util::random(1, this->consumerCount);
    this->produce(data);
  }
}
