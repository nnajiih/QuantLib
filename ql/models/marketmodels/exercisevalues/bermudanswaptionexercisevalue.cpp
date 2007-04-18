/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Mark Joshi

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/models/marketmodels/exercisevalues/bermudanswaptionexercisevalue.hpp>
#include <ql/models/marketmodels/curvestate.hpp>
#include <ql/payoff.hpp>

namespace QuantLib {

    BermudanSwaptionExerciseValue::BermudanSwaptionExerciseValue(
              const std::vector<Time>& rateTimes,
              const std::vector<boost::shared_ptr<Payoff> >&payoffs)
    : numberOfExercises_(rateTimes.size()-1), rateTimes_(rateTimes),
      payoffs_(payoffs), currentIndex_(0) {
        std::vector<Time> evolveTimes(rateTimes_);
        evolveTimes.pop_back();
        evolution_ = EvolutionDescription(rateTimes_,evolveTimes);
    }

    Size BermudanSwaptionExerciseValue::numberOfExercises() const {
        return numberOfExercises_;
    }

    const EvolutionDescription&
    BermudanSwaptionExerciseValue::evolution() const {
        return evolution_;
    }

    std::vector<Time>
    BermudanSwaptionExerciseValue::possibleCashFlowTimes() const {
        return rateTimes_;
    }

    void BermudanSwaptionExerciseValue::reset() {
        currentIndex_=0;
    }

    void BermudanSwaptionExerciseValue::nextStep(const CurveState& state) {
        const Payoff& p = (*payoffs_[currentIndex_]);
        Real value = state.coterminalSwapAnnuity(currentIndex_, currentIndex_) *
            p(state.coterminalSwapRate(currentIndex_));

     //   value /= state.discountRatios()[currentIndex_];
        value =  std::max(value, 0.0);
        cf_.timeIndex = currentIndex_;
        cf_.amount = value;
        ++currentIndex_;
    }


    std::vector<bool> BermudanSwaptionExerciseValue::isExerciseTime() const {
        return std::vector<bool>(numberOfExercises_,true);
    }

    MarketModelMultiProduct::CashFlow
    BermudanSwaptionExerciseValue::value(const CurveState& ) const {
         return cf_;
    }

    std::auto_ptr<MarketModelExerciseValue>
    BermudanSwaptionExerciseValue::clone() const {
        return std::auto_ptr<MarketModelExerciseValue>(
                                    new BermudanSwaptionExerciseValue(*this));
    }

}