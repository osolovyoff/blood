#pragma once
#ifndef blood_sys_environment_hxx_
#define blood_sys_environment_hxx_

namespace blood
{
	namespace sys
	{
		bool is_64_environment()
		{
			#if _WIN32 || _WIN64
			#if _WIN64
			return true;
			#else
			return false;
			#endif
			#endif // end _WIN32

			// Check GCC
			#if __GNUC__
			#if __x86_64__ || __ppc64__
			return true;
			#else
			return false;
			#endif
			#endif // end __GNUC__
		}
	}
}


#endif//blood_sys_environment_hxx_