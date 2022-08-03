#include "DefaultValues.hpp"

#include <string>

const std::string DefaultValues::kServerName = "";   // NOLINT
const std::string DefaultValues::kPath = "";         // NOLINT
const std::string DefaultValues::kIndexPage = "";    // NOLINT
const std::string DefaultValues::kRedirectUrl = "";  // NOLINT
// TODO(hayashi-ay): aliasのデフォルト値を設定する
const std::string DefaultValues::kAlias = "";         // NOLINT
const std::string DefaultValues::kCgiExtension = "";  // NOLINT
const int DefaultValues::kPort = 80;
const int DefaultValues::kClientMaxBodySize = 1024;
const std::string DefaultValues::kAutoIndex = "off";  // NOLINT
