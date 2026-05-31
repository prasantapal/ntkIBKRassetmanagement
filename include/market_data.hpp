#ifndef MARKET_DATA_HPP
#define MARKET_DATA_HPP
#include <string>
#include <iostream>
enum ASSET_CATAGORIES {
  STOCKS_ALL,
  ETFS_ALL
};

class MarketData {
  private:
    std::string eft_file_name_;
    std::string stocks_file_name_;
    std::vector<std::string> list_of_stocks_;
    std::vector<std::string> list_of_etfs_;
    static std::string trade_data_home_;
    static std::string_view constexpr path_separator_default_ = {"/"};
    std::string path_separator_;
    short int header_skip_lines_;
    static short int constexpr header_skip_lines_default_ = {1};

  public:
    decltype(auto) get_asset_list(int asset_category) const;
    std::vector<std::string> load_asset_file(std::string& filename, short int skip_header_lines = 0);
    void print_asset(std::vector<std::string>&);
    void load_stocks();
    void load_etfs();
    void ctor_helper();
    MarketData();
    void dtor_helper();
    ~MarketData();

};

std::string MarketData::trade_data_home_= {""};
void MarketData::dtor_helper() {

}

void MarketData::ctor_helper() {
  header_skip_lines_= header_skip_lines_default_;
  path_separator_ = std::string(path_separator_default_);
  trade_data_home_ = std::string(std::getenv("NEUTHEOS_TRADE_DATA_HOME"));
  eft_file_name_ = {"etfs_all_normalized.csv"};
  stocks_file_name_ = {"stocks_all_US.csv"};


  fmt::print(fg(fmt::color::orange),"{}","loading stocks\n");
  load_stocks();
  fmt::print(fg(fmt::color::tan),"{}","loaded stocks\n");
//  fmt::print(fg(fmt::color::green),"{}","printing stocks\n");
//  fmt::print(fg(fmt::color::gray),"{}\n",list_of_stocks_);
//  getchar();

  fmt::print(fg(fmt::color::orange),"{}","loading ETFS\n");
  load_etfs();
  fmt::print(fg(fmt::color::tan),"{}","loaded ETFS\n");
 // fmt::print(fg(fmt::color::green),"{}\n","printing etfs");
 // fmt::print(fg(fmt::color::silver),"{}\n",list_of_etfs_);
 // getchar();

}

std::vector<std::string> MarketData::load_asset_file(std::string& asset_filename, short int skip_header_lines) {
  std::vector<std::string> list_of_assets;
  std::ifstream file;
  std::string filename = trade_data_home_ + path_separator_ + asset_filename;

  file.open(filename.c_str());
  std::string line;
  //  short int skip_lines = {1};
  if(file.good()) {
    int line_no = {0};

    do {
      std::getline(file,line);
      if(!file.eof() && !line.empty()) {
        ++line_no;
        if(line_no > skip_header_lines) {
          //      std::cout << line << std::endl;
          std::vector<std::string> split_line = ::tokenize(line,',');
          //     std::cout << split_line.at(0) << std::endl;
          auto& stock = split_line.at(0);
          list_of_assets.push_back(stock);
        }
      }

    }while(!file.eof());
  }

  file.close();


  return list_of_assets;

}

void MarketData::print_asset(std::vector<std::string>& assets) {

  std::copy(assets.begin(),assets.end(), std::ostream_iterator<std::string>(std::cout," "));
}

void MarketData::load_stocks() {
  list_of_stocks_ = load_asset_file(stocks_file_name_, header_skip_lines_);
}
void MarketData::load_etfs(){
  list_of_etfs_ = load_asset_file(eft_file_name_,header_skip_lines_);
}

MarketData::MarketData() {
  ctor_helper();

}
MarketData::~MarketData() {
  dtor_helper();

}



decltype(auto) MarketData::get_asset_list(int asset_category) const {
  std::vector<std::string> assets;
  switch(asset_category) {
    case ASSET_CATAGORIES::STOCKS_ALL: {
                                          assets = list_of_stocks_; 
                                          break;

                                        }
    case ASSET_CATAGORIES::ETFS_ALL:{
                                          assets = list_of_etfs_; 

                                    }


  }
  return assets;

}

#endif
