#!/usr/bin/env python
# -*- coding: utf-8 -*-

from os.path import expanduser
import os

def world():
    return "Hello World"

def home():

    home = expanduser("~")

    #os.symlink(home + '/Music', './music')
    nh = str(home).replace('\\', '/').replace('C:', '')
    return str('file://' + nh + '/Music')

