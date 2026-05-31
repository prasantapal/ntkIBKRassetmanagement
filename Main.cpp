#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/ranges.h>
#include <numeric>
#include <execution>
#include <vector>
#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>

#include <cxxopts.hpp>
#include <chrono>
#include <thread>
#include <cstdlib>
#include "TestCppClient.h"
#include "OrderSamples.h"
#include "headers.hpp"
#include "vanilla_header.hpp"
#include "tmux_management.hpp"
#include "market_data.hpp"

const unsigned MAX_ATTEMPTS = 50;
const unsigned SLEEP_TIME = 10;


/* IMPORTANT: always use your paper trading account. The code below will submit orders as part of the demonstration. */
/* IB will not be responsible for accidental executions on your live account. */
/* Any stock or option symbols displayed are for illustrative purposes only and are not intended to portray a recommendation. */
/* Before contacting our API support team please refer to the available documentation. */
int main(int argc, char** argv) {


TmuxManagement tmux_management;

std::string msg = "SENDIGN MSG!";
tmux_management.send_msg_to_last_ttys(std::move(msg));




  MarketData market_data;

  int asset_category = ASSET_CATAGORIES::ETFS_ALL;

  std::vector<std::string> assets =  market_data.get_asset_list(asset_category);
  fmt::print(fg(fmt::color::green), "{}\n",assets);
  srand(static_cast<unsigned int>(time(NULL)));

  int random_etf_index = rand()%assets.size();
  asset_category = ASSET_CATAGORIES::STOCKS_ALL;
  assets =  market_data.get_asset_list(asset_category);
  std::string random_etf = assets.at(random_etf_index);


  srand(static_cast<unsigned int>(time(NULL)));

  int random_stock_index = rand()%assets.size();
  std::string random_stock = assets.at(random_stock_index);
  fmt::print(fg(fmt::color::purple), "{}\n",assets);

  std::transform(random_etf.begin(), random_etf.end(), random_etf.begin(), [](unsigned char c) {
      return std::toupper(c);
      });



  std::transform(random_stock.begin(), random_stock.end(), random_stock.begin(), [](unsigned char c) {
      return std::toupper(c);
      });



  fmt::print(fg(fmt::color::red),"{}\n",random_etf);

  fmt::print(fg(fmt::color::red),"{}\n",random_stock);


  // 1. Fetch the environment variable by name
  const char* env_p = std::getenv("NEUTHEOS_TRADE_DATA_HOME");

  // 2. Always check for nullptr before using the result
  if (env_p != nullptr) {
    std::cout << "Your PATH is: " << env_p << std::endl;
  } else {
    std::cout << "The requested environment variable does not exist." << std::endl;
  }





  cxxopts::Options options("IBKR_Neutheos", "IBKR-Neutheos interface");
  options.allow_unrecognised_options();
  options.add_options()
    ("d,debug", "Enable debugging") // a bool parameter
    ("c,clientID", "write code", cxxopts::value<int>()->default_value("0"))
    ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
    ;

  auto result = options.parse(argc, argv);

  int clientId = {0};
  std::cout << "parsing" << std::endl;
  clientId = result["c"].as<int>();
  std::cout << "clientId:" << clientId << std::endl;



  const char* host = "";
  //const char* host = argc > 1 ? argv[1] : "";
  int port = argc > 2 ? atoi(argv[2]) : 0;
  int IBKR_GATEWAY_PORT = {4002};
  //if (port <= 0)
  port = IBKR_GATEWAY_PORT;
  const char* connectOptions = "+PACEAPI";
  // const char* connectOptions = argc > 3 ? argv[3] : "+PACEAPI";

  unsigned attempt = 0;
  printf( "Start of C++ Socket Client Test %u\n", attempt);


  Contract contract;
  contract.symbol = "AAPL";
  contract.secType = "STK";
  contract.exchange = "SMART";
  contract.currency = "USD";

  Order order;
  order.action = "BUY";
  order.orderType = "LMT";
  order.totalQuantity = 100;
  order.lmtPrice = 150.00;
  order.tif = "DAY"; // Time in Force
                     //
  int m_orderId = {0};

  for (;;) {
    ++attempt;
    printf( "Attempt %u of %u\n", attempt, MAX_ATTEMPTS);

    TestCppClient client;

    // Run time error will occur (here) if TestCppClient.exe is compiled in debug mode but TwsSocketClient.dll is compiled in Release mode
    // TwsSocketClient.dll (in Release Mode) is copied by API installer into SysWOW64 folder within Windows directory 

    if( connectOptions) {
      client.setConnectOptions( connectOptions);
    }

    std::cout << "connecting..." << std::endl;
    client.connect( host, port, clientId);
    if(client.isConnected()) {
      std::cout << "client is connected with status " << client.isConnected() << std::endl;
    }else {

      std::cout << "client is NOT connected" << std::endl;
    }


    int napping_time = {3};
    std::cout << "doing my fav thing...napping for " <<  napping_time << " secs before starting work!"<< std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(napping_time));
    std::cout << "done doing my fav thing...napping for " << napping_time << " secs" << std::endl;
    while( client.isConnected()) {

      //   std::cout << "order.account:" << order.account << std::endl;

      //    std::cout << "historical data requests" << std::endl;
      //     client.historicalDataRequests();
      //
      //    std::cout << "processing messages:" << std::endl;
      //
      //    client.processMessages();
      //    std::cout << "performing  order Operations:" << std::endl;
      //
      //    client.accountOperations();

      //      client.getPositions();
      //client.reqAccn
      std::cout << "would you like to place order?y/n:";
      char c = getchar();
      if(c == 'y') {

        

        std::cout << "ticker:";
        std::string ticker;
        std::cin >> ticker;
        std::cout << "ticker-" << ticker << std::endl;

        std::cout << "buy/sell? b/s:";
        char c;
        std::cin >> c;
        int order_action_type = ORDER_ACTION_TYPE::UNDEFINED_ORDER_ACTION;

        if(c== 'b') {

          order_action_type = ORDER_ACTION_TYPE::BUY;
        }else if (c == 's'){

          order_action_type = ORDER_ACTION_TYPE::SELL;
        }else {
        }







        int quantity = 0;
        std::cout << "quantity:"; 
        std::cin >> quantity;
        std::cout << "quantity-" << quantity << std::endl;

        double limit_price = {0};
        std::cout << "limit_price:";
        std::cin >> limit_price;
        std::cout << "limit_price-" << limit_price << std::endl;

        std::string order_type_str = {""};
        switch(order_action_type){
          case ORDER_ACTION_TYPE::BUY:{
                                  order_type_str =  std::string("BUY");

                                  break;
                                }

          case ORDER_ACTION_TYPE::SELL:{

                                   order_type_str =  std::string("SELL");

                                   break;

                                 }

        }

        int order_type = ORDER_TYPES::UNDEFINED_ORDER_TYPES;


        client.placeOrderCustom(order_type, ticker, limit_price, quantity, order_type);

        fmt::print( fg(fmt::color::red),"placing {:>10} order:{} QTY:{} LMT:{}\n",order_type_str, ticker,limit_price,quantity);

        std::cout << "are you ready to place the order?y/n:";
        char answer = 'n';
        std::cin >> answer;

        if(answer == 'y') {

          fmt::print(fg(fmt::color::orange),"placing order..");

        }else if(answer == 'n') {

          std::cout << "not placing order" << std::endl;
        }else {
          std::cout << "not placing order" << std::endl;
        }


        //        client.placeOrder();
        std::cout << "done placing order" << std::endl;
      }else {

        std::cout << "didn't place order" << std::endl;
      }

      //    std::cout << "performing order Operations:" << std::endl;
      //    client.orderOperations();
      //    std::cout << "done performing order Operations:" << std::endl;
      //    std::cout << "done performing orderOperations:" << std::endl;
      //
      //  //  client.getPositions();
      //  //  std::cout << "done getPositions:" << std::endl;
      std::cout << "taking a nap...press enter to wake up" << std::endl;
      getchar();

    }
    if( attempt >= MAX_ATTEMPTS) {
      break;
    }

    printf( "Sleeping %u seconds before next attempt\n", SLEEP_TIME);
    std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
  }

  printf ( "End of C++ Socket Client Test\n");
}



