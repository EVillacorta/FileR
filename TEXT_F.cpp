#include <iostream>
#include <algorithm>
#include <wincrypt.h>
#include "TEXT_F.h"
#include "header.h"

TEXT_F::TEXT_F(){
    m_inputPath = "";
    m_outputPath = "";
}

TEXT_F::TEXT_F(const std::string& inputPath, std::string outputPath) {
    if (outputPath.empty()) outputPath = inputPath;
    m_inputPath = inputPath;
    m_outputPath = outputPath;

    m_inputFile.open(m_inputPath);
    m_outputFile.open(m_outputPath);

    if (!m_inputFile.is_open() || !m_outputFile.is_open())
        exit(-1);
}

TEXT_F::~TEXT_F() {
    m_inputFile.close();
    m_outputFile.close();
}

void TEXT_F::setInputFile(const std::string &inputPath)
{
    if (m_inputFile.is_open())
        m_inputFile.close();
    m_inputPath = inputPath;
    m_inputFile.open(m_inputPath);
}

void TEXT_F::setOutputFile(const std::string &outputPath)
{
    if (m_outputFile.is_open()) {
        m_outputFile.close();
    }
    m_outputPath = outputPath;
    m_outputFile.open(m_outputPath);
}

void TEXT_F::readFile()
{
    if (! m_inputFile.is_open() ) return;
    std::vector<std::string> lines;
    for (std::string line; getline( m_inputFile, line );)
        lines.push_back(line);
    m_fileContent = lines;
}

void TEXT_F::showContent()
{
    for (const auto& line : m_fileContent)
        std::cout << line << '\n';
}

void TEXT_F::showHighlights(std::vector<std::pair<unsigned int, unsigned int>> occurrences)
{
    uint64_t n = 0;
    auto nextOccurrence = occurrences[n];
    for (uint64_t i = 0; i < m_parsedContent.size(); ++i) {
        for (uint64_t j = 0; j < m_parsedContent[i].size(); ++j) {
            if (nextOccurrence.first == i && nextOccurrence.second == j) {
                setColor(3);
                ++n;
                try {
                    nextOccurrence = occurrences.at(n);
                } catch (std::out_of_range &) {}
            }
            else setColor();
            std::cout << m_parsedContent[i][j] << ' ';
        }
    }
    std::cout << std::endl;
}

void TEXT_F::fixContent(size_t SizeofBlock, int fixType)
{
    if (m_parsedContent.empty()) {
        m_parsedContent = parseText(m_fileContent);
    }
    m_parsedContent = adjustToBlock(m_parsedContent, SizeofBlock);

    fillDictionary();

    switch (fixType) {
        case 1: {
            m_fileContent = buildTextLeft(m_parsedContent);
            break;
        }
        case 2: {
            m_fileContent = buildTextRight(m_parsedContent, SizeofBlock);
            break;
        }
        case 3: {
            m_fileContent = buildTextCenter(m_parsedContent, SizeofBlock);
            break;
        }
        case 4: {
            m_fileContent = buildTextJustified(m_parsedContent, SizeofBlock, m_JAcceptance);
            break;
        }
        default:
            exit(-2);
    }

}

void TEXT_F::saveContent()
{
    std::ofstream temp(m_outputPath);
    for (const auto& lines : m_fileContent) {
        temp << lines << std::endl;
    }
}

