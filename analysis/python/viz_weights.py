#!/usr/bin/python

"""
visualizes the weight histogram - hopefully it changes as a consequence 
of STDP
"""

#!/usr/bin/python

"""
Python script for visualizing weights (because PyPlot is too slow in Julia)
"""

import matplotlib.pyplot as plt
from neural_nets_pb2 import *
import numpy as np
import math
import sys

import ipdb as pdb

# def plot_weight_hist(w_e, w_i):
# 	C = np.array([spike.t for spike in raster.spikes])
# 	R = np.array([spike.i for spike in raster.spikes])
# 	plt.vlines(C,R-0.5,R+0.5)
# 	plt.xlim([0,nsteps])
# 	xticks = np.linspace(0,T,10) # 0->T in 10 steps 
# 	plt.xticks(xticks)
# 	plt.ylim([0,N])
# 	plt.title(title)
# 	return plt

# for each conn with stdp_enabled, plot the weight histogram
def plot_weight_hists(results,stdp_only):
	EXC = 1
	INH = 2
	layers = results.nstate.layers
	
	nplots = 0
	for conn in results.nstate.connections:
		if stdp_only and not(conn.stdp_enabled):
			continue # skip this one
		nplots += 1
	i=0
	fig, axes = plt.subplots(nplots, 1)
	for conn in results.nstate.connections:
		if stdp_only and not(conn.stdp_enabled):
			continue
		w_e = []
		w_i = []
		for syn in conn.synapses:
			if syn.stype == EXC:
				w_e.append(syn.w)
			else:
				w_i.append(syn.w)
		plt.sca(axes[i])
		if len(w_e) > 0:
			plt.hist(w_e, bins=100, histtype='step')
		if len(w_i) > 0:
			plt.hist(w_i, bins=100, histtype='step')
		#plt.hist(w_e, bins=20, normed=True, histtype='step')
		#plt.hist(w_i, bins=20, normed=True, histtype='step')
		#plt.xlim((0,1.0))
		sname = layers[conn.source].name
		tname = layers[conn.target].name
		plt.title(sname + "->" + tname)
		i+=1
	#plt.tight_layout()
	plt.show()


if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Usage: ./viz_rasters.py <FILES...>")
		sys.exit()
	fnames = sys.argv[1:]
	for dat_name in fnames:
		data = open(dat_name,"rb").read() # file as string
		results = SimulationResults()
		results.ParseFromString(data)
		plot_weight_hists(results,True)