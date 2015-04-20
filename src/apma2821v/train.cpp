#include "train.h"
#include "networks/spikingnet.h"

#include "buildnet.h"
#include <algorithm>

#include <cassert>
#include "constants.h"
#include "utilities/indexing.h"
#include "common.h"
#include "proto/neural_nets.pb.h"
#include <QDirIterator>
#include <QDebug>
#include <QImage>
#include "stimuli.h"
#include <fstream>

extern "C" {
#include "progressbar.h"
}

#include <iostream>

using namespace std;


// runs on existing net
neuralnets::SimulationResults train_iter(SpikingNet *net, QString img_name) {
    QImage img;
    assert(img.load(img_name));
    int w = 10; // 10 x 10 image patches
    QImage cropped = getRandomPatch(img,w);
    float T = 300; // msec
    stim_vec stims = getStimsFromImage(cropped, net, 0, T);
    RunSettings* rs = new RunSettings(T,dt,stims,true); // progress meter disabled
    neuralnets::SimulationResults results = run(net, rs);

    // store the image in column-major format
    copy(results.mutable_img(), cropped);

    delete rs;
    return results;
}

bool resume_training(string fname) {
    fstream input(fname, ios::in | ios::binary);
    neuralnets::SimulationResults results;
    if (!results.ParseFromIstream(&input)) {
        cerr << "Failed to Parse " << fname << endl;
        return false;
    }
    SpikingNet *net = reconstruct_net(results.nstate());
    int iter = results.iter();
    cout << "Resuming training from iteration " << iter << endl;
    full_train_default(net, iter+1);
    return true;
}

void full_train_default() {
    SpikingNet *net = buildnet();
    full_train_default(net, 0);
}

// runs the full training sequence
void full_train_default(SpikingNet *net, int startIter) {

    // set up progress meter across iterations
    char msg [] = "Training Full Dataset...";
    progressbar *progress = progressbar_new(msg, TRAIN_ITER);

    // update progress
    for (int i=0; i<startIter; ++i) {
        progressbar_inc(progress);
    }

    QDirIterator it(QString("%1/images/").arg(ROOT),
                    QStringList() << "*.jpg",
                    QDir::Files, QDirIterator::Subdirectories);
    vector<QString> fnames;
    while (it.hasNext()) {
        QString str = it.next();
        fnames.push_back(str);
    }

    for (int itr=startIter; itr<=TRAIN_ITER; ++itr) {
        // pick random image.
        reset(net);
        neuralnets::SimulationResults results = train_iter(net, fnames[itr]);
        normalizeWeights(net);
        if (itr % SAVE_INTERVAL == 0) {
            QString num2str = QString("%1").arg(itr, 5, 10, QChar('0')); // 5 = padded zeros, 10 = radix (base 10)
            string saveName = QString("%1/data/default_%2.protodat").arg(ROOT,num2str).toStdString();
            saveResults(results, saveName);
        }
        progressbar_inc(progress);
    }
    progressbar_finish(progress);
    delete net;
}
