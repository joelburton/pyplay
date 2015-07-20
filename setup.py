"""Setup file."""

from distutils.core import setup, Extension

spam = Extension("spam", sources=['spammodule.c'])
bigram = Extension("bigram", sources=['bigram.c'])

setup(name='Spam',
      version='1.0',
      description='This is a demo package',
      ext_modules=[spam, bigram])