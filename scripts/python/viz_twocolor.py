#!/bin/python

"""
Visualizes the two-color results.
Some kind of "average cell information" for the V5 layer.
hopefully demonstrating that information is decreased?or entropy increases per neuron?

feed in the simulated data from the two-color experiments
Usage: python viz_twocolor.py ../../data/test/twocolor/iter_0000/

plots a 8x8 map of firing rate histogram for each pair, then iterated over layers V4, V23, V15

"""

import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle 
from matplotlib.colors import rgb2hex
from neural_nets_pb2 import *
import numpy as np
import ipdb as pdb
import sys
import os

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
	print("Usage: ./viz_twocolor.py [PROTO_DIR]")
	sys.exit()

proto_dir = os.path.abspath(sys.argv[1])

# for each pair of colors
fig_v4, axes1 = plt.subplots(9,9) # selectivity
fig_v23, axes2 = plt.subplots(9,9) # selectivity
fig_v5, axes3 = plt.subplots(9,9) # selectivity

figs = [fig_v4, fig_v23, fig_v5]
axes = [axes1, axes2, axes3]
layer_names = ["V1_4","V1_23","V1_5"]


rgbs = [[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1],[1,1,0],[1,1,1]]
hexes = [rgb2hex(col) for col in rgbs]


for i in range(1,9): # row
	for j in range(1,9): # col
		print((i,j))
		proto_file = os.path.join(proto_dir,"%d_%d.protodat" % (i,j))
		data = open(proto_file,"rb").read() # file as string
		results = SimulationResults()
		results.ParseFromString(data)		
		T = results.T
		# plot 
		for k in range(3):
			N = results.nstate.layers[-3+k].N
			raster = results.spikes[-3+k] # one lf the last 3 layers
			rates = get_rates(raster,N,T)
			fig = figs[k]
			ax = axes[k]
			plt.figure(fig.number)
			ax[i,j].hist(rates,bins=25,color='b')
			ax[i,j].set_ylim([0,N])
			ax[i,j].set_xlim([0,50])
			ax[i,j].set_axis_off()

# set boundary color refs

for k in range(3):
	fig = figs[k]
	ax = axes[k]
	for i in range(8):
		ax[0,i+1].patch.set_facecolor(hexes[i])
		ax[0,i+1].xaxis.set_ticklabels([])
		ax[0,i+1].yaxis.set_ticklabels([])
		ax[i+1,0].patch.set_facecolor(hexes[i])
		ax[i+1,0].xaxis.set_ticklabels([])
		ax[i+1,0].yaxis.set_ticklabels([])
	# remove the corner
	ax[0,0].axis('off')

# for each figure
for k in range(3):
	fig = figs[k]
	plt.figure(fig.number)	
	plt.axis('off')
	fig.suptitle("%s : Iter %d" % (layer_names[k],results.iter), fontsize=10)
	# save the figure
	fname = "twocolor30_%s_%d.png" % (layer_names[k],results.iter)	
	fig.savefig(fname, bbox_inches='tight')

print('Done!')

plt.show()

