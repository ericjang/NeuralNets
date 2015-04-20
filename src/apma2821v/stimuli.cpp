#include "stimuli.h"
#include "constants.h"
#include "common.h"
#include "stimuli/fixedratespikestim.h"
#include "stimuli/emptystim.h"
#include "networks/spikingnet.h"

#include <QColor>

stim_vec getConstantStim(float hz, SpikingNet *net, float tstart, float tstop) {
    int N = 100;

    int nstart = int(tstart/dt);
    int nstop = int(tstop/dt);

    stim_vec vec; // first 3 have data, the rest are null
    vec.resize(9); // 9 layers

    fvec rates(N,hz);
    float noise = .1; // unused

    vec[0] = new FixedRateSpikeStim(rates, noise, nstart, nstop);
    vec[1] = new FixedRateSpikeStim(rates, noise, nstart, nstop);
    vec[2] = new FixedRateSpikeStim(rates, noise, nstart, nstop);

    for (int i=3;i<9;++i) {
        vec[i] = new EmptyStim(net->layers[i]->units.size());
    }

    return vec;
}



// needs to take into account tstart, tstop, T
stim_vec getStimsFromImage(QImage img, SpikingNet *net, float tstart, float tstop) {

    // tmp - print out colors
    int i=0;

    int N = img.height() * img.width();
    assert(N == 100); // later on, this assumption will no longer be true

    fvec L_hz(N,0),
         M_hz(N,0),
         S_hz(N,0);
    float hz = 40; // maximum stim rate
    for (int r=0; r<img.height(); ++r) {
        for (int c=0; c<img.width(); ++c,++i) {
            QColor col(img.pixel(r,c));
//            cout << col.red() << " " << col.green()  << " " << col.blue() << endl;
            float R = float(col.red())/255;
            float G = float(col.green())/255;
            float B = float(col.blue())/255; // normalize 0-1

            L_hz[i] = hz*(R+G*.7+B*.25)/(1+.7+.25) + 0.01;
            M_hz[i] = hz*(G+R*.7+B*.25)/(1+.7+.25) + 0.01;
            S_hz[i] = hz*B+0.01;
        }
    }

    int nstart = int(tstart/dt);
    int nstop = int(tstop/dt);
    stim_vec vec; // first 3 have data, the rest are null
    vec.resize(9); // 9 layers
    float noise = .1; // 10% noise rate

    vec[0] = new FixedRateSpikeStim(L_hz, noise, nstart, nstop);
    vec[1] = new FixedRateSpikeStim(M_hz, noise, nstart, nstop);
    vec[2] = new FixedRateSpikeStim(S_hz, noise, nstart, nstop);

    for (int i=3;i<9;++i) {
        vec[i] = new EmptyStim(net->layers[i]->units.size());
    }

    return vec;
}

// returns a random square patch
QImage getRandomPatch(QImage img, int w) {
    int x = uniformInt(0,img.width()-w-1);
    int y = uniformInt(0,img.height()-w-1);
    QImage copy = img.copy(x,y,w,w);
    copy = copy.convertToFormat(QImage::Format_RGB888);
    return copy;
}

void copy(neuralnets::RGBImage *dest, QImage src) {
    // copies data over in column-major format.
    for (int r=0; r<src.height(); ++r) {
        for (int c=0; c<src.width(); ++c) {
            QColor col(src.pixel(r,c));
            dest->add_r(col.red());
            dest->add_g(col.green());
            dest->add_b(col.blue());
        }
    }
}
