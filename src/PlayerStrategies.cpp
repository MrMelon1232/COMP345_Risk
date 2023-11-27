#pragma once

#include "PlayerStrategies.h"





StrategyType getStrategyType(string str) {
    if (str == "Human" || str == "human")
        return StrategyType::Human;
    else if (str == "Aggressive" || str == "aggressive")
        return StrategyType::Aggressive;
    else if (str == "Benevolent" || str == "benevolent")
        return StrategyType::Benevolent;
    else if (str == "Neutral" || str == "neutral")
        return StrategyType::Neutral;
    else if (str == "Cheater" || str == "cheater")
        return StrategyType::Cheater;
    return StrategyType::None;
}

PlayerStrategy* loadStrategy(Player* player, StrategyType strategyType) {
    if (strategyType == StrategyType::Human)
        return new HumanPlayerStrategy(player);
    else if (strategyType == StrategyType::Aggressive)
        return new CheaterPlayerStrategy(player);
    else if (strategyType == StrategyType::Benevolent)
        return new BeneloventPlayerStrategy(player);
    else if (strategyType == StrategyType::Neutral)
        return new NeutralPlayerStrategy(player);
    else if (strategyType == StrategyType::Cheater)
        return new CheaterPlayerStrategy(player);
    return nullptr; // StrategyType::None, shouldn't happen if validation was done properly.
}