#ifndef CUSTOMCOMPARATOR_H_
#define CUSTOMCOMPARATOR_H_
#include "Order.h"
#include "LimitOrder.h"
#include "MarketOrder.h"

#include <memory>

using namespace std;

class CustomComparator {
    public:
        // (default) ctor
        CustomComparator() = default;

        // (default) dtor
        //~CustomComparator() = default;

        // overloaded operator()
        bool operator()(shared_ptr<Order> order1, shared_ptr<Order> order2);
};

#endif /* CUSTOMCOMPARATOR_H_ */