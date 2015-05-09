#!/usr/bin/python

"""
Are neurons more strongly activated by single uniform colors, or pairs of colors?
"""

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
	results_arr = [] # one for each pair
	it=0
	# load the files
	for i in range(8): # left color
		for j in range(8): # right color
			print((i,j))
			proto_file = os.path.join(data_dir,"%d_%d.protodat" % (i+1,j+1))
			data = open(proto_file,"rb").read() # file as string
			results = SimulationResults()
			results.ParseFromString(data)
			results_arr.append(results)
			it = results.iter

	fig1, axes1 = plt.subplots(1,6) # selectivity
	fig2, axes2 = plt.subplots(1,6) # entropy
	# COMPUTE MAPS
	for li in range(6): # for each layer from LGN -> V5 (6 total)
		N = results_arr[0].nstate.layers[li+3].N
		layer_name = results_arr[0].nstate.layers[li+3].name
		print(layer_name)
		rates_arr = [get_rates(results.spikes[li+3],N,results.T) for (ci,results) in enumerate(results_arr)] # get layer rates for each pair
		selectivity = np.zeros(N) # 1 if pair, 0 if single color
		H = np.zeros(N) # entropy 
		for i in range(N): # for each neuron, compute the color with maximum rate
			unit_rates = [rates[i] for rates in rates_arr]
			max_ci = np.argmax(unit_rates) # pair that yielded highest response
			(r,c) = np.unravel_index(max_ci,(8,8)) # equivalent to ind2sub
			selectivity[i] = 1 if r==c else 0
			# compute the entropy
			P = (np.array(unit_rates)+.1)/(np.sum(unit_rates)+.1) # normalized density function
			H[i] = -np.sum(P * np.log(P))
		w = int(np.sqrt(N))
		# SELECTIVITY
		selectivity_img = np.reshape(selectivity,(w,w))
		axes1[li].imshow(np.flipud(selectivity_img),cmap=plt.cm.bone)
		axes1[li].set_title(layer_name)
		axes1[li].set_axis_off()
		# ENTROPY
		H_img = np.reshape(H,(w,w))
		axes2[li].imshow(np.flipud(H_img),cmap=plt.cm.bone)
		axes2[li].set_title(layer_name)
		axes2[li].set_axis_off()
	return (fig1, fig2)

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print("Usage: python viz_max_color2.py [COLOR_DATA_DIR]")
		sys.exit()
	data_dir = sys.argv[1]
	(fig1, fig2) = plot_max_color(data_dir)	
	fig1.savefig('pair_or_single.png', bbox_inches='tight')
	fig2.savefig('pair_entropy.png',bbox_inches='tight')
	plt.show()

