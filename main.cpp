#include "Order.h"
#include "LimitOrder.h"
#include "MarketOrder.h"
#include "CustomComparator.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <tuple>

using namespace std;

string outputFilename; // global variable
int counter = 0;


void printPriorityQueue(std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CustomComparator> queue) {
    cout << endl << endl << "DEBUGGING" << endl;
    std::cout << "Priority Queue Contents:" << std::endl;

    // Iterate by popping elements (this will modify the queue, so work on a copy)
    while (!queue.empty()) {
        std::shared_ptr<Order> current = queue.top();
        std::cout << current->getOrderId() << " " << current->getLimitPrice() << " " << current->getNumOfShares() << std::endl; // Assuming Order has a `toString` method
        queue.pop(); // Remove the top element
    }

    std::cout << "End of Queue." << std::endl << endl;
}

// custom function to remove items from priority queue
priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator> popFromPriorityQueue(priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator> pQ, shared_ptr<Order> itemToPop) {
    priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator> updatedPQueue;
    while (!pQ.empty()) {
        if (pQ.top().get() != itemToPop.get()) {
            updatedPQueue.push(pQ.top());
        }
        pQ.pop();
    }
    return updatedPQueue;
}

// writes fulfilled orders to output file
void outputFulfilledOrders(shared_ptr<Order> buyOrder, shared_ptr<Order> sellOrder, float executionPrice) {
    ofstream outputFile;
    outputFile << std::fixed << std::setprecision(2);
    outputFile.open(outputFilename, fstream::app);

    outputFile << "order " << buyOrder->getOrderId() << " " << buyOrder->getNumOfShares() << " shares purchased at price " << executionPrice << endl;
    outputFile << "order " << sellOrder->getOrderId() << " " << sellOrder->getNumOfShares() << " shares sold at price " << executionPrice << endl;

    outputFile.close();
}

// writes unfulfilled orders to output file
void outputUnfulfilledOrders(priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator> pQ) {
    ofstream outputFile;
    outputFile << std::fixed << std::setprecision(2);
    outputFile.open(outputFilename, fstream::app);

    vector<shared_ptr<Order>> temp; // temp vector to store items from pQ

    // move items from queue to temp vector, then pop from queue
    while (!pQ.empty()) {
        temp.push_back(pQ.top());
        pQ.pop();
    }

    // sort vector based on getIterationsCounter()
    sort(temp.begin(), temp.end(), [](shared_ptr<Order>& a, shared_ptr<Order>& b) {
        return a->getIterationsCounter() < b->getIterationsCounter();
    });

    // write to output file
    for (auto& order : temp) {
        outputFile << "order " << order->getOrderId() << " " << order->getNumOfShares() << " shares unexecuted" << endl;
    }

    outputFile.close();
}

// displays on-screen information of all pending orders; also returns vector of orders for order-matching
vector<shared_ptr<Order>> displayOrderBook(float lastTransactionPrice, priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator> pendingOrdersCopy, string newOrderType) {
    vector<shared_ptr<Order>> buyOrders;
    vector<shared_ptr<Order>> sellOrders;
    vector<shared_ptr<Order>>::iterator it;

    while (!pendingOrdersCopy.empty()) {
        if (pendingOrdersCopy.top().get()->getOrderType() == "B") {
            shared_ptr<Order> order = pendingOrdersCopy.top();
            pendingOrdersCopy.pop();
            buyOrders.push_back(order);
        } else if (pendingOrdersCopy.top().get()->getOrderType() == "S") {
            shared_ptr<Order> order = pendingOrdersCopy.top();
            pendingOrdersCopy.pop();
            sellOrders.push_back(order);           
        }
    }
    
    cout << "***** ORDER BOOK OF PENDING TRANSACTIONS *****" << endl
    << "LAST TRADING PRICE: " << lastTransactionPrice << endl
    << "---BUY ORDERS---" << endl;

    for (it = buyOrders.begin(); it != buyOrders.end(); ++it) {
        if ((*it)->getLimitPrice() == 0) {
            cout << (*it)->getOrderId() << " M " << (*it)->getNumOfShares() << endl; // replaces limit price with "M" for market orders
        } else {
            cout << (*it)->getOrderId() << " " << (*it)->getLimitPrice() << " " << (*it)->getNumOfShares() << endl;
        }
    }
    
    cout << "---SELL ORDERS---" << endl;

    for (it = sellOrders.begin(); it != sellOrders.end(); ++it) {
        if ((*it)->getLimitPrice() == 0) {
            cout << (*it)->getOrderId() << " M " << (*it)->getNumOfShares() << endl; // replaces limit price with "M" for market orders
        } else {
            cout << (*it)->getOrderId() << " " << (*it)->getLimitPrice() << " " << (*it)->getNumOfShares() << endl;
        }
    }

    cout << endl << endl << endl;

    // return one of the Order vectors to facilitate order-matching, dependent on if latest order is buy or sell
    if (newOrderType == "B") {
        return sellOrders;
    } else if (newOrderType == "S") {
        return buyOrders;
    }

    return vector<shared_ptr<Order>>(); // otherwise, return empty vector
}

