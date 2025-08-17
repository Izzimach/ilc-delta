#include <vector>
#include <string>
#include <iostream>

#include <immer/vector.hpp>

#include "ilc-delta.hpp"
#include "tanuki.hpp"

int main() {

  std::vector<std::string> vec;
  vec.push_back("test_package");
  std::cout << "Package: " << vec[0] << "\n";
  
  const auto v0 = immer::vector<int>();
  const auto v1 = v0.push_back(13);
  std::cout << v1[0] << "\n";

  const auto v2 = v1.set(0,42);
  std::cout << v2[0] << "\n";
}
