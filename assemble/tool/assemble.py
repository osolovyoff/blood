import os
import cmake
import shutil

def clear_display():
	os.system('cls' if os.name == 'nt' else 'clear')

tools = []
tools.append("Unix Makefiles")
tools.append("MinGW Makefiles")
tools.append("Visual Studio 12 2013")
tools.append("Visual Studio 12 2013 Win64")
tools.append("Visual Studio 12 2015")
tools.append("Visual Studio 12 2015 Win64")
tools.append("CodeBlocks - Unix Makefiles")

is_success = False

while is_success == False:
	clear_display()
	print("Choose build tool:")
	for i in range(0,len(tools)):
		print("%s.%s" % (i+1,tools[i]))

	print
	n = input("Enter the tool number:")
	if (n > 0) and (n < len(tools)):
		is_success = True

selected_tool = tools[n-1]
selected_tool_dir = os.path.join('..',selected_tool)

if os.path.exists(selected_tool_dir):
 	shutil.rmtree(selected_tool_dir)
cmake.buildCmake(
	selected_tool,
	'../../',
	'../../assemble/'
)
