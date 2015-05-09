#!/usr/bin/python
"""
Better version of contour plots: visualize maps of maximum selectivity!
Usage: python viz_max_color.py [DATA_DIR]

(also computes the entropy of the distribution)
Entropy (lack of info) is maximized at a uniform distribution
"""

from neural_nets_pb2 import *
import numpy as np
import matplotlib.pyplot as plt
import os
import ipdb as pdb
from plots import *
import sys

# firing rate for each neuron
def get_rates(raster,N,T):
	numSpikes = np.zeros(N)
	w = int(np.sqrt(N))
	assert(w*w==N)
	for spike in raster.spikes:
		numSpikes[spike.i] += 1
	rates = numSpikes/(float(T)/1000)
	return rates

# returns max_color and entropy figures
def plot_max_color(data_dir):
	proto_files = [os.path.join(data_dir,fn) for fn in next(os.walk(data_dir))[2]]
	fig1, axes1 = plt.subplots(1,6) # selectivity
	fig2, axes2 = plt.subplots(1,6) # entropy
	it=0
	results_arr = [] # 1 for each color
	col_arr = [] # 1 for each col
	# SETUP
	for (r,proto_file) in enumerate(proto_files):
		data = open(proto_file,"rb").read() # file as string
		results = SimulationResults()
		results.ParseFromString(data)
		img = results.img
		col = (float(img.R[0])/255, float(img.G[0])/255, float(img.B[0])/255)
		results_arr.append(results)
		col_arr.append(col)
		it = results.iter
	# COMPUTE MAPS
	for li in range(6): # for each layer
		N = results_arr[0].nstate.layers[li+3].N
		layer_name = results_arr[0].nstate.layers[li+3].name
		rates_arr = [get_rates(results.spikes[li+3],N,results.T) for (ci,results) in enumerate(results_arr)] # get layer rates for each color
		selectivity = np.zeros((N,3))
		H = np.zeros(N)
		for i in range(N): # for each neuron, compute the color with maximum rate
			unit_rates = [col_rates[i] for col_rates in rates_arr]
			max_ci = np.argmax(unit_rates)
			selectivity[i,:] = col_arr[max_ci]
			# compute the entropy
			P = (np.array(unit_rates)+.1)/(np.sum(unit_rates)+.1) # normalized density function
			H[i] = -np.sum(P * np.log(P))
		w = int(np.sqrt(N))
		# SELECTIVITY
		selectivity_img = np.reshape(selectivity,(w,w,3))
		axes1[li].imshow(np.flipud(selectivity_img))
		axes1[li].set_title(layer_name)
		axes1[li].set_axis_off()
		# ENTROPY
		H_img = np.reshape(H,(w,w))
		axes2[li].imshow(np.flipud(H_img),cmap=plt.cm.bone)
		axes2[li].set_title(layer_name)
		axes2[li].set_axis_off()
	fig1.suptitle('Max Selective Color : Iter ' + str(it))
	fig2.suptitle('Entropy : Iter ' + str(it))
	return (fig1, fig2)

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print("Usage: python viz_max_color.py [COLOR_DATA_DIR]")
		sys.exit()
	data_dir = sys.argv[1]
	(fig1, fig2) = plot_max_color(data_dir)	
	fig1.savefig('max_color.png', bbox_inches='tight')
	fig2.savefig('emtropy.png',bbox_inches='tight')
	plt.show()

