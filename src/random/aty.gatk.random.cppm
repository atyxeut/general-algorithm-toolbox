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

export module aty.gatk.random;

import std;

import aty.gatk.data_structure.graph;
import aty.gatk.math.integer;

export namespace aty::gatk::random {

inline const auto time_seed = std::chrono::steady_clock::now().time_since_epoch().count();
inline std::random_device seed_generator;

inline std::mt19937 mt19937_engine(seed_generator());
inline std::mt19937_64 mt19937_64_engine(seed_generator());

template <std::integral T>
[[nodiscard]] auto uniform_distribution(T l, T r)
{
  return std::uniform_int_distribution<T>(l, r);
}

template <std::floating_point T>
[[nodiscard]] auto uniform_distribution(T l, T r)
{
  return std::uniform_real_distribution<T>(l, r);
}

template <typename T, typename TEngine = std::mt19937>
  requires std::is_arithmetic_v<T>
[[nodiscard]] T rand(T l, T r, TEngine& engine = mt19937_engine)
{
  return uniform_distribution(l, r)(engine);
}

} // namespace aty::gatk::random

namespace aty::gatk::random::generate {

constexpr char decimal_digit_character[10] {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

} // namespace aty::gatk::random::generate

export namespace aty::gatk::random::generate {

template <bool OnlyPositiveInteger = false>
[[nodiscard]] auto integer(usize integer_length)
{
  std::string data;
  data.reserve(integer_length);

  if (!OnlyPositiveInteger & rand(0uz, 1uz))
    data += '-';
  data += decimal_digit_character[rand(1uz, 9uz)];
  auto dist = uniform_distribution(0uz, 9uz);
  while (integer_length-- > 1)
    data += decimal_digit_character[dist(mt19937_engine)];
  return data;
}

template <std::integral T>
[[nodiscard]] auto permutation(T begin, T end)
{
  if (begin > end)
    throw std::invalid_argument("invalid integer range");

  std::vector<T> data(end - begin + 1);
  std::iota(data.begin(), data.end(), begin);
  std::ranges::shuffle(data, mt19937_engine);
  return data;
}

// get the edge list of a random unweighted tree
template <bool FlowerGraph = false, std::integral T>
[[nodiscard]] auto unweighted_tree(T vertex_begin, T vertex_end)
{
  if (vertex_begin > vertex_end)
    throw std::invalid_argument("invalid vertex index range");

  const auto p = permutation(vertex_begin, vertex_end);

  const auto edge_cnt = vertex_end - vertex_begin;
  graph::edge_list<T> data;
  data.reserve(edge_cnt);

  for (auto v = 1uz; v <= edge_cnt; ++v) {
    if constexpr (FlowerGraph)
      data.add_edge(p[0], p[v]);
    else
      data.add_edge(p[rand(0uz, v - 1)], p[v]);
  }

  return data;
}

// get the edge list of a random weighted tree
template <bool FlowerGraph = false, std::integral TVertex, std::integral TWeight>
[[nodiscard]] auto weighted_tree(TVertex vertex_begin, TVertex vertex_end, TWeight weight_begin, TWeight weight_end)
{
  if (vertex_begin > vertex_end)
    throw std::invalid_argument("invalid vertex index range");

  if (weight_begin > weight_end)
    throw std::invalid_argument("invalid weight range");

  const auto p = permutation(vertex_begin, vertex_end);

  const auto edge_cnt = vertex_end - vertex_begin;
  graph::edge_list<TVertex, TWeight> data;
  data.reserve(edge_cnt);

  auto w_dist = uniform_dist(weight_begin, weight_end);
  for (auto v = 1uz; v <= edge_cnt; ++v) {
    if constexpr (FlowerGraph)
      data.add_edge(p[0], p[v], w_dist(mt19937_engine));
    else
      data.add_edge(p[rand(0uz, v - 1)], p[v], w_dist(mt19937_engine));
  }

  return data;
}

} // namespace aty::gatk::random::generate
