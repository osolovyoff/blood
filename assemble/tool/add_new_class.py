import os

# additional function
#________________________________________________________________
def create_file(folder, filename, ext):
	file_name = os.path.join(folder, filename) + ext
	if not os.path.exists(folder):
		os.makedirs(folder)
		open(file_name,'w+')
	else:
		open(file_name,'w+')
#________________________________________________________________

s = os.path.dirname(__file__)
s = os.path.dirname(s)
s = os.path.dirname(s)
dir_include = os.path.join(os.path.join(s,'include'), 'blood')
dir_source = os.path.join(os.path.join(s,'source'), 'engine')
dir_code = os.path.join(os.path.join(s,'source'), 'code')

is_engine_code = input("You will want to create the: \n 1.engine(include/)\n 2.application(code/)\n\nEnter(1/2): ")

if is_engine_code == '1':
	namespace = input("Enter namespace: ")
	class_name = input("Enter classname: ")
	yesno = input("inline data y/n? ")

	header_dir = os.path.join(dir_include, namespace)
	source_dir = os.path.join(dir_source, namespace)

	create_file(header_dir, class_name, '.hxx')	

	if yesno != 'y':
		create_file(source_dir, class_name, '.cxx')

	input("press enter")	
else:
	class_name = input("Enter class name: ")
	print(dir_code)
	print(class_name)
	create_file(dir_code, class_name, '.cxx')
	create_file(dir_code, class_name, '.hxx')