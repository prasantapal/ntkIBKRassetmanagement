/// @brief read_json 
/// @tparam T
/// @param filename
/// @returns   


#ifndef READ_JSON_HPP
#define READ_JSON_HPP
#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json/json.h>
#define READ_JSON_HPP
/// @brief read_json 
/// @tparam T
/// @param filename
/// @returns   

template<typename T>
decltype(auto) read_json_with_status(const T& filename ){
  Json::Value root;    
  Json::Reader json_reader;
  bool is_reading_successful = {false};
  if(std::filesystem::exists(filename.c_str())){
    std::ifstream file(filename.c_str());
    if(file.good()) {
      if(!json_reader.parse(file, root, true)){
        //for some reason if it fails to parse  
        std::cerr << "failed to read JSON file " << json_reader.getFormattedErrorMessages() << std::endl;

      }else {
        std::cerr << "good json" << std::endl; }
      is_reading_successful = true;

      file.close();
    }else {

      std::cout << "sorry file " << filename << " is not good!" << std::endl;
    }

  }else{

    std::cout << "sorry file " << filename << " does not exist" << std::endl;
  }
  return std::tuple<bool, Json::Value>(is_reading_successful,root);
}

template<typename T>
Json::Value read_json(const T& filename ){
  Json::Value root;    
  Json::Reader json_reader;
  std::cout << "reading json " << filename << std::endl;
  bool is_reading_successful = {false};
  if(std::filesystem::exists(filename.c_str())){
    std::ifstream file(filename.c_str());
    if(file.good()) {
      if(!json_reader.parse(file, root, true)){
        //for some reason if it fails to parse  
        std::cerr << "failed to read JSON file " << json_reader.getFormattedErrorMessages() << std::endl;

      }else {
        std::cerr << "good json" << std::endl; }
      is_reading_successful = true;

      file.close();
    }else {

      std::cout << "sorry file " << filename << " is not good!" << std::endl;
    }

  }else{

    std::cout << "sorry file " << filename << " does not exist" << std::endl;
  }
  return root;
}

/// @brief read_json 
/// @tparam T
/// @param filename
/// @returns   

template<typename T>
std::tuple<bool,std::string> json2string(T json){

  Json::StreamWriterBuilder builder;
  builder["indentation"] = ""; // If you want whitespace-less output
  bool is_successful = {false};
  std::string state_string = {""};
  try{
    state_string = Json::writeString(builder, json);
    is_successful = true;
#ifdef IS_DEBUG
    std::cout <<"conversion success" <<std::endl;
#endif

  }catch(std::exception& e){
    std::cerr <<  "caught exception:"  << e.what() <<std::endl;
  }catch(...){

    std::cerr <<  "caught exception of unknown type" << std::endl;
  }
  std::tuple<bool,std::string> json_string(is_successful,state_string);
  return json_string;
}

template<typename T>
std::tuple<bool, Json::Value> string2json(T json_str){

  Json::Value root;

  Json::Reader reader;
  bool isParsingSuccessful = {false};
  isParsingSuccessful = reader.parse(json_str.c_str(), root); // parse process
                                                              //
  if (!isParsingSuccessful) {
    std::cout << "Failed to parse" << reader.getFormattedErrorMessages();
  } else {

    //    std::cout << "root json:" << root << std::endl;
  }
  return std::tuple<bool,Json::Value>(isParsingSuccessful,root);;
}

/// @brief read_json 
/// @tparam T
/// @param filename
/// @returns   
template<typename T, typename M>
bool json2file(M json, T filename ){

  bool is_success = {false};
  if(std::filesystem::exists(filename.c_str())){
    std::cerr << "sorry file " << filename << " already exists" << std::endl;
    bool is_success = {false};
  }else {
    std::ofstream file_id;
    file_id.open(filename.c_str());
    if(file_id.good()) {
      Json::StyledWriter styledWriter;
      file_id << styledWriter.write(json);
      file_id.close();
      is_success = {true};
    }else {
      is_success = false;
    }
  }
  return is_success;
}
#endif
