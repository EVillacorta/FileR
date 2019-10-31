#ifndef PROJECT64_TEXT_F_H
#define PROJECT64_TEXT_F_H


#include <string>
#include <fstream>
#include <vector>
#include <sstream>

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using Text = std::vector<std::string>;

enum {LEFT = 1, RIGHT = 2, CENTER = 3, JUSTIFIED = 4};

class TEXT_F {
private:
    static const int m_SizeOfBlock = 80;
    static const int m_JAcceptance = 40;

    std::string m_inputPath;
    std::string m_outputPath;

    std::ifstream m_inputFile;
    std::ofstream m_outputFile;

    Text m_fileContent;
    Matrix <std::string> m_parsedContent;

public:

    TEXT_F();
    explicit TEXT_F(const std::string& inputPath, std::string outputPath = "");
    ~TEXT_F();

    void setInputFile(const std::string& inputPath);
    void setOutputFile(const std::string& outputPath);

    void readFile ();
    Text getContent ();
    void showContent ();
    void fixContent (size_t SizeofBlock = m_SizeOfBlock, int fixType = LEFT);
    void saveContent ();

    void ShowMenu ();

    std::vector<std::pair<uint64_t, uint64_t>> findWord (const std::string & word);
};


#endif //PROJECT64_TEXT_F_H
