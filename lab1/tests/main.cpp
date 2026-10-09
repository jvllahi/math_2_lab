#include "TestSuites.hpp"

#include <iostream>

int main() {
    int failed = 0;

    runLinearAlgebraTests(failed);
    runSolverTests(failed);

    if (failed == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    }

    std::cerr << failed << " test assertion(s) failed.\n";
    return 1;
}
