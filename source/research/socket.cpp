//#include "socket.hpp"
//
//#include <stdio.h>
//#include <string.h>
//#include <curl/curl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <errno.h>
//#ifdef WIN32
//#include <io.h>
//#else
//#include <unistd.h>
//#endif
//
//#include <curl/curl.h>
//
////int Socket::initializeSock(const char* _ip, UInt _port)
////{
////    //const int signal = socket(AF_INET, SOCK_STREAM, 0);
////
////    //sockaddr_in address;
////    //address.sin_family = AF_INET;              // domen
////    //address.sin_addr.s_addr = inet_addr(_ip);  // ip
////    //address.sin_port = htons(_port);           // port
////    //const int len = sizeof(address);
////    //
////    //const int result = connect(signal, (sockaddr*)&address, len);
////    //if (result == -1)
////    //{
////    //    perror("ooops: client");
////    //    return -1;
////    //}
////
////    //return signal;
////    return 0;
////}
////
////int Socket::readServer(int _sygnal)
////{
////    //int rc;
////    //fd_set fdr;
////    //FD_ZERO(&fdr);
////    //FD_SET(_sygnal, &fdr);
////
////    //timeval timeout;
////    //timeout.tv_sec = 1;
////    //timeout.tv_usec = 0;
////
////    //do{
////    //    char buff[512] = { ' ' };
////    //    recv(_sygnal, buff, 512, 0);
////    //    std::cout << buff;
////    //    rc = select(_sygnal + 1, &fdr, NULL, NULL, &timeout);
////    //} while (rc);
////
////    return 2;
////}
////
////int Socket::login()
////{
////    /*std::wcout << L"¬ведите им€:"; char name[64]; std::cin >> name;
////    char str[512];
////
////    sprintf(str, "USER %s\r\n", name);
////    send(s, str, strlen(str), 0);
////
////    readServer();
////    std::cout << "¬ведите пароль: "; char pass[64]; std::cin >> pass;
////    sprintf(str, "PASS %s\r\n", pass);
////
////    send(s, str, strlen(str), 0);
////    readServer();*/
////    
////    return 0;
////}
////
////int Socket::getFile(const char* _file)
////{
////    //char str[512];
////    //sprintf(str, "RETR %s\r\n", _file);
////
////    //char size[512];
////    //recv(str, size, 512, 0);
////    //std::cout << size;
////    ///* */
////
////    return 0;
////}
//
//void Socket::getFile()
//{
//    CURL* curl;
//    CURLcode res;
//    struct FtpFile ftp_file = { "666.txt", NULL /* name to store the file as if succesful */ };
//
//    auto handle = curl_global_init(CURL_GLOBAL_DEFAULT);
//
//    curl = curl_easy_init();
//    if (curl)
//    {
//        //curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, void* ptr);
//
//        curl_easy_setopt(curl, CURLOPT_URL, "ftp://86.57.149.5/");
//        
//        curl_easy_setopt(curl, CURLOPT_USERPWD, "ch_cl_user:werPa119");
//
//        /*curl_easy_setopt(curl, CURLOPT_USERNAME, "ch_cl_user");
//        curl_easy_setopt(curl, CURLOPT_PASSWORD, "werPa119");*/
//
//        /* CRYPTING */
//        /*
//        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_SSLv2);
//        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
//
//        curl_easy_setopt(curl, CURLOPT_URL, "ftp://86.57.149.5/test.txt");
//        
//
//        curl_easy_setopt(curl, CURLOPT_PORT, 21);      
//        
//
//        curl_easy_setopt( curl
//                        , CURLOPT_WRITEFUNCTION
//                        , fnFileWrite);
//
//        curl_easy_setopt( curl
//                        , CURLOPT_WRITEDATA
//                        , &ftp_file);
//
//        curl_easy_setopt( curl
//                        , CURLOPT_VERBOSE
//                        , 1L);
//        */
//        handle = curl_easy_perform(curl);
//
//        if (CURLE_OK != handle)
//        {
//            std::cout << "error res CURLE_OK" << std::endl;
//            std::cout << "error type: " << curl_easy_strerror(handle) << std::endl;
//        }
//
//        std::cin.get();
//        curl_easy_cleanup(curl);
//    }
//
//    // JUST TODO IT 
//    /*if (ftp_file->stream)
//        fclose(stderr, "cult told us %d\n, res");*/
//
//    curl_global_cleanup();
//}
//
//#define local_file "c:/workspace/becloud_projects/rat/bin/Debug/666.txt"
//#define upload_file_as "666.txt"
//#define remote_url     "ftp://86.57.149.5/"
//#define rename_file_to "file.txt"
//
//static size_t read_Callback(void* ptr, size_t size, size_t nmemb, void* stream)
//{
//    curl_off_t nread;
//    size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);
//    nread = (curl_off_t)retcode;
//    fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from filen", nread);
//    return retcode;
//}
//
//void Socket::sendFile(const std::string& _address, const std::string& _port)
//{
//    CURL* curl;
//    CURLcode res;
//    FILE* hd_src;
//
//    //struct FtpFile ftp_file = { "test.txt", NULL /* name to store the file as if succesful */ };
//
//    struct stat file_info;
//
//    curl_off_t fsize;
//    struct curl_slist* headerlist = NULL;
//
//    static const char buf_1[] = "RNFR " upload_file_as;
//    static const char buf_2[] = "RNTO " rename_file_to;
//
//    if ( stat(local_file, &file_info) )
//    {
//        std::cout << "file not exist" << std::endl;
//    }
//
//    fsize = (curl_off_t)file_info.st_size;
//    printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.n", fsize);
//    hd_src = fopen(local_file, "rb");
//    curl_global_init(CURL_GLOBAL_ALL);
//    curl = curl_easy_init();
//
//    if (curl)
//    {
//        //headerlist = curl_slist_append(headerlist, buf_1);
//        //headerlist = curl_slist_append(headerlist, buf_2);
//        //
//        ///* SSL */
//        //curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_NONE);
//        //curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
//        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_SSLv2);
//        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
//        //curl_easy_setopt(curl, CURLOPT_FTP_SSL, CURLFTPSSL_TRY);
//        //curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS);
//
//        /*curl_easy_setopt(curl, CURLOPT_URL, remote_url);
//        curl_easy_setopt(curl, CURLOPT_PORT, 21);
//        curl_easy_setopt(curl, CURLOPT_USERNAME, "ch_cl_user");
//        curl_easy_setopt(curl, CURLOPT_PASSWORD, "werPa119");*/
//
//        curl_easy_setopt(curl, CURLOPT_URL, "ftp://vh49.hoster.by");
//        curl_easy_setopt(curl, CURLOPT_USERNAME, "remontas");
//        curl_easy_setopt(curl, CURLOPT_PASSWORD, "ushail0K");
//
//        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_Callback);
//        //curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
//        
//        //curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
//        //curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
//        //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
//
//        auto handle = curl_easy_perform(curl);
//        if (CURLE_OK != handle)
//        {
//            std::cout << "error res CURLE_OK" << std::endl;
//            std::cout << "error type: " << curl_easy_strerror(handle) << std::endl;
//        }
//
//        curl_global_cleanup();
//        std::cin.get();
//    }
//}
