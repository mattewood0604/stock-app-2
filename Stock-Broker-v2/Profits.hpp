//
//  Profits.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Profits_hpp
#define Profits_hpp

class Profits {
public:
    Profits();

    void reset();
    void resetDaily();
    void resetOverall();

    void add(const float& _profits);
    void log() const;

    const float& getDaily() const;
    const float& getOverall() const;

private:
    float daily;
    float overall;
};

#endif /* Profits_hpp */
