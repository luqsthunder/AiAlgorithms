#pragma once

#include <vector>
#include <eigen3/Eigen/Eigen>

namespace aia
{
namespace normalization
{

template<typename DB>
struct identity
{
  constexpr void operator()(DB&& database) {  }

};

}

template<typename T,template <typename,
                              typename = std::allocator<T>>
                              class container = std::vector,
                              typename norm = normalization::identity>
class Knn
{
public:
  Knn(const container<T>& database, const container<T>& cls);

  void train();
};

#include "knn.cpp"

}
