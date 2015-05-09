#!/usr/bin/python

"""
Python script that generates orientation responses from network
USAGE: python run_orientation.py ../../data/default_0000.protodat ../../images/orient10/ ../../data/orientations/iter_0/

Make sure that the net executable is running against the correct-sized inputs!

"""

ROOT = "/home/eric/NeuralNets/"

import os
import subprocess
import sys

if len(sys.argv) != 4:
	print("Usage: ./run_orientation.py [FILE.protodat] [ORIENT_IMGS] [OUTDIR]")
	sys.exit()

protofile = os.path.abspath(sys.argv[1])
img_dir = os.path.dirname(sys.argv[2]) 
outdir = os.path.dirname(sys.argv[3])

# sample intervals of 2 degrees
img_files = [img_dir + "/" + str(i*4) + ".jpg" for i in range(45)]

# verify all the files exist
for img_file in img_files:
	assert(os.path.isfile(img_file))
	print(img_file + " OK")

# every 4 degrees
for i in range(45):
	d = i*4
	img_file = img_files[i]
	cmd = [ROOT + "nets", "test", protofile, img_file, outdir + '/' + str(d) + ".protodat"]
	print(cmd)
	subprocess.call(cmd)