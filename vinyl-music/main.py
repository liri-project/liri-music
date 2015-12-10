#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import json

def world():
    return "Hello World"

def home():

    home = os.path.expanduser("~")

    #os.symlink(home + '/Music', './music')
    nh = str(home).replace('\\', '/').replace('C:', '')
    return str('file://' + nh + '/Music')



