#ifndef IMAGES_H
#define IMAGES_H

#include <QImage>
#include "sim/runsettings.h"
#include "proto/neural_nets.pb.h"
// stuff related to image manipulation
struct SpikingNet;
struct RunSettings;

QImage getRandomPatch(QImage img, int w);

stim_vec getStimsFromImage(QImage img, SpikingNet *net, float tstart, float tstop);
stim_vec getConstantStim(float hz, SpikingNet *net, float tstart, float tstop);

void copy(neuralnets::RGBImage *dest, QImage src);

#endif // IMAGES_H
