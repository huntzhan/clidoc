#ifndef INCLUDE_CLIDOC_CODEGEN_FILESYSTEM_H_
#define INCLUDE_CLIDOC_CODEGEN_FILESYSTEM_H_

#include <cstdint>
#include <string>

namespace clidoc {

using FlagType = std::uint32_t;

const FlagType RELATIVE_TO_RESOURCE_DIR = 1 << 0;

void CopyFile(
    const std::string &src, const std::string &dst, const FlagType flags);

void CopyDirectory(
    const std::string &src, const std::string &dst, const FlagType flags);

std::string LoadFileFromResource(const std::string &src);

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_CODEGEN_FILESYSTEM_H_
