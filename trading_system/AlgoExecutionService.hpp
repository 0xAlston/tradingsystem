/**
* AlgoExecutionService.hpp
* 
* @author Alston Gui
*/

#ifndef ALGO_EXECUTION_SERVICE_HPP
#define ALGO_EXECUTION_SERVICE_HPP

#include <string>
#include <vector>
#include <map>
#include "SOA.hpp"
#include "MarketDataService.hpp"
#include "ExecutionService.hpp"

using namespace std;

template <class T>
class AlgoExecutionService : public Service<string, ExecutionOrder<T> > 
{
private:
    map<string, ExecutionOrder<T>> execution_orders;
    int counter;
    double spread_tol;

public:
    //ctor
    AlgoExecutionService();

    // Get data on our service given a key
    ExecutionOrder<T>& GetData(string key);

    // update the execution order
    void OnMessage(ExecutionOrder<T>& data);

    // Execute an order
    void ExecuteOrder(OrderBook<T>& data);
    
};


template <typename T>
AlgoExecutionService<T>::AlgoExecutionService()
{   //ctor
    execution_orders = map<string, ExecutionOrder<T>>();
    counter = 0;
    spread_tol = 1.0 / 128;
}

template <typename T>
ExecutionOrder<T>& AlgoExecutionService<T>::GetData(string key)
{
    return execution_orders[key];
}

template <typename T>
void AlgoExecutionService<T>::OnMessage(ExecutionOrder<T>& data)
{    // update the execution order
    execution_orders[data.GetOrderId()] = data;
}

template <typename T>
void AlgoExecutionService<T>::ExecuteOrder(OrderBook<T>& data)
{    //order execution
    T product = data.GetProduct();
    vector<Order> bid_stack = data.GetBidStack();
    vector<Order> offer_stack = data.GetOfferStack();
    Order best_bid = bid_stack[0];
    Order best_offer = offer_stack[0];

    for (auto &e: bid_stack)
        if (e.GetPrice() > best_bid.GetPrice())
            best_bid = e;

    for (auto &e: offer_stack)
        if (e.GetPrice() < best_offer.GetPrice())
            best_offer = e;

    // match order if the spread is within tolerance
    double price, quantity;
    PricingSide side;
    if (best_offer.GetPrice() - best_bid.GetPrice() > spread_tol)
    {
        if (counter % 2 == 0)           //buy
        {
            price = best_bid.GetPrice();
            quantity = best_bid.GetQuantity();
            side = BID;
        }
        else                            //sell
        {
            price = best_offer.GetPrice();
            quantity = best_offer.GetQuantity();
            side = OFFER;
        }
        // create execution order
        string tradeID = "TRADEID_" + to_string(counter);
        ExecutionOrder<T> execu_order(product, side, tradeID, MARKET, price, quantity, 2 * quantity, "", false);
        execution_orders[execu_order.GetOrderId()] = execu_order;
        ++counter;
        Service<string, ExecutionOrder<T> >::Notify(execu_order);
    }
}

#endif