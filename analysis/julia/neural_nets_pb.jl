using Compat
using ProtoBuf
import ProtoBuf.meta
import Base: hash, isequal, ==

type Trace
    val::Array{Float32,1}
    Trace() = (o=new(); fillunset(o); o)
end #type Trace
hash(v::Trace) = ProtoBuf.protohash(v)
isequal(v1::Trace, v2::Trace) = ProtoBuf.protoisequal(v1, v2)
==(v1::Trace, v2::Trace) = ProtoBuf.protoeq(v1, v2)

type Spike
    t::Int32
    i::Int32
    Spike() = (o=new(); fillunset(o); o)
end #type Spike
hash(v::Spike) = ProtoBuf.protohash(v)
isequal(v1::Spike, v2::Spike) = ProtoBuf.protoisequal(v1, v2)
==(v1::Spike, v2::Spike) = ProtoBuf.protoeq(v1, v2)

type __enum_SpikingLayer_LayerType <: ProtoEnum
    Simple::Int32
    HodgkinHuxley::Int32
    __enum_SpikingLayer_LayerType() = new(0,1)
end #type __enum_SpikingLayer_LayerType
const SpikingLayer_LayerType = __enum_SpikingLayer_LayerType()

type SpikingLayer
    layer_type::Int32
    N::Int32
    name::String
    SpikingLayer() = (o=new(); fillunset(o); o)
end #type SpikingLayer
hash(v::SpikingLayer) = ProtoBuf.protohash(v)
isequal(v1::SpikingLayer, v2::SpikingLayer) = ProtoBuf.protoisequal(v1, v2)
==(v1::SpikingLayer, v2::SpikingLayer) = ProtoBuf.protoeq(v1, v2)

type __enum_Synapse_SynapseType <: ProtoEnum
    EXC::Int32
    INH::Int32
    __enum_Synapse_SynapseType() = new(1,2)
end #type __enum_Synapse_SynapseType
const Synapse_SynapseType = __enum_Synapse_SynapseType()

type Synapse
    stype::Int32
    s::Int32
    t::Int32
    w::Float32
    d::Float32
    Synapse() = (o=new(); fillunset(o); o)
end #type Synapse
hash(v::Synapse) = ProtoBuf.protohash(v)
isequal(v1::Synapse, v2::Synapse) = ProtoBuf.protoisequal(v1, v2)
==(v1::Synapse, v2::Synapse) = ProtoBuf.protoeq(v1, v2)

type SpikingConnection
    source::Int32
    target::Int32
    synapses::Array{Synapse,1}
    stdp_enabled::Bool
    avg_w::Float32
    SpikingConnection() = (o=new(); fillunset(o); o)
end #type SpikingConnection
hash(v::SpikingConnection) = ProtoBuf.protohash(v)
isequal(v1::SpikingConnection, v2::SpikingConnection) = ProtoBuf.protoisequal(v1, v2)
==(v1::SpikingConnection, v2::SpikingConnection) = ProtoBuf.protoeq(v1, v2)

type NetworkState
    layers::Array{SpikingLayer,1}
    connections::Array{SpikingConnection,1}
    NetworkState() = (o=new(); fillunset(o); o)
end #type NetworkState
hash(v::NetworkState) = ProtoBuf.protohash(v)
isequal(v1::NetworkState, v2::NetworkState) = ProtoBuf.protoisequal(v1, v2)
==(v1::NetworkState, v2::NetworkState) = ProtoBuf.protoeq(v1, v2)

type Raster
    spikes::Array{Spike,1}
    Raster() = (o=new(); fillunset(o); o)
end #type Raster
hash(v::Raster) = ProtoBuf.protohash(v)
isequal(v1::Raster, v2::Raster) = ProtoBuf.protoisequal(v1, v2)
==(v1::Raster, v2::Raster) = ProtoBuf.protoeq(v1, v2)

type RGBImage
    R::Array{Int32,1}
    G::Array{Int32,1}
    B::Array{Int32,1}
    RGBImage() = (o=new(); fillunset(o); o)
end #type RGBImage
hash(v::RGBImage) = ProtoBuf.protohash(v)
isequal(v1::RGBImage, v2::RGBImage) = ProtoBuf.protoisequal(v1, v2)
==(v1::RGBImage, v2::RGBImage) = ProtoBuf.protoeq(v1, v2)

type SimulationResults
    dt::Float32
    T::Float32
    nstate::NetworkState
    img::RGBImage
    spikes::Array{Raster,1}
    iter::Int32
    SimulationResults() = (o=new(); fillunset(o); o)
end #type SimulationResults
hash(v::SimulationResults) = ProtoBuf.protohash(v)
isequal(v1::SimulationResults, v2::SimulationResults) = ProtoBuf.protoisequal(v1, v2)
==(v1::SimulationResults, v2::SimulationResults) = ProtoBuf.protoeq(v1, v2)

export Trace, Spike, SpikingLayer_LayerType, SpikingLayer, Synapse_SynapseType, Synapse, SpikingConnection, NetworkState, Raster, RGBImage, SimulationResults
