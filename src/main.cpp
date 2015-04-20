#include <iostream>
#include "neural_nets.h"
#include "unittests/tests.h"
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc > 1) {
        // resume
        std::string saved_file(argv[1]);
        resume_training(saved_file);
    } else {
        //test_iter();
        full_train_default();
    }
    cout << "Done!" << endl;
}
