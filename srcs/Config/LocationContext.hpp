#ifndef SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#define SRCS_CONFIG_LOCATIONCONTEXT_HPP_

#include <map>
#include <set>
#include <string>

class LocationContext {
 public:
    LocationContext();
    LocationContext(const LocationContext& other);
    LocationContext& operator=(const LocationContext& other);
    ~LocationContext();

 private:
    std::map<int, std::string> error_pages_;
    int client_max_body_size_;
    bool auto_index_;
    std::string index_page_;
    std::set<std::string> allow_methods_;
    std::string alias_;
    std::string cgi_extension_;
};

#endif  // SRCS_CONFIG_LOCATIONCONTEXT_HPP_
