#include "FileReadExecutor.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ResponseBuilder.hpp"
#include "ServerLocation.hpp"

FileReadExecutor::FileReadExecutor() : logging_(Logging(__FUNCTION__)) {}

FileReadExecutor::FileReadExecutor(FileReadExecutor const &other) {
    *this = other;
}

FileReadExecutor &FileReadExecutor::operator=(FileReadExecutor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

FileReadExecutor::~FileReadExecutor() {}

HTTPResponse *FileReadExecutor::GetFileExec(std::string file_path) {
    std::ifstream ifs(file_path.c_str());
    std::string str;
    std::ostringstream oss;

    if (!ifs) {
        // TODO(takkatao):
        // ファイルが存在するが、権限がなくオープンできないときはここに入る。403を返す。
        return NULL;
    }

    oss << ifs.rdbuf();

    logging_.Debug("file read finished");
    logging_.Debug(oss.str());
    return ResponseBuilder::Build(oss.str());
}

std::string FileReadExecutor::BuildListPage(
    std::string uri, std::vector<std::string> filenames) {
    std::ostringstream oss;
    std::vector<std::string>::iterator iter;
    std::string title = "Index of " + uri;

    oss << "<html>" << std::endl
        << "<head><title>" << title << "</title></head>" << std::endl
        << "<body>" << std::endl
        << "<h1>" << title << "</h1>" << std::endl
        << "<hr>" << std::endl
        << "<pre>" << std::endl;

    for (iter = filenames.begin(); iter != filenames.end(); iter++) {
        oss << "<a href=" << '"' << *iter << '"' << "> " << *iter << "</a>"
            << std::endl;
    }

    oss << "</pre><hr></body>" << std::endl << "</html>" << std::endl;
    return oss.str();
}

HTTPResponse *FileReadExecutor::ListDirectoryExec(
    std::string request_uri, std::string alias_resolved_uri) {
    dirent *directory_read = NULL;
    std::vector<std::string> filenames;

    logging_.Debug("ListDirectoryExec starts");
    DIR *dir = opendir(alias_resolved_uri.c_str());
    if (dir == NULL) {
        // TODO(takkatao):
        // ディレクトリが存在するが、権限がなく内容を確認できないときはここに入る。403を返す。
        return NULL;
    }

    while ((directory_read = readdir(dir)) != NULL) {
        std::string filename = directory_read->d_name;
        if (directory_read->d_type == DT_DIR) {
            filename += "/";
        }
        filenames.push_back(filename);
    }
    closedir(dir);

    return ResponseBuilder::Build(
        FileReadExecutor::BuildListPage(request_uri, filenames));
}

HTTPResponse *FileReadExecutor::Exec(HTTPRequest const &request,
                                     ServerLocation const &sl) {
    logging_.Debug("Exec starts");
    struct stat stat_buf;
    std::string alias_resolved_uri = sl.ResolveAlias(request.request_target());

    logging_.Debug("alias_resolved_uri = [" + alias_resolved_uri + "]");

    if (stat(alias_resolved_uri.c_str(), &stat_buf) == -1) {
        // TODO(takkatao): ファイルが存在しないときはここに入る。404を返す。
        logging_.Fatal("stat failed");
        logging_.Fatal(strerror(errno));
        return NULL;
    }

    if (S_ISREG(stat_buf.st_mode)) {
        logging_.Debug("URI indicate regular file.");
        return GetFileExec(alias_resolved_uri);
        //        return GetFileExecutor(request, sl);
    }
    if (S_ISDIR(stat_buf.st_mode)) {
        logging_.Debug("URI indicate Directory.");
        return ListDirectoryExec(request.request_target(), alias_resolved_uri);
    }
    logging_.Info("Function ends abnormally");
    return NULL;
}
