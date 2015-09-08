import os
import sys

boost_root = os.path.expandvars('$BOOST_ROOT')


link = 'static'      # static|shared
runtime = 'static'   #
threading = 'multi'  # single|multi

address_model = '64' # 32|64|32_64
build_type = 'complete'
toolset = 'msvc-12.0'



install_directory = boost_root # You can will set your value !

if boost_root != "":
    os.chdir(boost_root)

    boost_installer = os.path.join(boost_root, 'b2.exe')        # You won't use the bjam.exe
    boost_bootstrap = os.path.join(boost_root, 'bootstrap.bat')

    if os.path.exists(boost_bootstrap):
        if not os.path.exists(boost_installer):
            os.system(boost_bootstrap)

        if os.path.exists(boost_installer):
            os.system("%s debug release toolset=%s address-model=%s link=%s threading=%s runtime-link=%s --build_type=%s install --prefix=%s"
            % (boost_installer,toolset,address_model,link,threading,runtime,build_type,boost_root))
        else:
            print("%s not found" % boost_installer)

    else:
        print("%s not found" % boost_bootstrap)
else:
    print("please, set $BOOST_ROOT to expand variables") 