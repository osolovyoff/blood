import os
import shutil


project = os.path.dirname(os.path.dirname(__file__))

for name in os.listdir(project):
    if name != 'cmake':
        full_path = os.path.join(project, name)
        shutil.rmtree(full_path)
