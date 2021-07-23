// Copyright (c) Facebook, Inc. and its affiliates.
// This source code is licensed under the MIT license found at https://github.com/facebookresearch/habitat-sim/blob/master/LICENSE

// based on https://github.com/facebookresearch/habitat-sim/tree/master/src/esp/io

#pragma once

namespace habitat_io {

// when writing to json, we often want to clamp tiny floats to 0.0 so they can
// be serialized concisely as "0.0"
template <typename T>
T squashTinyDecimals(T x) {
  const T eps = 1e-6f;
  return (x < eps && x > -eps) ? T(0.0) : x;
}

}