#!/bin/python

"""
Generates multiple resolutions of the dress
Uses ImageMagick's 'convert' utility
"""

ROOT = "/home/eric/NeuralNets/images/dress/"
ORIGINAL = ROOT + "thedress.jpg"

import os
import subprocess
import sys

for w in [1,2,4,8,10,20,30,50,800]:
	cmd = ["convert",ORIGINAL,"-resize","%dx%d!" % (w,w), ROOT + "thedress_%d.jpg" % w]
	print(cmd)
	subprocess.call(cmd)