#ifndef INCOMING_FIFO_MANAGEMENT_HPP
#define INCOMING_FIFO_MANAGEMENT_HPP
///////////////////////////////////////////////////////////////////////////////
#include <read_json.hpp>
#include <thread>
#include <chrono>
#include <regex>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/circular_buffer.hpp>
enum IncomingDataTypes {flag_type, value_type};
////////////////////////////////////////////////////////////////////////////
struct IncomingFifo: StateController {
  IncomingFifo();
  ~IncomingFifo();
  void ctor_helpers();
  std::unique_ptr<std::jthread> listner_thread_;

  std::string asset_;
  std::vector<boost::circular_buffer<float>> asset_buffers_;
  std::vector<int> asset_buffers_times_scales_;
  ///////////////////////////////////////////////////////////////////////////
  template<typename T>
    void start_listening_to_fifo(T);
  void stop_listening_to_fifo();
  std::mutex incoming_data_update_mutex_;
  static std::atomic<bool> has_listening_ended_;
  static std::atomic<bool> constexpr has_listening_ended_default_ = {false};
  template<typename T>
    void init_incoming_flags_size(T);
  std::unique_ptr<std::vector<bool>> incoming_flags_;
  std::unique_ptr<std::vector<int>> incoming_numbers_;
  std::unique_ptr<std::vector<float>> incoming_values_;
  std::unique_ptr<std::vector<bool>> incoming_values_data_indices_;
  template<typename T>
    void init_incoming_values_size(T);
  unsigned int incoming_data_type_;
  template<typename T>
    void init(T);
  template<typename T>
    void set_n_channels(T);
  unsigned short int  nchannels_;
  int count_number_of_flags() const;

  const std::string statement_pre_start_match = ".*";
  const std::string statement_start_match = "%%%";
  const std::string statement_end_match = "@@@";
  const std::string statement_post_end_match = ".*$";
  const std::string capture_left = "(";
  const std::string capture_right = ")";
  const std::string statement_any_mid_match = ".*";
  const std::string values_tag = {"values"};
  const std::string outliers_tag = {"outliers"};
  const std::string is_sentinel_tag = {"is_sentinel"};
  const std::string empty_string = {""};
  const std::string space_separator = {" "};
  const std::string word_sentinel = {"%END@"}; // Read this externally

};

void IncomingFifo::ctor_helpers() {
  int time_scale = {5};
  asset_buffers_times_scales_.push_back(time_scale);
  time_scale = 10;

  asset_buffers_times_scales_.push_back(time_scale);

  time_scale = 25;
  asset_buffers_times_scales_.push_back(time_scale);

  time_scale = 50;
  asset_buffers_times_scales_.push_back(time_scale);


  asset_buffers_.resize(asset_buffers_times_scales_.size());
  int i = {0};
  for(auto& it:asset_buffers_) {
    int time_scale = asset_buffers_times_scales_.at(i);
    std::cout << time_scale << " ";
    it.set_capacity(time_scale);
    ++i;
  }
  std::cout << std::endl;


}
int IncomingFifo::count_number_of_flags() const{
  /// count the  number of boolean flags

}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
template<typename T>
void IncomingFifo::set_n_channels(T nchannels) {
  nchannels_ = nchannels;
}

/////////////////////////////////////////////////////////////
template<typename T>
void IncomingFifo::init(T incoming_data_type) {
  incoming_data_type_ = incoming_data_type;
}

/////////////////////////////////////////////////////////////
template<typename T>
void IncomingFifo::init_incoming_flags_size(T flag_size){
  incoming_flags_.reset(new std::vector<bool>(flag_size, false));
}
/////////////////////////////////////////////////////////////
template<typename T>
void IncomingFifo::init_incoming_values_size(T values_size) {
  incoming_values_.reset(new std::vector<float>(values_size,std::numeric_limits<float>::lowest() ));

  incoming_values_data_indices_.reset(new std::vector<bool>(values_size, false));
}

/////////////////////////////////////////////////////////////
std::atomic<bool> IncomingFifo::has_listening_ended_(has_listening_ended_default_.load());
void IncomingFifo::stop_listening_to_fifo() {
  has_listening_ended_ = {true};
  listner_thread_->join();
}


