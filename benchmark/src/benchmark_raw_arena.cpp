#include <benchmark/benchmark.h>

static void BM_malloc(benchmark::State &state) {
  auto count = state.range(0);
  for (auto _ : state) {

  }
}
BENCHMARK(BM_malloc)->RangeMultiplier(2)->Range(1, 1 << 16);
