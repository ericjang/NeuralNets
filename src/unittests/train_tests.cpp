#include "train_tests.h"
#include "neural_nets.h"
#include "apma2821v/train.h"
#include <iostream>

// single iter with a sample loaded image
void train_tests::test_iter() {
    std::cout << "Single Trial Test" << std::endl;
    SpikingNet *net = buildnet();
    QString img_name = QString("%1/images/AT_01_2B_5493_35_l.jpg").arg(ROOT);
    neuralnets::SimulationResults results = train_iter(net,img_name);
    saveResults(results, QString("%1/data/test.protodat").arg(ROOT).toStdString());
    delete net;
}
