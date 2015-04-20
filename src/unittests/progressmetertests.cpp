#include "progressmetertests.h"
#include <iostream>

extern "C" {
#include "progressbar.h"
}

using namespace std;

void ProgressMeterTests::test() {
    progressbar *progress = progressbar_new("Testing...",50);
    for (int i=0; i<50; ++i) {
        progressbar_inc(progress);
    }
    progressbar_finish(progress);
    cout << "finished progress meter test" << endl;
}
