#!/usr/bin/python

"""
Demonstration of what saccades look like:
"""

import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import numpy as np
import ipdb as pdb


img = plt.imread("../../images/train/western_playroom_71_.jpg")
plt.figure()
plt.imshow(img)
w = 30
# compute max allowable X displacement
(x,y) = (0,0) # start at left hand corner
nsteps = 100
height = img.shape[0]
width = img.shape[1]
xy = np.zeros((2,nsteps))
stepsize=int(width/10)
ax = plt.gca()
for i in range(nsteps):
	xy[:,i] = [x,y]
	print([x,y])
	rect = Rectangle((x,y),w,w,edgecolor="c",fill=False)
	ax.add_patch(rect)
	x_stepL = np.min((x,stepsize))
	x_stepR = np.min((width-(x+w),stepsize))
	y_stepD = np.min((height-(y+w),stepsize))
	y_stepU = np.min((y,stepsize))
	x = np.random.randint(x-x_stepL,x+x_stepR+1)
	y = np.random.randint(y-y_stepU,y+y_stepD+1)

#pdb.set_trace()
plt.plot(xy[0,:],xy[1,:],linewidth=2.0,color='c')


plt.figure()
plt.imshow(img)
(x,y) = (0,0) # start at left hand corner
nsteps = 100
height = img.shape[0]
width = img.shape[1]
xy = np.zeros((2,nsteps))
stepsize=int(width/10)
ax = plt.gca()
for i in range(nsteps):
	rect = Rectangle((x,y),w,w,edgecolor="c",fill=False)
	ax.add_patch(rect)
	xy[:,i] = [x,y]
	x = np.random.randint(0,width-w)
	y = np.random.randint(0,height-w)

plt.show()
