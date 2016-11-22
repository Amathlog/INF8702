#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

std::string readFile(std::string fileName) {

    std::ifstream file;

    file.open(fileName.c_str());

    if (!file.good())
    {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        exit(-1);
    }

    // Create a string stream
    std::stringstream stream;

    // Dump the contents of the file into it
    stream << file.rdbuf();

    // Close the file
    file.close();

    return stream.str();
}
