#include "LimitOrder.h"

LimitOrder::LimitOrder(const string& id, const string& type, int shares, float price, int counter)
:Order(id, type, shares, counter) {
    this->limitPrice_ = price;
}

shared_ptr<Order> LimitOrder::createDeepCopy() {
    return make_shared<LimitOrder>(*this);
}

void LimitOrder::setLimitPrice(float price) {
    this->limitPrice_ = price;
}
float LimitOrder::getLimitPrice() const {
    return this->limitPrice_;
}