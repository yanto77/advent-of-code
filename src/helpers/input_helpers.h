#pragma once
#include "../setup/input_t.h"
#include <string>
#include <string_view>
#include <functional>

/// Input helpers

void load_input(input_t& input, const std::string& filename);

void free_input(input_t& input);

void parse_input(const input_t& input, std::function<void(const std::string_view&)> line_cb);
