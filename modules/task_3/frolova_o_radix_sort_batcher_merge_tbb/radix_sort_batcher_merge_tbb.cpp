// Copyright 2021 Frolova Olga
#include "../../../modules/task_3/frolova_o_radix_sort_batcher_merge_tbb/radix_sort_batcher_merge_tbb.h"

#include <math.h>
#include <tbb/tbb.h>
#include <tbb/tbb_stddef.h>
#include <algorithm>
#include <climits>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int const MAX = INT_MAX;
std::vector<std::pair<int, int>> comps;

std::vector<double> getRandomVector(int size) {
  std::vector<double> vec(size);
  std::mt19937 gen;
  gen.seed(static_cast<unsigned char>(time(0)));
  for (int i = 0; i < size; i++) {
    vec[i] = gen() / 10000;
  }
  return vec;
}

int countRadix(double num) {
  int res = 0;
  while (static_cast<int>(num) > 0) {
    num = num / 10;
    res++;
  }
  return res;
}

std::vector<double> checkVector(std::vector<double> vec) {
  int j = 0;
  double tmp = 0;
  int lengh = static_cast<int>(vec.size());
  for (int i = 0; i < lengh; i++) {
    j = i;
    for (int k = i; k < lengh; k++) {
      if (vec[j] > vec[k]) {
        j = k;
      }
    }
    tmp = vec[i];
    vec[i] = vec[j];
    vec[j] = tmp;
  }
  return vec;
}

double maxVal(std::vector<double> vec) {
  int size = static_cast<int>(vec.size());
  double res = vec[0];
  for (int i = 1; i < size; i++)
    if (res < vec[i]) res = vec[i];
  return res;
}

int countNegRadix(double num) {
  std::string str_num = std::to_string(num);
  if (str_num.find('.')) {
    while (str_num[str_num.size() - 1] == '0')
      str_num.erase(str_num.size() - 1);
    return static_cast<int>(str_num.size()) -
           static_cast<int>(str_num.find('.')) - 1;
  } else {
    return 0;
  }
}

int getRemainder(double num) {
  int rad = countNegRadix(num);
  if (rad == 0) return 0;
  return static_cast<int>(
      round((((num - static_cast<int>(num)) * pow(10, rad)))));
}

std::vector<double> radixSortNeg(std::vector<double> vec, int radix) {
  std::vector<int> counts(10, 0);
  std::vector<int> counts1(10, 0);
  std::vector<double> res(vec.size());
  int size = static_cast<int>(vec.size());
  for (int i = 0; i < size; i++) {
    int r = getRemainder(vec[i]);
    counts[static_cast<int>(
        (((r) % static_cast<unsigned int>(pow(10, (radix)))) /
         pow(10, (radix - 1))))]++;
  }
  for (int i = 1; i < 10; i++)
    for (int j = i - 1; j >= 0; j--) counts1[i] += counts[j];
  for (int i = 0; i < size; i++) {
    int r = getRemainder(vec[i]);
    int index =
        static_cast<int>((((r) % static_cast<unsigned int>(pow(10, (radix)))) /
                          pow(10, (radix - 1))));
    res[counts1[index]] = vec[i];
    counts1[index]++;
  }
  return res;
}

std::vector<double> radixSortPos(std::vector<double> vec, int radix) {
  std::vector<int> counts(10, 0);
  std::vector<int> counts1(10, 0);
  std::vector<double> res(vec.size());
  int size = static_cast<int>(vec.size());
  for (int i = 0; i < size; i++) {
    int step = (static_cast<int>(vec[i]) %
                static_cast<unsigned int>(pow(10, (radix))));
    counts[static_cast<int>((step / pow(10, (radix - 1))))]++;
  }
  for (int i = 1; i < 10; i++)
    for (int j = i - 1; j >= 0; j--) counts1[i] += counts[j];
  for (int i = 0; i < size; i++) {
    int step = (static_cast<int>(vec[i]) %
                static_cast<unsigned int>(pow(10, (radix))));
    int index = static_cast<int>((step / pow(10, (radix - 1))));
    res[counts1[index]] = vec[i];
    counts1[index]++;
  }
  return res;
}

std::vector<double> radixSort(std::vector<double> vec) {
  int size = static_cast<int>(vec.size());
  int maxRadNeg = 0;
  int maxRadPos = countRadix(maxVal(vec));
  std::vector<double> res;
  for (int i = 0; i < size; i++)
    if (countNegRadix(vec[i]) > maxRadNeg) maxRadNeg = countNegRadix(vec[i]);
  if (maxRadNeg != 0) {
    res = radixSortNeg(vec, 1);
    if (maxRadNeg > 1) {
      for (int j = 2; j <= maxRadNeg; j++) res = radixSortNeg(res, j);
    }
    for (int radix = 1; radix <= maxRadPos; radix++)
      res = radixSortPos(res, radix);
  } else {
    res = radixSortPos(vec, 1);
    for (int radix = 2; radix <= maxRadPos; radix++)
      res = radixSortPos(res, radix);
  }
  return res;
}

