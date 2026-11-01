# StreamForge

A C++20 streaming data processing engine, built incrementally with correctness tests and reproducible performance benchmarks.

## Current milestone: Foundation

- Event data model
- CMake build
- Minimal demo and unit test

## Build and test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/streamforge_demo
```

## Roadmap

1. Bounded thread-safe queue with explicit close semantics and tests
2. Concurrent event producers and consumers
3. Per-worker aggregation and deterministic merging
4. Throughput and latency benchmarking
5. Batching and memory allocation experiments
6. Python bindings with pybind11

## Engineering principles

Correctness first; bounded memory; measurable improvements; explicit shutdown behavior; tests for concurrency edge cases.
