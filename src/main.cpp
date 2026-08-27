#include "OrderBook.h"
#include <iostream>

int main() {
    OrderBook book;
    uint64_t nextId = 1;

    // Add some resting orders
    book.addOrder(Order(nextId++, Side::Buy, OrderType::Limit, 100.0, 50));
    book.addOrder(Order(nextId++, Side::Buy, OrderType::Limit, 99.5, 30));
    book.addOrder(Order(nextId++, Side::Sell, OrderType::Limit, 101.0, 40));

    book.printBook();

    // Incoming aggressive order that crosses the spread
    auto trades = book.addOrder(Order(nextId++, Side::Buy, OrderType::Limit, 101.0, 20));

    std::cout << "\nTrades executed: " << trades.size() << "\n";
    for (auto& t : trades) {
        std::cout << "Buy#" << t.buyOrderId << " x Sell#" << t.sellOrderId
                   << " @ " << t.price << " qty " << t.quantity << "\n";
    }

    std::cout << "\n";
    book.printBook();

    return 0;
}