void makeNetwork(int size) {
  std::vector<int> vec;
  for (int i = 0; i < size; i++) {
    vec.push_back(i);
  }
  net(vec);
}

void net(std::vector<int> vec) {
  int size = static_cast<int>(vec.size());
  if (size <= 1) {
    return;
  }
  std::vector<int> left(vec.begin(), vec.begin() + size / 2);
  std::vector<int> right(vec.begin() + size / 2, vec.end());
  net(left);
  net(right);
  oddEvenMerge(left, right);
}

void oddEvenMerge(std::vector<int> left, std::vector<int> right) {
  int size = static_cast<int>(left.size()) + static_cast<int>(right.size());
  if (size <= 1) {
    return;
  }
  if (size == 2) {
    comps.push_back(std::pair<int, int>(left[0], right[0]));
    return;
  }
  std::vector<int> left_odd;
  std::vector<int> right_odd;
  std::vector<int> left_even;
  std::vector<int> right_even;
  int size_left = static_cast<int>(left.size());
  for (int i = 0; i < size_left; i++) {
    if (i % 2 != 0) {
      left_even.push_back(left[i]);
    } else {
      left_odd.push_back(left[i]);
    }
  }
  int size_right = static_cast<int>(right.size());
  for (int i = 0; i < size_right; i++) {
    if (i % 2 != 0) {
      right_even.push_back(right[i]);
    } else {
      right_odd.push_back(right[i]);
    }
  }
  oddEvenMerge(left_odd, right_odd);
  oddEvenMerge(left_even, right_even);
  std::vector<int> res;
  for (int i = 0; i < size_left; i++) {
    res.push_back(left[i]);
  }
  for (int i = 0; i < size_right; i++) {
    res.push_back(right[i]);
  }
  for (int i = 1; i + 1 < size; i += 2) {
    comps.push_back(std::pair<int, int>(res[i], res[i + 1]));
  }
}

std::vector<double> radix_sort_batcher_tbb(std::vector<double> vec,
                                           int num_threads) {
  if (num_threads == 1) {
    return radixSort(vec);
  }
  makeNetwork(num_threads);
  int addition = 0;
  while (vec.size() % num_threads != 0) {
    vec.push_back(MAX);
    addition++;
  }
  int localSize = static_cast<int>(vec.size()) / num_threads;
  int currentPoint, pairPoint;
  std::vector<std::vector<double>> local_vecs(0);
  std::vector<double> localVec;
  for (int tId = 0; tId < num_threads; tId++) {
    localVec.insert(localVec.end(), vec.begin() + localSize * tId,
                    vec.begin() + localSize * (tId + 1));
    local_vecs.push_back(localVec);
    localVec.clear();
  }
  tbb::task_scheduler_init init(num_threads);
  tbb::parallel_for(
      tbb::blocked_range<size_t>(0, local_vecs.size(), 1),
      [&local_vecs](const tbb::blocked_range<size_t>& range) {
        for (size_t i = range.begin(); i != range.end(); ++i) {
          local_vecs[i] = radixSort(local_vecs[i]);
        }
      },
      tbb::simple_partitioner());
  int p = 0;
  int countLocals = static_cast<int>(local_vecs.size());
  for (int i = 0; i < countLocals; i++) {
    for (int j = 0; j < localSize; j++) vec[p++] = local_vecs[i][j];
  }
  int countPair = static_cast<int>(comps.size());
  std::vector<double> localVec1(localSize * 2);
  for (int i = 0; i < countPair; i++) {
    int step = 0;
    currentPoint = localSize * comps[i].first;
    pairPoint = localSize * comps[i].second;
    while (step != localSize) {
      if (vec[currentPoint] <= vec[pairPoint]) {
        localVec1[step] = (vec[currentPoint]);
        currentPoint++;
      } else {
        localVec1[step] = (vec[pairPoint]);
        pairPoint++;
      }
      step++;
    }
    currentPoint = localSize * comps[i].second + localSize - 1;
    pairPoint = localSize * comps[i].first + localSize - 1;
    step = 0;
    while (step < localSize) {
      if (vec[currentPoint] >= vec[pairPoint]) {
        localVec1[localVec1.size() - step - 1] = (vec[currentPoint]);
        currentPoint--;
      } else {
        localVec1[localVec1.size() - step - 1] = vec[pairPoint];
        pairPoint--;
      }
      step++;
    }
    int j = 0;
    for (int l = localSize * comps[i].first;
         l < localSize * comps[i].first + localSize; l++) {
      vec[l] = localVec1[j++];
    }
    for (int l = localSize * comps[i].second;
         l < localSize * comps[i].second + localSize; l++) {
      vec[l] = localVec1[j++];
    }
  }
  vec.assign(vec.begin(), vec.end() - addition);
  comps.clear();
  return vec;
}