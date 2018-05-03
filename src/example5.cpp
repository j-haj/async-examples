#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <set>
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

bool is_prime_fast(long long n) {
  std::set<long long> known_primes {3, 5, 7, 11, 13, 17, 23, 29, 31, 37};
  if (n % 2 == 0) return false;
  for (const auto& p : known_primes) {
    if (n % p == 0) return false;
  }
  for (long long i = 37; i < static_cast<long long>(std::sqrt(n)); i += 2) {
    if (n % i == 0) return false;
  }
  return true;
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
    if (is_prime_fast(n)) std::cout << n << " is prime" << std::endl;
    else std::cout << n << " is not prime" << std::endl;
  }
};

int main() {
  using steady_clock = std::chrono::steady_clock;

  auto start = steady_clock::now();

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

  auto stop = steady_clock::now();
  std::chrono::duration<double> elapsed = stop - start;
  std::cout << "Total elapsed time " << elapsed.count() << '\n';
  return 0;
}
