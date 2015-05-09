# plots weights & spike raster
tic()
using ProtoBuf
using neuralnets

println("Reading Serialized Data...")
fname = "../data/test.nsim"
stream = open(fname,"r")
sim_results = readproto(stream,SimulationResults())
close(stream)
include("plotting.jl")
plot_vision_rasters(sim_results)



println("Press [Enter] to Continue")
readline()
toc()