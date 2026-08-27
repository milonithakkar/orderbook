#pragma once
#include <cstdint>
#include <chrono>

enum class Side { Buy, Sell };
enum class OrderType { Limit, Market };

struct Order {
    uint64_t id;
    Side side;
    OrderType type;
    double price;      // ignored for market orders
    uint32_t quantity;
    std::chrono::steady_clock::time_point timestamp;

    Order(uint64_t id_, Side side_, OrderType type_, double price_, uint32_t qty_)
        : id(id_), side(side_), type(type_), price(price_), quantity(qty_),
          timestamp(std::chrono::steady_clock::now()) {}
};

struct Trade {
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    double price;
    uint32_t quantity;
};
