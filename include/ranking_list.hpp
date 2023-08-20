#pragma once

#include "pch.hpp"

class RankingList {
   public:
    class RankInfo {
       public:
        std::string id;
        std::string score;

        bool operator<(const RankInfo& other) const {
            return score < other.score;
        }

        std::string ToString() const {
            int spaces = 18 - id.length() - score.length();
            std::string result = std::string(id);
            for (int i = 0; i < spaces; i++) {
                result += " ";
            }
            result += score;
            return result;
        }
    };

    RankingList() { getRankingListFromFile(); }

    void add(const std::string& id, const int& score) {
        ranks_.Push(RankInfo{id, std::to_string(score)});
    }

    std::string ToString() const {
        std::string result = "排行榜：\n";
        for (auto& rank : ranks_) {
            result += rank.ToString() + "\n";
        }
        return result;
    }

    void WriteToFile() {
        // 写入文本文件，如果文件不存在则创建它
        std::ofstream outFile("ranking_list.txt");
        if (outFile.is_open()) {
            for (auto& rank : ranks_) {
                outFile << rank.id << std::endl;
                outFile << rank.score << std::endl;
            }
            outFile.close();
            std::cout << "File written successfully." << std::endl;
        } else {
            std::cerr << "Failed to open file for writing." << std::endl;
        }
    }

   private:
    struct CompareGreater {
        bool operator()(const RankInfo& lhs, const RankInfo& rhs) const {
            return !(lhs < rhs);  // 降序排列
        }
    };
    FixedSizeMultiset<RankInfo, 20, CompareGreater> ranks_;

    void getRankingListFromFile() {
        std::ifstream inFile("ranking_list.txt");
        if (inFile.is_open()) {
            std::string id;
            std::string score;
            while (std::getline(inFile, id) && std::getline(inFile, score)) {
                ranks_.Push({id, score});
            }
            inFile.close();
        } else {
            std::cerr << "Failed to open file for reading." << std::endl;
        }
    }
};