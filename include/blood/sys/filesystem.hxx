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

//    inline std::string getStringPath(blood::sys::fs::path _path)
    //{
//#if defined(BLOOD_VS) || defined(BLOOD_32)
        //return _path().string();
//#endif // defined windows
//#if defined(BLOOD_UNIX)
        //return fs::canonical(_path).string();
//#endif // defined
    //}

}} // end blood::sys
