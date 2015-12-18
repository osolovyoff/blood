

#include "file.hxx"
#include <iostream>


namespace fs = boost::filesystem;

File::File()
{

}

File::File(std::string file_string)
{
    m_file = fs::path(file_string);
    m_is_exist = fs::exists(m_file);
}

File::~File()
{

}

void File::zero()
{
    create();
}

void File::open()
{

}

void File::close()
{

}

void File::print()
{
    if (m_is_exist)
    {
        std::cout << m_file.string() << std::endl;
    }
}

void File::create()
{
    if (!m_is_exist)
    {
        std::ofstream file;
        file.open(m_file.string());
        if (file.is_open())
        {
            file << "";
            m_is_exist = true;
            file.close();
        }
    }
}

void File::remove()
{
    if (m_is_exist)
    {
        fs::remove(m_file);
    }
}

void File::operator=(const File& file)
{
    *this = file;
}

void File::operator=(std::string file_path)
{
    *this = File(file_path);
}

#endif // USE_BOOST

