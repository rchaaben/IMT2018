//
//  main.cpp
//  Quant lib
//
//  Created by Achraf Farah on 24/01/2018.
//  Copyright © 2018 Achraf Farah. All rights reserved.
//


#include "binomialtree.hpp"
#include "binomialengine.hpp"
#include <ql/methods/lattices/binomialtree.hpp>
#include <ql/pricingengines/vanilla/binomialengine.hpp>
#include <ql/instruments/vanillaoption.hpp>

#include <boost/timer.hpp>
#include <ql/time/calendars/target.hpp>
#include <iostream>


using namespace QuantLib;

int main() {
    
    try {
        // Calendar stuff set up
        Calendar calendar = TARGET();
        Date todaysDate(30, January, 2018);
        Settings::instance().evaluationDate() = todaysDate;
        DayCounter dayCounter = Actual365Fixed();
        
        // Option parameter
        Option::Type type(Option::Put);
        Real underlying = 50;
        Real strike = 47;
        Spread dividendYield = 0.00;
        Rate riskFreeRate = 0.04;
        Volatility volatility = 0.15;
        Date maturity(10, December, 2018);
        
        Size timeStep=100;
        // European exercise type handler
        boost::shared_ptr<Exercise> europeanExercise(new EuropeanExercise(maturity));
        
        // Quote (=underlying price) handler
        Handle<Quote> underlyingH(boost::shared_ptr<Quote>(new SimpleQuote(underlying)));
        
        // Yield term structure handler
        Handle<YieldTermStructure> flatTermStructure(boost::shared_ptr<YieldTermStructure>(new FlatForward(todaysDate,riskFreeRate,dayCounter)));
        
        // Dividend handler
        Handle<YieldTermStructure> flatDividendTermStructure(boost::shared_ptr<YieldTermStructure>(new FlatForward(todaysDate,dividendYield,dayCounter)));
        
        // Volatility handler
        Handle<BlackVolTermStructure> flatVolTermStructure(boost::shared_ptr<BlackVolTermStructure>(new BlackConstantVol(todaysDate,calendar,volatility,dayCounter)));
        
        // Payoff handler
        boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(type,strike));
        
        // Black Scholes
        boost::shared_ptr<BlackScholesMertonProcess> bsmProcess(new BlackScholesMertonProcess(underlyingH,flatDividendTermStructure,flatTermStructure,flatVolTermStructure));
        
        // Option characteristics
        VanillaOption europeanOption(payoff,europeanExercise);
        
        // Pricing Engine
        //europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(new BinomialVanillaEngine_2(bsmProcess,timeSteps)));
        
        std::cout << "Delta :\t\t"  << europeanOption.delta() << std::endl;
        std::cout << "Gamma :\t\t"  << europeanOption.gamma() << std::endl;
        
        
        
        return 0;
        
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }
}