template<typename T>
void IncomingFifo::start_listening_to_fifo(T fifo_incoming_curator_clamp_complete_path) {

#ifdef IS_DEBUG
  std::cout << "incoming fifo path:" << fifo_incoming_curator_clamp_complete_path << std::endl;
#endif

  auto fifo_listner_function = [this, &data_ready, &has_listening_ended_, fifo_incoming_curator_clamp_complete_path]()->void{
    std::cout << "starting fifo listener fuction" << std::endl;
    /// Note: Occassionally a clogging life effect is observed in the fifo-pipes.
    const std::string match_string = {statement_pre_start_match + statement_start_match + capture_left + statement_any_mid_match + capture_right + statement_end_match + statement_post_end_match};
    const std::regex match_regex = {std::regex(match_string)};
    const std::regex match_regex_start = {std::regex(statement_pre_start_match + statement_start_match + statement_pre_start_match)};
    const std::regex match_regex_end = {std::regex(statement_pre_start_match + statement_end_match + statement_pre_start_match)};
    std::string input_string_raw = {empty_string};

    std::cout << "regex initialization done" << std::endl;
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%🕰🕰🕰🕰🕰🕰🕰🕰🕰🕰🕰🕰%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%🕰🕰🕰🕰🕰🕰🕰🕰🕰🕰🕰🕰%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    while(!has_listening_ended_){

      auto open_result = open(fifo_incoming_curator_clamp_complete_path.c_str(), O_RDONLY);
      auto src_open = boost::iostreams::file_descriptor_source(open_result,  boost::iostreams::file_descriptor_flags::close_handle);
      boost::iostreams::stream<boost::iostreams::file_descriptor_source> fifo( src_open);

      std::string word;
      std::smatch base_match;
      std::string base = {empty_string};
      std::smatch base_match_start;
      std::smatch base_match_end;
      bool is_base_match_start = {false};
      bool is_base_match_end = {false};
      bool is_structural_match = {false};
      bool is_start = {true};
      bool intermediate_exit = {false};
      ///////////////////////////////////////////////
      ///////////////////////////////////////////////
      while ((fifo >> word) && !intermediate_exit) { // keep reading until things exhaust



        std::cout  << word << std::endl;                                                     
        if(word.compare(word_sentinel) == 0) {
          has_listening_ended_ = true;
          break;


        }
        //
        //        std::stringstream ss;
        //        ss << word;
        ////         float value;
        //         ss >> value;
        // 
        // for(int i=0; i<asset_buffers_times_scales_.size();++i) {
        //   asset_buffers_[i].push_back(value);
        // }

        // for(int i=0; i<asset_buffers_times_scales_.size();++i) {
        //   for(int j=0;j<asset_buffers_[i].size();++j) {
        //     float value = asset_buffers_[i].at(j);
        //     std::cout << value << " ";

        //   }
        //   std::cout << std::endl;

        // }
        // std::cout << std::endl;
        // std::cout << std::endl;

        // std::this_thread::sleep_for(std::chrono::milliseconds(2);


        if(is_start) {
          is_start = false;
        }else {
          input_string_raw += space_separator;
        }
        input_string_raw += word;
        std::string base_string = {empty_string};
        ////////////////////////////////////////////////////////////////
        // std::cout << "regex match start" << std::endl;
        if(std::regex_match(word, base_match_start, match_regex_start)){
          is_base_match_start = {true};
        }
        // std::cout << "regex match end" << std::endl;

        if(std::regex_match(word, base_match_end, match_regex_end)){
          is_base_match_end = {true};

        }
        if(is_base_match_start && is_base_match_end) {
          is_structural_match = {true};
        }




        if(is_structural_match) { // this means the basic structure is found
                                  // std::cout << "input_string_raw:" << input_string_raw << std::endl;
          is_structural_match = false;
          auto first_match = std::move(input_string_raw.find(statement_start_match));
          auto end_match = std::move(input_string_raw.find(statement_end_match));
          int start_string_index = std::move(first_match + statement_start_match.size());
          //TBD ///double check this particularly around -1 index
          int end_string_index = std::move(end_match - statement_end_match.size());

          if(end_string_index >= start_string_index ) {
            base = input_string_raw.substr(start_string_index,end_string_index);
            std::cout << "base:"<<  base << std::endl;
            auto json = string2json(base);


            auto converted_json = std::get<1>(json);
            if(std::get<0>(json)) {
              std::lock_guard<std::mutex> lock(mtx);
              {  
                data_ready = true;
                json_FIFO = std::move(converted_json);
              }
              // 2. Notify the waiting thread
              std::cout << "notifying..."  << data_ready << std::endl;
              cv.notify_one();

            }else {
              std::cout << "INVALID json:" << converted_json << std::endl;

            }

            //  // if(converted_json[is_sentinel_tag].asBool()) {

            //  //   //    std::cout << "sentinel reached" << std::endl;
            //  // }else { // fetch data
            //    switch(incoming_data_type_) {
            //      case IncomingDataTypes::flag_type: {
            //                                           const std::string outliers = converted_json[outliers_tag].asString();
            //                                           std::stringstream ss;
            //                                           int flag_index;
            //                                           do{
            //                                             ss >> flag_index;
            //                                             incoming_flags_->at(flag_index) =  true;

            //                                           }while(!ss.eof());
            //                                         }
            //      case IncomingDataTypes::value_type: {
            //                                            const std::string values = converted_json[values_tag].asString();
            //                                            std::stringstream ss(values.c_str());
            //                                            std::string curated_values;
            //                                            int data_index;
            //                                            float data_value;
            //                                            do{
            //                                              ss >> data_index;
            //                                              ss >> data_value;
            //                                              // std::cout << "data_index:" << data_index << " data_value:" << data_value << std::endl;
            //                                              incoming_values_->at(data_index) = data_value; // update the value
            //                                              incoming_values_data_indices_->at(data_index) = true;
            //                                              //incoming_flags_->at(flag_index) =  true;

            //                                            }while(!ss.eof());
            //                                          }
            //    } // data type


          }

          input_string_raw = {""};
          }

          // is_structural_match = false;
          // is_base_match_start = false;
          // is_base_match_end = false;

          /////////////////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////////////////
          /////////////////////////MATCH HANDLER///////////////////////////////
          /////////////////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////////////////



          /////////////////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////////////////
          /////////////////////////MATCH HANDLER ENDS///////////////////////////////
          /////////////////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////////////////




          //}//else {
          // std::cout << "string index match failure " << std::endl;
          // std::cout << "program quit" << std::endl;
          // throw ("string index match");
          // }
          //}

    } // keep listening to threads


  } // sentinel for ending the loop
  std::cout << "listening has ended...program quit!" << std::endl;


}; // function ends


listner_thread_.reset(new std::jthread(fifo_listner_function));
}

IncomingFifo::IncomingFifo() {
  std::cerr << "incoming fifo" << std::endl;
  ctor_helpers();
}

IncomingFifo::~IncomingFifo() {
#ifdef IS_DEBUG
  std::cout << "listening thread ended" << std::endl;
#endif
  has_listening_ended_ = false;
  //TBD time join the following thread

  if(listner_thread_.get() != nullptr && listner_thread_->joinable()) { /// Make it timed
    listner_thread_->join();
  }

}

#endif
