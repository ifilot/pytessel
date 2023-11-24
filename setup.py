import subprocess
from setuptools import Extension, setup
from Cython.Build import cythonize
import os
import sys

def find_windows_versions():
    """
    Autofind the msvc and winkit versions; this is mainly used for local development / compilation
    """
    root = os.path.join('C:', os.sep,'Program Files', 'Microsoft Visual Studio', '2022', 'Community', 'VC', 'Tools', 'MSVC')

    # for Gitlab actions, the above folder does not exist and this is communicated
    # back by providing None as the result
    if not os.path.exists(root):
        return None, None

    for file in os.listdir(root):
        if os.path.isdir(os.path.join(root, file)):
            msvcver = file
        
    root = os.path.join('C:', os.sep,'Program Files (x86)', 'Windows Kits', '10', 'Include')
    for file in os.listdir(root):
        if os.path.isdir(os.path.join(root, file)):
            winkitver = file

    return msvcver, winkitver

# specify paths on Windows to find compiler and libraries
if os.name == 'nt':
    msvc_ver, winkit_ver = find_windows_versions()

    if msvc_ver and winkit_ver:
        # only proceed with setting the paths for local development, i.e. when the
        # msvc_ver and winkit_ver variables are *not* None
        os.environ['PATH'] += r";C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\%s\bin\Hostx64\x64" % msvc_ver
        os.environ['PATH'] += r";C:\Program Files (x86)\Windows Kits\10\bin\%s\x64" % winkit_ver

        # set path to include folders
        os.environ['INCLUDE'] += r";C:\Program Files (x86)\Windows Kits\10\Include\%s\ucrt" % winkit_ver
        os.environ['INCLUDE'] += r";C:\Program Files (x86)\Windows Kits\10\Include\%s\shared" % winkit_ver
        os.environ['INCLUDE'] += r";C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\%s\include" % msvc_ver

        # some references to libraries
        os.environ['LIB'] += r";C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\%s\lib\x64" % msvc_ver
        os.environ['LIB'] += r";C:\Program Files (x86)\Windows Kits\10\Lib\%s\um\x64" % winkit_ver
        os.environ['LIB'] += r";C:\Program Files (x86)\Windows Kits\10\Lib\%s\ucrt\x64" % winkit_ver
    else:
        # re-order paths to ensure that the MSVC toolchain is in front; this needs to be done
        # because the Git bin folder precedes the MSVC bin folder, resulting in the wrong link.exe
        # executable to be used in the linking step
        paths = os.environ['PATH'].split(";")
        newpaths = []
        for path in paths:
            if "Microsoft Visual Studio" in path:
                newpaths = [path] + newpaths
            else:
                newpaths.append(path)
        os.environ['PATH'] = ";".join(newpaths)

if os.name == 'posix' and sys.platform != 'darwin':
    os.environ['CFLAGS'] = '-I/usr/include/glm'
    extra_compile_args = ["-Wno-date-time", "-fopenmp", "-fPIC"]
    extra_link_args = ["-fopenmp"]
elif os.name == 'nt':
    extra_compile_args = ["/wd4244"]
    extra_link_args = []
elif sys.platform == 'darwin':
    os.environ['CFLAGS'] = '-I/usr/local/Cellar/boost/1.81.0_1/include -I /usr/local/Cellar/glm/0.9.9.8/include'
    extra_compile_args = ["-Wno-date-time", "-fPIC", "-std=c++14"]
    extra_link_args = []

ext_modules = [
    Extension(
        "pytessel.pytessel",
        ["pytessel/pytessel.pyx"],
        extra_compile_args=extra_compile_args, # overrule some arguments
        extra_link_args=extra_link_args
    ),
]

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name='pytessel',
    version="1.1.0",
    author="Ivo Filot",
    author_email="ivo@ivofilot.nl",
    description="Python package for building isosurfaces from 3D scalar fields",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/ifilot/pytessel",
    ext_modules=cythonize(ext_modules[0],
                          language_level = "3",
                          build_dir="build"),
    packages=['pytessel'],
    include_package_data=True,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX",
    ],
    python_requires='>=3.5',
    install_requires=['numpy'],
)
