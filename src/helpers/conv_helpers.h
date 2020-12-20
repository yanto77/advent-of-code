#pragma once
#include <string_view>

/// Conversion helpers

/// Returns int or (-1) on failure.
int to_int(const std::string_view& sv);

char to_char(const std::string_view& sv);

/// Convert full 6-char hex
/// Expects input in form of "#123abc"
int hex_to_dec(const std::string_view& sv);
