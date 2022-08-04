#include "FileReadExecutor.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>

#include "HTTPException.hpp"
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
        // ファイルが存在するが、権限がなくオープンできないときはここに入る。
        throw HTTPException(403);
    }

    oss << ifs.rdbuf();

    logging_.Debug("file read finished");
    return ResponseBuilder::Build(oss.str());
}

std::string FileReadExecutor::BuildListPage(
    std::string absolute_path, std::vector<std::string> filenames) {
    std::ostringstream oss;
    std::vector<std::string>::iterator iter;
    std::string title = "Index of " + absolute_path;

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
    std::string absolute_path, std::string alias_resolved_path) {
    dirent *directory_read = NULL;
    std::vector<std::string> filenames;

    logging_.Debug("ListDirectoryExec starts");
    DIR *dir = opendir(alias_resolved_path.c_str());
    if (dir == NULL) {
        // ディレクトリが存在するが、権限がなく内容を確認できないときはここに入る。
        throw HTTPException(403);
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
        FileReadExecutor::BuildListPage(absolute_path, filenames));
}

int FileReadExecutor::CheckFileStat(std::string file_path) {
    struct stat stat_buf;

    if (stat(file_path.c_str(), &stat_buf) == -1) {
        // Requestされたファイルが存在しない場合に、stat失敗する。
        logging_.Info("stat failed");
        logging_.Info(strerror(errno));
        return STAT_FILE_NOT_EXIST;
    }
    if (S_ISREG(stat_buf.st_mode)) {
        logging_.Debug("file indicate regular file.");
        return STAT_REGULAR_FILE;
    }
    if (S_ISDIR(stat_buf.st_mode)) {
        logging_.Debug("URI indicate Directory.");
        return STAT_DIRECTORY;
    }
    return STAT_FILE_NOT_EXIST;
}

HTTPResponse *FileReadExecutor::Exec(HTTPRequest const &request,
                                     ServerLocation const &sl) {
    logging_.Debug("Exec starts");
    logging_.Debug("request.canonical_path = [" + request.canonical_path() +
                   "]");
    std::string alias_resolved_path = sl.ResolveAlias(request.canonical_path());

    logging_.Debug("alias_resolved_path = [" + alias_resolved_path + "]");
    int alias_resolved_path_stat = CheckFileStat(alias_resolved_path);

    if (alias_resolved_path_stat == STAT_FILE_NOT_EXIST) {
        throw HTTPException(404);
    }

    if (alias_resolved_path_stat == STAT_REGULAR_FILE) {
        return GetFileExec(alias_resolved_path);
    }
    if (alias_resolved_path_stat == STAT_DIRECTORY) {
        if (!sl.index_page().empty()) {
            std::string index_page_path =
                alias_resolved_path + "/" + sl.index_page();
            int index_page_stat = CheckFileStat(index_page_path);

            if (index_page_stat == STAT_REGULAR_FILE) {
                return GetFileExec(index_page_path);
            }

            if (index_page_stat == STAT_DIRECTORY) {
                // nginxは301だけど403にする。
                throw HTTPException(403);
            }
            // index_page_stat == STAT_FILE_NOT_EXISTの場合は、
            // index_pageがない場合と同じ挙動。
        }
        if (sl.IsAutoIndexEnabled()) {
            return ListDirectoryExec(request.canonical_path(),
                                     alias_resolved_path);
        } else {
            throw HTTPException(403);
        }
    }
    logging_.Info("Function ends abnormally");
    return NULL;
}
