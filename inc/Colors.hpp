#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <stdexcept>

template<typename T>
T mixColors(const size_t &n, T args...)
{
  va_list list;
  va_start(list, args);

  // Assure that the number of arguments is divisible by two.
  if (n % 2 != 0) {
    throw std::runtime_error("Number of arguments must be an multiple of two.");
  }

  // Initializes both sums.
  T valueSum = static_cast<T>(0.0);
  T percentageSum = static_cast<T>(0.0);

  // Sums all the percentages and values.
  for (size_t i = 0; i < n; ++i) {
    const T percentage = va_arg(list, T);
    const T value = va_arg(list, T);

    percentageSum += percentage;
    valueSum += value;
  }

  va_end(list);

  // Returns the result.
  return valueSum / percentageSum;
}
