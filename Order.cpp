#include "Order.h"
#include <memory>

// ctor
Order::Order(const string& id, const string& type, int shares, int counter) {
    this->orderId_ = id;
    this->orderType_ = type;
    this->numOfShares_ = shares;
    this->iterationsCounter_ = counter;
}

// deep copier for self
shared_ptr<Order> Order::createDeepCopy() {
    return nullptr;
}; // does nothing meaningful in base class

// setter & getter for orderId
void Order::setOrderId(const string& id) {
    this->orderId_ = id;
}
string Order::getOrderId() const {
    return this->orderId_;
}

// setter & getter for orderType
void Order::setOrderType(const string& type) {
    this->orderType_ = type;
}
string Order::getOrderType() const {
    return this->orderType_;
}

// setter & getter for iterationsCounter
void Order::setIterationsCounter(int counter) {
    this->iterationsCounter_ = counter;
}
int Order::getIterationsCounter() const {
    return this->iterationsCounter_;
}

// settter & getter for numOfShares
void Order::setNumOfShares(int shares) {
    this->numOfShares_= shares;
}
int Order::getNumOfShares() const {
    return this->numOfShares_;
}

// getter for limitPrice (for derived classes)
float Order::getLimitPrice() const {
    return -1; // does nothing meaningful in base class
}