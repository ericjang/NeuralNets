using PyPlot

function plot_raster(raster::Raster,N,nsteps)
	C = [spike.t for spike in raster.spikes]
	R = [spike.i for spike in raster.spikes]
	vlines(C,R-0.5,R+0.5) # vlines(x,ymin,ymax)
	xlim([0, nsteps])
	ylim([0, N])
end

# generic network plotting, from first layer to last
# function plot_all_rasters(results::RunResults)
# 	T = results.T
# 	dt = results.dt
# 	nsteps = length(0:dt:T)
# 	nL = length(results.names)
# 	i=0
# 	for (i,layer_name) in results.names
# 		subplot(1,nL,i)
# 		N = results.N[i]
# 		spikes = results.spikes[i]
# 		plot_raster(spikes,N,nsteps)
# 		title(layer_name)
# 		i+=1
# 	end
# end

# for plotting the vision architecture
function plot_vision_rasters(results::SimulationResults)
	T = results.T
	dt = results.dt
	nsteps = ceil(T/dt)	
	Dim = (3,13) 
	nL = length(results.nstate.layers)

	for i=1:nL
		layer = results.nstate.layers[i]
		raster = results.spikes[i]
		println(layer.name)
		if i in 1:3
			subplot2grid(Dim,(i-1,0),colspan=2)
		elseif i in 4:6
			subplot2grid(Dim,(i%4,2),colspan=2)
		elseif i in 7:9
			subplot2grid(Dim,(0,(i-6)*3+1),rowspan=3, colspan=3) # V1_4
		else
			error("plot_vision_rasters only works with the 9-layer network")
		end
		plot_raster(raster,layer.N,nsteps)
		#xticks(0:dt:T) # in ms
		title(layer.name)
	end
end