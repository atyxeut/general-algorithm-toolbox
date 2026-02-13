// Copyright 2026 atyxeut
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

export module aty.gatk.data_structure.graph;

import std;

import aty.gatk.math;

namespace aty::gatk::graph {

template <typename>
struct weight;

template <>
struct weight<void>
{
};

template <tmp::arithmetic T>
struct weight<T>
{
  T w;
};

template <std::integral TVertex, typename TWeight = void>
struct edge_from : weight<TWeight>
{
  TVertex u;
};

template <std::integral TVertex, typename TWeight = void>
struct edge_to : weight<TWeight>
{
  TVertex v;
};

template <std::integral TVertex, typename TWeight = void>
struct edge : weight<TWeight>
{
  TVertex u;
  TVertex v;
};

export template <std::integral TVertex, typename TWeight = void>
class edge_list
{
  std::vector<edge<TVertex, TWeight>> edges_;

public:
  [[nodiscard]] constexpr const auto& operator [](usize idx) const { return edges_[idx]; }
  [[nodiscard]] auto& operator [](usize idx) { return edges_[idx]; }

  constexpr auto begin() const noexcept { return edges_.begin(); }
  constexpr auto begin() noexcept { return edges_.begin(); }

  constexpr auto end() const noexcept { return edges_.end(); }
  constexpr auto end() noexcept { return edges_.end(); }

  [[nodiscard]] constexpr auto size() const noexcept { return edges_.size(); }
  constexpr void reserve(usize capacity) { edges_.reserve(capacity); }

  template <typename... TArgs>
  constexpr auto& add_edge(TArgs... args)
  {
    return edges_.emplace_back(std::forward<TArgs>(args)...);
  }
};

} // namespace aty::gatk::graph
