#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <queue>
#include <vector>
#include <future>
#include <utility>
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

std::pair<long long, bool> is_prime_slow(long long n) {
  bool result = true;
  for (long long i = 2; i < n; ++i) {
    if (n % i == 0) result = false;
  }
  return std::make_pair(n, result);
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

int main() {

  std::queue<long long> prime_q;
  Producer producer(1000000, 100000000);
  for (int i = 0; i < 10; i++) {
    auto val = producer.generate();
    std::cout << "Produced " << val << '\n';
    prime_q.push(val);
  }
  std::cout << prime_q << '\n';
  std::vector<std::future<std::pair<long long, bool>>> threads;
  while (!prime_q.empty()) {
    auto f = std::async(std::launch::async, is_prime_slow, prime_q.front());;
    threads.push_back(std::move(f));
    prime_q.pop();
  }
  // Wait for all threads to finish
  for (auto& t : threads) {
    auto r = t.get();
    std::cout << r.first << (r.second ? " is " : " is not ") << "prime\n";
  }
  return 0;
}
