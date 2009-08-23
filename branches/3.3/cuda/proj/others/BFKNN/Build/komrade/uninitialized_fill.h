/*
 *  Copyright 2008-2009 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


/*! \file uninitialized_fill.h
 *  \brief Defines the interface to the
 *         uninitialized_fill function.
 *  \see http://www.sgi.com/tech/stl/uninitialized_fill.html
 */

#pragma once

#include <komrade/detail/config.h>

namespace komrade
{

/*! \addtogroup filling
 *  \ingroup transformations
 *  \{
 */

/*! In \c komrade, the function \c komrade::device_new allocates memory for
 *  an object and then creates an object at that location by calling a
 *  constructor. Occasionally, however, it is useful to separate those two
 *  operations. If each iterator in the range <tt>[first, last)</tt> points
 *  to unitialized memory, then \p unitialized_fill creates copies of \c x
 *  in that range. That is, for each iterator \c i in the range <tt>[first, last)</tt>,
 *  \p uninitialized_fill creates a copy of \c x in the location pointed to \c i by
 *  calling \p ForwardIterator's \c value_type's copy constructor.
 *  
 *  \param first The first element of the range of interest.
 *  \param last The last element of the range of interest.
 *  \param x The value to use as the exemplar of the copy constructor.
 *
 *  \tparam ForwardIterator is a model of <a href="http://www.sgi.com/tech/stl/ForwardIterator">Forward Iterator</a>,
 *          \p ForwardIterator is mutable, and \p ForwardIterator's \c value_type has a constructor that
 *          takes a single argument of type \p T.
 *
 *  The following code snippet demonstrates how to use \p uninitialized_fill to initialize a range of
 *  uninitialized memory.
 *
 *  \code
 *  #include <komrade/uninitialized_fill.h>
 *  #include <komrade/device_malloc.h>
 *  ...
 *  struct Int
 *  {
 *    __host__ __device__
 *    Int(int x) : val(x) {}
 *    int val;
 *  };  
 *  ...
 *  const int N = 137;
 *
 *  Int val(46);
 *  komrade::device_ptr<Int> array = komrade::device_malloc<Int>(N);
 *  komrade::uninitialized_fill(array, array + N, val);
 *
 *  // Int x = array[i];
 *  // x.val == 46 for all 0 <= i < N
 *  \endcode
 *
 *  \see http://www.sgi.com/tech/stl/uninitialized_fill.html
 *  \see \c fill
 *  \see \c uninitialized_copy
 *  \see \c device_new
 *  \see \c device_malloc
 *
 *  \bug C++ placement new syntax is required by \c uninitialized_fill, but placement new is not yet
 *       supported in CUDA device functions by the CUDA compiler, <tt>nvcc</tt>.
 *       In the meantime, calls to \c uninitialized_fill on device memory will be replaced with
 *       \c fill.
 */
template<typename ForwardIterator, typename T>
  void uninitialized_fill(ForwardIterator first,
                          ForwardIterator last,
                          const T &x);

/*! \} // end filling
 *  \} // transformations
 */

} // end komrade

#include <komrade/detail/uninitialized_fill.inl>
