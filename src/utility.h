#pragma once

#include <iostream>
#include <fstream>
#include <vector>

std::vector<unsigned char> load_file(const std::string &filename)
{
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::cout << "load_file:" << filename.c_str() << std::endl;

    std::vector<unsigned char> buffer;
    //get filesize
    std::streamsize size = 0;
    if (file.seekg(0, std::ios::end).good())
        size = file.tellg();
    if (file.seekg(0, std::ios::beg).good())
        size -= file.tellg();
    std::cout << "load_file size:" << size << std::endl;

    //read contents of the file into the vector
    if (size > 0)
    {
        buffer.resize((size_t)size);
        file.read((char *)(&buffer[0]), size);
    }
    else
    {
        buffer.clear();
        printf("load_file: error\n");
    }

    return buffer;
}
