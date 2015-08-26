#pragma once
#include <string>

#if defined(BLOOD_VS) || defined(BLOOD_32)
#include <filesystem>
#endif

#if defined(BLOOD_UNIX) && defined(USE_BOOST)
#include <boost/filesystem.hpp>
#endif

namespace blood { namespace sys {
    namespace fs =
#if defined(BLOOD_VS) || defined(BLOOD_32)
        std::tr2::sys;
#endif
#if defined(BLOOD_UNIX) && defined(USE_BOOST)
    boost::filesystem;
#endif

}} // end blood::sys
