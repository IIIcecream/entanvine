// Copyright 2022. All rights reserved.
//
// Author: icecream

#include <functional>
#include <stddef.h>
#include <string>

#include "gtest/gtest.h"

#define private public
#include "vptr/vptr.h"

using namespace vptr;

typedef std::string(*pFunc)();
using Func = std::function<std::string ()>;

TEST(Virtual_pointer_test, no_virtual_base) {
  NoVirtualBase no_virtual_base;
  ASSERT_EQ(sizeof(no_virtual_base), 4);

  int* begin_addr = (int*)(&no_virtual_base);
  int* first_member_addr = &(no_virtual_base.base_);
  ASSERT_EQ(begin_addr, first_member_addr);
}

TEST(Virtual_pointer_test, no_virtual_derive) {
  NoVirtualDerive no_virtual_derive;
  ASSERT_EQ(sizeof(no_virtual_derive), 8);

  int* begin_addr = (int*)(&no_virtual_derive);
  int* first_member_addr_self = &(no_virtual_derive.derive_);
  int* first_member_addr_base = &(no_virtual_derive.base_);

  ASSERT_EQ(begin_addr, first_member_addr_base);
  ASSERT_EQ(begin_addr + 1, first_member_addr_self);
}

// vitrual pointer table pointer(起始地址为虚标指针）
// 紧接着是成员变量
TEST(Virtual_pointer_test, virtual_base) {
  Base base;
  ASSERT_EQ(sizeof(base), 16);

  uint64_t* begin_addr = (uint64_t*)(&base);
  uint64_t* first_member_addr = (uint64_t*)(&(base.base_));
  ASSERT_EQ(begin_addr + 1, first_member_addr);

  // 将begin_addr转成int64的指针，解引用获取虚指针表的地址
  // 再转成uint64_t*
  uint64_t* virtual_pointer_table_address = (uint64_t*)(*begin_addr);

  // *(virtual_pointer_table_address)是析构函数
  // *(virtual_pointer_table_address + 1)也是析构函数?

  pFunc func_f = (pFunc)*(virtual_pointer_table_address + 2);
  ASSERT_EQ("Base::f()", func_f());

  pFunc func_g = (pFunc)*(virtual_pointer_table_address + 3);
  ASSERT_EQ("Base::g()", func_g());

  pFunc func_h = (pFunc)*(virtual_pointer_table_address + 4);
  ASSERT_EQ("Base::h()", func_h());
}

TEST(Virtual_pointer_test, common_virtual_pointer_table) {
  Base base1, base2;
  // 虚表指针相等
  ASSERT_EQ(*(uint64_t*)&base1, *(uint64_t*)&base2);
  // 虚表中虚函数的地址也相等
  ASSERT_EQ(*(uint64_t*)*(uint64_t*)&base1,
            *(uint64_t*)*(uint64_t*)&base2);
}

TEST(Virtual_pointer_test, derive1) {
  Derive1 derive;
  ASSERT_EQ(sizeof(derive), 16);

  uint64_t* begin_addr = (uint64_t*)(&derive);
  uint64_t* first_member_addr = (uint64_t*)(&(derive.base_));
  ASSERT_EQ(begin_addr + 1, first_member_addr);

  uint64_t* begin_derive_address = (uint64_t*)(&(derive.derive_));
  // 此处的+1,，是指base的成员：base_，因为他是int，所以是四字节
  ASSERT_EQ((int*)first_member_addr + 1, (int*)begin_derive_address);

  // 将begin_addr转成int64的指针，解引用获取虚指针表的地址
  // 再转成uint64_t*
  uint64_t* virtual_pointer_table_address = (uint64_t*)(*begin_addr);

  // *(virtual_pointer_table_address)是析构函数
  // *(virtual_pointer_table_address + 1)也是析构函数?

  pFunc func_f = (pFunc)*(virtual_pointer_table_address + 2);
  ASSERT_EQ("Base::f()", func_f());

  pFunc func_g = (pFunc)*(virtual_pointer_table_address + 3);
  ASSERT_EQ("Base::g()", func_g());

  pFunc func_h = (pFunc)*(virtual_pointer_table_address + 4);
  ASSERT_EQ("Base::h()", func_h());
}

TEST(Virtual_pointer_test, derive2) {
  Derive2 derive;
  ASSERT_EQ(sizeof(derive), 16);

  uint64_t* begin_addr = (uint64_t*)(&derive);
  uint64_t* first_member_addr = (uint64_t*)(&(derive.base_));
  ASSERT_EQ(begin_addr + 1, first_member_addr);

  uint64_t* begin_derive_address = (uint64_t*)(&(derive.derive_));
  // 此处的+1,，是指base的成员：base_，因为他是int，所以是四字节
  ASSERT_EQ((int*)first_member_addr + 1, (int*)begin_derive_address);

  // 将begin_addr转成int64的指针，解引用获取虚指针表的地址
  // 再转成uint64_t*
  uint64_t* virtual_pointer_table_address = (uint64_t*)(*begin_addr);

  // *(virtual_pointer_table_address)是析构函数
  // *(virtual_pointer_table_address + 1)也是析构函数?

  pFunc func_f = (pFunc)*(virtual_pointer_table_address + 2);
  ASSERT_EQ("Derive2::f()", func_f());

  pFunc func_g = (pFunc)*(virtual_pointer_table_address + 3);
  ASSERT_EQ("Base::g()", func_g());

  pFunc func_h = (pFunc)*(virtual_pointer_table_address + 4);
  ASSERT_EQ("Base::h()", func_h());
}
