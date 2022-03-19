// C++11
#include <bits/stdc++.h>
#include <sys/time.h>

#ifdef LOCAL
#include "../logger.hpp"
#define LOG_TC(...) LOG_INFO(__VA_ARGS__);
#else
#define LOG_DEBUG(...);
#define LOG_INFO(...);
#define LOG_WARNING(...);
#define LOG_ERROR(...);
#define LOG_TC(...) {fprintf(stderr, __VA_ARGS__);fprintf(stderr, "\n"); }
#define PANIC(...);
#define ASSERT(...);
#endif

// function to get the time
static inline double get_time() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

const double END_TIME =  179.0;
double start_time = get_time();
double end_time = start_time + END_TIME;

// this is a random generator number 
struct rand_gen {
  static constexpr uint32_t kMax = 2147483647;
  static constexpr double kInvMax = 1.0 / (double)kMax;
  static constexpr __uint128_t kAdd = 0x60bee2bee120fc15ull;
  static constexpr __uint128_t kMul1 = 0xa3b195354a39b70dull;
  static constexpr __uint128_t kMul2 = 0x1b03738712fad5c9ull;
  uint64_t state = 42;
  inline void seed(uint64_t seed) { state = seed; }
  inline uint32_t next_int() {
    state += kAdd; __uint128_t tmp = (__uint128_t)state * kMul1;
    uint64_t m1 = (tmp >> 64) ^ tmp; tmp = (__uint128_t)m1 * kMul2;
    uint64_t m2 = (tmp >> 64) ^ tmp; return (uint32_t)m2;
  }
  inline uint32_t next_int(uint32_t max) { return next_int()%max; }
  inline uint32_t next_int(uint32_t min, uint32_t max) { return min + (next_int()%(max-min)); }
  inline double next_float() { return next_int(kMax) * kInvMax; }
} rng;

template <int Size = 1024>
class FastMap { 
  private:
    std::array<int, Size> pos_;
    std::array<int, Size> taken_;
    int size_;

  public:
    FastMap() { size_ = 0; std::fill(pos_.begin(), pos_.end(), -1); }
    void clear() {while (size_) pos_[taken_[--size_]] = -1;}

    inline void safeInsert(int v) {if (!has(v)) insert(v);}
    inline void insert(int v) {taken_[pos_[v] = size_++] = v;}
    inline void push_back(int v) {taken_[pos_[v] = size_++] = v;}
    inline void pop_back() {pos_[taken_[--size_]] = -1;}
    inline int back() {return taken_[size_-1];}
    inline void erase(int v) {
        const int p = pos_[v];
        const int last = taken_[--size_];
        pos_[last] = p;
        taken_[p] = last;
        pos_[v] = -1;
    }
    inline int operator[](int i) {return taken_[i];}
    inline void safeErase(int v) {if (has(v)) erase(v);}
    inline bool has(int v) {return pos_[v] >= 0;}
    std::vector<int> toVector() {
        std::vector<int> ret;
        ret.insert(ret.begin(), begin(), end());
        return ret;
    }
    inline int size() {return size_;}
    inline bool empty() {return size_ == 0;}
    inline int* begin() { return taken_.data(); }
    inline int* end() { return taken_.data() + size_; }

};

std::FILE* fin = stdin;
std::FILE* fout = stdout;

long long maxi = 0;

struct Demon {
  int s, t, r;
  std::vector<int> A;
  void Load() {
    int n;
    fscanf(fin, "%d %d %d %d", &s, &t, &r, &n);
    A.resize(n+1);
    A[0] = 0;
    for (int i = 1; i <= n; i++) {
      fscanf(fin, "%d", &A[i]);
      A[i] += A[i-1];
    }
    maxi += A.back();
  }
};

Demon demons[1010101];
int D, T, S, M;

long long Simulate(const std::vector<int>& order) {
  int t = 0;
  int s = S;
  int idx = 0;
  long long sc = 0;
  std::set<std::pair<int, int>> rec;
  while (idx < order.size() && t <= T) {
    while (!rec.empty() && rec.begin()->first <= t) {
      s = std::min(M, s + rec.begin()->second);
      rec.erase(rec.begin());
    }
    auto& de = demons[order[idx]];
    if (de.s <= s) {
      s -= de.s;
      int l = std::min<int>(de.A.size()-1, T - t);
      sc += de.A[l]; // FIX
      rec.insert({t + de.t, de.r});
      idx++;
      t++;
    } else {
      if (rec.empty()) return sc;
      t = rec.begin()->first;
    }
  }
  return sc;
}

std::vector<int> SA() {
  std::vector<int> ans;
  long long best = 0;
  std::vector<int> ids;
  for (int i = 0; i < D; i++) ids.push_back(i);
  std::sort(ids.begin(), ids.end(), [&](int a, int b) {
    return demons[a].s < demons[b].s;
  });

  const double stime = get_time();
  double delta = (end_time - stime);
  double tempmax = 50.0;
  double tempmin = 10.0;
  double temp = tempmax;
  const double update = 0.01;
  double next_update = update;
  long long sc = Simulate(ids);

  int it = 0;
  LOG_INFO("Start SA for %lf at %d", delta, sc);
  std::vector<std::pair<int, int>> list;
  for (;; it++) {
    if (it % 1 == 0) {
      double dd = (get_time() - stime) / delta;
      if (dd > 1.0) break;
      temp = tempmax * std::pow(tempmin/tempmax, dd);
      if (dd > next_update) {
        next_update += update;
        LOG_INFO("%.1lf%% - Iterations: %d, Score: %d, Best: %d, temp: %lf",
                 100.0 * dd, it, sc, best, temp);
      }
    }

    auto cpy = ids;
    for (int i = rng.next_int(5); i >= 0; i--) {
      int x = rng.next_int(D);
      int y = rng.next_int(D);
      int v = ids[x];
      ids.erase(ids.begin() + x);
      ids.insert(ids.begin() + y, v);
    }
    int nsc = Simulate(ids);

    if (nsc >= sc || rng.next_float() < std::exp((nsc - sc)/temp)) {
      sc = nsc;
      if (sc > best) {
        best = sc;
        ans = ids;
      }
    } else {
      ids = cpy;
    }
  }
  LOG_WARNING("Score: %lld (%d)", best, ans.size());

  return ans;
}

int main(int argc, char** argv) {
  std::string file_in = std::string(argv[1]) + ".txt";
  fin = std::fopen(file_in.c_str(), "r");
  fscanf(fin, "%d %d %d %d", &S, &M, &T, &D);
  for (int i = 0; i < D; i++) demons[i].Load();
    LOG_INFO("Start: %d/%d, %d demons in %d turns (%lld)", S, M, D, T, maxi);
  auto ans = SA();
  std::string file_out = std::string(argv[1]) + ".out";
  fout = std::fopen(file_out.c_str(), "w");
  for (int i : ans) fprintf(fout, "%d\n", i);
}
