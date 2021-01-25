#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

std::vector<std::string> explode(const std::string &delimiter, const std::string &str);

template<typename T> bool contains(std::vector<T> v, T x);

template<typename T> bool all_in_vec(std::vector<T> v1, std::vector<T> v2);

#endif // FUNCTIONS_H

