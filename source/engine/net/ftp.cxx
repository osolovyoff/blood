#include "../../../include/blood/net/ftp.hxx"

#if defined(USE_CURL)

#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

size_t loadFile(void* _buf, size_t _size, size_t _n, void* _stream);
size_t sendFile(void* _ptr, size_t _size, size_t _n, void* _stream);

size_t writeDataToStringFunction(void* _ptr, size_t _size, size_t _n, std::string* _string)
{
    const size_t size = _size*_n;
    _string->resize(size);
    memcpy((void*)_string->data(), _ptr, size);
    return _size*_n;
}


#if defined(BLOOD_VS) || defined(BLOOD_32)
//#include <io.h>
#else
//#include <unistd.h>
#endif

struct FTPFile
{
    const char* file_name;
    FILE*       stream;
};

struct MemoryStruct
{
    char*  memory;
    size_t size;
};

blood::net::FTP::FTP(std::string addr, std::string user, std::string pass)
: m_address(addr)
, m_user(user)
, m_pass(pass)
, m_enable_ftps(false)
{}

bool blood::net::FTP::downloadFile(const std::string file_path, const std::string local_file)
{
    initialize();

    if (m_curl)
    {
        FTPFile ftp_file = { local_file.c_str(), NULL };
        const std::string ftp_filepath = m_address + file_path;

        curl_easy_setopt(m_curl, CURLOPT_USERNAME, m_user.c_str());
        curl_easy_setopt(m_curl, CURLOPT_PASSWORD, m_pass.c_str());
        curl_easy_setopt(m_curl, CURLOPT_URL, ftp_filepath.c_str());

        initializeProtocol();
        //curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);

        curl_easy_setopt(m_curl, CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR);

        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, loadFile);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &ftp_file);
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);

        processing(m_curl);
        
        if (ftp_file.stream)
            fclose(ftp_file.stream);

        curl_easy_cleanup(m_curl);
    }
    curl_global_cleanup();
    return (CURLE_OK == m_code);
}


bool blood::net::FTP::uploadFile(const std::string local_file_path, const std::string ftp_path)
{
    initialize();
    m_file = nullptr;

    struct stat file_info;
    const int result = stat(local_file_path.c_str(), &file_info);
    if (result)
    {
        printf("Couldn't open'%s': %s\n", local_file_path, strerror(errno));
        return false;
    }

    if (m_curl)
    {
        curl_easy_setopt(m_curl, CURLOPT_USERNAME, m_user.c_str());
        curl_easy_setopt(m_curl, CURLOPT_PASSWORD, m_pass.c_str());

        const std::string full_path = m_address + ftp_path;

        curl_easy_setopt(m_curl, CURLOPT_URL, full_path.c_str());
        initializeProtocol();

        curl_off_t  file_size;
        curl_slist* header_list = nullptr;

        file_size = (curl_off_t)file_info.st_size;
        printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", file_size);
        m_file = fopen(local_file_path.c_str(), "rb");

        curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, sendFile);
        curl_easy_setopt(m_curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(m_curl, CURLOPT_INFILESIZE_LARGE, file_size);
        curl_easy_setopt(m_curl, CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR);

        processing(m_curl);


        curl_easy_cleanup(m_curl);

        fclose(m_file);
    }
    curl_global_cleanup();
    return (CURLE_OK == m_code);
}

bool blood::net::FTP::getFileToString(const std::string from_ftp_file, std::string& _file_string)
{
    initialize();

    if (m_curl)
    {
        const std::string file = m_address + from_ftp_file;
        initializeProtocol();

        curl_easy_setopt(m_curl, CURLOPT_USERNAME, m_user.c_str());
        curl_easy_setopt(m_curl, CURLOPT_PASSWORD, m_pass.c_str());
        curl_easy_setopt(m_curl, CURLOPT_URL, file.c_str());
        curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);

        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeDataToStringFunction);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &_file_string);
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);

        processing(m_curl);
        curl_easy_cleanup(m_curl);
    }
    curl_global_cleanup();
    return (CURLE_OK == m_code);
}

void blood::net::FTP::downloadoruploadarray()
{
    //const String buf_str_1 = String("RNFR ") + String(_from);
    //const String buf_str_2 = String("RNTO ") + String(_to);

    //const char* buf_1 = buf_str_1.c_str();
    //const char* buf_2 = buf_str_2.c_str();

    //header_list = curl_slist_append(header_list, buf_1);
    //header_list = curl_slist_append(header_list, buf_2);

    //curl_easy_setopt(m_curl, CURLOPT_POSTQUOTE, header_list);

    //curl_slist_free_all(header_list);
}

void blood::net::FTP::reconnect(std::string ftp_addr, std::string ftp_user, std::string ftp_pass)
{
    m_address = ftp_addr;
    m_user = ftp_user;
    m_pass = ftp_pass;
}

void blood::net::FTP::enableFTPS()
{
    m_enable_ftps = true;
}

void blood::net::FTP::initialize()
{
    m_curl = nullptr;
    m_code = curl_global_init(CURL_GLOBAL_ALL);
    m_curl = curl_easy_init();
    curl_easy_setopt
        ( m_curl
        , CURLOPT_PROTOCOLS
        , CURLPROTO_HTTP 
        | CURLPROTO_HTTPS
        | CURLPROTO_FTP
        | CURLPROTO_FTPS
        | CURLPROTO_SFTP
        | CURLPROTO_TFTP);
}

size_t readMemory(void* _buf, size_t _size, size_t _n, void* _stream)
{
    const size_t realsize = _size*_n;
    struct MemoryStruct *mem = (struct MemoryStruct *) _stream;

    mem->memory = (char*)realloc(&mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL)
    {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), _buf, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


void blood::net::FTP::initializeProtocol()
{
    if (m_enable_ftps)
    {
        curl_easy_setopt(m_curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
}

void blood::net::FTP::processing(CURL* _curl)
{
    m_code = curl_easy_perform(_curl);
    if (CURLE_OK != m_code)
    {
        printf_s("error, code of error: (%d).%s", m_code, curl_easy_strerror(m_code));
    }
}

size_t loadFile(void* _buf, size_t _size, size_t _n, void* _stream)
{
    struct FTPFile* out = static_cast<FTPFile*>(_stream);
    if (out && !out->stream){
        out->stream = fopen(out->file_name, "wb");
        if (!out->stream)
            return -1;
    }
    return fwrite(_buf, _size, _n, out->stream);
}

size_t sendFile(void* _ptr, size_t _size, size_t _n, void* _stream)
{
    FILE* out = static_cast<FILE*>(_stream);
    curl_off_t nread;

    size_t retcode = fread(_ptr, _size, _n, out);
    nread = (curl_off_t)retcode;

    fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
        " bytes from file\n", nread);
    return retcode;
}

#endif // end USE_CURL

