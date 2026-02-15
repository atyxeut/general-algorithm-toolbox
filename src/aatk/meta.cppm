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
export module aatk.meta;

import std;

export namespace aatk::meta {

template <typename... Ts>
using type_list = std::tuple<Ts...>;

template <typename>
struct is_no_cv_type_list : std::false_type
{
};

template <typename... Ts>
struct is_no_cv_type_list<type_list<Ts...>> : std::true_type
{
};

template <typename T>
constexpr bool is_no_cv_type_list_v = is_no_cv_type_list<T>::value;

template <typename T>
using is_type_list = is_no_cv_type_list<std::remove_cv_t<T>>;

template <typename T>
constexpr bool is_type_list_v = is_type_list<T>::value;

template <typename T>
concept list_of_types = is_type_list_v<T>;

template <typename T>
struct is_no_cv_empty_type_list : std::false_type
{
};

template <>
struct is_no_cv_empty_type_list<type_list<>> : std::true_type
{
};

template <typename T>
constexpr bool is_no_cv_empty_type_list_v = is_no_cv_empty_type_list<T>::value;

template <typename T>
using is_empty_type_list = is_no_cv_empty_type_list<std::remove_cv_t<T>>;

template <typename T>
constexpr bool is_empty_type_list_v = is_empty_type_list<T>::value;

// behave like Haskell List length
template <list_of_types T>
using length = std::tuple_size<T>;

template <list_of_types T>
constexpr std::size_t length_v = length<T>::value;

// behave like Haskell List head
template <list_of_types>
struct head;

template <>
struct head<type_list<>>;

template <typename T, typename... Ts>
struct head<type_list<T, Ts...>>
{
  using type = T;
};

template <list_of_types T>
using head_t = head<T>::type;

template <std::size_t Idx, list_of_types T>
  requires (Idx < length_v<T>)
struct nth;

template <std::size_t Idx, typename... Ts>
struct nth<Idx, type_list<Ts...>>
{
  using type = Ts...[Idx];
};

template <std::size_t Idx, list_of_types T>
using nth_t = nth<Idx, T>::type;

// behave like Haskell List last
template <list_of_types T>
struct last
{
  using type = nth_t<length_v<T> - 1, T>;
};

template <>
struct last<type_list<>>;

template <list_of_types T>
using last_t = last<T>::type;

// behave like Haskell List : operator (1 : [1, 2] --> [1, 1, 2])
template <typename, list_of_types>
struct cons;

template <typename T, typename... Ts>
struct cons<T, type_list<Ts...>>
{
  using type = type_list<T, Ts...>;
};

template <typename T, list_of_types U>
using cons_t = cons<T, U>::type;

// behave like Haskell Seq |> operator (fromList [1, 2] |> 3 --> fromList [1, 2, 3])
template <typename, list_of_types>
struct snoc;

template <typename T, typename... Ts>
struct snoc<T, type_list<Ts...>>
{
  using type = type_list<Ts..., T>;
};

template <typename T, list_of_types U>
using snoc_t = snoc<T, U>::type;

// behave like Haskell List concat
template <list_of_types...>
struct concat;

template <typename... Ts>
struct concat<type_list<Ts...>>
{
  using type = type_list<Ts...>;
};

template <typename... TsOfLeft, typename... TsOfRight>
struct concat<type_list<TsOfLeft...>, type_list<TsOfRight...>>
{
  using type = type_list<TsOfLeft..., TsOfRight...>;
};

template <list_of_types T0, list_of_types T1, list_of_types... Ts>
struct concat<T0, T1, Ts...>
{
  using type = concat<typename concat<T0, T1>::type, Ts...>::type;
};

template <list_of_types... Ts>
using concat_t = concat<Ts...>::type;

// behave like Haskell List reverse
template <list_of_types>
struct reverse;

template <>
struct reverse<type_list<>>
{
  using type = type_list<>;
};

template <typename T, typename... Ts>
struct reverse<type_list<T, Ts...>>
{
  using type = snoc_t<T, typename reverse<type_list<Ts...>>::type>;
};

template <list_of_types T>
using reverse_t = reverse<T>::type;

// behave like Haskell List tail
template <list_of_types>
struct tail;

template <>
struct tail<type_list<>>;

template <typename T, typename... Ts>
struct tail<type_list<T, Ts...>>
{
  using type = type_list<Ts...>;
};

template <list_of_types T>
using tail_t = tail<T>::type;

// behave like Haskell List init
template <list_of_types>
struct init;

template <>
struct init<type_list<>>;

template <typename T>
struct init<type_list<T>>
{
  using type = type_list<>;
};

template <typename T, typename... Ts>
struct init<type_list<T, Ts...>>
{
  using type = cons_t<T, typename init<type_list<Ts...>>::type>;
};

template <list_of_types T>
using init_t = init<T>::type;

} // namespace aatk::meta

namespace aatk::meta {

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

} // namespace aatk::meta
