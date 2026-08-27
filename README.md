# Order Book Matching Engine

A single-threaded limit order-book matching engine in C++ implementing
price-time priority matching for buy/sell limit and market orders.

## Status: In Progress
Currently implemented:
- Price-time priority matching (FIFO within price level)
- Limit order support (buy/sell)
- Order cancellation

Planned:
- Market order support
- Latency benchmarking (p50/p99 per order)
- Synthetic order-flow generator
- Comparison of price-level data structures for cache locality

## Build
\`\`\`
mkdir build && cd build
cmake ..
make
./orderbook
\`\`\`

## Structure of repo
'''
orderbook-matching-engine/
├── README.md
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── Order.h
│   ├── OrderBook.h
│   └── OrderBook.cpp
'''
