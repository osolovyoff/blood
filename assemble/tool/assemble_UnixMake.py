import os
import cmake

cmake.buildCmake(
    'UnixMake',
    '../../',
    '../../assemble/'
)

os.chdir("../../assemble/UnixMake/")
os.system("make -j")