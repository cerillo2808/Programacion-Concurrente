// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <climits>
#include <mutex>
#include <queue>

#include "common.hpp"
#include "Semaphore.hpp"

/**
 * @brief A thread-safe generic queue for consumer-producer pattern.
 *
 * @remark None of the methods of this class can be const because all
 * methods require lock the mutex to avoid race-conditions
 */
template <typename DataType>
class Queue {
  DISABLE_COPY(Queue);

 protected:
  /// All read or write operations are mutually exclusive
  std::mutex mutex;
  /// Indicates if there are room to store a new product in the queue
  Semaphore canProduce;
  /// Indicates if there are consumable elements in the queue
  Semaphore canConsume;
  /// Contains the actual data shared between producer and consumer
  std::queue<DataType> queue;

 public:
  /// Constructor
  explicit Queue(const unsigned capacity = SEM_VALUE_MAX)
    : canProduce(capacity)
    , canConsume(0) {
  }

  /// Destructor
  ~Queue() {
  }

  /// Produces an element that is pushed in the queue
  /// The semaphore is increased to wait potential consumers
  void enqueue(const DataType& data) {
    this->canProduce.wait();
    this->mutex.lock();
    this->queue.push(data);
    this->mutex.unlock();
    this->canConsume.signal();
  }

  /// Consumes the next available element. If the queue is empty, blocks the
  /// calling thread until an element is produced and enqueue
  /// @return A copy of the element that was removed from the queue
  DataType dequeue() {
    this->canConsume.wait();
    this->mutex.lock();
    DataType result = this->queue.front();
    this->queue.pop();
    this->mutex.unlock();
    this->canProduce.signal();
    return result;
  }
};

#endif  // QUEUE_HPP
