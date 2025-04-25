// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#include "ProducerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

ProducerTest::ProducerTest(size_t packageCount, int productorDelay
  , size_t consumerCount, const size_t producerNumber, const size_t producerCount,
  long long& remainingPackageToProduce,
  std::mutex& canAccessRemainingPackageToProduce)
  : packageCount(packageCount)
  , productorDelay(productorDelay)
  , consumerCount(consumerCount)
  , producerNumber(producerNumber)
  , producerCount(producerCount)
  , remainingPackageToProduce(remainingPackageToProduce)
  , canAccessRemainingPackageToProduce(canAccessRemainingPackageToProduce) {
}

int ProducerTest::run() {
  // Produce each asked message
  size_t myPackageCount = 0;
  while (true) {
    this->canAccessRemainingPackageToProduce.lock();
    const long long myPackageIndex = --this->remainingPackageToProduce;
    this->canAccessRemainingPackageToProduce.unlock();
    if (myPackageIndex >= 0) {
      this->produce(this->createMessage(this->packageCount - myPackageIndex));
      ++myPackageCount;
    } else {
      // Produce an empty message to communicate we finished
      if (static_cast<size_t>(-myPackageIndex) == this->producerCount) {
        this->produce(NetworkMessage());
      }
      break;
    }
  }

  // Report production is done
  Log::append(Log::INFO, "Producer", std::to_string(myPackageCount)
    + " messages sent");
  return EXIT_SUCCESS;
}

NetworkMessage ProducerTest::createMessage(size_t index) const {
  // Source is always 1: this producer
  const uint16_t source = this->producerNumber + 1;
  // Target is selected by random
  const uint16_t target = 1 + Util::random(0
    , static_cast<int>(this->consumerCount));
  // IMPORTANT: This simulation uses sleep() to mimics the process of
  // producing a message. However, you must NEVER use sleep() for real projects
  Util::sleepFor(this->productorDelay);
  // Create and return a copy of the network message
  return NetworkMessage(target, source, index);
}
