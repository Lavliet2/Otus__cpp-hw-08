#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <mutex>
#include <chrono>
#include <filesystem>

std::unordered_map<std::string, int> wordCountMap;
std::mutex wordCountMutex;

void countWords(const std::string &filename, int start, int end)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть единый файл " << filename << std::endl;
        return;
    }

    std::string word;
    file.seekg(start);

    for (int i = start; i < end; ++i) {
        char c = file.get();
        if (isalpha(c)) {
            word += std::tolower(c);
        } else if (!word.empty()) {
            std::lock_guard<std::mutex> lock(wordCountMutex);
            wordCountMap[word]++;
            word.clear();
        }
    }

    file.close();
}

std::string newFile(const std::vector<std::string> &fileNames)
{
    std::string outputFile = "C:\\temp\\output.txt";

    std::ofstream output(outputFile);
    if ( ! output.is_open() ) {
        std::cerr << "Не удалось создать единый файл: " << outputFile << std::endl;
        return "";
    }

    for ( const std::string &inputFile : fileNames ) {
        std::ifstream input(inputFile);
        if ( ! input.is_open() ) {
            std::cerr << "Файл не удалось открыть файл: " << inputFile << std::endl;
            continue;
        }

        output << "Contents of " << inputFile << ":\n";

        std::string line;
        while ( std::getline(input, line) ) {
            output << line << "\n";
        }

        output << "\n";
        input.close();
    }
    output.close();
    return outputFile;
}

int main()
{

    // Введите пути к фалам
    std::vector<std::string> fileNames;
    fileNames.push_back("C:\\temp\\1.txt");
    fileNames.push_back("C:\\temp\\1.txt");
    fileNames.push_back("C:\\temp\\1.txt");
    fileNames.push_back("C:\\temp\\2.txt");
    fileNames.push_back("C:\\temp\\1.txt");
    fileNames.push_back("C:\\temp\\1.txt");

    std::string filename = newFile(fileNames);

    const int numThreads = 4; // Количество потоков


    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file(filename, std::ios::ate);
    if ( ! file.is_open() ) {
        std::cerr << "Не удалось открыть единый фалйл: " << filename << std::endl;
        return 1;
    }

    std::streampos fileSize = file.tellg();
    file.close();

    std::vector<std::thread> threads;
    std::vector<std::streampos> positions(numThreads + 1);

    for ( int i = 0; i <= numThreads; ++i ) {
        positions[i] = (fileSize / numThreads) * i;
    }

    for ( int i = 0; i < numThreads; ++i ) {
        threads.emplace_back(countWords, filename, positions[i], positions[i + 1]);
    }

    for ( auto &thread : threads ) {
        thread.join();
    }

    // Сортировка и вывод результатов
    std::vector<std::pair<std::string, int>> sortedWords(wordCountMap.begin(), wordCountMap.end());
    std::sort(sortedWords.begin(), sortedWords.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });


    for ( int i = 0; i <=10; i++ ) {
        std::cout << sortedWords[i].first << ": " << sortedWords[i].second << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::filesystem::remove(filename);


    std::cout << "Время выполнения: " << duration.count() << " микросекунд" << std::endl;
    return 0;
}
