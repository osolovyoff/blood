import os
import cmake
import shutil

os.system('cls' if os.name == 'nt' else 'clear')
if os.path.exists('../VS2013x64'):
	shutil.rmtree('../VS2013x64')
cmake.buildCmake(
    'VS2013x64',
    '../../',
    '../../assemble/'
)
