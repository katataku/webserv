#ifndef SRCS_WEBSERV_REQUEST_HPP_
#define SRCS_WEBSERV_REQUEST_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "./Response.hpp"
#include "./Socket.hpp"
#include "./utils.hpp"

// Request class
class Request {
 public:
  Request() {}
  ~Request() {}
  Request(const Request& other) { *this = other; }
  Request& operator=(const Request& other) {
    if (this != &other) {
      this->method_ = other.method_;
      this->uri_ = other.uri_;
      this->path_ = other.path_;
      this->extension_ = other.extension_;
      this->query_ = other.query_;
      this->version_ = other.version_;
      this->mime_ = other.mime_;
    }
    return *this;
  }

  void SetMethod(const std::string& method) { method_ = method; }
  void SetURI(const std::string& uri) { uri_ = uri; }
  void SetHTTPVersion(const std::string& ver) { version_ = ver; }
  void SetPath(const std::string& path) { path_ = path; }
  void SetExtension(const std::string& ext) { extension_ = ext; }
  void SetMIME(const std::string& mime) { mime_ = mime; }
  std::string GetMethod() const { return method_; }
  std::string GetURI() const { return uri_; }
  std::string GetHTTPVersion() const { return version_; }
  std::string GetPath() const { return path_; }
  std::string GetExtension() const { return extension_; }
  std::string GetMIME() const { return mime_; }

  // TODO(iyamada) GETメソッドだけ
  Response ExecMethod() {
    Response resp;
    std::string body;

    if (method_ == "GET") {
      std::ifstream ifs(std::string("./sample_data/" + path_).c_str());
      std::stringstream ss;

      #if DEBUG
      char *tmp = getcwd(NULL, 0);
      std::cerr << "[debug] " << tmp << std::endl;
      std::cerr << "[debug] path : " << "./sample_data/" + path_ << std::endl;
      free(tmp);
      #endif

      if (!ifs) {
        std::string header = "HTTP/1.1 204 No Content\r\n";
        header += "Content-Type: text/html\r\n";
        std::string body = "<html>";
        body += "<title>surume Error</title>";
        body += "<body><p>204 No Content</p></body>";
        body += "</html>\r\n";
        // std::stringstream ss(body.length());
        header += "Content-length: " + numtostr(body.length()) + "\r\n\r\n";
        resp.SetHeader(header);
        resp.SetBody(body);
        #if DEBUG
        std::cerr << "[debug] header : " << header << std::endl;
        #endif
        return resp;
      }

      ss << ifs.rdbuf();
      body = ss.str();
      resp.SetBody(body);
    } else {
      std::string header = "HTTP/1.1 501 Not Implemented\r\n";
      std::string body = "<html>";
      body += "<title>surume Error</title>";
      body += "<body><p>501 Not Implemented</p></body>";
      body += "</html>\r\n";
      // std::stringstream ss(body.length());
      header += "Content-length: " + numtostr(body.length()) + "\r\n";
      header += "Content-Type: text/html\r\n\r\n";
      resp.SetHeader(header);
      resp.SetBody(body);
      #if DEBUG
      std::cerr << "[debug] header : " << header << std::endl;
      #endif
      return resp;
    }

    // Set header
    std::string header = "HTTP/1.1 200 OK\r\n";
    // std::stringstream ss(body.length());
    header += "Content-length: " + numtostr(body.length()) + "\r\n";
    header += "Content-Type: " + mime_ + "\r\n\r\n";
    resp.SetHeader(header);

    #if DEBUG
    std::cerr << "[debug] header : " << header << std::endl;
    #endif

    return resp;
  }

  // TODO(iyamada) 必要最低限、エッジケースはカバーしてない
  static Request Parse(const std::string& req) {
    Request ret;
    std::string request_line = GetToken(req, "\r\n");

    #if DEBUG
    std::cerr << "[debug] request_line   : " << request_line << std::endl;
    #endif

    // TODO(iyamada) とりあえず、すでに' 'や'\r\n'でトークナイズされているとした
    // Parse <method> <uri> <http version>
    std::string method = GetToken(request_line, " ");
    request_line = Consume(request_line, " ");
    std::string uri = GetToken(request_line, " ");
    std::string path;
    if (uri == "/") {
      path = "html/index.html";  // TODO(iyamada) configで決められるように？
    } else {
      // TODO(iyamada) URIからファイルパスを取得
      path = uri;
    }
    request_line = Consume(request_line, " ");

    std::string httpver = request_line;
    ret.SetMethod(method);
    ret.SetURI(uri);
    ret.SetPath(path);
    ret.SetHTTPVersion(httpver);

    // Get file extension
    std::string extension = GetFileExt(path);
    ret.SetExtension(extension);

    #if DEBUG
    std::cerr << "[debug] method         : " << method << std::endl;
    std::cerr << "[debug] URI            : " << uri << std::endl;
    std::cerr << "[debug] path           : " << path << std::endl;
    std::cerr << "[debug] http version   : " << httpver << std::endl;
    std::cerr << "[debug] file extension : " << extension << std::endl;
    #endif


    // Determine MIME from file extension
    std::string mime;
    if (extension == "html") {
      mime = "text/html";
    } else if (extension == "css") {
      mime = "text/css";
    } else if (extension == "ico") {
      mime = "image/vnd.microsoft.icon";
    } else if (extension == "jpeg"
        || extension == "jpg"
        || extension == "JPG") {
      mime = "image/jpeg";
    } else if (extension == "js") {
      mime = "text/javascript";
    } else if (extension == "collection") {
      mime = "font/collection";
    } else if (extension == "otf") {
      mime = "font/otf";
    } else if (extension == "sfnt") {
      mime = "font/sfnt";
    } else if (extension == "ttf") {
      mime = "font/ttf";
    } else if (extension == "woff") {
      mime = "font/woff";
    } else if (extension == "woff2") {
      mime = "font/woff2";
    } else if (extension == "ico") {
      mime = "image/x-icon";
    } else {
      error("Error: Unknown extension");
    }
    ret.SetMIME(mime);

    #if DEBUG
    std::cerr << "[debug] MIME           : " << mime << std::endl;
    #endif

    // TODO(iyamada) パースしないといけない
    std::string next = Consume(req, "\r\n");

    return ret;
  }

 private:
  std::string method_;
  std::string uri_;
  std::string path_;
  std::string extension_;
  std::string query_;
  std::string version_;
  std::string mime_;
};

inline std::ostream& operator<<(std::ostream& out, const Request& rhs) {
  out << "method       : " << rhs.GetMethod() << "\n";
  out << "URI          : " << rhs.GetURI() << "\n";
  out << "HTTP version : " << rhs.GetHTTPVersion() << "\n";
  out << "path         : " << rhs.GetPath() << "\n";
  out << "file ext     : " << rhs.GetExtension() << "\n";
  out << "MIME         : " << rhs.GetMIME() << "\n";
  return out;
}

#endif  // SRCS_WEBSERV_REQUEST_HPP_
