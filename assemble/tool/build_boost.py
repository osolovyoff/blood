import os
import sys

# BOOST_ROOT without boost boost_1_59_0
boost_root = os.path.expandvars('$BOOST_ROOT')
boost_dir_array = []

for name in os.listdir(boost_root):
    if name[:5] == 'boost':
        full_folder_name = os.path.join(boost_root, name)
        if os.path.isdir(full_folder_name):
            boost_dir_array.append(full_folder_name)

boost_root = max(boost_dir_array)

link = 'static'      # static|shared
runtime = 'static'   #
threading = 'single'  # single|multi

address_model = '32' # 32|64|32_64
build_type = 'complete' # complete|stage
toolset = 'gcc' # acc | borland | como | darwin | gcc | hp_cxx | intel | msvc | sun | vacpp

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
