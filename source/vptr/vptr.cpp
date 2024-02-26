// Copyright 2022. All rights reserved.
//
// Author: icecream

#include "vptr/vptr.h"

namespace vptr {

std::string NoVirtualBase::f() const {
  return "NoVirtualBase::f()";
}

std::string NoVirtualBase::g() const {
  return "NoVirtualBase::g()";
}

std::string NoVirtualBase::h() const {
  return "NoVirtualBase::h()";
}

std::string Base::f() const {
  return "Base::f()";
}

std::string Base::g() const {
  return "Base::g()";
}

std::string Base::h() const {
  return "Base::h()";
}

std::string Derive2::f() const {
  return "Derive2::f()";
}

}  // namespace vptr
