#include <iostream>
#include "neural_nets.h"
#include "unittests/tests.h"
#include <chrono>
#include <thread>
#include <QString>

using namespace std;

void printUsage() {
    cout << "USAGE:" << endl;
    cout << "Start Training from Scratch : ./nets <train10/train30>" << endl;
    cout << "Resume training : ./nets <train10|train30> <WEIGHTS.protodat>" << endl;
    cout << "Test Image (single) : ./nets test <WEIGHTS.protodat> <IMAGE.jpg> <OUT.protodat>" << endl;
 //   cout << "R\un Selectivity Tests : ./nets test <WEIGHTS.protodat> <OUT.protodat>" << endl;
}

int main(int argc, char const *argv[])
{
    if (argc > 1) {
        QString arg1(argv[1]);
        if (arg1 == "train10")
        {
            if (argc == 2) { // train new
                cout << "Training 10x10 HH Neurons From Scratch" << endl;
                train_new<HodgkinHuxley>(10);
            } else if (argc == 3) {
                QString proto_file(argv[2]);
                resume_training(proto_file,10);
            }
        }
        else if (arg1 == "train30")
        {
            if (argc == 2) { // train new
                cout << "Training 30x30 HH Neurons From Scratch" << endl;
                train_new<HodgkinHuxley>(30);
            } else if (argc == 3) {
                QString proto_file(argv[2]);
                resume_training(proto_file,30);
            }
        }
        else if (arg1 == "test") {
          if (argc == 5) {
              test_net(QString(argv[2]), QString(argv[3]), QString(argv[4]));
          } else if (argc == 2) {
              cout << "Testing Single Image" << endl;
              train_tests::test_iter();
          } else {
              printUsage();
          }
        }
    } else {
        printUsage();
    }

}
