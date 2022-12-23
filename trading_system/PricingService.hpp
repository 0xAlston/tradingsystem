/**
 * pricingservice.hpp
 * Defines the data types and Service for internal prices.
 *
 * @author Breman Thuraisingham
 * @coauthor Alston Gui
 */

#ifndef PRICING_SERVICE_HPP
#define PRICING_SERVICE_HPP

#include <string>
#include <map>
#include "SOA.hpp"

using namespace std;


template<typename T>
class Price
        //Price with mid-price and bid-ask spread so that we can reconstruct the bid and ask prices at real-time
{
public:
    // ctor for a price
    Price() = default;
    Price(const T& _product, double _mid, double _bidOfferSpread);

    // Get the product
    const T& GetProduct() const;

    // Get the mid price
    double GetMid() const;

    // Get the bid/offer spread around the mid
    double GetBidOfferSpread() const;

private:
    T product;
    double mid;
    double bidOfferSpread;
};


template<typename T>
class PricingService : public Service<string, Price <T> >
{
private:
    map<string, Price<T>> prices;
    
public:
    // ctor
    PricingService() = default;

    // Get data on our service given a key
    Price<T>& GetData(string key);

    // update
    void OnMessage(Price<T>& data);
};


template<typename T>
Price<T>::Price(const T& _product, double _mid, double _bidOfferSpread) :
    product(_product)
{
    mid = _mid;
    bidOfferSpread = _bidOfferSpread;
}

template<typename T>
const T& Price<T>::GetProduct() const
{
    return product;
}

template<typename T>
double Price<T>::GetMid() const
{
    return mid;
}

template<typename T>
double Price<T>::GetBidOfferSpread() const
{
    return bidOfferSpread;
}


template <typename T>
Price<T>& PricingService<T>::GetData(string key)
{
    return prices[key];
}

template <typename T>
void PricingService<T>::OnMessage(Price<T>& data)
{
    prices[data.GetProduct().GetProductId()] = data;
    Service<string, Price<T> >::Notify(data);
}

#endif
