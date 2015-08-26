#pragma once
#ifndef macros_h__
#define macros_h__

#define blood_code_line std::string(__FILE__) + "[" + std::to_string(__LINE__) + "]:"

#define blood_return_if(x) if (x){return;}
#define blood_is_null(x) x == 0
#define blood_is_not_null(x) != 0

#if defined(_THREAD_)
#define blood_join_all(x)              \
    std::for_each(x.begin(), x.end(),  \
    [](std::thread& t)                 \
    {                                  \
    t.join();                          \
    });

#define blood_thread_create_thread(method, obj, thread_count)      \
    std::vector< std::thread > thread_group;                 \
    for (unsigned int i = 0; i < thread_count; ++i)          \
    thread_group.push_back(std::thread(method, obj));
#endif // end _THREAD_

#if defined(_MEMORY_)                     
#define blood_synonym_for(x)              \
    typedef std::unique_ptr<x> x##Unique; \
    typedef std::shared_ptr<x> x##Shared; \
    typedef x&                 x##Ref;    \
    typedef x*                 x##Ptr;       
#else                                     
#define blood_synonym_for(x)              \
    typedef x&                 x##Ref;    \
    typedef x*                 x##Ptr;    
#endif // end _MEMORY_

#define blood_main_winapi(instance,instance_prev,cmdline,cmdshow)  \
    WINAPI WinMain                                                 \
    ( HINSTANCE instance                                           \
    , HINSTANCE instance_prev                                      \
    , LPSTR     cmdline                                            \
    , int       cmdshow)

#define blood_catch_windows(title)                          \
    catch (std::exception& _ex)                             \
{                                                           \
    MessageBox(NULL, _ex.what(), title                      \
    , MB_ICONERROR | MB_OK);                                \
}                                                           \
    catch (...)                                             \
{                                                           \
    MessageBox(NULL                                         \
    , "unhanded exception"                                  \
    , title                                                 \
    , MB_ICONERROR | MB_OK);                                \
}

#define blood_catch_console                                 \
    catch (std::exception& _ex)                             \
    {                                                       \
        std::cout << _ex.what() << std::endl;               \
        std::cout << "press enter..." << std::endl;         \
        std::cin.get();                                     \
    }                                                       \
    catch (...)                                             \
    {                                                       \
        std::cout << "unhanded exception" << std::endl;     \
        std::cout << "press enter..." << std::endl;         \
        std::cin.get();                                     \
    }                                                       

#endif // macros_h__