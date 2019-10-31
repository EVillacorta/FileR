#include <iostream>
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

void TEXT_F::setInputFile(const std::string &inputPath) {
    if (m_inputFile.is_open())
        m_inputFile.close();
    m_inputPath = inputPath;
    m_inputFile.open(m_inputPath);
}
void TEXT_F::setOutputFile(const std::string &outputPath)  {
    if (m_outputFile.is_open()) {
        m_outputFile.close();
    }
    m_outputPath = outputPath;
    m_outputFile.open(m_outputPath);
}
void TEXT_F::readFile() {
    if (! m_inputFile.is_open() ) return;
    std::vector<std::string> lines;
    for (std::string line; getline( m_inputFile, line );)
        lines.push_back(line);
    m_fileContent = lines;
}
void TEXT_F::showContent() {
    for (const auto& line : m_fileContent)
        std::cout << line << '\n';
}
Text TEXT_F::getContent() {
    return m_fileContent;
}

void TEXT_F::fixContent(size_t SizeofBlock, int fixType) {
    if (m_parsedContent.empty()) {
        m_parsedContent = parseText(m_fileContent);
    }
    m_parsedContent = adjustToBlock(m_parsedContent, SizeofBlock);
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

void TEXT_F::saveContent() {
    std::ofstream temp(m_outputPath);
    for (const auto& lines : m_fileContent) {
        temp << lines << std::endl;
    }
}

void TEXT_F::ShowMenu() {

    selection:
    std::cout << ":::::MENU::::\n 1: open files \n 2: save files \n 3: Adjustment \n 4: Search for word \n 5: Encrypt - Decrypt file \n 6: Exit \n";
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
        case 3:
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
        case 4:
            break;
        case 5:
            break;
        case 6:
            exit(0);
        default:
            break;
        }
    goto selection;

}

std::vector<std::pair<uint64_t, uint64_t>> TEXT_F::findWord(const std::string &word) {
    std::vector<std::pair<uint64_t, uint64_t>> positions;
    std::pair<uint64_t, uint64_t> position;
    for (uint64_t i = 0; i < m_parsedContent.size(); ++i) {
        for (uint64_t j = 0; j < m_parsedContent[i].size(); ++j) {
            if (m_parsedContent[i][j] == word) {
                position = {i, j};
                positions.push_back(position);
            }
        }
    }
    return positions;
}





