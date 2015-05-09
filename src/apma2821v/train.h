#ifndef TRAIN_H
#define TRAIN_H


// implements methods specific to training the neural net
#include <QString>
#include "sim/runsettings.h"
#include "proto/neural_nets.pb.h"
#include <QImage>
#include "buildnet.h"

struct SpikingNet;
struct RunSettings;
using std::string;

// single iter - assumes image has already been pre-cropped to specifications
neuralnets::SimulationResults run_iter(SpikingNet *net, QImage img, bool record_weights);


bool resume_training(QString fname,int w);
void train(SpikingNet *net, int startIter, QString saveprefix, int w);

void train_saccades(SpikingNet *net, int startIter, QString saveprefix, int w);


// IF or HodgkinHuxley-type
template <typename T>
bool train_new(int w) {
    SpikingNet *net = buildnet<T>(w);
    QString fprefix = QString("%1/data/default").arg(ROOT);

    if (SACCADES) {
        train_saccades(net, 0, fprefix, w);
    } else {
        train(net, 0, fprefix,w);
    }

    return true;
}

bool test_net(QString fname, QString imgname, QString outfile);


#endif // TRAIN_H
