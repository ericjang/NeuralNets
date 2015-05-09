import numpy as np
from neural_nets_pb2 import *
import matplotlib.pyplot as plt

def plot_image(ax,img):
	"""
	:parameters:
		- ax : <matplotlib.axes.AxesSubplot> 
			draws onto the provided axes reference
		- img : custom protobuf-defined image
	"""
	w = int(np.sqrt(len(img.R)))
	A = np.zeros((w,w,3), 'uint8')
	print(img.R[0],img.G[0],img.B[0])
	A[:,:,0] = np.reshape(img.R,(w,w)).astype('uint8')
	A[:,:,1] = np.reshape(img.G,(w,w)).astype('uint8')
	A[:,:,2] = np.reshape(img.B,(w,w)).astype('uint8')
	ax.set_axis_off()
	ax.imshow(A)