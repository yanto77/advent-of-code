#pragma once
#include "../setup/input_t.h"
#include <functional>
#include <string>
#include <string_view>

/// Input helpers

input_t load_input(const std::string& filename);

void free_input(input_t& input);

void parse_input(const input_t& input, std::function<void(const std::string_view&)> line_cb);
