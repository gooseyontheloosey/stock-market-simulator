#include "CustomComparator.h"

// overloaded operator()
bool CustomComparator::operator()(shared_ptr<Order> order1, shared_ptr<Order> order2) {
    
    // market orders (aka 0-price orders) have higher priority
    if (order1->getLimitPrice() == 0 && order2->getLimitPrice() != 0) {
        return false;
    } else if (order1->getLimitPrice() != 0 && order2->getLimitPrice() == 0) {
        return true;
    }
    
    // limit orders of same type have different priority criteria
    if (order1->getOrderType() == "B" && order2->getOrderType() == "B" && (order1->getLimitPrice() != order2->getLimitPrice())) {
            return order1->getLimitPrice() < order2->getLimitPrice(); // buying orders - higher price, higher priority
            }
    else if (order1->getOrderType() == "S" && order2->getOrderType() == "S" && (order1->getLimitPrice() != order2->getLimitPrice())) {
        return order1->getLimitPrice() > order2->getLimitPrice(); // selling orders - lower price, higher priority
    }
    
    // respect FIFO if both orders are market orders/have equal limit prices
    if (order1->getLimitPrice() == order2->getLimitPrice()) {
        return order1->getIterationsCounter() > order2->getIterationsCounter();
    }

    return false;
}