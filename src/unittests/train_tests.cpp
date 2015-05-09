#include "train_tests.h"
#include "neural_nets.h"
#include "apma2821v/train.h"
#include <iostream>

// single iter with a sample loaded image
void train_tests::test_iter() {
    std::cout << "Single Trial Test" << std::endl;
    int w = 30;
    SpikingNet *net = buildnet<IFNeuron>(w);
    QString img_name = QString("%1/images/train/skd_photo_playroom_69_.jpg").arg(ROOT);
    QImage img;
    assert(img.load(img_name));
    QImage cropped = getRandomPatch(img,w);
    neuralnets::SimulationResults results = run_iter(net,cropped,true);
    saveResults(results, QString("%1/data/tests/test_1.protodat").arg(ROOT));
    delete net;
}
