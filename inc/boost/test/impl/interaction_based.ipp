//  (C) Copyright Gennadiy Rozental 2005-2007.
//  Use, modification, and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 41369 $
//
//  Description : Facilities to perform interaction-based testing
// ***************************************************************************

#ifndef BOOST_TEST_INTERACTION_BASED_IPP_112105GER
#define BOOST_TEST_INTERACTION_BASED_IPP_112105GER

// Boost.Test
#include <boost/test/detail/config.hpp>

#if !BOOST_WORKAROUND(__GNUC__, < 3) && \
    !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)) && \
    !BOOST_WORKAROUND(BOOST_MSVC, <1310) && \
    !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x530))

// Boost.Test
#include <boost/test/detail/config.hpp>
#include <boost/test/utils/callback.hpp>
#include <boost/test/interaction_based.hpp>
#include <boost/test/mock_object.hpp>
#include <boost/test/framework.hpp>     // for setup_error

#include <boost/test/detail/suppress_warnings.hpp>

// STL
#include <stdexcept>
#include <string>

//____________________________________________________________________________//

namespace boost {

namespace itest { // interaction-based testing

// ************************************************************************** //
// **************                    manager                   ************** //
// ************************************************************************** //

manager::manager()
{
    instance_ptr( true, this );
}

//____________________________________________________________________________//

manager::~manager()
{
    instance_ptr( true );
}

//____________________________________________________________________________//
    
manager*
manager::instance_ptr( bool reset, manager* new_ptr )
{
    static manager dummy( 0 );
    
    static manager* ptr = &dummy;
    
    if( reset ) {
        if( new_ptr ) {
            if( ptr != &dummy )
                throw unit_test::framework::setup_error( BOOST_TEST_L( "Couldn't run two interation based test the same time" ) );
                
            ptr = new_ptr;
        }
        else
            ptr = &dummy;
    }
    
    return ptr;
}
    
}  // namespace itest

}  // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

#endif // not ancient compiler

#endif // BOOST_TEST_INTERACTION_BASED_IPP_112105GER
