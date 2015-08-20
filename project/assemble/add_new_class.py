import os

def create_file(folder, filename, ext):
	file_name = os.path.join(folder, filename) + ext
	if not os.path.exists(folder):
		os.makedirs(folder)
	open(file_name,'w+')
    #if not os.path.exists(path):

s = os.path.dirname(__file__)
s = os.path.dirname(s)
s = os.path.dirname(s)
dir_include = os.path.join(os.path.join(s,'include'), 'blood')
dir_source = os.path.join(os.path.join(s,'source'), 'engine')

namespace = input("Enter namespace: ")
class_name = input("Enter classname: ")
yesno = input("create only header y/n? ")

header_dir = os.path.join(dir_include, namespace)
source_dir = os.path.join(dir_source, namespace)

create_file(header_dir, class_name, '.hxx')	

if yesno != 'y':
	create_file(source_dir, class_name, '.cxx')

input("press enter")