#ifndef PROJECT64_TEXT_F_H
#define PROJECT64_TEXT_F_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>

template <typename T>
using Matrix = std::vector<std::vector<T>>;
using Text = std::vector<std::string>;
template <typename T>
using Coordinate = std::pair<T, T>;

enum {
    LEFT = 1,
    RIGHT = 2,
    CENTER = 3,
    JUSTIFIED = 4};

struct Dictionary {
    std::vector<std::pair<std::string, std::vector<Coordinate<unsigned int>>>> m_entries;

    void addWord (const std::string& word, Coordinate<unsigned int> occurence) {
        m_entries.push_back({word, {occurence}});
    }
    void entrySet (const std::string& word, Coordinate<unsigned int> occurrence) {
        unsigned int i = 0;
        for (auto & pair : m_entries)
            if (pair.first == word) {
                bool flag = true;
                for (auto item : pair.second) {
                    if (item.first == occurrence.first && item.second == occurrence.second)
                        flag = false;
                }
                if (flag) pair.second.push_back(occurrence);
                return;
            }
        addWord(word, occurrence);
    }
};

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

    Dictionary m_dict;

public:

    TEXT_F();
    explicit TEXT_F(const std::string& inputPath, std::string outputPath = "");
    ~TEXT_F();

    void setInputFile(const std::string& inputPath);
    void setOutputFile(const std::string& outputPath);

    void readFile ();
    void fillDictionary();
    void showContent ();
    void showHighlights(std::vector<std::pair<unsigned int, unsigned int>>);
    void fixContent (size_t SizeofBlock = m_SizeOfBlock, int fixType = LEFT);
    void saveContent ();
    void encryptContent ();
    void decryptContent ();

    std::vector<Coordinate<unsigned int>> getOccurrences (const std::string& word);

    void ShowMenu ();

    //std::vector<std::pair<uint64_t, uint64_t>> findWord (const std::string & word);
};



#endif //PROJECT64_TEXT_F_H
