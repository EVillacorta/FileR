#ifndef PROJECT64_HEADER_H
#define PROJECT64_HEADER_H

#include <string>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using Text = std::vector<std::string>;
using int_t = long long int;

std::vector<int_t> getLength (const Matrix <std::string> & parsedText, const bool & countSpaces);

Matrix<std::string> parseText (const Text & lines);
Matrix<std::string> adjustToBlock (const Matrix <std::string> & parsedText, size_t sizeofBlock);
Text buildTextLeft(const Matrix <std::string> & parsedText);
Text buildTextRight(const Matrix <std::string> & parsedText, size_t blockSize);
Text buildTextCenter(const Matrix <std::string> & parsedText, size_t blockSize);
Text buildTextJustified (const Matrix <std::string> & parsedText, size_t blockSize, size_t acceptance);

char* setColor(int color=7, char* Message = (char*)"");

template <typename T>
bool inVect (T item, std::vector<T> vec);

#endif //PROJECT64_HEADER_H
