#include "../game/gameoflife.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace gol;

static void testGameRules(std::set<int> survival, std::set<int> birth) {
    GameRules rules(birth, survival);
    for(int i = 0; i < 20; i++) {
        if(rules.isActive(true, i) != (survival.find(i) != survival.end())) {
            std::cerr << "Wrong result for game rule in survival";
        }
        if(rules.isActive(false, i) != (birth.find(i) != birth.end())) {
            std::cerr << "Wrong result for game rule in birth";
        }
    }
}

static bool equals(GameRules rules1, GameRules rules2) {
    return rules1.getBrith() == rules2.getBrith() && rules2.getSurvive() == rules1.getSurvive();
}

static void testGameRuleBuilder() {
    char fileFormatExceptionStrings[6][100]{
            {""},
            {"A"},
            {"B2324"},
            {"B2324/"},
            {"B2324A"},
            {"B2324/A"}
    };
    for (int i = 0; i < 6; i++) {
        try {
            GameRules rules(fileFormatExceptionStrings[i]);
            std::cerr << "expect exception for GameRules constructor for string \"" << fileFormatExceptionStrings[i] <<"\"" << std::endl;
        } catch (FileFormatException e) {}

    }
    char notCriticalErrors[4][100] {
            {"B22/S33"},
            {"B345/S445"},
            {"B344/S456"},
            {"B344/S456G"}
    };
    GameRules expectedRules[4] {
            GameRules({2}, {3}),
            GameRules({3, 4, 5}, {4, 5}),
            GameRules({3, 4}, {4, 5, 6}),
            GameRules({3, 4}, {4, 5, 6})
    };

    for (int i = 0; i < 4; i++) {
        try {
            GameRules rules(notCriticalErrors[i]);
            if(!equals(rules, expectedRules[i])) {
                std::cerr << "wrong GameRules for string \"" << notCriticalErrors[i] <<"\"" << std::endl;
            }
        } catch (FileFormatException e) {
            std::cerr << "expect no exceptions in GameRules constructor for string \"" << notCriticalErrors[i] << "\"\n" << e.getReason() << std::endl;
        }
    }

    for (int i = 0; i < 4; i++) {
        GameRules rule(expectedRules[i]);
        if (!equals(rule, expectedRules[i])) {
            std::cerr << "wrong constructor GameRules(GameRules &) in GameRules " << expectedRules[i] << std::endl;
        }
    }

}

static void testGameRuleOperators() {
    char buildStrings[4][100] {
            {"B2/S3"},
            {"B345/S45"},
            {"B34/S456"},
            {"B34/S456"}
    };
    GameRules rules[4] {
            GameRules({2}, {3}),
            GameRules({3, 4, 5}, {4, 5}),
            GameRules({3, 4}, {4, 5, 6}),
            GameRules({3, 4}, {4, 5, 6})
    };
    GameRules nrules;
    for(int i = 0; i < 4; i++) {
        nrules = rules[i];
        if (!equals(nrules, rules[i])) {
            std::cerr << "wrong operator = in GameRules " << rules << std::endl;
        }
        std::string result;
        std::stringstream stream(result);
        stream << rules[i];
        if(stream.str() != buildStrings[i]) {
            std::cerr << "wrong operator << in GameRules print \"" << stream.str() << "\" expect \"" << buildStrings[i] << "\"" << std::endl;
        }
    }
}

void gameRulesTest() {
    testGameRuleBuilder();
    testGameRules({1, 2}, {1, 2});
    testGameRules({3, 4}, {1, 2});
    testGameRules({1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8});
    testGameRules({0}, {0});
    testGameRuleOperators();
}
