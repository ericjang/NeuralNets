
"""
Python script generates selectivity data for 8 colors, given a set of pre-trained
weights.

NOTE : colors not in order? Doesn't matter, but would screw things up for orientation.
"""

ROOT = "/home/eric/NeuralNets/"
COLORS_DIR = ROOT + "images/colors/"


import os
import matplotlib.pyplot as plt
import ipdb as pdb

img_files = [os.path.join(COLORS_DIR,fn) for fn in next(os.walk(COLORS_DIR))[2]]
for i,img_file in enumerate(img_files):
	print(i)
	plt.subplot(1,8,i+1)
	img = plt.imread(img_file)
	print(img[0,0,0], img[0,0,1], img[0,0,2])
	plt.imshow(img)
plt.show()


# ALTERNATE PROGRAM:

# DATA_DIR = ROOT + "data/colors/"
# from neural_nets_pb2 import *
# import numpy as np

# proto_files = [os.path.join(DATA_DIR,fn) for fn in next(os.walk(DATA_DIR))[2]]
# # plot map for each layer
# nColors = len(proto_files)
# print(nColors)
# for (r,proto_file) in enumerate(proto_files):
# 	data = open(proto_file,"rb").read() # file as string
# 	results = SimulationResults()
# 	results.ParseFromString(data)
# 	img = results.img
# 	print(img.R[0],img.G[0],img.B[0])
# 	w = int(np.sqrt(len(img.R)))
# 	print(w)
# 	A = np.zeros((w,w,3), 'uint8')
# 	A[:,:,0] = np.reshape(img.R,(w,w)).astype('uint8')
# 	A[:,:,1] = np.reshape(img.G,(w,w)).astype('uint8')
# 	A[:,:,2] = np.reshape(img.B,(w,w)).astype('uint8')
# 	print(A.shape)
# 	plt.subplot(1,8,r)
# 	plt.imshow(A)
# plt.show()

