#ifndef ORDER_H_
#define ORDER_H_

#include <string>
#include <memory>
using namespace std;

class Order {
    public:
        // ctor
        Order(const string& id, const string& type, int shares, int counter);

        // (default) copy ctor
       Order(const Order&) = default;

        // (default) dtor
        virtual ~Order() = default;
        
        // VIRTUAL deep copier for self
        virtual shared_ptr<Order> createDeepCopy();

        // setter & getter for orderId
        void setOrderId(const string& id);
        string getOrderId() const;

        // setter & getter for orderType
        void setOrderType(const string& type);
        string getOrderType() const;

        // setter & getter for iterationsCounter
        void setIterationsCounter(int counter);
        int getIterationsCounter() const;

        // settter & getter for numOfShares
        void setNumOfShares(int shares);
        int getNumOfShares() const;

        // VIRTUAL getter for limitPrice (for derived classes)
        virtual float getLimitPrice() const;

    protected:
        string orderId_; // alphanumeric identifier
        string orderType_; // either B or S
        int iterationsCounter_; // timestamp for when obj was instantiated
        int numOfShares_; // trading quantity

};

#endif /* ORDER_H_ */