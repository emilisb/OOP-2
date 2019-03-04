//
//  RandomGenerator.hpp
//  Uzduotis 2
//
//  Created by Emilis Baliukonis on 04/03/2019.
//  Copyright © 2019 Emilis Baliukonis. All rights reserved.
//

#ifndef RandomGenerator_hpp
#define RandomGenerator_hpp

#include <random>

class RandomGenerator {
public:
    RandomGenerator();
    int getNumber(int from, int to);
private:
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng; // seed the generator
};

#endif /* RandomGenerator_hpp */
