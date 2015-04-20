#!/usr/bin/python

"""
Python script for visualizing weights (because PyPlot is too slow in Julia)
"""

import matplotlib.pyplot as plt
from neural_nets_pb2 import *
import numpy as np
import math
import sys

def plot_raster(raster, N, nsteps,T,title):
	C = np.array([spike.t for spike in raster.spikes])
	R = np.array([spike.i for spike in raster.spikes])
	plt.vlines(C,R-0.5,R+0.5)
	plt.xlim([0,nsteps])
	xticks = np.linspace(0,T,10) # 0->T in 10 steps 
	plt.xticks(xticks)
	plt.ylim([0,N])
	plt.title(title)
	return plt

def plot_vision_rasters(results):
	T = results.T
	dt = results.dt
	nsteps = int(math.ceil(T/dt))
	nL = len(results.nstate.layers)
	Dim = (3,13)
	fig, axes = plt.subplots(nrows=Dim[0], ncols=Dim[1])
	for i in range(nL):
		layer = results.nstate.layers[i]
		raster = results.spikes[i]
		print(layer.name)
		if i in range(3): # 0,1,2 -> 0,1,2
			ax = plt.subplot2grid(Dim,(i,0),colspan=2)
		elif i in range(3,6): # 3,4,5 -> 0,1,2
			ax = plt.subplot2grid(Dim,(i%3,2),colspan=2)
		elif i in range(6,9): # 6, 7, 8 -> 3, 6, 9
			 ax = plt.subplot2grid(Dim,(0,4+(i-6)*3),rowspan=3, colspan=3)
		plot_raster(raster,layer.N,nsteps,T,layer.name)


import ipdb as pdb

def plot_image(img):
	w = int(np.sqrt(len(img.R)))
	A = np.zeros((w,w,3))
	A[:,:,0] = np.reshape(img.R,(w,w))
	A[:,:,1] = np.reshape(img.G,(w,w))
	A[:,:,2] = np.reshape(img.B,(w,w))
	#pdb.set_trace()
	plt.imshow(A)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Usage: ./viz_rasters.py [FILE.protodat]")
		sys.exit()
	dat_name = sys.argv[1]
	data = open(dat_name,"rb").read() # file as string
	results = SimulationResults()
	results.ParseFromString(data)
	plot_image(results.img)
	plot_vision_rasters(results)
	plt.show()