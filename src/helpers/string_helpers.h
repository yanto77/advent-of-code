#pragma once
#include <string_view>
#include <vector>

using sv = std::string_view;

std::vector<sv> split(sv str, sv delims = " ");

std::pair<sv, sv> split_single(const sv& input, const sv& delim);

std::vector<sv> split_multi(const sv& input, char delim);
