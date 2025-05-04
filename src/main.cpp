#include "ilc-delta.h"
#include <vector>
#include <string>

int main() {
    ilc_delta();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    ilc_delta_print_vector(vec);
}
