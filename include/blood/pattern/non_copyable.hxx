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