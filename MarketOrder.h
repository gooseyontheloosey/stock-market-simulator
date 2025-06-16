#ifndef MARKETORDER_H_
#define MARKETORDER_H_
#include "Order.h"

#include <string>
using namespace std;

class MarketOrder : public Order {
    public:
        // ctor
        MarketOrder(const string& id, const string& type, int shares, int counter);

        // (default) copy ctor
        MarketOrder(const MarketOrder&) = default;

        // (default) dtor
        ~MarketOrder() = default;

        // deep copier for self
        shared_ptr<Order> createDeepCopy() override;

        // setter & getter for limitPrice
        void setLimitPrice(float price);
        float getLimitPrice() const override;
    
    private:
        float limitPrice_; // useful to have limitPrice even if market orders have no target price
};

#endif /* MARKETORDER_H_ */