// searches for matching orders for each newly added order; returns updated priority queue of pending orders and lastTransactionPrice
tuple<priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator>, float> findMatchingOrders(float lastTransactionPrice, shared_ptr<Order> newOrder, priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator> pendingOrdersQueue) {
    vector<shared_ptr<Order>> pendingOrdersVector; // vector to store pending buy orders OR sell orders
    vector<shared_ptr<Order>>::iterator it; // iterator - to store matching order for newOrder, if found

    // display updated OrderBook and get vector of pending orders to search through
    if (newOrder.get()->getOrderType() == "B") {
        pendingOrdersVector = displayOrderBook(lastTransactionPrice, pendingOrdersQueue, "B");
        it = find_if(pendingOrdersVector.begin(), pendingOrdersVector.end(), [newOrder](shared_ptr<Order> matchingOrder) {
            return (newOrder.get()->getLimitPrice() >= matchingOrder.get()->getLimitPrice() || matchingOrder.get()->getLimitPrice() == 0); // buying order must have price limit >= selling order OR selling order is market order
        });
    } else if (newOrder.get()->getOrderType() == "S") {
        pendingOrdersVector = displayOrderBook(lastTransactionPrice, pendingOrdersQueue, "S");
        it = find_if(pendingOrdersVector.begin(), pendingOrdersVector.end(), [newOrder](shared_ptr<Order> matchingOrder) {
            return (newOrder.get()->getLimitPrice() <= matchingOrder.get()->getLimitPrice() || matchingOrder.get()->getLimitPrice() == 0); // selling order must have price limit <= buying order OR buying order is market order
        });
    }
    
    // if newest order is market order, vector doesn't need to be searched through
    if (newOrder.get()->getLimitPrice() == 0) {
        it = pendingOrdersVector.begin(); // if new order is market order, choose highest priority pending order
    }

    if (it != pendingOrdersVector.end() && *it) {

        // CHECK IF LIMIT PRICES DIFFER
        // if different: use older order's or use limit order's if other is market order
        if ((*it)->getLimitPrice() != newOrder->getLimitPrice()) {

            if(((*it)->getIterationsCounter() < newOrder->getIterationsCounter()) || (newOrder->getLimitPrice() == 0 && (*it)->getLimitPrice() != 0)) {
                lastTransactionPrice = (*it)->getLimitPrice();
                
            } else if ((newOrder->getIterationsCounter() < (*it)->getIterationsCounter()) || ((*it)->getLimitPrice() == 0 && newOrder->getLimitPrice() != 0)) {
                lastTransactionPrice = newOrder->getLimitPrice();

            } else if ((*it)->getLimitPrice() == 0 && newOrder->getLimitPrice() == 0) {
                lastTransactionPrice = lastTransactionPrice; // do nothing if both are market orders
            }

        } else if ((*it)->getLimitPrice() == 0 && newOrder->getLimitPrice() == 0) {
            lastTransactionPrice = lastTransactionPrice; // use lastTransactionPrice if both are market orders
        } else {
            lastTransactionPrice = newOrder->getLimitPrice(); // if price limits aren't different, either newOrder or (*it) limit price is fine
        }

        // CHECK IF TRADING QUANTITY DIFFERS
        if((*it)->getNumOfShares() != newOrder->getNumOfShares()) {
            shared_ptr<Order> residualOrder; // to store order with residual trading quantity

            // create deep copy of order with residual shares and add it to pendingOrdersQueue; update original order's numOfShares
            if ((*it)->getNumOfShares() > newOrder->getNumOfShares()) {
                residualOrder = (*it)->createDeepCopy();
                residualOrder->setNumOfShares(abs(newOrder->getNumOfShares() - (*it)->getNumOfShares()));
                pendingOrdersQueue.emplace(residualOrder);

                (*it)->setNumOfShares(newOrder->getNumOfShares());

                // remove matched order from queue before recursive call
                pendingOrdersQueue = popFromPriorityQueue(pendingOrdersQueue, newOrder);
                
            } else {
                residualOrder = newOrder->createDeepCopy();
                residualOrder->setNumOfShares(abs(newOrder->getNumOfShares() - (*it)->getNumOfShares()));
                pendingOrdersQueue.emplace(residualOrder);

                newOrder->setNumOfShares((*it)->getNumOfShares());

                // remove matched order from queue before recursive call
                pendingOrdersQueue = popFromPriorityQueue(pendingOrdersQueue, (*it));
            }

            tie(pendingOrdersQueue, lastTransactionPrice) = findMatchingOrders(lastTransactionPrice, newOrder, pendingOrdersQueue);
        }
        
        // write fulfilled orders to output file (buy order logged first)
        if (newOrder->getOrderType() == "B") {
            outputFulfilledOrders(newOrder, (*it), lastTransactionPrice);
        } else {
            outputFulfilledOrders((*it), newOrder, lastTransactionPrice);
        }

        // remove matched orders from queue (double checking there is a match for newOrder)
        pendingOrdersQueue = popFromPriorityQueue(pendingOrdersQueue, newOrder);
        pendingOrdersQueue = popFromPriorityQueue(pendingOrdersQueue, (*it));  
    }

    // return tuple of updated queue and transaction price
    return make_tuple(pendingOrdersQueue, lastTransactionPrice);
}

