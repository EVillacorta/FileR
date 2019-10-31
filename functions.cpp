#include <vector>
#include <string>
#include <sstream>

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using Text = std::vector<std::string>;
using int_t = long long int;

std::vector<int_t> getLength(const Matrix<std::string> &parsedText, const bool &countSpaces) {
    std::vector<int_t> lengths;
    int_t restart = (countSpaces)?(-1): 0;
    int_t length = restart;
    for (const auto& lines : parsedText) {
        for (const auto& word : lines) {
            length += (int_t)word.size() + (int_t)countSpaces;
        }
        lengths.push_back(length);
        length = restart;
    }
    return lengths;
}

Matrix<std::string> parseText(const Text &lines) {
    std::vector<std::vector<std::string>> parsedText;
    for (const auto& line : lines) {
        std::vector<std::string> parsedLine;
        std::string word;
        std::stringstream ss(line);
        while (getline (ss, word, ' '))
            parsedLine.push_back(word);
        parsedText.push_back(parsedLine);
    }
    return parsedText;
}
Matrix<std::string> adjustToBlock(const Matrix<std::string> &parsedText, size_t sizeofBlock) {
    Matrix <std::string> newText;
    Text alignedLine;
    for (const auto& parsedLines : parsedText) {
        int c = -1;
        for (const auto& word : parsedLines) {
            if (c + word.size() + 1 > sizeofBlock) {
                newText.push_back(alignedLine);
                c = -1;
                alignedLine = {};
            }
            alignedLine.push_back(word);
            c += (int) word.size() + 1;
        }
        newText.push_back(alignedLine);
        alignedLine = {};
    }
    return newText;
}
Text buildTextLeft(const Matrix <std::string> & parsedText) {
    Text alignedText;
    for (uint64_t i = 0; i < parsedText.size(); ++i) {
        alignedText.push_back(parsedText[i][0]);
        for (uint64_t j = 1; j < parsedText[i].size(); ++j) {
            alignedText[i] = alignedText[i] + ' ' + parsedText[i][j];
        }
    }
    return alignedText;
}
Text buildTextRight(const Matrix <std::string> & parsedText, size_t blockSize) {
    Text alignedText(parsedText.size());
    std::vector<int_t> sizes = getLength(parsedText, true);
    for (auto & size : sizes)
        size = blockSize - size;
    for (uint64_t i = 0; i < sizes.size(); ++i) {
        for (uint64_t j = 0; j < sizes[i]; ++j) {
            alignedText[i] = alignedText[i] + ' ';
        }
    }
    for (uint64_t i = 0; i < parsedText.size(); ++i) {
        alignedText[i] = alignedText[i] + parsedText[i][0];
        for (uint64_t j = 1; j < parsedText[i].size(); ++j) {
            alignedText[i] = alignedText[i] + ' ' + parsedText[i][j];
        }
    }
    return alignedText;
}
Text buildTextCenter(const Matrix <std::string> & parsedText, size_t blockSize) {
    Text alignedText(parsedText.size());
    std::vector<int_t> sizes = getLength(parsedText, true);
    for (auto & size : sizes)
        size = (blockSize - size) / 2;
    for (size_t i = 0; i < sizes.size(); ++i) {
        for (int_t j = 0; j < sizes[i]; ++j) {
            alignedText[i] = alignedText[i] + ' ';
        }
    }
    for (uint64_t i = 0; i < parsedText.size(); ++i) {
        alignedText[i] = alignedText[i] + parsedText[i][0];
        for (uint64_t j = 1; j < parsedText[i].size(); ++j) {
            alignedText[i] = alignedText[i] + ' ' + parsedText[i][j];
        }
    }
    return alignedText;
}
Text buildTextJustified (const Matrix <std::string> & parsedText, size_t blockSize, size_t acceptance) {
    Text alignedText(parsedText.size());
    std::vector<int_t> sizes = getLength(parsedText, false);
    std::vector<int_t> spacesBlocksInLine;
    spacesBlocksInLine.reserve(parsedText.size());
    for (const auto & parsedLine : parsedText)
        spacesBlocksInLine.push_back(parsedLine.size() - 1);
    for (size_t i = 0; i < alignedText.size(); ++i) {
        alignedText[i] = parsedText[i][0];
        for (size_t j = 1; j < parsedText[i].size(); ++j) {
            size_t expression;
            if (sizes[i] <= acceptance) expression = 1;
            else expression = (blockSize - sizes[i]) / spacesBlocksInLine[i] + ((blockSize - sizes[i]) % spacesBlocksInLine[i] >= j);
            for (size_t spaces = 0; spaces < expression; spaces++)
                alignedText[i] = alignedText[i] + ' ';
            alignedText[i] = alignedText[i] + parsedText[i][j];
        }
    }
    return alignedText;
}