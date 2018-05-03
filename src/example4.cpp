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
  os << "[ "; while (!tmp.empty()) {
    os << tmp.front() << ' ';
    tmp.pop();
  }
  os << ']';
  return os;
}

std::pair<long long, bool> is_prime_slow(std::future<long long> n) {
  bool result = true;
  auto val = n.get();
  for (long long i = 2; i < val; ++i) {
    if (val % i == 0) result = false;
  }
  return std::make_pair(val, result);
}

void random_sleep() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<long> dist(10000, 1000000);
  struct timespec tspec {tv_sec: static_cast<time_t>(kSleepDuration), tv_nsec: dist(gen)};
  nanosleep(&tspec, NULL);
}

long long slow_random_number(long long low, long long high) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  random_sleep();
  std::uniform_int_distribution<long long> dist(low, high);
  return dist(gen);
}

int main() {
  using steady_clock = std::chrono::steady_clock;
  auto start = steady_clock::now(); 
  std::queue<std::future<long long>> prime_q;
  for (int i = 0; i < 10; i++) {
    auto val = std::async(std::launch::async, slow_random_number, 1000000, 100000000);
    prime_q.push(std::move(val));
  }
  std::vector<std::future<std::pair<long long, bool>>> futures;
  while (!prime_q.empty()) {
    auto f = std::async(std::launch::async, is_prime_slow, std::move(prime_q.front()));
    futures.push_back(std::move(f));
    prime_q.pop();
  }
  // Wait for all threads to finish
  for (auto& t : futures) {
    auto r = t.get();
    std::cout << r.first << (r.second ? " is " : " is not ") << "prime\n";
  }

  auto stop = steady_clock::now();
  std::chrono::duration<double> elapsed = stop - start;
  std::cout << "Total elapsed time " << elapsed.count() << '\n';
  return 0;
}
