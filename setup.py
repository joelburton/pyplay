"""Setup file."""

from setuptools import setup, Extension, find_packages

spam = Extension("spam", sources=['spammodule.c'])
bigram = Extension("bigram", sources=['bigram.c'])

setup(name='Spam',
      version='1.0',
      description='This is a demo package',
      packages = ["."],
      ext_modules=[spam, bigram])
