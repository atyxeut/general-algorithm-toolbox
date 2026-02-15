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

export module test.aatk.meta;

import std;

import aatk.meta;

namespace test::aatk::meta {

using empty_type_list = ::aatk::meta::type_list<>;

using type_list_1 = ::aatk::meta::type_list<double, float, std::vector<int>, long long>;
using type_list_2 = ::aatk::meta::type_list<std::string, unsigned, const volatile bool, bool>;
using type_list_3 = ::aatk::meta::type_list<int, char, const int, volatile char>;
using type_list_4 = ::aatk::meta::type_list<int, int, int, int, int, int, int>;

using merged_type_list_of_1_2 = ::aatk::meta::type_list<double, float, std::vector<int>, long long, std::string, unsigned, const volatile bool, bool>;
using merged_type_list_of_1_2_3 = ::aatk::meta::type_list<double, float, std::vector<int>, long long, std::string, unsigned, const volatile bool, bool, int, char, const int, volatile char>;
using merged_type_list_of_1_2_3_4 = ::aatk::meta::type_list<double, float, std::vector<int>, long long, std::string, unsigned, const volatile bool, bool, int, char, const int, volatile char, int, int, int, int, int, int, int>;

export consteval void does_meta_concat_works() noexcept
{
  static_assert(std::same_as<::aatk::meta::concat_t<type_list_1>, type_list_1>);
  static_assert(std::same_as<::aatk::meta::concat_t<type_list_2>, type_list_2>);

  static_assert(std::same_as<::aatk::meta::concat_t<type_list_1, type_list_2>, merged_type_list_of_1_2>);
  static_assert(std::same_as<::aatk::meta::concat_t<type_list_1, type_list_2, type_list_3>, merged_type_list_of_1_2_3>);
  static_assert(std::same_as<::aatk::meta::concat_t<type_list_1, type_list_2, type_list_3, type_list_4>, merged_type_list_of_1_2_3_4>);
}

} // namespace test::aatk::meta
