//
//  Maximizer.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/13/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Maximizer_hpp
#define Maximizer_hpp

#include "ZigZagMaximizer.hpp"

class Maximizer {
public:
    Maximizer(const std::string& _stockSymbol);

    void maximizeZigZag();
    void maximizeOnYourMark();

private:
    void readStockTicks();
    
    static ZigZagMaximize* runZigZag(ZigZagMaximize* _zigZagMaximize);

    static std::string stockSymbol;
    
};

#endif /* Maximizer_hpp */
