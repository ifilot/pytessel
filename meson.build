# ---------------------------------------------------------------------------
# Top-level project definition
# ---------------------------------------------------------------------------
#
# This project builds a Python extension module backed by C++ and Cython.
# We explicitly require a modern Meson version to ensure stable behavior
# with meson-python and PEP 517 builds.
#
project(
    'pytessel',
    ['cpp', 'cython'],
    meson_version: '>=1.0.0',
    default_options: [
        'buildtype=release',  # Optimized builds by default
        'cpp_std=c++17',      # Required by the C++ sources
    ]
)

# ---------------------------------------------------------------------------
# Python and compiler discovery
# ---------------------------------------------------------------------------
#
# We ask Meson for the *non-pure* Python installation because this project
# builds a compiled extension (.pyd / .so), not a pure-Python wheel.
#
# meson-python ensures that this Python installation corresponds to the
# wheel being built inside the PEP 517 environment.
#
py = import('python').find_installation(pure: false)

# Fetch the C++ compiler object. We do not force a specific compiler here;
# the actual toolchain selection is handled externally (CI configuration).
#
# On Windows CI we intentionally rely on MinGW for robustness.
#
cpp = meson.get_compiler('cpp')


# ---------------------------------------------------------------------------
# OpenMP support
# ---------------------------------------------------------------------------
openmp_dep = []
openmp = dependency('openmp', required: false)
if openmp.found()
    openmp_dep = [openmp]
endif

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
#
# Project headers live inside the Python package directory itself.
# This allows:
#   - a clean package layout
#   - easy access to headers from both C++ and Cython sources
#
inc = include_directories('pytessel')


# ---------------------------------------------------------------------------
# Compiled Python extension module
# ---------------------------------------------------------------------------
#
# This builds the core extension module (pytessel_core) which is imported from
# Python as:
#
#     from pytessel import pytessel_core
#
# Notes:
#   - Sources include both C++ and a Cython .pyx file
#   - cython_language=cpp ensures Cython generates C++ code
#   - subdir='pytessel' ensures the extension is installed inside the package
#   - install=true is required for wheel builds
#
# Toolchain notes (important for CI):
#   - On Windows, this is built with MinGW in CI and repaired with delvewheel
#   - We intentionally avoid MSVC-specific logic here to keep meson.build
#     platform-agnostic and CI-stable
#
py.extension_module(
    'pytessel_core',
    [
        'pytessel/pytessel_core.pyx',
        'pytessel/isosurface_mesh.cpp',
        'pytessel/isosurface.cpp',
        'pytessel/scalar_field.cpp',
    ],
    subdir: 'pytessel',
    include_directories: inc,
    override_options: ['cython_language=cpp'],
    dependencies: openmp_dep,
    install: true,
)

# ---------------------------------------------------------------------------
# Pure-Python source files
# ---------------------------------------------------------------------------
py.install_sources(
    'pytessel/__init__.py',
    'pytessel/_version.py',

    subdir: 'pytessel',
)