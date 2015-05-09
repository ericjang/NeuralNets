#!/usr/bin/python

"""
Visualizes firing rates for each neuron
Usage: python viz_color_selectivity.py [DATA_DIR]
"""


from neural_nets_pb2 import *
import numpy as np
import matplotlib.pyplot as plt
import os
import ipdb as pdb
from plots import *
import sys

def plot_layer(ax,raster,N,T,title):
	numSpikes = np.zeros(N)
	w = int(np.sqrt(N))
	assert(w*w==N)
	for spike in raster.spikes:
		numSpikes[spike.i] += 1
	rates = numSpikes/(float(T)/1000)
	#pdb.set_trace()
	X,Y = np.meshgrid(range(w),range(w))
	rate_img = np.reshape(rates,[w,w])
	levels_small = np.arange(0,50,1)
	ax.contourf(np.flipud(rate_img),levels_small,cmap=plt.cm.bone)
	ax.set_axis_off()
	ax.set_title(title)

# test to make sure plot_image is working.

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print("Usage: python viz_color_contours.py [COLOR_DATA_DIR]")
		sys.exit()
	data_dir = sys.argv[1]
	proto_files = [os.path.join(data_dir,fn) for fn in next(os.walk(data_dir))[2]]
	# plot map for each layer
	nColors = len(proto_files)
	fig, axes = plt.subplots(nColors,7)
	it=0
	for (r,proto_file) in enumerate(proto_files):
		data = open(proto_file,"rb").read() # file as string
		results = SimulationResults()
		results.ParseFromString(data)
		for (c,layer) in enumerate(results.nstate.layers[3:]):
			#col = (float(img.R[0])/255, float(img.G[0])/255, float(img.B[0])/255) # needs to be passed in as array of colors, so just 1 elem
			plot_layer(axes[r,c+1],results.spikes[c+3],layer.N,results.T,layer.name)
		plot_image(axes[r,0],results.img)
		it = results.iter
		# PART II: generate another figure with overlaps

	# fine-tuning : remove title from plots not on top
	for ax_row in axes[1:,:]:
		plt.setp([a.set_title(None) for a in ax_row], visible=False)
	plt.suptitle('Iteration: ' + str(it))
	plt.show()

	