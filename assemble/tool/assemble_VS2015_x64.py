import os
import cmake
import shutil

os.system('cls' if os.name == 'nt' else 'clear')
if os.path.exists('../VS2015x64'):
	shutil.rmtree('../VS2015x64')
cmake.buildCmake(
    'VS2015x64',
    '../../',
    '../../assemble/'
)
