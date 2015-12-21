import os
import cmake
import shutil

os.system('cls' if os.name == 'nt' else 'clear')
shutil.rmtree('../VS2013')
cmake.buildCmake(
    'VS2013',
    '../../',
    '../../assemble/'
)
