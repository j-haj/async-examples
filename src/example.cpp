#include <chrono>
#include <iostream>
#include <random>
#include <queue>
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
  void work(long long n) {
    if (is_prime_slow(n)) std::cout << n << " is prime" << std::endl;
    else std::cout << n << " is not prime" << std::endl;
  }
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
  Worker worker;
  while (!prime_q.empty()) {
    worker.work(prime_q.front());
    prime_q.pop();
  }
  return 0;
}
