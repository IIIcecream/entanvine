// Copyright 2022. All rights reserved.
//
// Author: icecream
//
// 虚函数表相关细节的测试

#pragma once

#include <string>

namespace vptr {

// |base_|
// sizeof(NoVirtualBase) = 4
class NoVirtualBase {
public:
  std::string f() const;
  std::string g() const;
  std::string h() const;

private:
  int base_ = 0;
};

// |base_|dereive_|
// sizeof(NoVirtualDerive) = 8
class NoVirtualDerive : public NoVirtualBase {
private:
  int derive_ = 0;
};

// |   vptr    | --------> | destrutor | 虚函数指针表，不在对象中
// |base_| pad |           | destrutor | 不知道为什么会有两个destructor
// sizeof(Base) = 16       |  base::f  |
//                         |  base::g  |
//                         |  base::h  |
class Base {
public:
  virtual ~Base() {}

  virtual std::string f() const;
  virtual std::string g() const;
  virtual std::string h() const;

private:
  int base_ = 0;
};

// 没有重写虚函数
// |     vptr     | --------> | destrutor |
// |base_ |derive_|           | destrutor | 不知道为什么会有两个destructor
// sizeof(Derive1) = 16       |  base::f  |
//                            |  base::g  |
//                            |  base::h  |
class Derive1 : public Base {
private:
  int derive_ = 0;
};

// 重写虚函数
// |     vptr     | --------> | destrutor |
// |base_ |derive_|           | destrutor | 不知道为什么会有两个destructor
// sizeof(Derive1) = 16       | Derive2::f|
//                            |  base::g  |
//                            |  base::h  |
class Derive2 : public Base {
public:
  virtual std::string f() const override;

private:
  int derive_ = 0;
};


}  // namespace vptr
