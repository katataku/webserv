#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <set>

class LocationContext {
public:
  LocationContext() {}
  LocationContext(const LocationContext& other) { *this = other; }
  LocationContext& operator=(const LocationContext& other) {
    if (this != &other) {

    }
    return *this;
  }
  ~LocationContext() {}

private:
  std::map<int, std::string>   error_pages_;
  int                          client_max_body_size_;
  bool                         auto_index_;
  std::string                  index_page_;
  std::set<std::string>        allow_methods_;
  std::string                  alias_;
  std::string                  cgi_extension_;
};

#endif
