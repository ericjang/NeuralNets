TEMPLATE = app
TARGET = nets
CONFIG += console
CONFIG -= app_bundle
#CONFIG -= qt
QT += core
CONFIG += c++11

# OpenMP support
QMAKE_CXXFLAGS += -fopenmp

# profiling support
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

# THIRD PARTY
INCLUDEPATH += \
    /usr/local/include/google/protobuf \
    $$PWD/third_party/progressbar/include

LIBS += \
    -L/usr/local/lib -lprotobuf \
    -lncurses # required for progress_bar

# CUSTOM PROTOBUFS

HEADERS += \
    apma2821v/buildnet.h \
    apma2821v/connectivities.h \
    apma2821v/train.h \
    common.h \
    constants.h \
    layers/layer.h \
    layers/spikinglayer.h \
    networks/simplenet.h \
    networks/spikingnet.h \
    neurons/hodgkin_huxley.h \
    neurons/spiking_neuron.h \
    proto/neural_nets.pb.h \
    sim/runsettings.h \
    stimuli/emptystim.h \
    stimuli/fixedratespikestim.h \
    stimuli/stim.h \
    synapses/spikingconnection.h \
    synapses/spikingsynapse.h \
    synapses/stdp.h \
    unittests/progressmetertests.h \
    unittests/stim_tests.h \
    unittests/tests.h \
    utilities/indexing.h \
    unittests/conn_tests.h \
    unittests/singleneurontests.h \
    unittests/train_tests.h \
    neural_nets.h \
    apma2821v/stimuli.h

SOURCES += \
    apma2821v/buildnet.cpp \
    apma2821v/connectivities.cpp \
    apma2821v/train.cpp \
    common.cpp \
    layers/layer.cpp \
    layers/spikinglayer.cpp \
    main.cpp \
    networks/simplenet.cpp \
    networks/spikingnet.cpp \
    neurons/hodgkin_huxley.cpp \
    neurons/spiking_neuron.cpp \
    proto/neural_nets.pb.cc \
    sim/runsettings.cpp \
    stimuli/emptystim.cpp \
    stimuli/fixedratespikestim.cpp \
    synapses/spikingconnection.cpp \
    synapses/spikingsynapse.cpp \
    synapses/stdp.cpp \
    unittests/progressmetertests.cpp \
    unittests/stim_tests.cpp \
    utilities/indexing.cpp \
    unittests/conn_tests.cpp \
    unittests/singleneurontests.cpp \
    unittests/train_tests.cpp \
    apma2821v/stimuli.cpp

# C sources
SOURCES += third_party/progressbar/lib/progressbar.c \

# OTHER FILES
OTHER_FILES += \
    ../dev_log.txt \
    ../README.md



