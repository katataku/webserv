#include "DefaultValues.hpp"

#include <string>

const std::string DefaultValues::kServerName = "";
// TODO(iyamada)
// デフォルトのpathはどうするか。正規表現で全部マッチみたいになってそう。
const std::string DefaultValues::kPath = "";
const std::string DefaultValues::kIndexPage = "";
const std::string DefaultValues::kRedirectUrl = "";
const std::string DefaultValues::kAlias = "";
const std::string DefaultValues::kCgiExtension = "";
const int DefaultValues::kPort = 80;
const int DefaultValues::kClientMaxBodySize = 1024;
const bool DefaultValues::kAutoIndex = false;
