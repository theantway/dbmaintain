#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension

import glob

src_files = glob.glob('../../core/src/*/*.cpp')
src_files += glob.glob('../../core/src/*.cpp')
src_files += ['dbmaintain_wrap.cpp']

dbmaintain_module = Extension('_dbmaintain',
                           sources=src_files,
                           include_dirs = ['../../core/include', '../../lib/', '/usr/local/boost_1_44_0', '/opt/local/include/postgresql90'],
                           library_dirs=["/opt/local/lib/postgresql90"],
                           libraries=['crypto', 'pq']
                           )

setup (name = 'dbmaintain',
       version = '0.5',
       author      = "wxu",
       description = """dbmaintain""",
       ext_modules = [dbmaintain_module],
       py_modules = ["dbmaintain"],
       )
