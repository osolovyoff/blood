#pragma once
#if defined(BLOOD_VS) || defined(BLOOD_32)
#include <filesystem>
#endif

#if defined(BLOOD_UNIX) && defined(USE_BOOST)
#include <boost/filesystem.hpp>
#endif

#include <string>

namespace blood { namespace sys {
    namespace fs =
#if defined(BLOOD_VS) || defined(BLOOD_32)
        std::tr2::sys;
#endif // defined windows
#if defined(BLOOD_UNIX) && defined(USE_BOOST)
    boost::filesystem;
#endif // defined

}} // end blood::sys
