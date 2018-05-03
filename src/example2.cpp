#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <queue>
#include <vector>
#include <thread>
#include <time.h>

constexpr unsigned int kSleepDuration = 1;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::queue<T>& q) {
  std::queue<T> tmp(q);
  os << "[ ";
  while (!tmp.empty()) {
    os << tmp.front() << ' ';
    tmp.pop();
  }
  os << ']';
  return os;
}

bool is_prime_slow(long long n) {
  bool result = true;
  for (long long i = 2; i < n; ++i) {
    if (n % i == 0) result = false;
  }
  return result;
}

void random_sleep() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<long> dist(10000, 1000000);
  struct timespec tspec {tv_sec: static_cast<time_t>(kSleepDuration), tv_nsec: dist(gen)};
  nanosleep(&tspec, NULL);
}

class Producer {
  public:
  Producer(long long low, long long high) : low_(low), high_(high) {}
  long long generate() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    random_sleep();
    std::uniform_int_distribution<long long> dist(low_, high_);
    return dist(gen);
  }

  private:
  long long low_;
  long long high_;
};

class Worker {
  public:
  Worker(long long n) : n_(n) {}

  void operator()() {
    if (is_prime_slow(n_)) std::cout << n_ << " is prime" << std::endl;
    else std::cout << n_ << " is not prime" << std::endl;
  }
  private:
  long long n_;
};

int main() {

  std::queue<long long> prime_q;
  Producer producer(1000000, 100000000);
  for (int i = 0; i < 10; i++) {
    auto val = producer.generate();
    std::cout << "Produced " << val << '\n';
    prime_q.push(val);
  }
  std::cout << prime_q << '\n';
  std::vector<std::thread> threads;
  while (!prime_q.empty()) {
    Worker w(prime_q.front());
    std::thread t(w);
    threads.push_back(std::move(t));
    prime_q.pop();
  }
  // Wait for all threads to finish
  for (auto& t : threads) {
    t.join();
  }
  return 0;
}
