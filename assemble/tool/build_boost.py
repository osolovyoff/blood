import os
import sys

# please, set BOOST_ROOT - to system variables


def buildBoost(boost_root):
    # bootstrap
    os.chdir(boost_root)
    bootstrap = 'bootstrap.bat'

    command = boost_root + bootstrap
    os.system(command)

    # bjam
    bjam = 'bjam'
    options = ' --toolset=msvc-12.0 address-model=64 --build-type=complete'

    command = boost_root + bjam + options
    os.system(command)


boost_already_builded = False

if not boost_already_builded:
    boost_dir = '../../extern/boost/'
    boost_dir = os.path.join(os.path.dirname(__file__), boost_dir)
    boost_file = os.path.join(boost_dir, 'bjam.exe')
    print(boost_file)
    if os.path.exists(boost_file):
        buildBoost(boost_dir)
        boost_already_builded = True

if not boost_already_builded:
    boost_dir = os.path.expandvars('$BOOST_ROOT')
    boost_dir = os.path.join(os.path.dirname(__file__), boost_dir)
    boost_file = os.path.join(boost_dir, 'bjam.exe')
    print(boost_file)
    if os.path.exists(boost_file):
        buildBoost(boost_dir)
        boost_already_builded = True
