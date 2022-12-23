/**
* AlgoStreamingService.hpp
* 
* @author Alston Gui
*/

#ifndef ALGO_STREAMING_SERVICE_HPP
#define ALGO_STREAMING_SERVICE_HPP

#include <map>
#include <random>
#include <string>
#include "SOA.hpp"
#include "Products.hpp"
#include "PricingService.hpp"
#include "StreamingService.hpp"

template <class V>
class AlgoStreamingService : public Service<string, PriceStream <V> > 
{
private:
    map<string, PriceStream<V>> pricestreams;

public:
    // Get data on our service given a key
    PriceStream<V>& GetData(string key);

    // update the price stream
    void OnMessage(PriceStream<V>& data);

    // Publish the price stream
    void PublishPrice(Price<V>& data);
};


template <typename V>
PriceStream<V>& AlgoStreamingService<V>::GetData(string key)
{// Get data on our service given a key
    return pricestreams[key];
}

template <typename V>
void AlgoStreamingService<V>::OnMessage(PriceStream<V>& data)
{// update the price stream
    pricestreams[data.GetProduct().GetProductId()] = data;
}

template <typename V>
void AlgoStreamingService<V>::PublishPrice(Price<V>& data)
{// Publish the price stream
    V product = data.GetProduct();
    //generate price stream
    double bid_price = data.GetMid() - data.GetBidOfferSpread() / 2;
    double ask_price = data.GetMid() + data.GetBidOfferSpread() / 2;
    long visible_size = rand() % 1000000 + 1000000;
    PriceStreamOrder bid_order(bid_price, visible_size, 2 * visible_size, BID);
    PriceStreamOrder ask_order(ask_price, visible_size, 2 * visible_size, OFFER);
    PriceStream<V> price_stream(data.GetProduct(), bid_order, ask_order);
    pricestreams[price_stream.GetProduct().GetProductId()] = price_stream;
    //publish the price stream
    string product_id = product.GetProductId();
    //cout << "Publishing price stream for " << product_id << endl;
    Service<string, PriceStream <V> >::Notify(price_stream);
}

#endif 
