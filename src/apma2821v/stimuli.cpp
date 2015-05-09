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
    int N = img.height() * img.width();

    // hacky fix
    assert(N == 100 || N == 900);

    fvec L_hz(N,0),
         M_hz(N,0),
         S_hz(N,0);
    int i=0;
    float hz = 40; // maximum stim rate
    for (int r=0; r<img.height(); ++r) {
        for (int c=0; c<img.width(); ++c,++i) {
            QColor col(img.pixel(c,r));
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
    float noise = NOISE; // 10% noise rate

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

QImage getSaccadePatch(QImage img, int w, int stepsize, int &x, int &y) {
    // existing x,y get passed in and modified

    int x_stepL = std::min(x,stepsize);
    int x_stepR = std::min(img.width()-(x+w),stepsize);
    int y_stepD = std::min(img.height()-(y+w),stepsize);
    int y_stepU = std::min(y,stepsize);

    x = uniformInt(x-x_stepL,x+x_stepR+1);
    y = uniformInt(y-y_stepU,y+y_stepD+1);

    QImage copy = img.copy(x,y,w,w);
    copy = copy.convertToFormat(QImage::Format_RGB888);
    return copy;
}




void copy(neuralnets::RGBImage *dest, QImage src) {
    // copies data over in column-major format.
    dest->clear_r(); dest->clear_g(); dest->clear_b();
    for (int r=0; r<src.height(); ++r) {
        for (int c=0; c<src.width(); ++c) {
            QColor col(src.pixel(c,r));
            dest->add_r(col.red());
            dest->add_g(col.green());
            dest->add_b(col.blue());
        }
    }
}
