#define FMT_HEADER_ONLY
#include "data_types.hpp"
#include "TestCppClient.h"
#include "headers.hpp"
#include "OrderSamples.h"
#include "vanilla_header.hpp"
#include "market_data.hpp"
const unsigned MAX_ATTEMPTS = 50;
const unsigned SLEEP_TIME = 10;
const unsigned SLEEP_TIME_WATCH = 15000; //ms
#define TESTING_MODE
#undef TESTING_MODE


// Define a custom facet for grouping digits
struct separate_thousands : std::numpunct<char> {
    char_type do_thousands_sep() const override { return ','; } // Use comma
    std::string do_grouping() const override { return "\3"; }    // Group by 3 digits
};




/* IMPORTANT: always use your paper trading account. The code below will submit orders as part of the demonstration. */
/* IB will not be responsible for accidental executions on your live account. */
/* Any stock or option symbols displayed are for illustrative purposes only and are not intended to portray a recommendation. */
/* Before contacting our API support team please refer to the available documentation. */
#ifndef TESTING_MODE
int main(int argc, char** argv) {
  std::ios_base::sync_with_stdio(true);
  setlocale(LC_NUMERIC, "");

  std::cout.imbue(std::locale(std::cout.getloc(), new separate_thousands));


  TmuxManagement tmux_management;
  //  std::string msg = "SENDIGN MSG!";
  //  tmux_management.send_msg_to_last_ttys(std::move(msg));
  MarketData market_data;
  /// LOAD DATA

  int asset_category = ASSET_CATAGORIES::ETFS_ALL;
  std::vector<std::string> assets =  market_data.get_asset_list(asset_category);
  fmt::print(fg(fmt::color::green), "{}\n",assets);
  srand(static_cast<unsigned int>(time(NULL)));
  int random_etf_index = rand()%assets.size();
  // std::string random_etf = assets.at(random_etf_index);
  //  .................................................................
  asset_category = ASSET_CATAGORIES::STOCKS_ALL;
  assets =  market_data.get_asset_list(asset_category);
  srand(static_cast<unsigned int>(time(NULL)));
  int random_stock_index = rand()%assets.size();
  std::string random_stock = assets.at(random_stock_index);
//   fmt::print(fg(fmt::color::purple), "{}\n",assets);

  //  std::transform(random_etf.begin(), random_etf.end(), random_etf_upper.begin(), [](unsigned char c) {
  //      return std::toupper(c);
  //      });
  //
  //  std::transform(random_stock.begin(), random_stock.end(), random_stock_upper.begin(), [](unsigned char c) {
  //      return std::toupper(c);
  //      });
  //
  //
  //
  //  fmt::print(fg(fmt::color::red),"{}\n",random_etf);
  //
  //  fmt::print(fg(fmt::color::red),"{}\n",random_stock);
  //

  // 1. Fetch the environment variable by name
  const char* env_p = std::getenv("NEUTHEOS_TRADE_DATA_HOME");

  // 2. Always check for nullptr before using the result
  if (env_p != nullptr) {
    std::cout << "Your NEUTHEOS_TRADE_DATA_HOME is: " << env_p << std::endl;
  } else {
    std::cout << "The requested environment variable does not exist." << std::endl;
    return 0;
  }




  cxxopts::Options options("IBKR_Neutheos", "IBKR-Neutheos interface");
  options.allow_unrecognised_options();
  options.add_options()
    ("d,debug", "Enable debugging") // a bool parameter
    ("c,clientID", "client id", cxxopts::value<int>()->default_value("0"))
    ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
    ;

  auto result = options.parse(argc, argv);

  int clientId = {0};
  std::cout << "parsing" << std::endl;
  clientId = result["c"].as<int>();
  std::cout << "clientId:" << clientId << std::endl;



  const char* host = {""};
  //const char* host = argc > 1 ? argv[1] : "";
  int port = argc > 2 ? atoi(argv[2]) : 0;
  int IBKR_GATEWAY_PORT = {4002};
  //if (port <= 0)
  port = IBKR_GATEWAY_PORT;
  const char* connectOptions = {"+PACEAPI"};
  // const char* connectOptions = argc > 3 ? argv[3] : "+PACEAPI";

  unsigned attempt = 0;
  printf( "Start of C++ Socket Client Test %u\n", attempt);


  Contract contract;
  contract.symbol = "AAPL";
  contract.secType = "STK";
  //  contract.exchange = "SMART";
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
    std::cout << "napping for " << napping_time << " secs" << std::endl;
    client.account_summary();
    client.trigger_account_update();
    client.start_tracking();

    while( client.isConnected()) {

      // client.print_position_details();

      //     std::string asset = {"SOXS"};
      //     auto price = client.get_ticker_price(asset);

      //     std::cout << "ticker price of " << asset << " is " << price << std::endl;
      //         //    std::cout << "performing order Operations:" << std::endl;
      //     //    client.orderOperations();
      //     //    std::cout << "done performing order Operations:" << std::endl;
      //     //    std::cout << "done performing orderOperations:" << std::endl;
      //     //
      //     //  //  client.getPositions();
      //     //  //  std::cout << "done getPositions:" << std::endl;

      std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_WATCH));
    }
    if( attempt >= MAX_ATTEMPTS) {
      break;
    }

    printf( "Sleeping %u seconds before next attempt\n", SLEEP_TIME);
    std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
  }

  printf ( "End of C++ Socket Client Test\n");
}
#else
int main(int argc, char** argv) {

  std::cout << "Hello world of TESTING ZONE!" << std::endl;
  return 0;
}
#endif


