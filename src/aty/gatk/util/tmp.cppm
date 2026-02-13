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

// general template metaprogramming utilities
export module aty.gatk.util.tmp;

import std;

export namespace aty::gatk::tmp {

template <typename TTypeList>
struct front;

template <typename T, typename... Ts>
struct front<std::tuple<T, Ts...>>
{
  using type = T;
};

template <typename TTypeList>
using front_t = front<TTypeList>::type;

template <typename TTypeList>
struct back;

template <typename... Ts>
  requires (sizeof...(Ts) > 0)
struct back<std::tuple<Ts...>>
{
  using type = Ts...[sizeof...(Ts) - 1];
};

template <typename TTypeList>
using back_t = back<TTypeList>::type;

template <typename TTypeList, typename T>
struct push_front;

template <typename... Ts, typename T>
struct push_front<std::tuple<Ts...>, T>
{
  using type = std::tuple<T, Ts...>;
};

template <typename TTypeList, typename T>
using push_front_t = push_front<TTypeList, T>::type;

template <typename TTypeList, typename T>
struct push_back;

template <typename... Ts, typename T>
struct push_back<std::tuple<Ts...>, T>
{
  using type = std::tuple<Ts..., T>;
};

template <typename TTypeList, typename T>
using push_back_t = push_back<TTypeList, T>::type;

template <typename TTypeList>
struct pop_front;

template <typename T, typename... Ts>
struct pop_front<std::tuple<T, Ts...>>
{
  using type = std::tuple<Ts...>;
};

template <typename TTypeList>
using pop_front_t = pop_front<TTypeList>::type;

template <typename TTypeList>
struct pop_back;

template <typename T>
struct pop_back<std::tuple<T>>
{
  using type = std::tuple<>;
};

template <typename T, typename... Ts>
struct pop_back<std::tuple<T, Ts...>>
{
  using type = push_front_t<typename pop_back<std::tuple<Ts...>>::type, T>;
};

template <typename TTypeList>
using pop_back_t = pop_back<TTypeList>::type;

template <typename TTypeList>
struct reverse;

template <>
struct reverse<std::tuple<>>
{
  using type = std::tuple<>;
};

template <typename T, typename... Ts>
struct reverse<std::tuple<T, Ts...>>
{
  using type = push_back_t<typename reverse<std::tuple<Ts...>>::type, T>;
};

template <typename TTypeList>
using reverse_t = reverse<TTypeList>::type;

template <typename TTypeListL, typename TTypeListR>
struct concat;

template <typename... TsL, typename... TsR>
struct concat<std::tuple<TsL...>, std::tuple<TsR...>>
{
  using type = std::tuple<TsL..., TsR...>;
};

template <typename TTypeListL, typename TTypeListR>
using concat_t = concat<TTypeListL, TTypeListR>::type;

} // namespace aty::gatk::tmp

namespace aty::gatk::tmp {

template <typename TFrom, typename TTo, bool = std::is_const_v<TFrom>, bool = std::is_volatile_v<TFrom>>
struct claim_cv_selector;

// branch 1: has both cv qualifiers
template <typename TFrom, typename TTo>
struct claim_cv_selector<TFrom, TTo, true, true>
{
  using type = std::add_cv_t<TTo>;
};

// branch 2: has only const qualifier
template <typename TFrom, typename TTo>
struct claim_cv_selector<TFrom, TTo, true, false>
{
  using type = std::add_const_t<TTo>;
};

// branch 3: has only volatile qualifier
template <typename TFrom, typename TTo>
struct claim_cv_selector<TFrom, TTo, false, true>
{
  using type = std::add_volatile_t<TTo>;
};

// branch 4: has no cv qualifiers
template <typename TFrom, typename TTo>
struct claim_cv_selector<TFrom, TTo, false, false>
{
  using type = TTo;
};

// extract the cv-qualifiers of TFrom and apply them to TTo
export template <typename TFrom, typename TTo>
using claim_cv = claim_cv_selector<TFrom, TTo>;

export template <typename TFrom, typename TTo>
using claim_cv_t = claim_cv<TFrom, TTo>::type;

export template <typename T, typename U>
concept no_cvref_same_as = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

export template <typename T, typename U>
concept no_cvref_not_same_as = !no_cvref_same_as<T, U>;

} // namespace aty::gatk::tmp
