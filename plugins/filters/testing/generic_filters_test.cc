/** generic_filters_test.cc                                 -*- C++ -*-
    Rémi Attab, 15 Aug 2013
    Copyright (c) 2013 Datacratic.  All rights reserved.

    Tests for the generic filters

*/

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "rtbkit/plugins/filters/generic_filters.h"

#include <boost/test/unit_test.hpp>

using namespace std;
using namespace RTBKIT;
using namespace Datacratic;

void check(const ConfigSet& configs, const initializer_list<size_t>& expected)
{
    ConfigSet ex;
    for (size_t cfg : expected) ex.set(cfg);

    ConfigSet diff = configs;
    diff ^= ex;

    if (diff.empty()) return;

    cerr << "val=" << configs.print() << endl
        << "exp=" << ex.print() << endl
        << "dif=" << diff.print() << endl;
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(listFilterTest)
{
    ListFilter<size_t> filter;

    BOOST_CHECK(filter.isEmpty({ }));
    BOOST_CHECK(!filter.isEmpty({ 1 }));

    cerr << "[ ListFilter-1 ]--------------------------------------------------"
        << endl;

    filter.addConfig(0, { 1, 2, 3 });
    filter.addConfig(1, { 3, 4, 5 });
    filter.addConfig(2, { 1, 5 });
    filter.addConfig(3, { 0, 6 });

    check(filter.filter(0), { 3 });
    check(filter.filter(1), { 0, 2 });
    check(filter.filter(2), { 0 });
    check(filter.filter(3), { 0, 1 });
    check(filter.filter(4), { 1 });
    check(filter.filter(5), { 1, 2 });
    check(filter.filter(6), { 3 });
    check(filter.filter(7), {  });

    cerr << "[ ListFilter-2 ]--------------------------------------------------"
        << endl;

    filter.removeConfig(3, { 0, 6 });

    check(filter.filter(0), {  });
    check(filter.filter(1), { 0, 2 });
    check(filter.filter(2), { 0 });
    check(filter.filter(3), { 0, 1 });
    check(filter.filter(4), { 1 });
    check(filter.filter(5), { 1, 2 });
    check(filter.filter(6), {  });
    check(filter.filter(7), {  });

    cerr << "[ ListFilter-3 ]--------------------------------------------------"
        << endl;

    filter.removeConfig(0, { 1, 2, 3 });

    check(filter.filter(0), {  });
    check(filter.filter(1), { 2 });
    check(filter.filter(2), {  });
    check(filter.filter(3), { 1 });
    check(filter.filter(4), { 1 });
    check(filter.filter(5), { 1, 2 });
    check(filter.filter(6), {  });
    check(filter.filter(7), {  });
}

BOOST_AUTO_TEST_CASE(intervalFilterTest)
{
    auto range = [] (size_t first, size_t last) {
        return make_pair(first, last);
    };

    typedef pair<size_t, size_t> RangeT;
    auto list = [] (const initializer_list<RangeT>& list) {
        return vector<RangeT>(list.begin(), list.end());
    };

    IntervalFilter<size_t> filter;

    cerr << "[ Interval-1 ]----------------------------------------------------"
        << endl;

    filter.addConfig(0, list({ range(1, 2) }));
    filter.addConfig(1, list({ range(1, 3), range(2, 4) }));
    filter.addConfig(2, list({ range(2, 4), range(5, 6) }));
    filter.addConfig(3, list({ range(3, 4), range(4, 5) }));
    filter.addConfig(4, list({ range(1, 2) }));

    check(filter.filter(0), { });
    check(filter.filter(1), { 0, 1, 4 });
    check(filter.filter(2), { 1, 2 });
    check(filter.filter(3), { 1, 2, 3 });
    check(filter.filter(4), { 3 });
    check(filter.filter(5), { 2 });
    check(filter.filter(6), {  });
    check(filter.filter(7), {  });

    cerr << "[ Interval-2 ]----------------------------------------------------"
        << endl;

    filter.removeConfig(3, list({ range(3, 4), range(4, 5) }));

    check(filter.filter(0), { });
    check(filter.filter(1), { 0, 1, 4 });
    check(filter.filter(2), { 1, 2 });
    check(filter.filter(3), { 1, 2 });
    check(filter.filter(4), {  });
    check(filter.filter(5), { 2 });
    check(filter.filter(6), {  });
    check(filter.filter(7), {  });

    cerr << "[ Interval-3 ]----------------------------------------------------"
        << endl;

    filter.removeConfig(1, list({ range(1, 3), range(2, 4) }));

    check(filter.filter(0), { });
    check(filter.filter(1), { 0, 4 });
    check(filter.filter(2), { 2 });
    check(filter.filter(3), { 2 });
    check(filter.filter(4), {  });
    check(filter.filter(5), { 2 });
    check(filter.filter(6), {  });
    check(filter.filter(7), {  });

    cerr << "[ Interval-4 ]----------------------------------------------------"
        << endl;

    filter.removeConfig(4, list({ range(1, 2) }));

    check(filter.filter(0), { });
    check(filter.filter(1), { 0 });
    check(filter.filter(2), { 2 });
    check(filter.filter(3), { 2 });
    check(filter.filter(4), {  });
    check(filter.filter(5), { 2 });
    check(filter.filter(6), {  });
    check(filter.filter(7), {  });

}