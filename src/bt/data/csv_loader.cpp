#include "bt/data/csv_loader.hpp"
#include "bt/data/parse.hpp"
#include <stdexcept>

namespace bt::data{

    // Constructor is here
    CsvLoader::CsvLoader(std::string path, SymbolTable& symtab, Config cfg)
        : path_(std::move(path)), symtab_(symtab), cfg_(cfg), fin_(path_){
            if (!fin_.is_open()){
                throw std::runtime_error("Failed to open file: " + path_);
            }
        }

    // 读取某一行
    std::vector<std::string_view> CsvLoader::split_csv_line(std::string &line){
        std::vector<std::string_view> out;
        out.reserve(16);  //一次性分配16个，避免多次扩容

        size_t start = 0; //size_t是一个不定长的int型数据
        for (size_t i=0;i<=line.size();++i){
            if (i == line.size() || line[i] == ','){
                out.emplace_back(line.data()+start,i-start);
                start = i+1;
            }
        }
        return out;
    }

    //递归式地获取下一行数据
    std::optional<Event> CsvLoader::next(){
        while (std::getline(fin_,line_)){
            if (!header_skipped_ && cfg_.has_header) {
                header_skipped_ = true;
                continue; // 跳过 header
            }
            if (line_.empty()) continue;
            auto cols = split_csv_line(line_);
            if (cols.size()<11) continue;

            Timestamp ts = static_cast<Timestamp>(parse_i64(cols[0]));
            SymbolId sym = symtab_.get_or_add(cols[2]);
            char et = cols[3].empty() ? '\0' : cols[3][0];
            Event ev;

            if (et == 'Q'){
                ev.type = EventType::Quote;
                ev.q.ts_ns = ts;
                ev.q.sym = sym;
                ev.q.bid_px = parse_price_ticks(cols[4], cfg_.tick_size);
                ev.q.ask_px = parse_price_ticks(cols[5], cfg_.tick_size);
                ev.q.bid_sz = parse_qty_lots(cols[6]);
                ev.q.ask_sz = parse_qty_lots(cols[7]);
                return ev;
            }

            if (et == 'T'){
                ev.type = EventType::Trade;
                ev.t.ts_ns = ts;
                ev.t.sym = sym;
                ev.t.trade_px = parse_price_ticks(cols[8], cfg_.tick_size);
                ev.t.trade_sz = parse_qty_lots(cols[9]);
                ev.t.side = parse_side(cols[10]);
                return ev;
            }
        }
        return std::nullopt;
    }

}