#pragma once
#ifndef blood_pattern_non_copyable_hxx_
#define blood_pattern_non_copyable_hxx_

namespace blood { namespace pattern { 

class NonCopyable
{
protected:
    NonCopyable(){}

private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

}} // blood::pattern

#endif // blood_pattern_non_copyable_hxx_