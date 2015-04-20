using Gadfly
using ProtoBuf
using neuralnets

# plotting stuff in gadfly

println("Reading Serialized Data...")
fname = "../data/test.nsim"
stream = open(fname,"r")
sim_results = readproto(stream,SimulationResults())
close(stream)

function plot_raster(raster::Raster,N,T)
    raster = sim_results.spikes[1];
    times = [spike.t for spike in raster.spikes]
    neurons = [spike.i for spike in raster.spikes];
    p = plot(x=times, y=neurons, Guide.XLabel("Time"), Guide.YLabel("Neuron"), Geom.point)
    p
end

p = plot_raster(raster)

draw(PNG("myplot.png", 6inch, 3inch), some_plot)