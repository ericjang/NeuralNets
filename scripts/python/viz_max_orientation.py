#!/usr/bin/python
"""
Just like max color plots, except for orientation
Usage: python viz_max_orientation.py [DATA_DIR]
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


if len(sys.argv) != 2:
	print("Usage: python viz_max_orientation.py [ORIENTATION_DATA_DIR]")
	sys.exit()
data_dir = sys.argv[1]


fig1, axes1 = plt.subplots(1,6)
fig2, axes2 = plt.subplots(1,6)
it=0
results_arr = [] # 1 for each color
col_arr = []

# SETUP
for i in range(45):
	d = 4*i # degrees
	proto_file = data_dir + str(d) + '.protodat'
	data = open(proto_file,"rb").read() # file as string
	results = SimulationResults()
	results.ParseFromString(data)
	img = results.img
	col = float(d) / 180
	results_arr.append(results)
	col_arr.append(col)
	it = results.iter

# COMPUTE MAPS
for li in range(6): # for each layer
	N = results_arr[0].nstate.layers[li+3].N
	layer_name = results_arr[0].nstate.layers[li+3].name
	rates_arr = [get_rates(results.spikes[li+3],N,results.T) for results in results_arr] # get layer rates for each color
	selectivity = np.zeros(N)
	H = np.zeros(N) # entropy
	for i in range(N): # for each neuron, compute the color with maximum rate
		unit_rates = [col_rates[i] for col_rates in rates_arr]
		max_ci = np.argmax(unit_rates)
		selectivity[i] = col_arr[max_ci]
		P = (np.array(unit_rates)+.1)/(np.sum(unit_rates)+.1) # normalized density function
		H[i] = -np.sum(P * np.log(P))
	w = int(np.sqrt(N))
	# max orientation
	selectivity_img = np.reshape(selectivity,(w,w))
	axes1[li].imshow(np.flipud(selectivity_img))
	axes1[li].set_title(layer_name)
	axes1[li].set_axis_off()
	# entropy
	H_img = np.reshape(H,(w,w))
	axes2[li].imshow(np.flipud(H_img),cmap=plt.cm.bone)
	axes2[li].set_title(layer_name)
	axes2[li].set_axis_off()

fig1.suptitle("Max Orientation : Iter " + str(it))	
fig2.suptitle('Entropy : Iter ' + str(it))

fig1.savefig('orientation30_max_.png', bbox_inches='tight')
fig2.savefig('orientation30_entropy_.png',bbox_inches='tight')
	

plt.show()

