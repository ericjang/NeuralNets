#!/usr/bin/python

"""
Python script generates selectivity data for 8 colors, given a set of pre-trained
weights.
Usage: Usage: ./run_colors.py [FILE.protodat] [OUTDIR]
"""

ROOT = "/home/eric/NeuralNets/"

import os
import subprocess
import sys

if len(sys.argv) != 4:
	print("Usage: ./run_clustering.py [FILE.protodat] [IMG_DIR] [OUTDIR]")
	sys.exit()

protofile = os.path.abspath(sys.argv[1])
colors_dir = os.path.abspath(sys.argv[2])
outdir = os.path.dirname(sys.argv[3])

img_files = [os.path.join(colors_dir,fn) for fn in next(os.walk(colors_dir))[2]]
for i,img_file in enumerate(img_files):
	cmd = [ROOT + "nets", "test", protofile, img_file, outdir + '/' + str(i) + ".protodat"]
	print(cmd)
	subprocess.call(cmd)