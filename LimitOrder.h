#ifndef LIMITORDER_H_
#define LIMITORDER_H_
#include "Order.h"

#include <string>
using namespace std;

class LimitOrder : public Order {
    public:
        // ctor
        LimitOrder(const string& id, const string& type, int shares, float price, int counter);

        // (default) copy ctor
        LimitOrder(const LimitOrder&) = default;

        // (default) dtor
        ~LimitOrder() = default;

        // deep copier for self
        shared_ptr<Order> createDeepCopy() override;

        // setter & getter for limitPrice
        void setLimitPrice(float price);
        float getLimitPrice() const override;
    
    private:
        float limitPrice_;
};

#endif /* LIMITORDER_H_ */