void TEXT_F::ShowMenu()
{

    selection:
    std::cout << ":::::MENU::::\n 1: open files \n 2: save files \n 3: Adjustment \n 4: Search for word \n 5: Substitute a word \n 6: Encrypt - Decrypt file \n 7: Exit \n";
    int choice; std::cin >> choice;

    switch (choice) {
        case 1: {
            if  (! m_inputFile.is_open() ) {
                std::cout << "\nInput the path of your target file \n";
                std::string inputPath;
                std::cin >> inputPath;
                setInputFile(inputPath);
            }
            if ( ! m_inputFile.is_open() ) {
                std::cout << "\nFile could not be opened. Try again";
                m_inputPath = "";
                m_inputFile.close();
                goto selection;
            }
            readFile();
            fixContent(m_SizeOfBlock, LEFT);
            std::cout << "\nFile contains the following content: \n\n";
            showContent();
            std::cout << '\n';
            break;
        }
        case 2: {
            if ( ! m_outputFile.is_open() ) {
                std::cout << "Input the path of your target file \n";
                std::string outputPath;
                std::cin >> outputPath;
                setOutputFile(outputPath);
            }
            if ( ! m_outputFile.is_open() ) {
                std::cout << "File could not be saved. Try again\n";
                m_outputPath = "";
                m_outputFile.close();
                break;
            }
            saveContent();
            std::cout << "File was saved successfully \n";
            break;
        }
        case 3: {
            std::cout << "1. Format File \n 2. Settings \n 3. Back \n";
            int FChoice; std::cin >> FChoice;
            switch (FChoice) {
                case 1: {
                    std::cout << "1. Left \n 2. Right \n 3. Center \n 4. Justified \n";
                    int TChoice; std::cin >> TChoice;
                    fixContent(m_SizeOfBlock, TChoice);
                    std::cout << "File was correctly formatted\n";
                    showContent();
                    break;
                }
                case 2:
                    break;
                case 3:
                    goto selection;
                default:
                    break;
            }
            break;
        }
        case 4: {
            std::cout << "\n Type in the word you're looking for: \n";
            std::string word; std::cin >> word;

            auto occurrences = getOccurrences(word);

            std::cout << "Occurrences at: " << std::endl;
            if (! occurrences.empty()) {
                for (auto item : occurrences) std::cout << item.first + 1 << ' ' << item.second + 1 << std::endl;
                showHighlights(occurrences);
            } else {
                std::cout << "No occurrences! \n";
                showContent();
            }
            goto selection;
        }
        case 5: {
            std::cout << "Type in the word you wanna substitute: \n";
            std::string baseword; std::cin >> baseword;

            auto occurrences = getOccurrences(baseword);

            std::cout << "The following occurrences were found: " << std::endl;
            if (! occurrences.empty()) {
                for (auto item : occurrences) std::cout << item.first << ' ' << item.second << std::endl;
                showHighlights(occurrences);
            } else {
                std::cout << "No occurrences!";
                showContent();
            }

            int a = 0;
            do {
                a = 0;
                std::cout << "\nType in the occurrence(s) you wanna correct: \n";
                std::cin >> a;
                if (!(a > 0 && a <= occurrences.size())) break;
                std::cout << "Type in the substitution: \n";
                std::string subs; std::cin >> subs;
                m_parsedContent[occurrences[a - 1].first][occurrences[a - 1].second] = subs;
                std::cout << "Continue editing this word?: \n";
                std::string check; std::cin >> check ;
                if (check == "n" || check == "N") break;
            } while (a > 0 && a <= occurrences.size());

            std::cout << "Corrections finished" << std::endl;
            adjustToBlock(m_parsedContent, m_SizeOfBlock);

            goto selection;
        }
        case 6: {
            std::cout << "\n 1: Encrypt \n 2: Decrypt \n";
            int EChoice; std::cin >> EChoice;
            switch (EChoice) {
                case 1: {
                    encryptContent();
                    std::cout << "\nContent is now encrypted once: \n";
                    goto selection;
                }
                case 2: {
                    decryptContent();
                    std::cout << "\nContent is now decrypted once: \n";
                    goto selection;
                }
                default:
                    goto selection;
            }
        }
        case 7:
            exit(0);
        default:
            break;
        }
    goto selection;

}
/*
std::vector<std::pair<uint64_t, uint64_t>> TEXT_F::findWord(const std::string &word)
{
    std::vector<std::pair<uint64_t, uint64_t>> positions;
    std::pair<uint64_t, uint64_t> position;
    for (uint64_t i = 0; i < m_parsedContent.size(); ++i)
    {
        for (uint64_t j = 0; j < m_parsedContent[i].size(); ++j)
        {
            if (m_parsedContent[i][j] == word)
            {
                position = {i, j};
                positions.push_back(position);
            }
        }
    }
    return positions;
}
*/
void TEXT_F::fillDictionary()
{
    m_dict = {};
    std::vector<char> nonChars {',', '.', '\"', ':', ';', '!', '?'};
    for (unsigned int i = 0; i < m_parsedContent.size(); ++i)
        for (unsigned int j = 0; j < m_parsedContent[i].size(); ++j) {
            auto aux = m_parsedContent[i][j];
            for (auto c : nonChars)
                aux.erase(std::remove(aux.begin(), aux.end(), c), aux.end());
            m_dict.entrySet(aux, {i, j});
        }
}

std::vector<Coordinate<unsigned int>> TEXT_F::getOccurrences(const std::string& word) {
    for (const auto& pair : m_dict.m_entries) {
        if (pair.first == word) return pair.second;
    }
    return std::vector<Coordinate<unsigned int>>();
}

void TEXT_F::encryptContent() {
    for (auto & line : m_parsedContent) {
        for (auto & word : line) {
            for(int i = 0; (i < 100 && word[i] != '\0'); i++)
                word[i] = word[i] + 67;
        }
    }
}

void TEXT_F::decryptContent() {
    for (auto & line : m_parsedContent) {
        for (auto & word : line) {
            for(int i = 0; (i < 100 && word[i] != '\0'); i++)
                word[i] = word[i] - 67;
        }
    }
}


