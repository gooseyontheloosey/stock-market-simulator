#include "MarketOrder.h"

MarketOrder::MarketOrder(const string& id, const string& type, int shares, int counter)
:Order(id, type, shares, counter) {
    this->limitPrice_ = 0.00;
}

shared_ptr<Order> MarketOrder::createDeepCopy() {
    return make_shared<MarketOrder>(*this);
}

void MarketOrder::setLimitPrice(float price) {
    this->limitPrice_ = price;
}
float MarketOrder::getLimitPrice() const {
    return this->limitPrice_;
}