int main(int argc, char* argv[]) {
    cout << std::fixed << std::setprecision(2);
    int iterationsCounter = 0;
    float lastTransactionPrice;

    priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CustomComparator> pendingOrders; // priority queue of PENDING orders

    ifstream inputFile;
    string line;
    string inputFilename = argv[1];

    inputFile.open(inputFilename);
    outputFilename = inputFilename.replace(0, 5, "output"); // update output file name
    if (!inputFile) {
        cout << "Error - unable to open the specified file!" << endl;
        return 1;
    }

    // split each file line into order fields and store in orderVec
    while(getline(inputFile, line)) {
        vector<string> orderVec; // stores each order and its fields
        vector<string>::iterator it; // iterator for orderVec

        string orderField;
        stringstream ss(line);

        iterationsCounter+=1;
        
        while (ss >> orderField) {
            orderVec.push_back(orderField);
        }

        if (iterationsCounter == 1 && !orderVec.empty()) {
            lastTransactionPrice = stof(orderVec.at(0)); // store previous day's final transaction price
        }

        // instantiate order object, add to pendingOrders and check for any matching orders
        // 4 order fields = LimitOrder, 3 = MarketOrder
        if (orderVec.size() == 4) {
            auto newOrder = make_shared<LimitOrder>(orderVec.at(0), orderVec.at(1), stoi(orderVec.at(2)), stof(orderVec.at(3)), iterationsCounter);
            pendingOrders.emplace(newOrder);
            tie(pendingOrders, lastTransactionPrice) = findMatchingOrders(lastTransactionPrice, newOrder, pendingOrders);
        } else if (orderVec.size() == 3) {
            auto newOrder = make_shared<MarketOrder>(orderVec.at(0), orderVec.at(1), stoi(orderVec.at(2)), iterationsCounter);
            pendingOrders.emplace(newOrder);
            tie(pendingOrders, lastTransactionPrice) = findMatchingOrders(lastTransactionPrice, newOrder, pendingOrders);
        }
    }

    // update orderBook once more after all orders read in
    displayOrderBook(lastTransactionPrice, pendingOrders, "");

    // write unfulfilled orders to output file
    outputUnfulfilledOrders(pendingOrders);

    inputFile.close();

    return 0;
}