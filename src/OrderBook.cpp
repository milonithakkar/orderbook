#include "OrderBook.h"
#include <unordered_map>
#include <iostream>

std::vector<Trade> OrderBook::addOrder(Order order) {
    std::vector<Trade> trades = matchOrder(order);

    // If limit order has remaining quantity, rest it in the book
    if (order.type == OrderType::Limit && order.quantity > 0) {
        if (order.side == Side::Buy) {
            bids[order.price].push_back(order);
        } else {
            asks[order.price].push_back(order);
        }
        orderIndex[order.id] = {order.side, order.price};
    }

    return trades;
}

std::vector<Trade> OrderBook::matchOrder(Order& incoming) {
    std::vector<Trade> trades;

    if (incoming.side == Side::Buy) {
        while (incoming.quantity > 0 && !asks.empty()) {
            auto bestAsk = asks.begin();
            double askPrice = bestAsk->first;

            if (incoming.type == OrderType::Limit && incoming.price < askPrice)
                break; // no more matchable prices

            auto& queue = bestAsk->second;
            while (incoming.quantity > 0 && !queue.empty()) {
                Order& resting = queue.front();
                uint32_t matchedQty = std::min(incoming.quantity, resting.quantity);

                trades.push_back({incoming.id, resting.id, askPrice, matchedQty});

                incoming.quantity -= matchedQty;
                resting.quantity -= matchedQty;

                if (resting.quantity == 0) {
                    orderIndex.erase(resting.id);
                    queue.pop_front();
                }
            }
            if (queue.empty()) asks.erase(bestAsk);
        }
    } else { // Sell
        while (incoming.quantity > 0 && !bids.empty()) {
            auto bestBid = bids.begin();
            double bidPrice = bestBid->first;

            if (incoming.type == OrderType::Limit && incoming.price > bidPrice)
                break;

            auto& queue = bestBid->second;
            while (incoming.quantity > 0 && !queue.empty()) {
                Order& resting = queue.front();
                uint32_t matchedQty = std::min(incoming.quantity, resting.quantity);

                trades.push_back({resting.id, incoming.id, bidPrice, matchedQty});

                incoming.quantity -= matchedQty;
                resting.quantity -= matchedQty;

                if (resting.quantity == 0) {
                    orderIndex.erase(resting.id);
                    queue.pop_front();
                }
            }
            if (queue.empty()) bids.erase(bestBid);
        }
    }

    return trades;
}

bool OrderBook::cancelOrder(uint64_t orderId) {
    auto it = orderIndex.find(orderId);
    if (it == orderIndex.end()) return false;

    auto [side, price] = it->second;
    auto& book = (side == Side::Buy) ? bids : asks;

    auto priceLevel = book.find(price);
    if (priceLevel == book.end()) return false;

    auto& queue = priceLevel->second;
    for (auto qIt = queue.begin(); qIt != queue.end(); ++qIt) {
        if (qIt->id == orderId) {
            queue.erase(qIt);
            break;
        }
    }
    if (queue.empty()) book.erase(priceLevel);

    orderIndex.erase(it);
    return true;
}

void OrderBook::printBook() const {
    std::cout << "--- ASKS ---\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        uint32_t total = 0;
        for (auto& o : it->second) total += o.quantity;
        std::cout << it->first << " : " << total << "\n";
    }
    std::cout << "--- BIDS ---\n";
    for (auto& [price, queue] : bids) {
        uint32_t total = 0;
        for (auto& o : queue) total += o.quantity;
        std::cout << price << " : " << total << "\n";
    }
}
