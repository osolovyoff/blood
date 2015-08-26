import os
import shutil

def getFullNameByAcronym(acronym):
    return {
        'VS2013': 'Visual Studio 12 2013',
        'VS2013x64': 'Visual Studio 12 2013 Win64',
        'VS2015':'Visual Studio 12 2015',
        'VS2015x64':'Visual Studio 14 2015 Win64',
        'VS2015ARM':'Visual Studio 14 2015 ARM',
        'UnixMake': 'Unix Makefiles',
        'MinGW': 'MinGW Makefiles',
        'CodeBlockMinGW': 'CodeBlocks - MinGw Makefiles',
        'CodeBlockNMake': 'CodeBlocks - NMake Makefiles',
        'CodeBlockNinja': 'CodeBlocks - Ninja',
        'CodeBlockUnixMake': 'CodeBlocks - Unix Makefiles'
    }[acronym]

def buildCmake(
        tool,
        cmake_file,
        projects
):
    tool_name = getFullNameByAcronym(tool)
    build_project = os.path.join(projects, tool)

    if not os.path.exists(build_project):
        os.makedirs(build_project)
    os.chdir(build_project)

    command = "cmake"
    command += " -G \"%s\"" % tool_name
    command += " \"%s\"" % cmake_file
    print(command)
    os.system(command)


def compile(tool, dir):
    if (tool == 'UnixMake' or tool == 'Unix Makefiles'):
        os.chdir(dir)
        os.system('make')


def clear(out):
    os.chdir(out)

    paths = [
        'cmake_install.cmake',
        'CMakeCache.txt',
        'CMakeFiles',
        'Makefile',
        'tool.dir',
        'Debug',
        '__pycache__',
        'ALL_BUILD.vcxproj',
        'ALL_BUILD.vcxproj.filters',
        'tool.sln',
        'tool.vcxproj',
        'tool.vcxproj.filters',
        'ZERO_CHECK.vcxproj',
        'ZERO_CHECK.vcxproj.filters'
    ]

    for path in paths:
        if os.path.exists(path):
            if os.path.isdir(path):
                shutil.rmtree(path)
            else:
                os.remove(path)