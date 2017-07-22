//
//  Broker.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/20/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Broker_hpp
#define Broker_hpp

#include "Stock.hpp"

class Broker {
public:
    Broker();
    ~Broker();

    void run();
    void test();

private:
    std::vector<Stock*> stocks;

    void initializeForRun();

    void logProfits(const Stock& _stock);

};

#endif /* Broker_hpp */
