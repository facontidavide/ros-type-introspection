/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright 2016-2017 Davide Faconti
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
* *******************************************************************/

#include "ros_type_introspection/stringtree_leaf.hpp"
#include "ros_type_introspection/helper_functions.hpp"

namespace RosIntrospection{



int StringTreeLeaf::toStr(char* buffer) const
{
  const StringTreeNode* leaf_node = this->node_ptr;
  if( !leaf_node ){
    return -1;
  }

  std::array<const std::string*, 32> strings_from_leaf_to_root;
  int index = 0;

  while(leaf_node)
  {
    const std::string& str = leaf_node->value();
    strings_from_leaf_to_root[index] = &str;
    index++;
    leaf_node = leaf_node->parent();
  };

  strings_from_leaf_to_root[index] = nullptr;
  index--;

  int array_count = 0;
  int off = 0;

  while ( index >=0 )
  {
    const std::string* str = strings_from_leaf_to_root[index];
    const size_t S = str->size();
    if( S == 1 && str->at(0) == '#' )
    {
      buffer[off-1] = '.';
      off += print_number(&buffer[off], this->index_array[ array_count++ ] );
    }
    else{
      memcpy( &buffer[off], str->data(), S );
      off += S;
    }
    if( index > 0 ){
      buffer[off++] = '/';
    }
    index--;
  }
  buffer[off] = '\0';
  return off;
}

}
