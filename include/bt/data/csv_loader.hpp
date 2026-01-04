#pragma once
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include "bt/data/schema.hpp"
#include "bt/data/symbol_table.hpp"

namespace bt::data{

    class CsvLoader{
        public:
            struct Config{
                double tick_size = 0.01;
                bool has_header = true; //csv是否有表头
            };
            CsvLoader(std::string path, SymbolTable & symtab, Config cfg);
            std::optional<Event> next();

        private:
            std::string path_;
            SymbolTable& symtab_;
            Config cfg_;

            std::ifstream fin_;
            bool header_skipped_ = false;

            static std::vector<std::string_view> split_csv_line(std::string& line);
            std::string line_;


    };

}