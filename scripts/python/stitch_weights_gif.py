# assemble a video of weights evolving over time!

import matplotlib.pyplot as plt
from viz_weights import gen_weight_plot

import os
import sys

if len(sys.argv) != 3:
	print("Usage: ./stitch_weights_gif.py [PROTO_DIR] [OUT_IMAGE_DIR]")
	sys.exit()

proto_dir = os.path.abspath(sys.argv[1])
outdir = os.path.dirname(sys.argv[2])

proto_files = [os.path.join(proto_dir,fn) for fn in next(os.walk(proto_dir))[2]]
for proto_file in proto_files:
	itr_string = proto_file.split("_")[-1].split(".protodat")[0]
	fig = gen_weight_plot(proto_file)
	imgname = os.path.join(outdir,itr_string + '.png')
	print('saving ' + imgname)
	fig.savefig(imgname, bbox_inches='tight')
	plt.clf()
	