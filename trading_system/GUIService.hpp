/**
* GUIService.hpp
* 
* @author Alston Gui
*/

#ifndef GUISERVICE_HPP
#define GUISERVICE_HPP

#include <string>
#include <map>
#include <boost/date_time.hpp>
#include "SOA.hpp"
#include "PricingService.hpp"
#include "Connectors.hpp"

using namespace std;
using namespace boost::posix_time;

/**
* GUI service that manages price throttles.
* Keyed on product identifier.
* Type T is the product type.
*/
template<typename T>
class GUIService : public Service<string, Price<T> >
{
private:
    map<string, Price<T>> guis;
    GUIConnector<T>* connector;
    ptime last_time;
    time_duration throttle;

public:
    // ctor
    GUIService();
    GUIService(GUIConnector<T>* _connector);
    
    // Get data on our service given a key
    Price<T>& GetData(string key);

    // update data
    void OnMessage(Price<T>& data);
};


template <typename T>
GUIService<T>::GUIService()
{
    guis = map<string, Price<T>>();
    connector = new GUIConnector<T>;
    last_time = microsec_clock::local_time();
    throttle = millisec(300);
}

template <typename T>
GUIService<T>::GUIService(GUIConnector<T>* _connector)
{
    guis = map<string, Price<T>>();
    connector = _connector;
    last_time = microsec_clock::local_time();
    throttle = millisec(300);
}

template <typename T>
Price<T>& GUIService<T>::GetData(string key)
{
    return guis[key];
}

template <typename T>
void GUIService<T>::OnMessage(Price<T>& data)
{

    ptime now = microsec_clock::local_time();
    if (now - last_time > throttle)
    {
        last_time = now;
        connector->Publish(data);
    }
}

#endif 