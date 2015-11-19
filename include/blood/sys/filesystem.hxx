#pragma once
#include <string>

#if defined(WIN32) && defined(MSVS)
#include <filesystem>
#endif // WIN32 && MSVS

#if defined(BLOOD_UNIX) && defined(USE_BOOST)
#include <boost/filesystem.hpp>
#endif // UNIX && USE_BOOST

namespace blood {
    namespace sys {
#if defined(WIN32) || defined(MSVS)
    namespace fs = std::tr2::sys;
#endif
#if defined(UNIX) && defined(USE_BOOST)
    namespace fs = boost::filesystem;
#endif
}}; // end blood::sys
