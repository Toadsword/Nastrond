/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#ifndef SFGE_FILE_UTILITY_H
#define SFGE_FILE_UTILITY_H
#include <functional>

#ifdef WIN32
#include <experimental/filesystem>
// for convenience
namespace fs = std::experimental::filesystem;
#endif
#include <string>
#include <fstream>

namespace sfge
{	

bool FileExists(const std::string& filename);

bool IsRegularFile(std::string& filename);

bool IsDirectory(const std::string& filename);

void IterateDirectory(std::string& dirname, std::function<void(std::string)>);

std::ifstream::pos_type CalculateFileSize(const std::string& filename);

bool CreateDirectory(const std::string& dirname);

bool RemoveDirectory(const std::string& dirname, bool removeAll=true);
}

#endif
