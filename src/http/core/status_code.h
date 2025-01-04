// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef STATUS_CODE_H
#define STATUS_CODE_H

namespace http::internals {
enum class StatusCode : unsigned {
  kOK = 200,
  kNotImplemented = 501,
};
}  // namespace http::internals

#endif  // STATUS_CODE_H
