import os
import sys
import shutil

#_________________________________________________________________________
def copyLibraryFromOpenSSLDirectory(openssl_directory, is_64: bool):
    files = []
    if is_64:
        build_dir = os.path.join(openssl_dir, "build\\Win64\\VC12\\")
    else:
        build_dir = os.path.join(openssl_dir, "build\\Win32\\VC12\\")

    print(build_dir)

    if os.path.exists(build_dir):
        files.append(os.path.join(build_dir, "DLL Release\\openssl.exe"))
        files.append(os.path.join(build_dir, "DLL Release\\libeay32.dll"))
        files.append(os.path.join(build_dir, "DLL Release\\ssleay32.dll"))
        files.append(os.path.join(build_dir, "LIB Release\\libeay32.lib"))
        files.append(os.path.join(build_dir, "LIB Release\\ssleay32.lib"))
        files.append(os.path.join(openssl_directory, "inc32\\"))

    for file in files:
        print(file)

    return files
#_________________________________________________________________________
libcurl_build_filepath = "..\\..\\extern\\curl\\projects\\build-openssl.bat"
openssl_dir = "..\\..\\extern\\openssl\\"

this = os.path.dirname(__file__)
run_script = os.path.join(this, libcurl_build_filepath)

# SET PLATFORM !
is_64 = True

if is_64:
    parameters = ['x64 debug', 'x64 release']
else:
    parameters = ['x86 debug', 'x86 release']

print()
for param in parameters:
    command = "%s vc12 %s %s" % (run_script, param, openssl_dir)
    #print(command)
    # print()
    os.system(command)


files_for_copy = copyLibraryFromOpenSSLDirectory(openssl_dir, is_64)
for file in files_for_copy:
    if os.path.exists(file):
        if os.path.isdir(os.path.isdir):
            dest = os.path.join()
#        dest = os.path.
#        shutil.copyfile(file,)

print("finish...")