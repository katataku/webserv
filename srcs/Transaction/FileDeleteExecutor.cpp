#include "FileDeleteExecutor.hpp"

#include <sys/stat.h>

#include <cstdio>
#include <string>

#include "HTTPException.hpp"
#include "ResponseBuilder.hpp"
#include "ServerLocation.hpp"
#include "utils.hpp"

FileDeleteExecutor::FileDeleteExecutor() {}

FileDeleteExecutor::FileDeleteExecutor(FileDeleteExecutor const &other) {
    *this = other;
}

FileDeleteExecutor &FileDeleteExecutor::operator=(
    FileDeleteExecutor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

FileDeleteExecutor::~FileDeleteExecutor() {}

HTTPResponse *FileDeleteExecutor::Exec(HTTPRequest const &request,
                                       ServerLocation const &sl) {
    std::string path = sl.ResolveAlias(request.canonical_path());

    if (!IsExist(path)) {
        throw HTTPException(404);
    }

    // ファイルの削除に失敗する場合、権限がないので403を返す
    if (std::remove(path.c_str()) != 0) {
        throw HTTPException(403);
    }

    return ResponseBuilder::BuildNoBody(200);
}
