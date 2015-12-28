import os
import cmake
import shutil

os.system('cls' if os.name == 'nt' else 'clear')
if os.path.exists('../CodeBlockUnixMake'):
	shutil.rmtree('../CodeBlockUnixMake')
cmake.buildCmake(
    'CodeBlockUnixMake',
    '../../',
    '../../assemble/'
)
