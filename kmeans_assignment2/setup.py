from distutils.core import setup, Extension

extension_mod = Extension('mykmeanssp',
                          sources=['kmeansmodule.c'],
                          )

setup(name='mykmeanssp',
      version='1.0',
      description='A Python extension module for k-means clustering',
      ext_modules=[extension_mod],
      )