/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Ferdinando Ametrano

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


#include <ql/Instruments/makecapfloor.hpp>

namespace QuantLib {

    MakeCapFloor::MakeCapFloor(CapFloor::Type capFloorType,
                               const Period& capFloorTenor, 
                               const boost::shared_ptr<Xibor>& index,
                               Rate strike)
    : capFloorType_(capFloorType), capFloorTenor_(capFloorTenor),
      index_(index), strike_(strike),
      firstCapletExcluded_(true),
      engine_(),
      makeVanillaSwap_(MakeVanillaSwap(capFloorTenor_, index_, 0.0)) {}

    MakeCapFloor::operator CapFloor() const {

        VanillaSwap swap = makeVanillaSwap_;

        std::vector<boost::shared_ptr<CashFlow> > leg = swap.floatingLeg();
        if (firstCapletExcluded_)
            leg.erase(leg.begin());

        std::vector<Rate> strikeVector(1, strike_);
        if (strike_ == Null<Rate>())
            strikeVector[0] = CapFloor(capFloorType_, leg,
                                       std::vector<Rate>(1, 0.03),
                                       swap.termStructure(),
                                       engine_).atmRate();

        return CapFloor(capFloorType_, leg, strikeVector,
                        swap.termStructure(), engine_);
    }

    MakeCapFloor::operator boost::shared_ptr<CapFloor>() const {

        VanillaSwap swap = makeVanillaSwap_;

        std::vector<boost::shared_ptr<CashFlow> > leg = swap.floatingLeg();
        if (firstCapletExcluded_)
            leg.erase(leg.begin());

        std::vector<Rate> strikeVector(1, strike_);
        if (strike_ == Null<Rate>())
            strikeVector[0] = CapFloor(capFloorType_, leg,
                                       std::vector<Rate>(1, 0.03),
                                       swap.termStructure(),
                                       engine_).atmRate();

        return boost::shared_ptr<CapFloor>(new
            CapFloor(capFloorType_, leg, strikeVector, swap.termStructure(),
                                                                   engine_));
    }

    MakeCapFloor& MakeCapFloor::withNominal(Real n) {
        makeVanillaSwap_.withNominal(n);
        return *this;
    }

    MakeCapFloor& MakeCapFloor::withEffectiveDate(const Date& effectiveDate,
                                                  bool firstCapletExcluded) {
        makeVanillaSwap_.withEffectiveDate(effectiveDate);
        firstCapletExcluded_ = firstCapletExcluded;
        return *this;
    }

    MakeCapFloor& MakeCapFloor::withForwardStart(const Period& forwardPeriod,
                                                 bool firstCapletExcluded) {
        makeVanillaSwap_.withForwardStart(forwardPeriod);
        firstCapletExcluded_ = firstCapletExcluded;
        return *this;
    }

    MakeCapFloor& MakeCapFloor::withDiscountingTermStructure(
                const Handle<YieldTermStructure>& discountingTS) {
        makeVanillaSwap_.withDiscountingTermStructure(discountingTS);
        return *this;
    }

    MakeCapFloor& MakeCapFloor::withTenor(const Period& t) {
        makeVanillaSwap_.withFixedLegTenor(t);
        makeVanillaSwap_.withFloatingLegTenor(t);
        return *this;
    }


    MakeCapFloor& MakeCapFloor::withCalendar(const Calendar& cal) {
        makeVanillaSwap_.withFixedLegCalendar(cal);
        makeVanillaSwap_.withFloatingLegCalendar(cal);
        return *this;
    }


    MakeCapFloor& MakeCapFloor::withConvention(BusinessDayConvention bdc) {
        makeVanillaSwap_.withFixedLegConvention(bdc);
        makeVanillaSwap_.withFloatingLegConvention(bdc);
        return *this;
    }


    MakeCapFloor&
    MakeCapFloor::withTerminationDateConvention(BusinessDayConvention bdc) {
        makeVanillaSwap_.withFixedLegTerminationDateConvention(bdc);
        makeVanillaSwap_.withFloatingLegTerminationDateConvention(bdc);
        return *this;
    }


    MakeCapFloor& MakeCapFloor::withForward(bool flag) {
        makeVanillaSwap_.withFixedLegForward(flag);
        makeVanillaSwap_.withFloatingLegForward(flag);
        return *this;
    }

    MakeCapFloor& MakeCapFloor::withEndOfMonth(bool flag) {
        makeVanillaSwap_.withFixedLegEndOfMonth(flag);
        makeVanillaSwap_.withFloatingLegEndOfMonth(flag);
        return *this;
    }


    MakeCapFloor& MakeCapFloor::withFirstDate(const Date& d) {
        makeVanillaSwap_.withFixedLegFirstDate(d);
        makeVanillaSwap_.withFloatingLegFirstDate(d);
        return *this;
    }

    MakeCapFloor& MakeCapFloor::withNextToLastDate(const Date& d) {
        makeVanillaSwap_.withFixedLegNextToLastDate(d);
        makeVanillaSwap_.withFloatingLegNextToLastDate(d);
        return *this;
    }

    MakeCapFloor& MakeCapFloor::withDayCount(const DayCounter& dc) {
        makeVanillaSwap_.withFixedLegDayCount(dc);
        makeVanillaSwap_.withFloatingLegDayCount(dc);
        return *this;
    }

}
