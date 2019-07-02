//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads, pre-sm-60

// <cuda/std/atomic>

// template <class Integral>
//     Integral
//     atomic_fetch_and_explicit(volatile atomic<Integral>* obj, Integral op);
//
// template <class Integral>
//     Integral
//     atomic_fetch_and_explicit(atomic<Integral>* obj, Integral op);

#include <cuda/std/atomic>
#include <cuda/std/type_traits>
#include <cuda/std/cassert>

#include "atomic_helpers.h"

template <class T, cuda::thread_scope>
struct TestFn {
  __host__ __device__
  void operator()() const {
    {
        typedef cuda::std::atomic<T> A;
        A t;
        cuda::std::atomic_init(&t, T(1));
        assert(cuda::std::atomic_fetch_and_explicit(&t, T(2),
               cuda::std::memory_order_seq_cst) == T(1));
        assert(t == T(0));
    }
    {
        typedef cuda::std::atomic<T> A;
        volatile A t;
        cuda::std::atomic_init(&t, T(3));
        assert(cuda::std::atomic_fetch_and_explicit(&t, T(2),
               cuda::std::memory_order_seq_cst) == T(3));
        assert(t == T(2));
    }
  }
};

int main(int, char**)
{
    TestEachIntegralType<TestFn>()();

  return 0;
}