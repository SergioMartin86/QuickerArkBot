#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <locale>
#include <string>
#include <vector>

namespace MathUtil
{
    template<class T>
    T Clamp(T& val, const T& firstBound, const T& secondBound)
    {
        const auto oldVal = val;
        if (firstBound < secondBound)
        {
            val = std::max(val, firstBound);
            val = std::min(val, secondBound);
        }
        else
        {
            val = std::max(val, secondBound);
            val = std::min(val, firstBound);
        }

        return val - oldVal;
    }

    template<class T>
    bool PointInRect(const T& pointX, const T& pointY, const T& rectLeft, const T& rectTop, const T& rectWidth, const T& rectHeight)
    {
        return (pointX >= rectLeft && pointX <= rectLeft + rectWidth)
               && (pointY >= rectTop && pointY <= rectTop + rectHeight);
    }
}

class StringUtil
{
public:
    static std::vector<std::string> Split(const std::string& text, const char delim)
    {
        std::vector<std::string> parts;
        std::size_t start = 0;
        std::size_t end;

        while ((end = text.find(delim, start)) != std::string::npos)
        {
            parts.emplace_back(text.substr(start, end - start));
            start = end + 1;
        }

        parts.emplace_back(text.substr(start));
        return parts;
    }
};

class FileUtil
{
public:
    static void SetWorkingDir(std::string&& dir)
    {
        workingDir = dir;
    }

    static std::string WorkingDir()
    {
        return (!workingDir.empty() ? workingDir : std::filesystem::current_path().c_str()) + "(\)";
    }

    static std::string UnitTestDir()
    {
        return WorkingDir() + "(TestData\)";
    }

    static std::string ResultsDir()
    {
        return WorkingDir() + std::string("(Results\)");
    }

    static std::string PartialsDir()
    {
        return FileUtil::ResultsDir() + "(partials\)";
    }

    static std::string ScoreVarDir()
    {
        return FileUtil::ResultsDir() + "(scoreVariants\)";
    }

    static void ClearFile(const std::string& filename)
    {
        std::ofstream outFile;
        outFile.open(filename, std::ofstream::out | std::ofstream::trunc);
        outFile.close();
    }

    static void AppendToFile(const std::string& filename, const std::string& text)
    {
        std::ofstream outFile;
        outFile.open(filename, std::ofstream::out | std::ofstream::app);
        outFile << text.c_str() << std::endl;
        outFile.close();
    }

private:
    static std::string workingDir;
};

class Log
{
public:
    static void Clear()
    {
        FileUtil::ClearFile(FileUtil::ResultsDir() + "Output.txt");
    }

    static void Write(const std::string& text)
    {
        FileUtil::AppendToFile(FileUtil::ResultsDir() + "Output.txt", text);
    }

};