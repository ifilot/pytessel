import os
import sys
import yaml
from yaml.loader import SafeLoader
import re

ROOT = os.path.join(os.path.dirname(__file__), '..')

#
# Check that versions are defined consistently across the program
#

def main():
    version_setup = read_setup()
    meta_yaml_version = read_yaml()
    version_source = read_source()
    
    print('setup.py: %s' % version_setup)
    print('meta.yaml: %s' % meta_yaml_version)
    print('_version.py: %s' % version_source)
    print()
    
    if (version_setup != meta_yaml_version) or \
       (version_setup != version_source) or \
       (meta_yaml_version != version_source):
           raise Exception('Versions do not match')
    else:
        print('All OK!')

def read_setup():
    """
    Read the version string from the setup file
    """
    f = open(os.path.join(ROOT, 'setup.py'))
    lines = f.readlines()
    f.close()
    
    pattern = r'\s*version="(\d+\.\d+\.\d+)"'
    for line in lines:
        res = re.match(pattern, line)
        if res:
            return res.group(1)

def read_yaml():
    """
    Read the version string from the YAML file
    """
    with open(os.path.join(ROOT, 'meta.yaml')) as f:
        data = yaml.load(f, Loader=SafeLoader)
        return data['package']['version']
    
def read_source():
    """
    Read the version string from the package source files
    """
    f = open(os.path.join(ROOT, 'pytessel', '_version.py'))
    lines = f.readlines()
    f.close()
    
    pattern = r'\s*__version__\s*=\s*"(\d+\.\d+\.\d+)"'
    for line in lines:
        res = re.match(pattern, line)
        if res:
            return res.group(1)
    
if __name__ == '__main__':
    main()
    