import os
import cmake
import shutil

os.system('cls' if os.name == 'nt' else 'clear')
if os.path.exists('../UnixMake'):
	shutil.rmtree('../UnixMake')
cmake.buildCmake(
    'UnixMake',
    '../../',
    '../../assemble/'
)

os.chdir("../../assemble/UnixMake/")
os.system("make -j")