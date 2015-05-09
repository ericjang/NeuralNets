#include "train.h"
#include "networks/spikingnet.h"


#include <algorithm>

#include <cassert>
#include "constants.h"
#include "utilities/indexing.h"
#include "common.h"
#include "proto/neural_nets.pb.h"
#include <QDirIterator>
#include <QDebug>

#include "stimuli.h"
#include <fstream>

extern "C" {
#include "progressbar.h"
}

#include <iostream>
#include "utilities/filesystem_utils.h"

using namespace std;


// runs on existing net
neuralnets::SimulationResults run_iter(SpikingNet *net, QImage img, bool record_weights) {
    stim_vec stims = getStimsFromImage(img, net, 0, SIM_DURATION);
    RunSettings* rs = new RunSettings(SIM_DURATION,dt,stims,true); // progress meter disabled
    neuralnets::SimulationResults results = run(net, rs, record_weights);
    // store the image in column-major format
    copy(results.mutable_img(), img);
    delete rs;
    return results;
}

// train from scratch


bool resume_training(QString fname,int w) {
    if (!fileExists(fname)) {
        cerr << "Could not find file " << fname.toStdString() << endl;
        return false;
    }
    fstream input(fname.toStdString(), ios::in | ios::binary);
    neuralnets::SimulationResults results;
    if (!results.ParseFromIstream(&input)) {
        cerr << "Failed to Parse " << fname.toStdString() << endl;
        return false;
    }
    SpikingNet *net = reconstruct_net(results.nstate());
    int iter = results.iter();
    cout << "Resuming training from iteration " << iter << endl;

    // TODO - this shit doesn't work right now
    QString fprefix = fname.section("_",0,-1); // truncate everything after the last _ in the file path
    // check to make sure fprefix is correct

    // TODO - TEMPORARY HACK!
    //train(net, iter+1, fprefix,w);
    fprefix = "/home/eric/NeuralNets/data/default";
    train_saccades(net, iter+1,fprefix, w);

    return true;
}

// runs the full training sequence
void train(SpikingNet *net, int startIter, QString saveprefix, int w) {
    QDirIterator it(QString("%1/images/train/").arg(ROOT),
                    QStringList() << "*.jpg",
                    QDir::Files, QDirIterator::Subdirectories);
    vector<QString> fnames;
    while (it.hasNext()) {
        QString str = it.next();
        fnames.push_back(str);
    }
    int nFiles = fnames.size();
    cout << nFiles << " images registered" << endl;
    cout << TRAIN_ITER-startIter-1 << " iterations left to go" << endl;

    // set up progress meter across iterations
    char msg [] = "Full Train...";
    progressbar *progress = progressbar_new(msg, TRAIN_ITER);

    // update progress
    for (int i=0; i<startIter; ++i) {
        progressbar_inc(progress);
    }

    for (int itr=startIter; itr<=TRAIN_ITER; ++itr) {
        // pick random image.
        reset(net); // clear activations
        bool record_weights = itr % SAVE_INTERVAL == 0;

        QImage img;
        assert(img.load(fnames[itr % nFiles]));

        QImage cropped = getRandomPatch(img,w);

        neuralnets::SimulationResults results = run_iter(net, cropped, record_weights);
        results.set_iter(itr);
        normalizeWeights(net);
        if (record_weights) {
            QString numstr = QString("%1").arg(itr, 4, 10, QChar('0')); // 4 = padded zeros, 10 = radix (base 10)
            QString saveName = QString("%1_%2.protodat").arg(saveprefix,numstr);
            saveResults(results, saveName);
        }
        progressbar_inc(progress);
    }
    progressbar_finish(progress);
    delete net;
}


void train_saccades(SpikingNet *net, int startIter, QString saveprefix, int w) {


    QDirIterator it(QString("%1/images/train/").arg(ROOT),
                    QStringList() << "*.jpg",
                    QDir::Files, QDirIterator::Subdirectories);
    vector<QString> fnames;
    while (it.hasNext()) {
        QString str = it.next();
        fnames.push_back(str);
    }
    int nFiles = fnames.size();


    // set up progress meter across iterations
    char msg [] = "Saccade Train...";
    progressbar *progress = progressbar_new(msg, TRAIN_ITER);

    // update progress


    int iters_per_image = 100;
    int nImages = TRAIN_ITER/iters_per_image;

    int itr=0;
    for (itr=0; itr<startIter; ++itr) {
        progressbar_inc(progress);
    }

    for (int i=0; i<nImages; ++i) {
        QImage img;
        assert(img.load(fnames[i % nFiles]));

        // initialize random x,y starting point
        int x = uniformInt(0,img.width()-w);
        int y = uniformInt(0,img.height()-w);
        int stepsize = img.width()/10;

        for (int j=0; j<=iters_per_image, itr<TRAIN_ITER; ++j,++itr) {
            // pick random image.
            reset(net); // clear activations
            bool record_weights = itr % SAVE_INTERVAL == 0;

            // get next saccade patch
            QImage cropped = getSaccadePatch(img,w,stepsize,x,y);

            neuralnets::SimulationResults results = run_iter(net, cropped, record_weights);
            results.set_iter(itr);
            normalizeWeights(net);
            if (record_weights) {
                QString numstr = QString("%1").arg(itr, 4, 10, QChar('0')); // 4 = padded zeros, 10 = radix (base 10)
                QString saveName = QString("%1_%2.protodat").arg(saveprefix,numstr);
                saveResults(results, saveName);
            }
            progressbar_inc(progress);
        }
    }
    progressbar_finish(progress);
    delete net;
}



bool test_net(QString fname, QString img_name, QString outfile) {
    // reconstruct the network
    fstream input(fname.toStdString(), ios::in | ios::binary);
    neuralnets::SimulationResults saved_weights_results;
    if (!saved_weights_results.ParseFromIstream(&input)) {
        cerr << "Failed to Parse " << fname.toStdString() << endl;
        return false;
    }
    SpikingNet *net = reconstruct_net(saved_weights_results.nstate());

    // load the image
    QImage img;
    assert(img.load(img_name));

    // simulate
    neuralnets::SimulationResults results = run_iter(net, img, false);
    results.set_iter(saved_weights_results.iter());
    // save the result
    saveResults(results, outfile);

    delete net;
    return true;
}

