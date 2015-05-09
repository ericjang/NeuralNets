#!/usr/bin/python

"""
Python script generates selectivity data for 8x8 color patches, given a set of pre-trained
weights.
Usage: Usage: ./run_colors.py [FILE.protodat] [OUTDIR]
"""

ROOT = "/home/eric/NeuralNets/"

import os
import subprocess
import sys

if len(sys.argv) != 4:
	print("Usage: ./run_clustering.py [FILE.protodat] [IMAGE_DIR] [OUTDIR]")
	sys.exit()

protofile = os.path.abspath(sys.argv[1])
colors_dir = os.path.abspath(sys.argv[2])
outdir = os.path.dirname(sys.argv[3])

# run check
for i in range(1,9):
	for j in range(1,9):
		img_file = os.path.join(colors_dir,"%d_%d.jpg" % (i,j))
		assert(os.path.isfile(img_file))
		print(img_file + " OK")

for i in range(1,9):
	for j in range(1,9):
		img_file = os.path.join(colors_dir,"%d_%d.jpg" % (i,j))
		cmd = [ROOT + "nets", "test", protofile, img_file, outdir + '/' +"%d_%d.protodat" % (i,j)]
		print(cmd)
		subprocess.call(cmd)