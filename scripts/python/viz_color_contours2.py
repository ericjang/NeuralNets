#!/usr/bin/python

"""
Visualizes overlaid contour plots
Usage: python viz_color_contours2.py
"""


from neural_nets_pb2 import *
import numpy as np
import matplotlib.pyplot as plt
import os
import ipdb as pdb
import sys

def plot_layer(ax,raster,N,T,title,col_arr):
	numSpikes = np.zeros(N)
	w = int(np.sqrt(N))
	assert(w*w==N)
	for spike in raster.spikes:
		numSpikes[spike.i] += 1
	rates = numSpikes/(float(T)/1000)
	X,Y = np.meshgrid(range(w),range(w))
	rate_img = np.reshape(rates,[w,w])
	#levels = np.arange(0,50,1)
	levels = np.array([30]) # arbitrary threshold
	ax.contour(np.flipud(rate_img),levels,colors=col_arr)
	ax.set_axis_off()
	ax.set_title(title)

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print("Usage: python viz_max_color.py [COLOR_DATA_DIR]")
		sys.exit()
	data_dir = sys.argv[1]
	proto_files = [os.path.join(data_dir,fn) for fn in next(os.walk(data_dir))[2]]
	fig, axes = plt.subplots(1,6) # ignore Photoreceptor layers
	it = 0
	for proto_file in proto_files:
		data = open(proto_file,"rb").read() # file as string
		results = SimulationResults()
		results.ParseFromString(data)
		img = results.img
		col = (float(img.R[0])/255, float(img.G[0])/255, float(img.B[0])/255) # needs to be passed in as array of colors, so just 1 elem
		for (c,layer) in enumerate(results.nstate.layers[3:]):
			plot_layer(axes[c], results.spikes[c+3], layer.N, results.T, layer.name,[col])
		it = results.iter
	plt.suptitle('Iteration: ' + str(it))
	plt.show()
