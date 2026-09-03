/* Copyright (C) 2026 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"
#include "TestCppClient.h"
#include <locale.h>
#include <fmt/format.h> // For general formatting configurations
#include <fmt/base.h>   // For basic core printing utilities
#include <locale>
#include "EClientSocket.h"
#include "EPosixClientSocketPlatform.h"

#include "Contract.h"
#include "Order.h"
#include "Execution.h"
#include "CommissionAndFeesReport.h"
#include "ContractSamples.h"
#include "OrderSamples.h"
#include "ScannerSubscription.h"
#include "ScannerSubscriptionSamples.h"
#include "ExecutionCondition.h"
#include "PriceCondition.h"
#include "MarginCondition.h"
#include "PercentChangeCondition.h"
#include "TimeCondition.h"
#include "VolumeCondition.h"
#include "AvailableAlgoParams.h"
#include "FAMethodSamples.h"
#include "CommonDefs.h"
#include "AccountSummaryTags.h"
#include "Utils.h"
#include "IneligibilityReason.h"
#include "ConfigSamples.h"
#include "ContractSamplesProto.h"
#include "OrderSamplesProto.h"
#include "NewsSamplesProto.h"
#include "MarketDataSamplesProto.h"
#include "CassandraInterface.hpp"

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <ctime>
#include <fstream>
#include <cstdint>
#include <sstream>



#define BOLD "\033[1m"
#define GRAY "\033[90m"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define BRICK_RED "\033[38;2;178;34;34m"
#define BRIGHT_BRICK_RED "\033[91m"

#define PURPLE   "\033[35m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
const int PING_DEADLINE = 2; // seconds
const int SLEEP_BETWEEN_PINGS = 30; // seconds





void CurrentAccountState::ctor_helpers() {
  SMA_ = SMA_default_;
  buying_power_ = buying_power_default_;
}

void CurrentAccountState::dtor_helpers() {
}

CurrentAccountState::CurrentAccountState() {
  ctor_helpers();

}
CurrentAccountState::~CurrentAccountState() {
  dtor_helpers();

}


void CurrentAccountState::print() const {


}
///////////////////////////////////////////////////////////
// member funcs
//! [socket_init]
TestCppClient::TestCppClient() :
  m_osSignal(2000)//2-seconds timeout
  , m_pClient(new EClientSocket(this, &m_osSignal))
  , m_state(ST_CONNECT)
  , m_sleepDeadline(0)
  , m_orderId(0)
  , m_extraAuth(false)
{
}



void TestCppClient::printAccountSummary() {
  m_pClient->reqAccountUpdates(true, "U150462");

}

//! [socket_init]
TestCppClient::~TestCppClient()
{
  // destroy the reader before the client
  if( m_pReader )
    m_pReader.reset();

  delete m_pClient;
}

double TestCppClient::get_ticker_price(std::string ticker) {

  return 0;

  double price = {0.0};

  Contract contract;
  contract.symbol = std::string(ticker);
  contract.secType = "STK";
  contract.exchange = "SMART";
  contract.currency = "USD";
  std::cout << "requesting market data with id:" << m_orderId << std::endl;
  // m_pReader->processMsgs();     // Processes messages
  m_pClient->reqMarketDataType(1); 
  m_pClient->reqMktData(m_orderId++, contract, "", true, false, TagValueListSPtr());
  //  m_pClient->reqTickByTickData(m_orderId++, contract, "Last", 0, false);

  std::cout << "done requesting market data" << std::endl;

  return 0;

  //
  //    // Request ID, Contract, End Date (YYYYMMDD HH:MM:SS), Duration, Bar Size, WhatToShow, UseRTH, FormatDate
  //   m_pClient->reqHistoricalData(
  //       m_orderId++, 
  //       contract1, 
  //       "20260605-04:00:00", 
  //       "1 D",          // Duration (e.g., 1 D, 1 W, 1 M, 1 Y)
  //       "5 secs",       // Bar size (e.g., 1 min, 5 mins, 1 hour, 1 day)
  //       "TRADES",       // What to show: TRADES, MIDPOINT, BID, ASK, HISTORICAL_VOLATILITY, etc.
  //       1,              // Use Regular Trading Hours (1 = Yes, 0 = No)
  //       1,              // Date format (1 = yyyymmdd hh:mm:ss, 2 = epoch)
  //       false,          // Keep up to date
  //       TagValueListSPtr()
  //   );
  //   getchar();
  ////
  //
  //
  //  //m_osSignal.waitForSignal();
  //

  print_position_details();

  getchar();
  //  ExecutionFilter executionFilter1;
  //  executionFilter1.m_lastNDays = 1;
  //  m_pClient->reqExecutions(m_orderId++, executionFilter1);
  //  return 0;
  //ExecutionFilter executionFilter2;
  //executionFilter2.m_specificDates = {20250303, 20250302};
  //m_pClient->reqExecutions(m_orderId++, executionFilter2);

  //return 0;
  // m_pClient->reqAccountUpdates(true, "DUQ526162");
  //  m_pClient->reqAllOpenOrders();
  //delayedTickDataOperation();

  //  Contract contract;
  //  contract.symbol = std::string(ticker);
  //  contract.secType = "STK";
  //  contract.exchange = "SMART";
  //  contract.currency = "USD";
  //  m_osSignal.waitForSignal();


  //  //int tickerId = 1001;
  //  //int numberOfTicks = 1; // 0 for streaming
  //  //bool ignoreSize = false;
  ////  m_pClient->reqTickByTickData(m_orderId++, contract, "Last", numberOfTicks, ignoreSize);



  // // m_pClient->checkMessages();
  // // 3. Request Market Data
  // //  reqMktData(tickerId, contract, genericTickList, snapshot, regulatorySnapshot, mktDataOptions)
  // Here, snapshot is set to 'false' for streaming real-time data
  //  reqMktData(1001, contract, "", false, false, TagValueListSPtr());




  return price;

}

bool TestCppClient::connect(const char *host, int port, int clientId)
{
  // trying to connect
  //	printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "192.168.50.84" : host, port, clientId);
  //  host = {"192.168.50.84"};
  //  host = {"100.17.37.154"};
  printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientId);

  //! [connect]
  bool bRes = m_pClient->eConnect( host, port, clientId, m_extraAuth);
  //! [connect]

  if (bRes) {
    printf( "Connected to %s:%d clientId:%d serverVersion: %d\n", m_pClient->host().c_str(), m_pClient->port(), clientId, m_pClient->EClient::serverVersion());
    //! [ereader]
    m_pReader = std::unique_ptr<EReader>( new EReader(m_pClient, &m_osSignal) );
    m_pReader->start();
    //! [ereader]
    reader_thread.reset(new std::jthread([this](){ 

          while (m_pClient->isConnected()) {
          m_osSignal.waitForSignal(); // Waits for notification
          m_pReader->processMsgs();     // Processes messages
          }}));
    //                                  // std::this_thread::sleep_for(std::chrono::seconds(2));
    // reader_thread->request_stop();




    // 2. Create the reader
    //     ..m_pReader = std::make_unique<EReader>(m_pClient, &m_osSignal);
    //     ..m_pReader->start();
    //
    //     ..// 3. Keep the thread alive and continuously process messages
    //       ..while (m_pClient->isConnected()) {
    //         ..    m_osSignal.waitForSignal();
    //         ..    m_pReader->processMsgs();
    //         ..}
  } else
    printf( "Cannot connect to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

  return bRes;
}

void TestCppClient::disconnect() const
{
  m_pClient->eDisconnect();

  printf ( "Disconnected\n");
}

bool TestCppClient::isConnected() const
{
  return m_pClient->isConnected();
}

void TestCppClient::setConnectOptions(const std::string& connectOptions)
{
  m_pClient->setConnectOptions(connectOptions);
}

void TestCppClient::setOptionalCapabilities(const std::string& optionalCapabilities)
{
  m_pClient->setOptionalCapabilities(optionalCapabilities);
}

void TestCppClient::placeOrderCustom(std::string order_action_type_str, std::string symbol, double limit_price, int total_quantity_input, std::string order_type_input) {

  // 1. Define the Contract (e.g., AAPL Stock)
  Contract contract;
  contract.symbol = symbol;
  contract.secType =  {"STK"};
  contract.exchange = {"SMART"};
  contract.currency = {"USD"};
  contract.exchange = {"SMART"};
  contract.primaryExchange = {"NASDAQ"};


  // 2. Define the Order (e.g., Buy 100 shares at $150)
  //  int m_orderId = {0};
  //  // reqCurrentTime();
  //  // 3. Place the order
  //  // m_orderId should be managed/incremented from nextValidId()
  //  //  m_pClient->placeOrder(m_orderId++, contract, order);
  //  std::cout << "requesting contract details" << std::endl;
  //  std::cout << "m_pClient:" << m_pClient << std::endl;
  //   m_pClient->reqContractDetails(++m_orderId, contract);
  //



  // 1. Create the contract
  //contract.symbol = "LLY";
  //contract.secType = "STK";
  //contract.exchange = "SMART";
  //contract.currency = "USD";



  //  std::string order_type_str = {""};
  //  switch(order_type){
  //    case ORDER_TYPES::LIMIT:{
  //                            order_action_str =  std::string("LMT");
  //
  //                            break;
  //                          }
  //
  //    case ORDER_TYPES::MARKET:{
  //
  //                               order_action_str =  std::string("MKT");
  //
  //                             break;
  //
  //                           }
  //    case ORDER_TYPES::UNDEFINED_ORDER_TYPES:{
  //
  //                               order_action_str =  std::string("UNDEFINED_ORDER_TYPES");
  //
  //                             break;
  //
  //                           }
  //
  //
  //  }
  //  // 2. Create the order
  Order order;
  order.action = order_action_type_str;
  order.totalQuantity = total_quantity_input;
  order.orderType = order_type_input;
  order.lmtPrice = limit_price;
  order.tif = "GCT"; // Time in force


  fmt::print(fg(fmt::color::red),"order details: {},{},{},{:0.2f}\n", order.action, symbol, order.totalQuantity, order.lmtPrice);

  char user_confirmation;
  std::cout << "are you sure you want to place this order?y/n:";
  std::cin >> user_confirmation ;

  if(user_confirmation == 'y') {
    fmt::print(fg(fmt::color::orange),"you confirmed to place the order: {},{},{},{:0.2f}\n", order.action, symbol, order.totalQuantity, order.lmtPrice);

    std::cout << "requesting IDS" << std::endl;

    m_osSignal.waitForSignal(); // Waits for notification
                                //    m_pReader->processMsgs();     // Processes messages

    m_pClient->reqIds(0);
    std::cout << "placing order with ID:"  << m_orderId << std::endl;
    m_pClient->placeOrder(++m_orderId, contract, order);
    fmt::print(bg(fmt::color::silver)|fg(fmt::color::green),"order just placed...good luck!\n");

  }else {

    fmt::print(bg(fmt::color::silver) | fg(fmt::color::pink),"NO order placed!\n");
  }
  // 3. Place the order





}

void TestCppClient::placeOrder() {

  // 1. Define the Contract (e.g., AAPL Stock)
  Contract contract;
  contract.symbol = {"LLY"};
  contract.secType =  {"STK"};
  contract.exchange = {"SMART"};
  contract.currency = {"USD"};
  contract.exchange = {"SMART"};
  contract.primaryExchange = {"NASDAQ"};


  // 2. Define the Order (e.g., Buy 100 shares at $150)
  //  int m_orderId = {0};
  //  // reqCurrentTime();
  //  // 3. Place the order
  //  // m_orderId should be managed/incremented from nextValidId()
  //  //  m_pClient->placeOrder(m_orderId++, contract, order);
  //  std::cout << "requesting contract details" << std::endl;
  //  std::cout << "m_pClient:" << m_pClient << std::endl;
  //   m_pClient->reqContractDetails(++m_orderId, contract);
  //



  // 1. Create the contract
  contract.symbol = "LLY";
  contract.secType = "STK";
  contract.exchange = "SMART";
  contract.currency = "USD";

  // 2. Create the order
  Order order;
  order.action = "BUY";
  order.totalQuantity = 100;
  order.orderType = "LMT";
  order.lmtPrice = 10.00;
  order.tif = "GCT"; // Time in force

  std::cout << "placing order:"  << m_orderId << std::endl;
  // 3. Place the order
  int orderID= 10015;
  std::cout << "order.totalQuantity:" << order.totalQuantity << std::endl;

  m_pClient->placeOrder(++m_orderId, contract, order);

  std::cout << "placed order" << std::endl;



}



void TestCppClient::processMessages()
{
  time_t now = time(NULL);

  /*****************************************************************/
  /* Below are few quick-to-test examples on the IB API functions grouped by functionality. Uncomment the relevant methods. */
  /*****************************************************************/
  switch (m_state) {
    case ST_PNLSINGLE:
      pnlSingleOperation();
      break;
    case ST_PNLSINGLE_ACK:
      break;
    case ST_PNL:
      pnlOperation();
      break;
    case ST_PNL_ACK:
      break;
    case ST_TICKDATAOPERATION:
      tickDataOperation();
      break;
    case ST_TICKDATAOPERATION_ACK:
      break;
    case ST_TICKOPTIONCOMPUTATIONOPERATION:
      tickOptionComputationOperation();
      break;
    case ST_TICKOPTIONCOMPUTATIONOPERATION_ACK:
      break;
    case ST_DELAYEDTICKDATAOPERATION:
      delayedTickDataOperation();
      break;
    case ST_DELAYEDTICKDATAOPERATION_ACK:
      break;
    case ST_MARKETDEPTHOPERATION:
      marketDepthOperations();
      break;
    case ST_MARKETDEPTHOPERATION_ACK:
      break;
    case ST_REALTIMEBARS:
      realTimeBars();
      break;
    case ST_REALTIMEBARS_ACK:
      break;
    case ST_MARKETDATATYPE:
      marketDataType();
      break;
    case ST_MARKETDATATYPE_ACK:
      break;
    case ST_HISTORICALDATAREQUESTS:
      historicalDataRequests();
      break;
    case ST_HISTORICALDATAREQUESTS_ACK:
      break;
    case ST_OPTIONSOPERATIONS:
      optionsOperations();
      break;
    case ST_OPTIONSOPERATIONS_ACK:
      break;
    case ST_CONTRACTOPERATION:
      contractOperations();
      break;
    case ST_CONTRACTOPERATION_ACK:
      break;
    case ST_MARKETSCANNERS:
      marketScanners();
      break;
    case ST_MARKETSCANNERS_ACK:
      break;
    case ST_FUNDAMENTALS:
      fundamentals();
      break;
    case ST_FUNDAMENTALS_ACK:
      break;
    case ST_BULLETINS:
      bulletins();
      break;
    case ST_BULLETINS_ACK:
      break;
    case ST_ACCOUNTOPERATIONS:
      accountOperations();
      break;
    case ST_ACCOUNTOPERATIONS_ACK:
      break;
    case ST_ORDEROPERATIONS:
      orderOperations();
      break;
    case ST_ORDEROPERATIONS_ACK:
      break;
    case ST_OCASAMPLES:
      ocaSamples();
      break;
    case ST_OCASAMPLES_ACK:
      break;
    case ST_CONDITIONSAMPLES:
      conditionSamples();
      break;
    case ST_CONDITIONSAMPLES_ACK:
      break;
    case ST_BRACKETSAMPLES:
      bracketSample();
      break;
    case ST_BRACKETSAMPLES_ACK:
      break;
    case ST_HEDGESAMPLES:
      hedgeSample();
      break;
    case ST_HEDGESAMPLES_ACK:
      break;
    case ST_TESTALGOSAMPLES:
      testAlgoSamples();
      break;
    case ST_TESTALGOSAMPLES_ACK:
      break;
    case ST_FAORDERSAMPLES:
      financialAdvisorOrderSamples();
      break;
    case ST_FAORDERSAMPLES_ACK:
      break;
    case ST_FAOPERATIONS:
      financialAdvisorOperations();
      break;
    case ST_FAOPERATIONS_ACK:
      break;
    case ST_DISPLAYGROUPS:
      testDisplayGroups();
      break;
    case ST_DISPLAYGROUPS_ACK:
      break;
    case ST_MISCELANEOUS:
      miscellaneous();
      break;
    case ST_MISCELANEOUS_ACK:
      break;
    case ST_FAMILYCODES:
      reqFamilyCodes();
      break;
    case ST_FAMILYCODES_ACK:
      break;
    case ST_SYMBOLSAMPLES:
      reqMatchingSymbols();
      break;
    case ST_SYMBOLSAMPLES_ACK:
      break;
    case ST_REQMKTDEPTHEXCHANGES:
      reqMktDepthExchanges();
      break;
    case ST_REQMKTDEPTHEXCHANGES_ACK:
      break;
    case ST_REQNEWSTICKS:
      reqNewsTicks();
      break;
    case ST_REQNEWSTICKS_ACK:
      break;
    case ST_REQSMARTCOMPONENTS:
      reqSmartComponents();
      break;
    case ST_REQSMARTCOMPONENTS_ACK:
      break;
    case ST_NEWSPROVIDERS:
      reqNewsProviders();
      break;
    case ST_NEWSPROVIDERS_ACK:
      break;
    case ST_REQNEWSARTICLE:
      reqNewsArticle();
      break;
    case ST_REQNEWSARTICLE_ACK:
      break;
    case ST_REQHISTORICALNEWS:
      reqHistoricalNews();
      break;
    case ST_REQHISTORICALNEWS_ACK:
      break;
    case ST_REQHEADTIMESTAMP:
      reqHeadTimestamp();
      break;
    case ST_REQHISTOGRAMDATA:
      reqHistogramData();
      break;
    case ST_REROUTECFD:
      rerouteCFDOperations();
      break;
    case ST_MARKETRULE:
      marketRuleOperations();
      break;
    case ST_CONTFUT:
      continuousFuturesOperations();
      break;
    case ST_REQHISTORICALTICKS:
      reqHistoricalTicks();
      break;
    case ST_REQHISTORICALTICKS_ACK:
      break;
    case ST_REQTICKBYTICKDATA:
      reqTickByTickData();
      break;
    case ST_REQTICKBYTICKDATA_ACK:
      break;
    case ST_WHATIFSAMPLES:
      whatIfSamples();
      break;
    case ST_WHATIFSAMPLES_ACK:
      break;
    case ST_IBKRATSSAMPLE:
      ibkratsSample();
      break;
    case ST_IBKRATSSAMPLE_ACK:
      break;
    case ST_WSH:
      wshCalendarOperations();
      break;
    case ST_WSH_ACK:
      break;
    case ST_CONFIG:
      configOperations();
      break;
    case ST_CONFIG_ACK:
      break;
    case ST_ORDER_PARENTCHILD_OPERATIONS:
      orderParentChildOperations();
      break;
    case ST_ORDER_PARENTCHILD_OPERATIONS_ACK:
      break;
    case ST_NEWS_OPERATIONS_PROTO:
      newsOperationsProto();
      break;
    case ST_NEWS_OPERATIONS_PROTO_ACK:
      break;
    case ST_TICK_DATA_OPERATIONS_PROTO:
      tickDataOperationsProto();
      break;
    case ST_TICK_DATA_OPERATIONS_PROTO_ACK:
      break;
    case ST_PING:
      reqCurrentTime();
      break;
    case ST_PING_ACK:
      if( m_sleepDeadline < now) {
        disconnect();
        return;
      }
      break;
    case ST_IDLE:
      if( m_sleepDeadline < now) {
        m_state = ST_PING;
        return;
      }
      break;
  }

  // m_osSignal.waitForSignal();
  errno = 0;
  //  m_pReader->processMsgs();
}

//////////////////////////////////////////////////////////////////
// methods
//! [connectack]
void TestCppClient::connectAck() {
  if (!m_extraAuth && m_pClient->asyncEConnect())
    m_pClient->startApi();
}
//! [connectack]

void TestCppClient::reqCurrentTime()
{
  printf( "Requesting Current Time\n");

  // set ping deadline to "now + n seconds"
  m_sleepDeadline = time( NULL) + PING_DEADLINE;

  m_state = ST_PING_ACK;

  m_pClient->reqCurrentTime();
}

void TestCppClient::pnlOperation()
{
  //! [reqpnl]
  m_pClient->reqPnL(7001, "DUD00029", "");
  //! [reqpnl]

  std::this_thread::sleep_for(std::chrono::seconds(2));

  //! [cancelpnl]
  m_pClient->cancelPnL(7001);
  //! [cancelpnl]

  m_state = ST_PNL_ACK;
}

void TestCppClient::pnlSingleOperation()
{
  //! [reqpnlsingle]
  m_pClient->reqPnLSingle(7002, "DUD00029", "", 268084);
  //! [reqpnlsingle]

  std::this_thread::sleep_for(std::chrono::seconds(2));

  //! [cancelpnlsingle]
  m_pClient->cancelPnLSingle(7002);
  //! [cancelpnlsingle]

  m_state = ST_PNLSINGLE_ACK;
}

void TestCppClient::tickDataOperation()
{
  //! [reqmarketdatatype]
  m_pClient->reqMarketDataType(4);
  //! [reqmarketdatatype]

  /*** Requesting real time market data ***/
  std::this_thread::sleep_for(std::chrono::seconds(1));
  //! [reqmktdata]
  m_pClient->reqMktData(1001, ContractSamples::StockComboContract(), "", false, false, TagValueListSPtr());
  m_pClient->reqMktData(1002, ContractSamples::OptionWithLocalSymbol(), "", false, false, TagValueListSPtr());
  //! [reqmktdata]
  //! [reqmktdata_snapshot]
  m_pClient->reqMktData(1003, ContractSamples::FutureComboContract(), "", true, false, TagValueListSPtr());
  //! [reqmktdata_snapshot]

  /*
  //! [regulatorysnapshot]
  // Each regulatory snapshot incurs a fee of 0.01 USD
  m_pClient->reqMktData(1013, ContractSamples::USStock(), "", false, true, TagValueListSPtr());
  //! [regulatorysnapshot]
  */

  //! [reqmktdata_genticks]
  //Requesting RTVolume (Time & Sales) and shortable generic ticks
  m_pClient->reqMktData(1004, ContractSamples::USStockAtSmart(), "233,236", false, false, TagValueListSPtr());
  //! [reqmktdata_genticks]

  //! [reqmktdata_contractnews]
  // Without the API news subscription this will generate an "invalid tick type" error
  m_pClient->reqMktData(1005, ContractSamples::USStock(), "mdoff,292:BZ", false, false, TagValueListSPtr());
  m_pClient->reqMktData(1006, ContractSamples::USStock(), "mdoff,292:BT", false, false, TagValueListSPtr());
  m_pClient->reqMktData(1007, ContractSamples::USStock(), "mdoff,292:FLY", false, false, TagValueListSPtr());
  m_pClient->reqMktData(1008, ContractSamples::USStock(), "mdoff,292:DJ-RT", false, false, TagValueListSPtr());
  //! [reqmktdata_contractnews]
  //! [reqmktdata_broadtapenews]
  m_pClient->reqMktData(1009, ContractSamples::BTbroadtapeNewsFeed(), "mdoff,292", false, false, TagValueListSPtr());
  m_pClient->reqMktData(1010, ContractSamples::BZbroadtapeNewsFeed(), "mdoff,292", false, false, TagValueListSPtr());
  m_pClient->reqMktData(1011, ContractSamples::FLYbroadtapeNewsFeed(), "mdoff,292", false, false, TagValueListSPtr());
  //! [reqmktdata_broadtapenews]

  //! [reqoptiondatagenticks]
  //Requesting data for an option contract will return the greek values
  m_pClient->reqMktData(1013, ContractSamples::USOptionContract(), "", false, false, TagValueListSPtr());
  //! [reqoptiondatagenticks]

  //! [reqfuturesopeninterest]
  //Requesting data for a futures contract will return the futures open interest
  m_pClient->reqMktData(1014, ContractSamples::SimpleFuture(), "mdoff,588", false, false, TagValueListSPtr());
  //! [reqfuturesopeninterest]

  //! [reqpreopenbidask]
  //Requesting data for a futures contract will return the pre-open bid/ask flag
  m_pClient->reqMktData(1015, ContractSamples::SimpleFuture(), "", false, false, TagValueListSPtr());
  //! [reqpreopenbidask]

  //! [reqavgoptvolume]
  //Requesting data for a stock will return the average option volume
  m_pClient->reqMktData(1016, ContractSamples::USStockAtSmart(), "mdoff,105", false, false, TagValueListSPtr());
  //! [reqavgoptvolume]

  //! [reqetfticks]
  //Requesting data for an ETF will return the ETF ticks
  m_pClient->reqMktData(1017, ContractSamples::etf(), "mdoff,577,614,623", false, false, TagValueListSPtr());
  //! [reqetfticks]

  //! [reqmktdatacrypto]
  m_pClient->reqMktData(1018, ContractSamples::CryptoContract(), "", false, false, TagValueListSPtr());
  //! [reqmktdatacrypto]

  //! [IPOPrice]
  m_pClient->reqMktData(1019, ContractSamples::StockWithIPOPrice(), "mdoff,586", false, false, TagValueListSPtr());
  //! [IPOPrice]

  //! [yieldbidask]
  m_pClient->reqMktData(1020, ContractSamples::Bond(), "", false, false, TagValueListSPtr());
  //! [yieldbidask]

  std::this_thread::sleep_for(std::chrono::seconds(1));
  /*** Canceling the market data subscription ***/
  //! [cancelmktdata]
  m_pClient->cancelMktData(1001);
  m_pClient->cancelMktData(1002);
  m_pClient->cancelMktData(1003);
  m_pClient->cancelMktData(1014);
  m_pClient->cancelMktData(1015);
  m_pClient->cancelMktData(1016);
  m_pClient->cancelMktData(1017);
  m_pClient->cancelMktData(1018);
  m_pClient->cancelMktData(1019);
  m_pClient->cancelMktData(1020);
  //! [cancelmktdata]

  m_state = ST_TICKDATAOPERATION_ACK;
}

void TestCppClient::tickOptionComputationOperation()
{
  /*** Requesting real time market data ***/
  std::this_thread::sleep_for(std::chrono::seconds(1));

  m_pClient->reqMarketDataType(4);

  //! [reqmktdata]
  m_pClient->reqMktData(2001, ContractSamples::OptionWithLocalSymbol(), "", false, false, TagValueListSPtr());
  //! [reqmktdata]

  std::this_thread::sleep_for(std::chrono::seconds(10));
  /*** Canceling the market data subscription ***/
  //! [cancelmktdata]
  m_pClient->cancelMktData(2001);
  //! [cancelmktdata]

  m_state = ST_TICKOPTIONCOMPUTATIONOPERATION_ACK;
}

void TestCppClient::delayedTickDataOperation()
{
  /*** Requesting delayed market data ***/

  //! [reqmktdata_delayedmd]
  m_pClient->reqMarketDataType(4); // send delayed-frozen (4) market data type
  m_pClient->reqMktData(1013, ContractSamples::HKStk(), "", false, false, TagValueListSPtr());
  m_pClient->reqMktData(1014, ContractSamples::USOptionContract(), "", false, false, TagValueListSPtr());
  //! [reqmktdata_delayedmd]

  std::this_thread::sleep_for(std::chrono::seconds(10));

  /*** Canceling the delayed market data subscription ***/
  //! [cancelmktdata_delayedmd]
  m_pClient->cancelMktData(1013);
  m_pClient->cancelMktData(1014);
  //! [cancelmktdata_delayedmd]

  m_state = ST_DELAYEDTICKDATAOPERATION_ACK;
}

void TestCppClient::marketDepthOperations()
{
  /*** Requesting the Deep Book ***/
  //! [reqmarketdepth]
  m_pClient->reqMktDepth(2001, ContractSamples::EurGbpFx(), 5, false, TagValueListSPtr());
  //! [reqmarketdepth]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  /*** Canceling the Deep Book request ***/
  //! [cancelmktdepth]
  m_pClient->cancelMktDepth(2001, false);
  //! [cancelmktdepth]

  /*** Requesting the Deep Book ***/
  //! [reqmarketdepth]
  m_pClient->reqMktDepth(2002, ContractSamples::EuropeanStock(), 5, true, TagValueListSPtr());
  //! [reqmarketdepth]
  std::this_thread::sleep_for(std::chrono::seconds(5));
  /*** Canceling the Deep Book request ***/
  //! [cancelmktdepth]
  m_pClient->cancelMktDepth(2002, true);
  //! [cancelmktdepth]

  m_state = ST_MARKETDEPTHOPERATION_ACK;
}

void TestCppClient::realTimeBars()
{
  /*** Requesting real time bars ***/
  //! [reqrealtimebars]
  m_pClient->reqRealTimeBars(3001, ContractSamples::EurGbpFx(), 5, "MIDPOINT", true, TagValueListSPtr());
  //! [reqrealtimebars]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  /*** Canceling real time bars ***/
  //! [cancelrealtimebars]
  m_pClient->cancelRealTimeBars(3001);
  //! [cancelrealtimebars]

  m_state = ST_REALTIMEBARS_ACK;
}

void TestCppClient::marketDataType()
{
  //! [reqmarketdatatype]
  /*** By default only real-time (1) market data is enabled
    Sending frozen (2) enables frozen market data
    Sending delayed (3) enables delayed market data and disables delayed-frozen market data
    Sending delayed-frozen (4) enables delayed and delayed-frozen market data
    Sending real-time (1) disables frozen, delayed and delayed-frozen market data ***/
  m_pClient->reqMarketDataType(2);
  //! [reqmarketdatatype]

  m_state = ST_MARKETDATATYPE_ACK;
}

void TestCppClient::historicalDataRequests()
{
  /*** Requesting historical data ***/
  //! [reqhistoricaldata]
  std::time_t rawtime;
  std::tm timeinfo;
  char queryTime[80];

  std::time(&rawtime);
#if defined(IB_WIN32)
  gmtime_s(&timeinfo, &rawtime);
#else
  gmtime_r(&rawtime, &timeinfo);
#endif
  std::strftime(queryTime, sizeof queryTime, "%Y%m%d-%H:%M:%S", &timeinfo);

  m_pClient->reqHistoricalData(4001, ContractSamples::EurGbpFx(), queryTime, "1 M", "1 day", "MIDPOINT", 1, 1, false, TagValueListSPtr());
  m_pClient->reqHistoricalData(4002, ContractSamples::EuropeanStock(), queryTime, "10 D", "1 min", "TRADES", 1, 1, false, TagValueListSPtr());
  m_pClient->reqHistoricalData(4003, ContractSamples::USStockAtSmart(), queryTime, "1 M", "1 day", "SCHEDULE", 1, 1, false, TagValueListSPtr());
  //! [reqhistoricaldata]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  /*** Canceling historical data requests ***/
  m_pClient->cancelHistoricalData(4001);
  m_pClient->cancelHistoricalData(4002);
  m_pClient->cancelHistoricalData(4003);

  m_state = ST_HISTORICALDATAREQUESTS_ACK;
}

void TestCppClient::optionsOperations()
{
  //! [reqsecdefoptparams]
  m_pClient->reqSecDefOptParams(0, "IBM", "", "STK", 8314);
  //! [reqsecdefoptparams]

  //! [calculateimpliedvolatility]
  m_pClient->calculateImpliedVolatility(5001, ContractSamples::OptionWithLocalSymbol(), 0.5, 55, TagValueListSPtr());
  //! [calculateimpliedvolatility]

  //** Canceling implied volatility ***
  m_pClient->cancelCalculateImpliedVolatility(5001);

  //! [calculateoptionprice]
  m_pClient->calculateOptionPrice(5002, ContractSamples::OptionWithLocalSymbol(), 0.6, 55, TagValueListSPtr());
  //! [calculateoptionprice]

  //** Canceling option's price calculation ***
  m_pClient->cancelCalculateOptionPrice(5002);

  //! [exercise_options]
  //** Exercising options ***
  m_pClient->exerciseOptions(5003, ContractSamples::OptionWithTradingClass(), 1, 1, "", 1, "20231018-12:00:00", "CustAcct", true);
  //! [exercise_options]

  m_state = ST_OPTIONSOPERATIONS_ACK;
}

void TestCppClient::contractOperations()
{
  m_pClient->reqContractDetails(209, ContractSamples::EurGbpFx());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [reqcontractdetails]
  m_pClient->reqContractDetails(210, ContractSamples::OptionForQuery());
  m_pClient->reqContractDetails(212, ContractSamples::IBMBond());
  m_pClient->reqContractDetails(213, ContractSamples::IBKRStk());
  m_pClient->reqContractDetails(214, ContractSamples::Bond());
  m_pClient->reqContractDetails(215, ContractSamples::FuturesOnOptions());
  m_pClient->reqContractDetails(216, ContractSamples::SimpleFuture());
  m_pClient->reqContractDetails(219, ContractSamples::Fund());
  m_pClient->reqContractDetails(220, ContractSamples::USStock());
  m_pClient->reqContractDetails(221, ContractSamples::USStockAtSmart());
  m_pClient->reqContractDetails(222, ContractSamples::OptForecastx());
  m_pClient->reqContractDetails(223, ContractSamples::OptForecastxZeroStrike());
  m_pClient->reqContractDetails(224, ContractSamples::OptForecastxByConId());
  //! [reqcontractdetails]

  //! [reqcontractdetailsnews]
  m_pClient->reqContractDetails(211, ContractSamples::NewsFeedForQuery());
  //! [reqcontractdetailsnews]

  //! [reqcontractdetailscrypto]
  m_pClient->reqContractDetails(217, ContractSamples::CryptoContract());
  //! [reqcontractdetailscrypto]

  //! [reqcontractdetailsbyissuerid]
  m_pClient->reqContractDetails(218, ContractSamples::ByIssuerId());
  //! [reqcontractdetailsbyissuerid]

  //! [cancelcontractdata]
  m_pClient->cancelContractData(209);
  //! [cancelcontractdata]

  m_state = ST_CONTRACTOPERATION_ACK;
}

void TestCppClient::marketScanners()
{
  /*** Requesting all available parameters which can be used to build a scanner request ***/
  //! [reqscannerparameters]
  m_pClient->reqScannerParameters();
  //! [reqscannerparameters]
  std::this_thread::sleep_for(std::chrono::seconds(2));

  /*** Triggering a scanner subscription ***/
  //! [reqscannersubscription]
  m_pClient->reqScannerSubscription(7001, ScannerSubscriptionSamples::HotUSStkByVolume(), TagValueListSPtr(), TagValueListSPtr());

  TagValueSPtr t1(new TagValue("usdMarketCapAbove", "10000"));
  TagValueSPtr t2(new TagValue("optVolumeAbove", "1000"));
  TagValueSPtr t3(new TagValue("avgVolumeAbove", "100000000"));

  TagValueListSPtr TagValues(new TagValueList());
  TagValues->push_back(t1);
  TagValues->push_back(t2);
  TagValues->push_back(t3);

  m_pClient->reqScannerSubscription(7002, ScannerSubscriptionSamples::HotUSStkByVolume(), TagValueListSPtr(), TagValues); // requires TWS v973+

  //! [reqscannersubscription]

  //! [reqcomplexscanner]

  TagValueSPtr t(new TagValue("underConID", "265598"));
  TagValueListSPtr AAPLConIDTag(new TagValueList());
  AAPLConIDTag->push_back(t);
  m_pClient->reqScannerSubscription(7003, ScannerSubscriptionSamples::ComplexOrdersAndTrades(), TagValueListSPtr(), AAPLConIDTag); // requires TWS v975+

  //! [reqcomplexscanner]

  std::this_thread::sleep_for(std::chrono::seconds(2));
  /*** Canceling the scanner subscription ***/
  //! [cancelscannersubscription]
  m_pClient->cancelScannerSubscription(7001);
  m_pClient->cancelScannerSubscription(7002);
  //! [cancelscannersubscription]

  m_state = ST_MARKETSCANNERS_ACK;
}

void TestCppClient::fundamentals()
{
  /*** Requesting Fundamentals ***/
  //! [reqfundamentaldata]
  m_pClient->reqFundamentalData(8001, ContractSamples::USStock(), "ReportsFinSummary", TagValueListSPtr());
  //! [reqfundamentaldata]
  std::this_thread::sleep_for(std::chrono::seconds(2));

  /*** Canceling fundamentals request ***/
  //! [cancelfundamentaldata]
  m_pClient->cancelFundamentalData(8001);
  //! [cancelfundamentaldata]

  m_state = ST_FUNDAMENTALS_ACK;
}

void TestCppClient::bulletins()
{
  /*** Requesting Interactive Broker's news bulletins */
  //! [reqnewsbulletins]
  m_pClient->reqNewsBulletins(true);
  //! [reqnewsbulletins]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  /*** Canceling IB's news bulletins ***/
  //! [cancelnewsbulletins]
  m_pClient->cancelNewsBulletins();
  //! [cancelnewsbulletins]

  m_state = ST_BULLETINS_ACK;
}

void TestCppClient::accountOperations()
{
  /*** Requesting managed accounts***/
  //! [reqmanagedaccts]
  m_pClient->reqManagedAccts();
  //! [reqmanagedaccts]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  /*** Requesting accounts' summary ***/
  //! [reqaaccountsummary]
  m_pClient->reqAccountSummary(9001, "All", AccountSummaryTags::getAllTags());
  //! [reqaaccountsummary]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [reqaaccountsummaryledger]
  m_pClient->reqAccountSummary(9002, "All", "$LEDGER");
  //! [reqaaccountsummaryledger]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [reqaaccountsummaryledgercurrency]
  m_pClient->reqAccountSummary(9003, "All", "$LEDGER:EUR");
  //! [reqaaccountsummaryledgercurrency]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [reqaaccountsummaryledgerall]
  m_pClient->reqAccountSummary(9004, "All", "$LEDGER:ALL");
  //! [reqaaccountsummaryledgerall]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [cancelaaccountsummary]
  m_pClient->cancelAccountSummary(9001);
  m_pClient->cancelAccountSummary(9002);
  m_pClient->cancelAccountSummary(9003);
  m_pClient->cancelAccountSummary(9004);
  //! [cancelaaccountsummary]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  /*** Subscribing to an account's information. Only one at a time! ***/
  //! [reqaaccountupdates]
  m_pClient->reqAccountUpdates(true, "U150462");
  //! [reqaaccountupdates]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [cancelaaccountupdates]
  m_pClient->reqAccountUpdates(false, "U150462");
  //! [cancelaaccountupdates]
  std::this_thread::sleep_for(std::chrono::seconds(2));

  //! [reqaaccountupdatesmulti]
  m_pClient->reqAccountUpdatesMulti(9002, "U150462", "EUstocks", true);
  //! [reqaaccountupdatesmulti]
  std::this_thread::sleep_for(std::chrono::seconds(2));

  /*** Requesting all accounts' positions. ***/
  //! [reqpositions]
  m_pClient->reqPositions();
  //! [reqpositions]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [cancelpositions]
  m_pClient->cancelPositions();
  //! [cancelpositions]

  //! [reqpositionsmulti]
  m_pClient->reqPositionsMulti(9003, "U150462", "EUstocks");
  //! [reqpositionsmulti]

  //! [userinfo]
  m_pClient->reqUserInfo(0);
  //! [userinfo]

  m_state = ST_ACCOUNTOPERATIONS_ACK;
}

void TestCppClient::orderOperations()
{
  /*** Requesting the next valid id ***/
  //! [reqids]
  //The parameter is always ignored.
  m_pClient->reqIds(-1);
  //! [reqids]
  //! [reqallopenorders]
  m_pClient->reqAllOpenOrders();
  //! [reqallopenorders]
  //! [reqautoopenorders]
  m_pClient->reqAutoOpenOrders(true);
  //! [reqautoopenorders]
  //! [reqopenorders]
  m_pClient->reqOpenOrders();
  //! [reqopenorders]

  /*** Placing/modifying an order - remember to ALWAYS increment the nextValidId after placing an order so it can be used for the next one! ***/
  //! [order_submission]
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::LimitOrder("SELL", DecimalFunctions::stringToDecimal("1"), 50));
  //! [order_submission]

  //m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtBox(), OrderSamples::Block("BUY", DecimalFunctions::stringToDecimal("50"), 20));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtBox(), OrderSamples::BoxTop("SELL", DecimalFunctions::stringToDecimal("10")));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::FutureComboContract(), OrderSamples::ComboLimitOrder("SELL", DecimalFunctions::stringToDecimal("1"), 1, false));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::StockComboContract(), OrderSamples::ComboMarketOrder("BUY", DecimalFunctions::stringToDecimal("1"), false));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::OptionComboContract(), OrderSamples::ComboMarketOrder("BUY", DecimalFunctions::stringToDecimal("1"), true));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::StockComboContract(), OrderSamples::LimitOrderForComboWithLegPrices("BUY", DecimalFunctions::stringToDecimal("1"), std::vector<double>(10, 5), true));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::Discretionary("SELL", DecimalFunctions::stringToDecimal("1"), 45, 0.5));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtBox(), OrderSamples::LimitIfTouched("BUY", DecimalFunctions::stringToDecimal("1"), 30, 34));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::LimitOnClose("SELL", DecimalFunctions::stringToDecimal("1"), 34));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::LimitOnOpen("BUY", DecimalFunctions::stringToDecimal("1"), 35));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketIfTouched("BUY", DecimalFunctions::stringToDecimal("1"), 35));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketOnClose("SELL", DecimalFunctions::stringToDecimal("1")));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketOnOpen("BUY", DecimalFunctions::stringToDecimal("1")));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketOrder("SELL", DecimalFunctions::stringToDecimal("1")));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketToLimit("BUY", DecimalFunctions::stringToDecimal("1")));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtIse(), OrderSamples::MidpointMatch("BUY", DecimalFunctions::stringToDecimal("1")));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::Stop("SELL", DecimalFunctions::stringToDecimal("1"), 34.4));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::StopLimit("BUY", DecimalFunctions::stringToDecimal("1"), 35, 33));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::StopWithProtection("SELL", DecimalFunctions::stringToDecimal("1"), 45));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::SweepToFill("BUY", DecimalFunctions::stringToDecimal("1"), 35));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::TrailingStop("SELL", DecimalFunctions::stringToDecimal("1"), 0.5, 30));
  //m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::TrailingStopLimit("BUY", DecimalFunctions::stringToDecimal("100"), 2, 5, 50));

  //! [place_midprice]
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), OrderSamples::Midprice("BUY", DecimalFunctions::stringToDecimal("1"), 150));
  //! [place_midprice]

  //! [place order with cashQty]
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), OrderSamples::LimitOrderWithCashQty("BUY", 111.11, 5000));
  //! [place order with cashQty]

  std::this_thread::sleep_for(std::chrono::seconds(1));

  /*** Cancel one order ***/
  //! [cancelorder]
  m_pClient->cancelOrder(m_orderId-1, OrderSamples::OrderCancelEmpty());
  //! [cancelorder]

  /*** Cancel all orders for all accounts ***/
  //! [reqglobalcancel]
  m_pClient->reqGlobalCancel(OrderSamples::OrderCancelEmpty());
  //! [reqglobalcancel]

  /*** Request the day's executions ***/
  //! [reqexecutions]
  ExecutionFilter executionFilter1;
  executionFilter1.m_lastNDays = 7;
  m_pClient->reqExecutions(10001, executionFilter1);
  ExecutionFilter executionFilter2;
  executionFilter2.m_specificDates = {20250303, 20250302};
  m_pClient->reqExecutions(10002, executionFilter2);
  //! [reqexecutions]

  //! [reqexecutions_protobuf]
#if !defined(USE_WIN_DLL)
  protobuf::ExecutionRequest executionRequestProto1;
  executionRequestProto1.set_reqid(10003);
  protobuf::ExecutionFilter* executionFilterProto1 = executionRequestProto1.mutable_executionfilter();
  executionFilterProto1->set_lastndays(7);
  m_pClient->reqExecutionsProtoBuf(executionRequestProto1);

  protobuf::ExecutionRequest executionRequestProto2;
  executionRequestProto2.set_reqid(10004);
  protobuf::ExecutionFilter* executionFilterProto2 = executionRequestProto2.mutable_executionfilter();
  executionFilterProto2->add_specificdates(20250514);
  executionFilterProto2->add_specificdates(20250513);
  executionFilterProto2->add_specificdates(20250512);
  m_pClient->reqExecutionsProtoBuf(executionRequestProto2);
#endif
  //! [reqexecutions_protobuf]

  //! [reqcompletedorders]
  m_pClient->reqCompletedOrders(false);
  //! [reqcompletedorders]

  //! [order_submission]
  m_pClient->placeOrder(m_orderId++, ContractSamples::CryptoContract(), OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("0.12345678"), 3700));
  //! [order_submission]

  //! [manual_order_time]
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), OrderSamples::LimitOrderWithManualOrderTime("BUY", DecimalFunctions::stringToDecimal("100"), 111.11, "20220314-13:00:00"));
  //! [manual_order_time]

  //! [manual_order_cancel_time]
  m_pClient->cancelOrder(m_orderId - 1, OrderSamples::OrderCancelWithManualTime("20240614-00:00:05"));
  //! [manual_order_cancel_time]

  //! [pegbest_up_to_mid_order_submission]
  m_pClient->placeOrder(m_orderId++, ContractSamples::IBKRATSContract(), OrderSamples::PegBestUpToMidOrder("BUY", DecimalFunctions::stringToDecimal("100"), 111.11, 100, 200, 0.02, 0.025));
  //! [pegbest_up_to_mid_order_submission]

  //! [pegbest_order_submission]
  m_pClient->placeOrder(m_orderId++, ContractSamples::IBKRATSContract(), OrderSamples::PegBestOrder("BUY", DecimalFunctions::stringToDecimal("100"), 111.11, 100, 200, 0.03));
  //! [pegbest_order_submission]

  //! [pegmid_order_submission]
  m_pClient->placeOrder(m_orderId++, ContractSamples::IBKRATSContract(), OrderSamples::PegMidOrder("BUY", DecimalFunctions::stringToDecimal("100"), 111.11, 100, 0.02, 0.025));
  //! [pegmid_order_submission]

  //! [customer_account]
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), OrderSamples::LimitOrderWithCustomerAccount("BUY", DecimalFunctions::stringToDecimal("100"), 111.11, "CustAcct"));
  //! [customer_account]

  //! [include_overnight]
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), OrderSamples::LimitOrderWithIncludeOvernight("BUY", DecimalFunctions::stringToDecimal("100"), 111.11));
  //! [include_overnight]

  //! [cme_tagging_fields]
  m_pClient->placeOrder(m_orderId++, ContractSamples::SimpleFuture(), OrderSamples::LimitOrderWithCmeTaggingFields("BUY", DecimalFunctions::stringToDecimal("1"), 5333, "ABCD", 1));
  std::this_thread::sleep_for(std::chrono::seconds(5));
  m_pClient->cancelOrder(m_orderId - 1, OrderSamples::OrderCancelWithCmeTaggingFields("BCDE", 0));
  std::this_thread::sleep_for(std::chrono::seconds(2));
  m_pClient->placeOrder(m_orderId++, ContractSamples::SimpleFuture(), OrderSamples::LimitOrderWithCmeTaggingFields("BUY", DecimalFunctions::stringToDecimal("1"), 5444, "CDEF", 0));
  std::this_thread::sleep_for(std::chrono::seconds(5));
  m_pClient->reqGlobalCancel(OrderSamples::OrderCancelWithCmeTaggingFields("DEFG", 1));
  //! [cme_tagging_fields]

  //! [imbalance_only]
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), OrderSamples::LimitOnCloseOrderWithImbalanceOnly("BUY", DecimalFunctions::stringToDecimal("100"), 44.44));
  //! [imbalance_only]

  //! [zero_strike_opt_order]
  m_pClient->placeOrder(m_orderId++, ContractSamples::OptForecastxZeroStrike(), OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("1"), 0.05));
  //! [zero_strike_opt_order]

  //! [limit_order_with_stop_loss_and_profit_taker]
  int orderId = m_orderId++;
  int slOrderId = m_orderId++;
  int ptOrderId = m_orderId++;
  m_pClient->placeOrder(orderId, ContractSamples::USStockAtSmart(), OrderSamples::LimitOrderWithStopLossAndProfitTaker("BUY", DecimalFunctions::stringToDecimal("100"), 40, slOrderId, ptOrderId));
  //! [limit_order_with_stop_loss_and_profit_taker]

  m_state = ST_ORDEROPERATIONS_ACK;
}

void TestCppClient::ocaSamples()
{
  //OCA ORDER
  //! [ocasubmit]
  std::vector<Order> ocaOrders;
  ocaOrders.push_back(OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("1"), 10));
  ocaOrders.push_back(OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("1"), 11));
  ocaOrders.push_back(OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("1"), 12));
  for(unsigned int i = 0; i < ocaOrders.size(); i++){
    OrderSamples::OneCancelsAll("TestOca", ocaOrders[i], 2);
    m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), ocaOrders[i]);
  }
  //! [ocasubmit]

  m_state = ST_OCASAMPLES_ACK;
}

void TestCppClient::conditionSamples()
{
  //! [order_conditioning_activate]
  Order lmt = OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("100"), 10);
  //Order will become active if conditioning criteria is met
  PriceCondition* priceCondition = dynamic_cast<PriceCondition *>(OrderSamples::Price_Condition(8314, "SMART", 600, PriceCondition::Method::BidAsk, true, false));
  ExecutionCondition* execCondition = dynamic_cast<ExecutionCondition *>(OrderSamples::Execution_Condition("EUR.USD", "CASH", "IDEALPRO", true));
  MarginCondition* marginCondition = dynamic_cast<MarginCondition *>(OrderSamples::Margin_Condition(30, true, false));
  PercentChangeCondition* pctChangeCondition = dynamic_cast<PercentChangeCondition *>(OrderSamples::Percent_Change_Condition(15.0, 208813720, "SMART", true, true));
  TimeCondition* timeCondition = dynamic_cast<TimeCondition *>(OrderSamples::Time_Condition("20220808 10:00:00 US/Eastern", true, false));
  VolumeCondition* volumeCondition = dynamic_cast<VolumeCondition *>(OrderSamples::Volume_Condition(208813720, "SMART", false, 100, true));

  lmt.conditions.push_back(std::shared_ptr<PriceCondition>(priceCondition));
  lmt.conditions.push_back(std::shared_ptr<ExecutionCondition>(execCondition));
  lmt.conditions.push_back(std::shared_ptr<MarginCondition>(marginCondition));
  lmt.conditions.push_back(std::shared_ptr<PercentChangeCondition>(pctChangeCondition));
  lmt.conditions.push_back(std::shared_ptr<TimeCondition>(timeCondition));
  lmt.conditions.push_back(std::shared_ptr<VolumeCondition>(volumeCondition));
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(),lmt);
  //! [order_conditioning_activate]

  //Conditions can make the order active or cancel it. Only LMT orders can be conditionally canceled.
  //! [order_conditioning_cancel]
  Order lmt2 = OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("100"), 20);
  //The active order will be cancelled if conditioning criteria is met
  lmt2.conditionsCancelOrder = true;
  PriceCondition* priceCondition2 = dynamic_cast<PriceCondition *>(OrderSamples::Price_Condition(208813720, "SMART", 600, PriceCondition::Method::BidAsk, false, false));
  lmt2.conditions.push_back(std::shared_ptr<PriceCondition>(priceCondition2));
  m_pClient->placeOrder(m_orderId++, ContractSamples::EuropeanStock(), lmt2);
  //! [order_conditioning_cancel]

  m_state = ST_CONDITIONSAMPLES_ACK;
}

void TestCppClient::bracketSample(){
  Order parent;
  Order takeProfit;
  Order stopLoss;
  //! [bracketsubmit]
  OrderSamples::BracketOrder(m_orderId++, parent, takeProfit, stopLoss, "BUY", DecimalFunctions::stringToDecimal("100"), 30, 40, 20);
  m_pClient->placeOrder(parent.orderId, ContractSamples::EuropeanStock(), parent);
  m_pClient->placeOrder(takeProfit.orderId, ContractSamples::EuropeanStock(), takeProfit);
  m_pClient->placeOrder(stopLoss.orderId, ContractSamples::EuropeanStock(), stopLoss);
  //! [bracketsubmit]

  m_state = ST_BRACKETSAMPLES_ACK;
}

void TestCppClient::hedgeSample(){
  //F Hedge order
  //! [hedgesubmit]
  //Parent order on a contract which currency differs from your base currency
  Order parent = OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("100"), 10);
  parent.orderId = m_orderId++;
  parent.transmit = false;
  //Hedge on the currency conversion
  Order hedge = OrderSamples::MarketFHedge(parent.orderId, "BUY");
  //Place the parent first...
  m_pClient->placeOrder(parent.orderId, ContractSamples::EuropeanStock(), parent);
  //Then the hedge order
  m_pClient->placeOrder(m_orderId++, ContractSamples::EurGbpFx(), hedge);
  //! [hedgesubmit]

  m_state = ST_HEDGESAMPLES_ACK;
}

void TestCppClient::testAlgoSamples(){
  //! [algo_base_order]
  Order baseOrder = OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("1000"), 1);
  //! [algo_base_order]

  //! [arrivalpx]
  AvailableAlgoParams::FillArrivalPriceParams(baseOrder, 0.1, "Aggressive", "09:00:00 US/Eastern", "16:00:00 US/Eastern", true, true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [arrivalpx]

  //! [darkice]
  AvailableAlgoParams::FillDarkIceParams(baseOrder, 10, "09:00:00 US/Eastern", "16:00:00 US/Eastern", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [darkice]

  //! [ad]
  // The Time Zone in "startTime" and "endTime" attributes is ignored and always defaulted to GMT
  AvailableAlgoParams::FillAccumulateDistributeParams(baseOrder, 10, 60, true, true, 1, true, true, "12:00:00", "16:00:00");
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [ad]

  //! [twap]
  AvailableAlgoParams::FillTwapParams(baseOrder, "Marketable", "09:00:00 US/Eastern", "16:00:00 US/Eastern", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [twap]

  //! [vwap]
  AvailableAlgoParams::FillBalanceImpactRiskParams(baseOrder, 0.1, "Aggressive", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [vwap]

  //! [balanceimpactrisk]
  AvailableAlgoParams::FillBalanceImpactRiskParams(baseOrder, 0.1, "Aggressive", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [balanceimpactrisk]

  //! [minimpact]
  AvailableAlgoParams::FillMinImpactParams(baseOrder, 0.3);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [minimpact]

  //! [adaptive]
  AvailableAlgoParams::FillAdaptiveParams(baseOrder, "Normal");
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [adaptive]

  //! [closepx]
  AvailableAlgoParams::FillClosePriceParams(baseOrder, 0.5, "Neutral", "12:00:00 US/Eastern", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [closepx]

  //! [pctvol]
  AvailableAlgoParams::FillPctVolParams(baseOrder, 0.5, "12:00:00 US/Eastern", "14:00:00 US/Eastern", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [pctvol]

  //! [pctvolpx]
  AvailableAlgoParams::FillPriceVariantPctVolParams(baseOrder, 0.1, 0.05, 0.01, 0.2, "12:00:00 US/Eastern", "14:00:00 US/Eastern", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [pctvolpx]

  //! [pctvolsz]
  AvailableAlgoParams::FillSizeVariantPctVolParams(baseOrder, 0.2, 0.4, "12:00:00 US/Eastern", "14:00:00 US/Eastern", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [pctvolsz]

  //! [pctvoltm]
  AvailableAlgoParams::FillTimeVariantPctVolParams(baseOrder, 0.2, 0.4, "12:00:00 US/Eastern", "14:00:00 US/Eastern", true);
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
  //! [pctvoltm]

  //! [jeff_vwap_algo]
  AvailableAlgoParams::FillJefferiesVWAPParams(baseOrder, "10:00:00 US/Eastern", "16:00:00 US/Eastern", 10, 10, "Exclude_Both", 130, 135, 1, 10, "Patience", false, "Midpoint");
  m_pClient->placeOrder(m_orderId++, ContractSamples::JefferiesContract(), baseOrder);
  //! [jeff_vwap_algo]

  //! [csfb_inline_algo]
  AvailableAlgoParams::FillCSFBInlineParams(baseOrder, "10:00:00 US/Eastern", "16:00:00 US/Eastern", "Patient", 10, 20, 100, "Default", false, 40, 100, 100, 35);
  m_pClient->placeOrder(m_orderId++, ContractSamples::CSFBContract(), baseOrder);
  //! [csfb_inline_algo]

  m_state = ST_TESTALGOSAMPLES_ACK;
}

void TestCppClient::financialAdvisorOrderSamples()
{
  //! [faorderoneaccount]
  Order faOrderOneAccount = OrderSamples::MarketOrder("BUY", DecimalFunctions::stringToDecimal("100"));
  // Specify the Account Number directly
  faOrderOneAccount.account = "DU119915";
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), faOrderOneAccount);
  //! [faorderoneaccount]
  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [faordergroup]
  Order faOrderGroup = OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("200"), 10);
  faOrderGroup.faGroup = "MyTestGroup1";
  faOrderGroup.faMethod = "AvailableEquity";
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), faOrderGroup);
  //! [faordergroup]
  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [faorderuserdefinedgroup]
  Order faOrderUserDefinedGroup = OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("200"), 10);
  faOrderUserDefinedGroup.faGroup = "MyTestProfile1";
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), faOrderUserDefinedGroup);
  //! [faorderuserdefinedgroup]
  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [modelorder]
  Order modelOrder = OrderSamples::LimitOrder("BUY", DecimalFunctions::stringToDecimal("200"), 100);
  modelOrder.account = "DF12345";
  modelOrder.modelCode = "Technology";
  m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), modelOrder);
  //! [modelorder]

  m_state = ST_FAORDERSAMPLES_ACK;
}

void TestCppClient::financialAdvisorOperations()
{
  /*** Requesting FA information ***/
  //! [requestfaaliases]
  m_pClient->requestFA(faDataType::ALIASES);
  //! [requestfaaliases]

  //! [requestfagroups]
  m_pClient->requestFA(faDataType::GROUPS);
  //! [requestfagroups]

  /*** Replacing FA information - Fill in with the appropriate XML string. ***/
  //! [replacefaupdatedgroup]
  m_pClient->replaceFA(1000, faDataType::GROUPS, FAMethodSamples::FAUpdatedGroup());
  //! [replacefaupdatedgroup]

  //! [reqSoftDollarTiers]
  m_pClient->reqSoftDollarTiers(4001);
  //! [reqSoftDollarTiers]

  m_state = ST_FAOPERATIONS_ACK;
}

void TestCppClient::testDisplayGroups(){
  //! [querydisplaygroups]
  m_pClient->queryDisplayGroups(9001);
  //! [querydisplaygroups]

  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [subscribetogroupevents]
  m_pClient->subscribeToGroupEvents(9002, 1);
  //! [subscribetogroupevents]

  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [updatedisplaygroup]
  m_pClient->updateDisplayGroup(9002, "8314@SMART");
  //! [updatedisplaygroup]

  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [subscribefromgroupevents]
  m_pClient->unsubscribeFromGroupEvents(9002);
  //! [subscribefromgroupevents]

  m_state = ST_TICKDATAOPERATION_ACK;
}

void TestCppClient::miscellaneous()
{
  /*** Request TWS' current time ***/
  m_pClient->reqCurrentTime();

  std::this_thread::sleep_for(std::chrono::seconds(3));

  /*** Request TWS' current time in millis ***/
  m_pClient->reqCurrentTimeInMillis();

  /*** Setting TWS logging level  ***/
  m_pClient->setServerLogLevel(5);

  m_state = ST_MISCELANEOUS_ACK;
}

void TestCppClient::reqFamilyCodes()
{
  /*** Request TWS' family codes ***/
  //! [reqfamilycodes]
  m_pClient->reqFamilyCodes();
  //! [reqfamilycodes]

  m_state = ST_FAMILYCODES_ACK;
}

void TestCppClient::reqMatchingSymbols()
{
  /*** Request TWS' matching symbols ***/
  //! [reqmatchingsymbols]
  m_pClient->reqMatchingSymbols(11001, "IBM");
  //! [reqmatchingsymbols]
  m_state = ST_SYMBOLSAMPLES_ACK;
}

void TestCppClient::reqMktDepthExchanges()
{
  /*** Request TWS' market depth exchanges ***/
  //! [reqMktDepthExchanges]
  m_pClient->reqMktDepthExchanges();
  //! [reqMktDepthExchanges]

  m_state = ST_REQMKTDEPTHEXCHANGES_ACK;
}

void TestCppClient::reqNewsTicks()
{
  //! [reqmktdata_ticknews]
  m_pClient->reqMktData(12001, ContractSamples::USStockAtSmart(), "mdoff,292", false, false, TagValueListSPtr());
  //! [reqmktdata_ticknews]

  std::this_thread::sleep_for(std::chrono::seconds(5));

  //! [cancelmktdata2]
  m_pClient->cancelMktData(12001);
  //! [cancelmktdata2]

  m_state = ST_REQNEWSTICKS_ACK;
}

void TestCppClient::reqSmartComponents()
{
  static bool bFirstRun = true;

  if (bFirstRun) {
    m_pClient->reqMktData(13001, ContractSamples::USStockAtSmart(), "", false, false, TagValueListSPtr());

    bFirstRun = false;
  }

  std::this_thread::sleep_for(std::chrono::seconds(5));

  if (m_bboExchange.size() > 0) {
    m_pClient->cancelMktData(13001);

    //! [reqsmartcomponents]
    m_pClient->reqSmartComponents(13002, m_bboExchange);
    //! [reqsmartcomponents]
    m_state = ST_REQSMARTCOMPONENTS_ACK;
  }
}

void TestCppClient::reqNewsProviders()
{
  /*** Request TWS' news providers ***/
  //! [reqNewsProviders]
  m_pClient->reqNewsProviders();
  //! [reqNewsProviders]

  m_state = ST_NEWSPROVIDERS_ACK;
}

void TestCppClient::reqNewsArticle()
{
  /*** Request TWS' news article ***/
  //! [reqNewsArticle]
  TagValueList* list = new TagValueList();
  // list->push_back((TagValueSPtr)new TagValue("manual", "1"));
  m_pClient->reqNewsArticle(12001, "MST", "MST$06f53098", TagValueListSPtr(list));
  //! [reqNewsArticle]

  m_state = ST_REQNEWSARTICLE_ACK;
}

void TestCppClient::reqHistoricalNews(){

  //! [reqHistoricalNews]
  TagValueList* list = new TagValueList();
  list->push_back((TagValueSPtr)new TagValue("manual", "1"));
  m_pClient->reqHistoricalNews(12001, 8314, "BZ+FLY", "", "", 5, TagValueListSPtr(list));
  //! [reqHistoricalNews]

  std::this_thread::sleep_for(std::chrono::seconds(1));

  m_state = ST_REQHISTORICALNEWS_ACK;
}


void TestCppClient::reqHeadTimestamp() {
  //! [reqHeadTimeStamp]
  m_pClient->reqHeadTimestamp(14001, ContractSamples::EurGbpFx(), "MIDPOINT", 1, 1);
  //! [reqHeadTimeStamp]
  std::this_thread::sleep_for(std::chrono::seconds(1));
  //! [cancelHeadTimestamp]
  m_pClient->cancelHeadTimestamp(14001);
  //! [cancelHeadTimestamp]

  m_state = ST_REQHEADTIMESTAMP_ACK;
}

void TestCppClient::reqHistogramData() {
  //! [reqHistogramData]
  m_pClient->reqHistogramData(15001, ContractSamples::IBMUSStockAtSmart(), false, "1 weeks");
  //! [reqHistogramData]
  std::this_thread::sleep_for(std::chrono::seconds(2));
  //! [cancelHistogramData]
  m_pClient->cancelHistogramData(15001);
  //! [cancelHistogramData]
  m_state = ST_REQHISTOGRAMDATA_ACK;
}

void TestCppClient::rerouteCFDOperations()
{
  //! [reqmktdatacfd]
  m_pClient->reqMktData(16001, ContractSamples::USStockCFD(), "", false, false, TagValueListSPtr());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  m_pClient->reqMktData(16002, ContractSamples::EuropeanStockCFD(), "", false, false, TagValueListSPtr());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  m_pClient->reqMktData(16003, ContractSamples::CashCFD(), "", false, false, TagValueListSPtr());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  //! [reqmktdatacfd]

  //! [reqmktdepthcfd]
  m_pClient->reqMktDepth(16004, ContractSamples::USStockCFD(), 10, false, TagValueListSPtr());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  m_pClient->reqMktDepth(16005, ContractSamples::EuropeanStockCFD(), 10, false, TagValueListSPtr());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  m_pClient->reqMktDepth(16006, ContractSamples::CashCFD(), 10, false, TagValueListSPtr());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  //! [reqmktdepthcfd]

  m_state = ST_REROUTECFD_ACK;
}

void TestCppClient::marketRuleOperations()
{
  m_pClient->reqContractDetails(17001, ContractSamples::IBMBond());
  m_pClient->reqContractDetails(17002, ContractSamples::IBKRStk());

  std::this_thread::sleep_for(std::chrono::seconds(2));

  //! [reqmarketrule]
  m_pClient->reqMarketRule(26);
  m_pClient->reqMarketRule(635);
  m_pClient->reqMarketRule(1388);
  //! [reqmarketrule]

  m_state = ST_MARKETRULE_ACK;
}

void TestCppClient::continuousFuturesOperations()
{
  m_pClient->reqContractDetails(18001, ContractSamples::ContFut());

  //! [reqhistoricaldatacontfut]
  m_pClient->reqHistoricalData(18002, ContractSamples::ContFut(), "", "1 Y", "1 month", "TRADES", 0, 1, false, TagValueListSPtr());

  std::this_thread::sleep_for(std::chrono::seconds(10));

  m_pClient->cancelHistoricalData(18002);
  //! [reqhistoricaldatacontfut]

  m_state = ST_CONTFUT_ACK;
}

void TestCppClient::reqHistoricalTicks()
{
  //! [reqhistoricalticks]
  m_pClient->reqHistoricalTicks(19001, ContractSamples::IBMUSStockAtSmart(), "20170621 09:38:33 US/Eastern", "", 10, "BID_ASK", 1, true, TagValueListSPtr());
  m_pClient->reqHistoricalTicks(19002, ContractSamples::IBMUSStockAtSmart(), "20170621 09:38:33 US/Eastern", "", 10, "MIDPOINT", 1, true, TagValueListSPtr());
  m_pClient->reqHistoricalTicks(19003, ContractSamples::IBMUSStockAtSmart(), "20170621 09:38:33 US/Eastern", "", 10, "TRADES", 1, true, TagValueListSPtr());
  //! [reqhistoricalticks]

  //! [cancelhistoricalticks]
  m_pClient->cancelHistoricalTicks(19001);
  //! [cancelhistoricalticks]

  m_state = ST_REQHISTORICALTICKS_ACK;
}

void TestCppClient::reqTickByTickData()
{
  /*** Requesting tick-by-tick data (only refresh) ***/

  m_pClient->reqTickByTickData(20001, ContractSamples::EuropeanStock(), "Last", 0, false);
  m_pClient->reqTickByTickData(20002, ContractSamples::EuropeanStock(), "AllLast", 0, false);
  m_pClient->reqTickByTickData(20003, ContractSamples::EuropeanStock(), "BidAsk", 0, true);
  m_pClient->reqTickByTickData(20004, ContractSamples::EurGbpFx(), "MidPoint", 0, false);

  std::this_thread::sleep_for(std::chrono::seconds(10));

  //! [canceltickbytick]
  m_pClient->cancelTickByTickData(20001);
  m_pClient->cancelTickByTickData(20002);
  m_pClient->cancelTickByTickData(20003);
  m_pClient->cancelTickByTickData(20004);
  //! [canceltickbytick]

  /*** Requesting tick-by-tick data (historical + refresh) ***/
  //! [reqtickbytick]
  m_pClient->reqTickByTickData(20005, ContractSamples::EuropeanStock(), "Last", 10, false);
  m_pClient->reqTickByTickData(20006, ContractSamples::EuropeanStock(), "AllLast", 10, false);
  m_pClient->reqTickByTickData(20007, ContractSamples::EuropeanStock(), "BidAsk", 10, false);
  m_pClient->reqTickByTickData(20008, ContractSamples::EurGbpFx(), "MidPoint", 10, true);
  //! [reqtickbytick]

  std::this_thread::sleep_for(std::chrono::seconds(10));

  m_pClient->cancelTickByTickData(20005);
  m_pClient->cancelTickByTickData(20006);
  m_pClient->cancelTickByTickData(20007);
  m_pClient->cancelTickByTickData(20008);

  m_state = ST_REQTICKBYTICKDATA_ACK;
}

void TestCppClient::whatIfSamples()
{
  /*** Placing waht-if order ***/
  //! [whatiforder]
  m_pClient->placeOrder(m_orderId++, ContractSamples::BondWithCusip(), OrderSamples::WhatIfLimitOrder("BUY", DecimalFunctions::stringToDecimal("100"), 20));
  //! [whatiforder]

  m_state = ST_WHATIFSAMPLES_ACK;
}

void TestCppClient::ibkratsSample(){
  //! [ibkratssubmit]
  Order ibkratsOrder = OrderSamples::LimitIBKRATS("BUY", DecimalFunctions::stringToDecimal("100"), 330);
  m_pClient->placeOrder(m_orderId++, ContractSamples::IBKRATSContract(), ibkratsOrder);
  //! [ibkratssubmit]

  m_state = ST_IBKRATSSAMPLE_ACK;
}

void TestCppClient::wshCalendarOperations() {
  //! [reqmetadata]
  m_pClient->reqWshMetaData(30001);
  //! [reqmetadata]

  std::this_thread::sleep_for(std::chrono::seconds(10));

  m_pClient->cancelWshMetaData(30001);

  //! [reqeventdata]
  m_pClient->reqWshEventData(30002, WshEventData(8314, false, false, false, "20220511", "", 5));
  //! [reqeventdata]

  std::this_thread::sleep_for(std::chrono::seconds(3));

  //! [reqeventdata]
  m_pClient->reqWshEventData(30003, WshEventData("{\"watchlist\":[\"8314\"]}", false, false, false, "", "20220512", INT_MAX));
  //! [reqeventdata]

  std::this_thread::sleep_for(std::chrono::seconds(10));

  m_pClient->cancelWshEventData(30002);
  m_pClient->cancelWshEventData(30003);

  m_state = ST_WSH_ACK;
}

void TestCppClient::configOperations() {
#if !defined(USE_WIN_DLL)
  //! [request_config]
  protobuf::ConfigRequest configRequestProto;
  configRequestProto.set_reqid(20001);
  m_pClient->reqConfigProtoBuf(configRequestProto);
  //! [request_config]

  //! [update_api_settings_config_request]
  m_pClient->updateConfigProtoBuf(ConfigSamples::UpdateConfigApiSettings(20002));
  //! [update_api_settings_config_request]

  //! [update_orders_config_request]
  m_pClient->updateConfigProtoBuf(ConfigSamples::UpdateOrdersConfig(20003));
  //! [update_orders_config_request]

  //! [update_message_config_request]
  m_pClient->updateConfigProtoBuf(ConfigSamples::UpdateMessageConfigConfirmMandatoryCapPriceAccepted(20004));
  //! [update_message_config_request]

  //! [update_config_request_order_id_reset]
  m_pClient->updateConfigProtoBuf(ConfigSamples::UpdateConfigOrderIdReset(20005));
  //! [update_config_request_order_id_reset]
#endif

  m_state = ST_CONFIG_ACK;
}

void TestCppClient::orderParentChildOperations()
{
#if !defined(USE_WIN_DLL)
  //! [beta_hedge_order]
  int parentOrderId = m_orderId++;
  int childOrderId = m_orderId++;
  m_pClient->placeOrderProtoBuf(OrderSamplesProto::CreatePlaceOrderRequest(parentOrderId, ContractSamplesProto::IBMStockAtSmart(), OrderSamplesProto::LimitOrder("BUY", DecimalFunctions::stringToDecimal("100"), 40, false)));
  std::this_thread::sleep_for(std::chrono::seconds(1));
  m_pClient->placeOrderProtoBuf(OrderSamplesProto::CreatePlaceOrderRequest(childOrderId, ContractSamplesProto::MSFTStockAtSmart(), OrderSamplesProto::BetaHedgeOrder(parentOrderId, "SELL", "0.05", 75, true)));
  //! [beta_hedge_order]
#endif

  m_state = ST_ORDER_PARENTCHILD_OPERATIONS_ACK;
}

void TestCppClient::newsOperationsProto()
{
#if !defined(USE_WIN_DLL)
  //! [request_historical_news_with_end_time]
  m_pClient->reqHistoricalNewsProtoBuf(NewsSamplesProto::HistoricalNewsRequestWithEndTime(10001));
  //! [request_historical_news_with_end_time]

  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [request_historical_news_with_start_time]
  m_pClient->reqHistoricalNewsProtoBuf(NewsSamplesProto::HistoricalNewsRequestWithStartTime(10002));
  //! [request_historical_news_with_start_time]
#endif

  m_state = ST_NEWS_OPERATIONS_PROTO_ACK;
}

void TestCppClient::tickDataOperationsProto()
{
#if !defined(USE_WIN_DLL)
  //! [request_odd_lot_bid_ask_quotes]
  m_pClient->reqMarketDataProtoBuf(MarketDataSamplesProto::OddLotBidAskQuotesRequest(20001, ContractSamplesProto::IBMStockAtSmart()));
  //! [request_odd_lot_bid_ask_quotes]

  std::this_thread::sleep_for(std::chrono::seconds(10));

  //! [cancel_market_data]
  m_pClient->cancelMarketDataProtoBuf(MarketDataSamplesProto::CancelMarketDataRequest(20001));
  //! [cancel_market_data]

  std::this_thread::sleep_for(std::chrono::seconds(1));

  //! [request_regulatory_snapshot]
  m_pClient->reqMarketDataProtoBuf(MarketDataSamplesProto::RegulatorySnapshotRequest(20002, ContractSamplesProto::IBMStockAtSmart()));
  //! [request_regulatory_snapshot]

  std::this_thread::sleep_for(std::chrono::seconds(10));
#endif

  m_state = ST_TICK_DATA_OPERATIONS_PROTO_ACK;
}

//! [nextvalidid]
void TestCppClient::nextValidId(int orderId) {
  printf("Next Valid Id: %d\n", orderId);
  m_orderId = orderId;
  //! [nextvalidid]

  //m_state = ST_TICKOPTIONCOMPUTATIONOPERATION;
  //m_state = ST_TICKDATAOPERATION;
  //m_state = ST_OPTIONSOPERATIONS;
  //m_state = ST_REQTICKBYTICKDATA;
  //m_state = ST_REQHISTORICALTICKS;
  //m_state = ST_CONTFUT;
  //m_state = ST_PNLSINGLE;
  //m_state = ST_PNL;
  //m_state = ST_DELAYEDTICKDATAOPERATION;
  //m_state = ST_MARKETDEPTHOPERATION;
  //m_state = ST_REALTIMEBARS;
  //m_state = ST_MARKETDATATYPE;
  //m_state = ST_HISTORICALDATAREQUESTS;
  //m_state = ST_CONTRACTOPERATION;
  //m_state = ST_MARKETSCANNERS;
  //m_state = ST_FUNDAMENTALS;
  //m_state = ST_BULLETINS;
  //m_state = ST_ACCOUNTOPERATIONS;
  //m_state = ST_ORDEROPERATIONS;
  //m_state = ST_OCASAMPLES;
  //m_state = ST_CONDITIONSAMPLES;
  //m_state = ST_BRACKETSAMPLES;
  //m_state = ST_HEDGESAMPLES;
  //m_state = ST_TESTALGOSAMPLES;
  //m_state = ST_FAORDERSAMPLES;
  //m_state = ST_FAOPERATIONS;
  //m_state = ST_DISPLAYGROUPS;
  //m_state = ST_MISCELANEOUS;
  //m_state = ST_FAMILYCODES;
  //m_state = ST_SYMBOLSAMPLES;
  //m_state = ST_REQMKTDEPTHEXCHANGES;
  //m_state = ST_REQNEWSTICKS;
  //m_state = ST_REQSMARTCOMPONENTS;
  //m_state = ST_NEWSPROVIDERS;
  //m_state = ST_REQNEWSARTICLE;
  //m_state = ST_REQHISTORICALNEWS;
  //m_state = ST_REQHEADTIMESTAMP;
  //m_state = ST_REQHISTOGRAMDATA;
  //m_state = ST_REROUTECFD;
  //m_state = ST_MARKETRULE;
  //m_state = ST_PING;
  //m_state = ST_WHATIFSAMPLES;
  //m_state = ST_WSH;
  //m_state = ST_CONFIG;
  //m_state = ST_ORDER_PARENTCHILD_OPERATIONS;
  //m_state = ST_NEWS_OPERATIONS_PROTO;
  m_state = ST_TICK_DATA_OPERATIONS_PROTO;
}


void TestCppClient::currentTime( long long time) {
  time_t t = (time_t)time;
  struct tm timeinfo;
  char currentTime[80];

#if defined(IB_WIN32)
  localtime_s(&timeinfo, &t);
#else
  localtime_r(&t, &timeinfo);
#endif
  strftime(currentTime, sizeof(currentTime), "%b %d, %Y %H:%M:%S", &timeinfo);
  printf("The current date/time is %llu : %s\n", static_cast<unsigned long long>(t), currentTime);

  if ( m_state == ST_PING_ACK) {

    time_t now = ::time(NULL);
    m_sleepDeadline = now + SLEEP_BETWEEN_PINGS;

    m_state = ST_PING_ACK;
  }
}

//! [error]
void TestCppClient::error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson)
{
  char errorTimeStr[80];
  if (errorTime > 0) {
#if defined(IB_WIN32)
    ctime_s(errorTimeStr, sizeof(errorTimeStr), &(errorTime /= 1000));
#else
    ctime_r(&(errorTime /= 1000), errorTimeStr);
#endif
    errorTimeStr[strlen(errorTimeStr) - 1] = '\0';
  }
  else {
    errorTimeStr[0] = '\0';
  }
  std::string msg = {""};

  if (!advancedOrderRejectJson.empty()) {
    // printf("Error. Id: %d, Time: %s, Code: %d, Msg: %s, AdvancedOrderRejectJson: %s\n", id, errorTimeStr, errorCode, errorString.c_str(), advancedOrderRejectJson.c_str());

    std::stringstream ss;

    ss << "Error. Id:" << " " << id << " Time:" << errorTimeStr << " Code:" <<  errorCode << " Msg:" << errorString << " AdvancedOrderRejectJson:" << advancedOrderRejectJson << std::endl;
    msg = ss.str();

  } else {
    // printf("Error. Id: %d, Time: %s, Code: %d, Msg: %s\n", id, errorTimeStr, errorCode, errorString.c_str());
    std::stringstream ss;

    ss << "Error. Id:" << " " << id << " Time:" << errorTimeStr << " Code:" <<  errorCode << " Msg:" << errorString << " AdvancedOrderRejectJson:" << advancedOrderRejectJson << std::endl;
    msg = ss.str();

  }

  tmux_management.send_msg_to_last_ttys(std::move(msg));

}
//! [error]

void TestCppClient::tickPrice(int reqId, TickType field, double price, const TickAttrib& attribs) {

  // std::cout << "reqId:" << reqId << std::endl;
  switch (field) {
    case LAST: {
                 // std::cout << "Last Price: " << reqId << ": " << price << "\n";
                 current_price_list_[reqId].last_price_ = price;
               }
               break;
    case BID: {
                // std::cout << "Bid Price: " << price << "\n";
                current_price_list_[reqId].bid_price_ = price;
              }
              break;
    case ASK: {

                // std::cout << "Ask Price: " << price << "\n";
                current_price_list_[reqId].ask_price_ = price;

              }
              break;
    default:
              break;
  }



}
void TestCppClient::tickSize(int reqId, TickType field, Decimal size) {

  //std::cout << "reqId:" << reqId << std::endl;
  //std::cout << "reqId:" << reqId << " field:" << field << " " << size << std::endl;

}
void TestCppClient::tickOptionComputation(int reqId, TickType tickType, int tickAttrib, double impliedVol, double delta, double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) {}
void TestCppClient::tickGeneric(int reqId, TickType tickType, double value) {

  //  std::cout << "tick price trigger" << std::endl;
}
void TestCppClient::tickString(int reqId, TickType tickType, const std::string& value) {}
void TestCppClient::tickEFP(int reqId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints, double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) {}
void TestCppClient::orderStatus(int orderId, 
    const std::string& status, 
    Decimal filled, 
    Decimal remaining, 
    double avgFillPrice, 
    long long permId, 
    int parentId, 
    double lastFillPrice, 
    int clientId, 
    const std::string& whyHeld, 
    double mktCapPrice) {


     std::cout << "Order Status Update -> "
              << " ID: " << orderId
              << " | Status: " << status
              << " | Filled: " << std::to_string(filled)
              << " | Remaining: " << std::to_string(remaining)
              << " | Avg Price: " << avgFillPrice 
              << std::endl;


}

void TestCppClient::openOrder(int orderId, const Contract& contract, const Order& order, const OrderState& orderState) {

  //  std::cout << "here are the open orders:" << orderId << std::endl;

  //printf("UpdatePortfolio. %s, %s @ %s: 
  //Position: %s, MarketPrice: %s, MarketValue: %s, AverageCost: %s, UnrealizedPNL: %s, RealizedPNL: %s, AccountName: %s\n",

}
void TestCppClient::openOrderEnd() {
  req_open_oder_cond_var_trigger_ = true;
  req_open_oder_end_cond_var_.notify_one();
  std::cerr << "open order ends" << std::endl;
}

void TestCppClient::winError( const std::string& str, int lastError) {}
void TestCppClient::connectionClosed() {
  printf( "Connection Closed\n");
}

//! [updateaccountvalue]
void TestCppClient::updateAccountValue(const std::string& key, const std::string& val,
    const std::string& currency, const std::string& accountName) {
  // printf("UpdateAccountValue. Key: %s, Value: %s, Currency: %s, Account Name: %s\n", key.c_str(), val.c_str(), currency.c_str(), accountName.c_str());

  std::cout << "UPDATING ACCOUNT VALUE:" << std::endl;

  if (key.contains( "EquityWithLoanValue")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.EquityWithLoanValue_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "NetLiquidation")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.NetLiquidation_ = std::stod(val);
    // Do something with buyingpower
  }



  if (key.contains( "BuyingPower")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.BuyingPower_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "GrossPositionValue")) {
    // val contains the maximum dollar value of securities you can buy
    //   current_account_state_.GrossPositionValue_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "InitMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.InitMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "MaintMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.MaintMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "AvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.AvailableFunds_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "ExcessLiquidity")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.ExcessLiquidity_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "LookAheadInitMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadInitMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "LookAheadMaintMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadMaintMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "LookAheadAvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadAvailableFunds_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "LookAheadExcessLiquidity")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadExcessLiquidity_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "FullInitMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullInitMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "FullAvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullAvailableFunds_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "FullExcessLiquidity")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullExcessLiquidity_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "Cushion")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.Cushion_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "EquityWithLoanValue")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.EquityWithLoanValue_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "NetLiquidation")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.NetLiquidation_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "BuyingPower")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.BuyingPower_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "GrossPositionValue")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.GrossPositionValue_ = std::stod(val);
    // Do something with buyingpower
  }


  if (key.contains( "InitMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.InitMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "MaintMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.MaintMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "AvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.AvailableFunds_ = std::stod(val);
    // Do something with buyingpower
  }
  if (key.contains( "MaintMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.MaintMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }
  if (key.contains( "ExcessLiquidity")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.ExcessLiquidity_ = std::stod(val);
    // Do something with buyingpower
  }
  if (key.contains( "LookAheadInitMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadInitMarginReq+ = std::stod(val);
    // Do something with buyingpower
  }
  if (key.contains( "LookAheadMaintMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadMaintMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "LookAheadAvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadAvailableFunds_ = std::stod(val);
    // Do something with buyingpower_
  }

  if (key.contains( "LookAheadExcessLiquidity")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadExcessLiquidity_ = std::stod(val);
    // Do something with buyingpower
  }if (key.contains( "FullInitMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullInitMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }if (key.contains( "LookAheadAvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadAvailableFunds_ = std::stod(val);
    // Do something with buyingpower
  }if (key.contains( "FullMaintMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullMaintMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }if (key.contains( "LookAheadAvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadAvailableFunds_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "LookAheadExcessLiquidity")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.LookAheadExcessLiquidity_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "FullInitMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullInitMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "FullMaintMarginReq")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullMaintMarginReq_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "FullAvailableFunds")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullAvailableFunds_ = std::stod(val);
    // Do something with buyingpower
  }

  if (key.contains( "FullExcessLiquidity")) {
    // val contains the maximum dollar value of securities you can buy
    //    current_account_state_.FullExcessLiquidity_ = std::stod(val);
    // Do something with buyingpower
  }
  if (key.contains("SMA")) {
    // val contains the maximum dollar value of securities you can buy
    current_account_state_.SMA_ = std::stod(val);
    // Do something with buyingpower
  }






}
//! [updateaccountvalue]

//! [updateportfolio]
void TestCppClient::updatePortfolio(const Contract& contract, Decimal position,
    double marketPrice, double marketValue, double averageCost,
    double unrealizedPNL, double realizedPNL, const std::string& accountName){
  //printf("UpdatePortfolio. %s, %s @ %s: Position: %s, MarketPrice: %s, MarketValue: %s, AverageCost: %s, UnrealizedPNL: %s, RealizedPNL: %s, AccountName: %s\n",
  //    (contract.symbol).c_str(), (contract.secType).c_str(), (contract.exchange).c_str(), DecimalFunctions::decimalStringToDisplay(position).c_str(),
  //    Utils::doubleMaxString(marketPrice).c_str(), Utils::doubleMaxString(marketValue).c_str(), Utils::doubleMaxString(averageCost).c_str(),
  //    Utils::doubleMaxString(unrealizedPNL).c_str(), Utils::doubleMaxString(realizedPNL).c_str(), accountName.c_str());
}
//! [updateportfolio]

//! [updateaccounttime]
void TestCppClient::updateAccountTime(const std::string& timeStamp) {
  // printf( "UpdateAccountTime. Time: %s\n", timeStamp.c_str());
}
//! [updateaccounttime]

//! [accountdownloadend]
void TestCppClient::accountDownloadEnd(const std::string& accountName) {
  printf( "Account download finished: %s\n", accountName.c_str());

  //request_mutex_.unlock();
}
//! [accountdownloadend]

//! [contractdetails]
void TestCppClient::contractDetails( int reqId, const ContractDetails& contractDetails) {
  printf( "ContractDetails begin. ReqId: %d\n", reqId);
  printContractMsg(contractDetails.contract);
  printContractDetailsMsg(contractDetails);
  printf( "ContractDetails end. ReqId: %d\n", reqId);
}
//! [contractdetails]

//! [bondcontractdetails]
void TestCppClient::bondContractDetails( int reqId, const ContractDetails& contractDetails) {
  printf( "BondContractDetails begin. ReqId: %d\n", reqId);
  printBondContractDetailsMsg(contractDetails);
  printf( "BondContractDetails end. ReqId: %d\n", reqId);
}
//! [bondcontractdetails]

void TestCppClient::printContractMsg(const Contract& contract) {
  printf("\tConId: %d\n", contract.conId);
  printf("\tSymbol: %s\n", contract.symbol.c_str());
  printf("\tSecType: %s\n", contract.secType.c_str());
  printf("\tLastTradeDateOrContractMonth: %s\n", contract.lastTradeDateOrContractMonth.c_str());
  printf("\tLastTradeDate: %s\n", contract.lastTradeDate.c_str());
  printf("\tStrike: %s\n", Utils::doubleMaxString(contract.strike).c_str());
  printf("\tRight: %s\n", contract.right.c_str());
  printf("\tMultiplier: %s\n", contract.multiplier.c_str());
  printf("\tExchange: %s\n", contract.exchange.c_str());
  printf("\tPrimaryExchange: %s\n", contract.primaryExchange.c_str());
  printf("\tCurrency: %s\n", contract.currency.c_str());
  printf("\tLocalSymbol: %s\n", contract.localSymbol.c_str());
  printf("\tTradingClass: %s\n", contract.tradingClass.c_str());
}

void TestCppClient::printContractDetailsMsg(const ContractDetails& contractDetails) {
  printf("\tMarketName: %s\n", contractDetails.marketName.c_str());
  printf("\tMinTick: %s\n", Utils::doubleMaxString(contractDetails.minTick).c_str());
  printf("\tPriceMagnifier: %s\n", Utils::intMaxString(contractDetails.priceMagnifier).c_str());
  printf("\tOrderTypes: %s\n", contractDetails.orderTypes.c_str());
  printf("\tValidExchanges: %s\n", contractDetails.validExchanges.c_str());
  printf("\tUnderConId: %s\n", Utils::intMaxString(contractDetails.underConId).c_str());
  printf("\tLongName: %s\n", contractDetails.longName.c_str());
  printf("\tContractMonth: %s\n", contractDetails.contractMonth.c_str());
  printf("\tIndystry: %s\n", contractDetails.industry.c_str());
  printf("\tCategory: %s\n", contractDetails.category.c_str());
  printf("\tSubCategory: %s\n", contractDetails.subcategory.c_str());
  printf("\tTimeZoneId: %s\n", contractDetails.timeZoneId.c_str());
  printf("\tTradingHours: %s\n", contractDetails.tradingHours.c_str());
  printf("\tLiquidHours: %s\n", contractDetails.liquidHours.c_str());
  printf("\tEvRule: %s\n", contractDetails.evRule.c_str());
  printf("\tEvMultiplier: %s\n", Utils::doubleMaxString(contractDetails.evMultiplier).c_str());
  printf("\tAggGroup: %s\n", Utils::intMaxString(contractDetails.aggGroup).c_str());
  printf("\tUnderSymbol: %s\n", contractDetails.underSymbol.c_str());
  printf("\tUnderSecType: %s\n", contractDetails.underSecType.c_str());
  printf("\tMarketRuleIds: %s\n", contractDetails.marketRuleIds.c_str());
  printf("\tRealExpirationDate: %s\n", contractDetails.realExpirationDate.c_str());
  printf("\tLastTradeTime: %s\n", contractDetails.lastTradeTime.c_str());
  printf("\tStockType: %s\n", contractDetails.stockType.c_str());
  printf("\tMinSize: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.minSize).c_str());
  printf("\tSizeIncrement: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.sizeIncrement).c_str());
  printf("\tSuggestedSizeIncrement: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.suggestedSizeIncrement).c_str());
  printf("\tMinAlgoSize: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.minAlgoSize).c_str());
  printf("\tLastPricePrecision: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.lastPricePrecision).c_str());
  printf("\tLastSizePrecision: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.lastSizePrecision).c_str());
  printf("\tEventContract1: %s\n", contractDetails.eventContract1.c_str());
  printf("\tEventContractDescription1: %s\n", contractDetails.eventContractDescription1.c_str());
  printf("\tEventContractDescription2: %s\n", contractDetails.eventContractDescription2.c_str());
  if (contractDetails.contract.secType == "FUND") {
    printf("\tFund Data: \n");
    printf("\t\tFundName: %s\n", contractDetails.fundName.c_str());
    printf("\t\tFundFamily: %s\n", contractDetails.fundFamily.c_str());
    printf("\t\tFundType: %s\n", contractDetails.fundType.c_str());
    printf("\t\tFundFrontLoad: %s\n", contractDetails.fundFrontLoad.c_str());
    printf("\t\tFundBackLoad: %s\n", contractDetails.fundBackLoad.c_str());
    printf("\t\tFundBackLoadTimeInterval: %s\n", contractDetails.fundBackLoadTimeInterval.c_str());
    printf("\t\tFundManagementFee: %s\n", contractDetails.fundManagementFee.c_str());
    printf("\t\tFundClosed: %s\n", contractDetails.fundClosed ? "yes" : "no");
    printf("\t\tFundClosedForNewInvestors: %s\n", contractDetails.fundClosedForNewInvestors ? "yes" : "no");
    printf("\t\tFundClosedForNewMoney: %s\n", contractDetails.fundClosedForNewMoney ? "yes" : "no");
    printf("\t\tFundNotifyAmount: %s\n", contractDetails.fundNotifyAmount.c_str());
    printf("\t\tFundMinimumInitialPurchase: %s\n", contractDetails.fundMinimumInitialPurchase.c_str());
    printf("\t\tFundSubsequentMinimumPurchase: %s\n", contractDetails.fundSubsequentMinimumPurchase.c_str());
    printf("\t\tFundBlueSkyStates: %s\n", contractDetails.fundBlueSkyStates.c_str());
    printf("\t\tFundBlueSkyTerritories: %s\n", contractDetails.fundBlueSkyTerritories.c_str());
    printf("\t\tFundDistributionPolicyIndicator: %s\n", Utils::getFundDistributionPolicyIndicatorName(contractDetails.fundDistributionPolicyIndicator).c_str());
    printf("\t\tFundAssetType: %s\n", Utils::getFundAssetTypeName(contractDetails.fundAssetType).c_str());
  }
  printContractDetailsSecIdList(contractDetails.secIdList);
  printContractDetailsIneligibilityReasonList(contractDetails.ineligibilityReasonList);
}

void TestCppClient::printContractDetailsSecIdList(const TagValueListSPtr &secIdList) {
  const size_t secIdListCount = secIdList.get() ? secIdList->size() : 0;
  if (secIdListCount > 0) {
    printf("\tSecIdList: {");
    for (size_t i = 0; i < secIdListCount; ++i) {
      const TagValue* tagValue = ((*secIdList)[i]).get();
      printf("%s=%s;",tagValue->tag.c_str(), tagValue->value.c_str());
    }
    printf("}\n");
  }
}

void TestCppClient::printSoftDollarTier(const SoftDollarTier& softDollarTier) {
  printf("\tSoftDollarTier:{name=%s, val=%s, displayName=%s}\n", softDollarTier.name().c_str(), softDollarTier.val().c_str(), softDollarTier.displayName().c_str());
}

void TestCppClient::printContractDetailsIneligibilityReasonList(const IneligibilityReasonListSPtr &ineligibilityReasonList) {
  const size_t ineligibilityReasonListCount = ineligibilityReasonList.get() ? ineligibilityReasonList->size() : 0;
  if (ineligibilityReasonListCount > 0) {
    printf("\tIneligibilityReasonList: {");
    for (size_t i = 0; i < ineligibilityReasonListCount; ++i) {
      const IneligibilityReason* ineligibilityReason = ((*ineligibilityReasonList)[i]).get();
      printf("[id: %s, description: %s];", ineligibilityReason->id.c_str(), ineligibilityReason->description.c_str());
    }
    printf("}\n");
  }
}

void TestCppClient::printBondContractDetailsMsg(const ContractDetails& contractDetails) {
  printf("\tSymbol: %s\n", contractDetails.contract.symbol.c_str());
  printf("\tSecType: %s\n", contractDetails.contract.secType.c_str());
  printf("\tCusip: %s\n", contractDetails.cusip.c_str());
  printf("\tCoupon: %s\n", Utils::doubleMaxString(contractDetails.coupon).c_str());
  printf("\tMaturity: %s\n", contractDetails.maturity.c_str());
  printf("\tIssueDate: %s\n", contractDetails.issueDate.c_str());
  printf("\tRatings: %s\n", contractDetails.ratings.c_str());
  printf("\tBondType: %s\n", contractDetails.bondType.c_str());
  printf("\tCouponType: %s\n", contractDetails.couponType.c_str());
  printf("\tConvertible: %s\n", contractDetails.convertible ? "yes" : "no");
  printf("\tCallable: %s\n", contractDetails.callable ? "yes" : "no");
  printf("\tPutable: %s\n", contractDetails.putable ? "yes" : "no");
  printf("\tDescAppend: %s\n", contractDetails.descAppend.c_str());
  printf("\tExchange: %s\n", contractDetails.contract.exchange.c_str());
  printf("\tCurrency: %s\n", contractDetails.contract.currency.c_str());
  printf("\tMarketName: %s\n", contractDetails.marketName.c_str());
  printf("\tTradingClass: %s\n", contractDetails.contract.tradingClass.c_str());
  printf("\tConId: %s\n", Utils::intMaxString(contractDetails.contract.conId).c_str());
  printf("\tMinTick: %s\n", Utils::doubleMaxString(contractDetails.minTick).c_str());
  printf("\tOrderTypes: %s\n", contractDetails.orderTypes.c_str());
  printf("\tValidExchanges: %s\n", contractDetails.validExchanges.c_str());
  printf("\tNextOptionDate: %s\n", contractDetails.nextOptionDate.c_str());
  printf("\tNextOptionType: %s\n", contractDetails.nextOptionType.c_str());
  printf("\tNextOptionPartial: %s\n", contractDetails.nextOptionPartial ? "yes" : "no");
  printf("\tNotes: %s\n", contractDetails.notes.c_str());
  printf("\tLong Name: %s\n", contractDetails.longName.c_str());
  printf("\tTimeZoneId: %s\n", contractDetails.timeZoneId.c_str());
  printf("\tTradingHours: %s\n", contractDetails.tradingHours.c_str());
  printf("\tLiquidHours: %s\n", contractDetails.liquidHours.c_str());
  printf("\tEvRule: %s\n", contractDetails.evRule.c_str());
  printf("\tEvMultiplier: %s\n", Utils::doubleMaxString(contractDetails.evMultiplier).c_str());
  printf("\tAggGroup: %s\n", Utils::intMaxString(contractDetails.aggGroup).c_str());
  printf("\tMarketRuleIds: %s\n", contractDetails.marketRuleIds.c_str());
  printf("\tTimeZoneId: %s\n", contractDetails.timeZoneId.c_str());
  printf("\tLastTradeTime: %s\n", contractDetails.lastTradeTime.c_str());
  printf("\tMinSize: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.minSize).c_str());
  printf("\tSizeIncrement: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.sizeIncrement).c_str());
  printf("\tSuggestedSizeIncrement: %s\n", DecimalFunctions::decimalStringToDisplay(contractDetails.suggestedSizeIncrement).c_str());
  printContractDetailsSecIdList(contractDetails.secIdList);
}

//! [contractdetailsend]
void TestCppClient::contractDetailsEnd( int reqId) {
  printf( "ContractDetailsEnd. %d\n", reqId);
}
//! [contractdetailsend]

//! [execdetails]
void TestCppClient::execDetails( int reqId, const Contract& contract, const Execution& execution) {
  printf("ExecDetails. ReqId: %d, Contract - ConId: %d, Symbol: %s, SecType: %s, LastTradeDateOrContractMonth: %s, Strike: %s, Right: %s, Multiplier: %s, Exchange: %s, "
      "Currency: %s, LocalSymbol: %s, TradingClass: %s, "
      "Execution - OrderId: %s, ExecId: %s, Time: %s, AcctNumber: %s, Exchange: %s, Side: %s, Shares: %s, Price: %s, PermId: %s, ClientId: %s, "
      "Liquidation: %s, CumQty: %s, AvgPrice: %s, OrderRef: %s, EvRule: %s, EvMultiplier: %s, ModelCode: %s, LastLiquidity: %s, PendingPriceRevision: %s, Submitter: %s, "
      "OptExerciseOrLapseType: %s\n",
      reqId, contract.conId, contract.symbol.c_str(), contract.secType.c_str(), contract.lastTradeDateOrContractMonth.c_str(), Utils::doubleMaxString(contract.strike).c_str(),
      contract.right.c_str(), contract.multiplier.c_str(), contract.exchange.c_str(),	contract.currency.c_str(), contract.localSymbol.c_str(), contract.tradingClass.c_str(),
      Utils::intMaxString(execution.orderId).c_str(), execution.execId.c_str(), execution.time.c_str(), execution.acctNumber.c_str(), execution.exchange.c_str(),
      execution.side.c_str(), DecimalFunctions::decimalStringToDisplay(execution.shares).c_str(), Utils::doubleMaxString(execution.price).c_str(), Utils::llongMaxString(execution.permId).c_str(),
      Utils::intMaxString(execution.clientId).c_str(), execution.liquidation == 1 ? "true" : "false", DecimalFunctions::decimalStringToDisplay(execution.cumQty).c_str(),
      Utils::doubleMaxString(execution.avgPrice).c_str(), execution.orderRef.c_str(), execution.evRule.c_str(), Utils::doubleMaxString(execution.evMultiplier).c_str(),
      execution.modelCode.c_str(), Utils::intMaxString(execution.lastLiquidity).c_str(), (execution.pendingPriceRevision ? "yes" : "no"), execution.submitter.c_str(),
      Utils::getOptionExerciseTypeName(execution.optExerciseOrLapseType).c_str());
}
//! [execdetails]

//! [execdetailsend]
void TestCppClient::execDetailsEnd( int reqId) {
  printf( "ExecDetailsEnd. ReqId: %d\n", reqId);
}
//! [execdetailsend]

//! [updatemktdepth]
void TestCppClient::updateMktDepth(int reqId, int position, int operation, int side,
    double price, Decimal size) {
  printf( "UpdateMarketDepth. %d - Position: %s, Operation: %d, Side: %d, Price: %s, Size: %s\n", reqId, Utils::intMaxString(position).c_str(), operation, side,
      Utils::doubleMaxString(price).c_str(), DecimalFunctions::decimalStringToDisplay(size).c_str());
}
//! [updatemktdepth]

//! [updatemktdepthl2]
void TestCppClient::updateMktDepthL2(int reqId, int position, const std::string& marketMaker, int operation,
    int side, double price, Decimal size, bool isSmartDepth) {
  printf( "UpdateMarketDepthL2. %d - Position: %s, Operation: %d, Side: %d, Price: %s, Size: %s, isSmartDepth: %d\n", reqId, Utils::intMaxString(position).c_str(), operation, side,
      Utils::doubleMaxString(price).c_str(), DecimalFunctions::decimalStringToDisplay(size).c_str(), isSmartDepth);
}
//! [updatemktdepthl2]

//! [updatenewsbulletin]
void TestCppClient::updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) {
  printf( "News Bulletins. %d - Type: %d, Message: %s, Exchange of Origin: %s\n", msgId, msgType, newsMessage.c_str(), originExch.c_str());
}
//! [updatenewsbulletin]

//! [managedaccounts]
void TestCppClient::managedAccounts( const std::string& accountsList) {


  // managed_accounts_str_ = accountsList;

  printf( "Account List: %s\n", accountsList.c_str());
}
//! [managedaccounts]

//! [receivefa]
void TestCppClient::receiveFA(faDataType pFaDataType, const std::string& cxml) {
  std::cout << "Receiving FA: " << (int)pFaDataType << std::endl << cxml << std::endl;
}
//! [receivefa]

//! [historicaldata]
void TestCppClient::historicalData(int reqId, const Bar& bar) {
  printf( "HistoricalData. ReqId: %d - Date: %s, Open: %s, High: %s, Low: %s, Close: %s, Volume: %s, Count: %s, WAP: %s\n", reqId, bar.time.c_str(),
      Utils::doubleMaxString(bar.open).c_str(), Utils::doubleMaxString(bar.high).c_str(), Utils::doubleMaxString(bar.low).c_str(), Utils::doubleMaxString(bar.close).c_str(),
      DecimalFunctions::decimalStringToDisplay(bar.volume).c_str(), Utils::intMaxString(bar.count).c_str(), DecimalFunctions::decimalStringToDisplay(bar.wap).c_str());
}
//! [historicaldata]

//! [historicaldataend]
void TestCppClient::historicalDataEnd(int reqId, const std::string& startDateStr, const std::string& endDateStr) {
  std::cout << "HistoricalDataEnd. ReqId: " << reqId << " - Start Date: " << startDateStr << ", End Date: " << endDateStr << std::endl;
}
//! [historicaldataend]

//! [scannerparameters]
void TestCppClient::scannerParameters(const std::string& xml) {
  printf( "ScannerParameters. %s\n", xml.c_str());
}
//! [scannerparameters]

//! [scannerdata]
void TestCppClient::scannerData(int reqId, int rank, const ContractDetails& contractDetails,
    const std::string& distance, const std::string& benchmark, const std::string& projection,
    const std::string& legsStr) {
  printf( "ScannerData. %d - Rank: %d, Symbol: %s, SecType: %s, Currency: %s, Distance: %s, Benchmark: %s, Projection: %s, Legs String: %s\n", reqId, rank, contractDetails.contract.symbol.c_str(), contractDetails.contract.secType.c_str(), contractDetails.contract.currency.c_str(), distance.c_str(), benchmark.c_str(), projection.c_str(), legsStr.c_str());
}
//! [scannerdata]

//! [scannerdataend]
void TestCppClient::scannerDataEnd(int reqId) {
  printf( "ScannerDataEnd. %d\n", reqId);
}
//! [scannerdataend]

//! [realtimebar]
void TestCppClient::realtimeBar(int reqId, long long time, double open, double high, double low, double close,
    Decimal volume, Decimal wap, int count) {
  printf( "RealTimeBars. %d - Time: %s, Open: %s, High: %s, Low: %s, Close: %s, Volume: %s, Count: %s, WAP: %s\n", reqId, Utils::llongMaxString(time).c_str(),
      Utils::doubleMaxString(open).c_str(), Utils::doubleMaxString(high).c_str(), Utils::doubleMaxString(low).c_str(), Utils::doubleMaxString(close).c_str(),
      DecimalFunctions::decimalStringToDisplay(volume).c_str(), Utils::intMaxString(count).c_str(), DecimalFunctions::decimalStringToDisplay(wap).c_str());
}
//! [realtimebar]

//! [fundamentaldata]
void TestCppClient::fundamentalData(int reqId, const std::string& data) {
  printf( "FundamentalData. ReqId: %d, %s\n", reqId, data.c_str());
}
//! [fundamentaldata]

void TestCppClient::deltaNeutralValidation(int reqId, const DeltaNeutralContract& deltaNeutralContract) {
  printf( "DeltaNeutralValidation. %d, ConId: %d, Delta: %s, Price: %s\n", reqId, deltaNeutralContract.conId, Utils::doubleMaxString(deltaNeutralContract.delta).c_str(), Utils::doubleMaxString(deltaNeutralContract.price).c_str());
}

void TestCppClient::tickSnapshotEnd(int reqId) {

  //  std::cout << "tick snapshot ends of reqId:" << reqId << std::endl;

  if(std::get<0>(price_request_counter_).contains(reqId)) {
    //    std::cout << "reqId:" << reqId << " belongs to search space" << std::endl;
    std::get<0>(std::get<1>( std::get<1>(price_request_counter_)))++;
    std::get<1>(std::get<1>( std::get<1>(price_request_counter_)))++;

    const auto& counter_value = std::get<0>( std::get<1>(price_request_counter_));

    if(std::get<0>(std::get<1>( std::get<1>(price_request_counter_))) == counter_value) {
      //     std::cout << "price request counter are finished" << std::endl;

      //    std::cout << "counter value:" << counter_value << std::endl;
      req_position_price_end_cond_var_trigger_ = true;
      req_position_price_end_cond_var_.notify_one();
      //   std::cout << "Price request done!" << counter_value << std::endl;

    }

  }else {

    //    std::cout << "reqId:" << reqId << " DOES NOT belong to search space" << std::endl;
    for(const auto& it:std::get<0>(price_request_counter_)) {
      std::cout << it.first << " " << it.second << std::endl;
    }
    std::cout << std::endl;
  }


  //     std::tuple<std::set<int>, std::tuple<int, std::tuple<std::atomic<int>,std::atomic<int>>> >  price_request_counter_;


}

//! [marketdatatype]
void TestCppClient::marketDataType(int reqId, int marketDataType) {
  //  printf( "MarketDataType. ReqId: %d, Type: %d\n", reqId, marketDataType);
}
//! [marketdatatype]

//! [commissionandfeesreport]
void TestCppClient::commissionAndFeesReport( const CommissionAndFeesReport& commissionAndFeesReport) {
  printf( "CommissionAndFeesReport. %s - %s %s RPNL %s\n", commissionAndFeesReport.execId.c_str(), Utils::doubleMaxString(commissionAndFeesReport.commissionAndFees).c_str(), commissionAndFeesReport.currency.c_str(), Utils::doubleMaxString(commissionAndFeesReport.realizedPNL).c_str());
}
//! [commissionandfeesreport]

void TestCppClient::print_position_details() {



  // fmt::print("{}","Printing position details!");
  m_pClient->reqOpenOrders();
  std::cout << "waiting on open order trigger" << std::endl;

  std::unique_lock<std::mutex> req_open_order_end_lock(req_open_oder_end_mtx_);
  //  // 2. Wait until the predicate condition evaluates to true
  req_open_oder_end_cond_var_.wait(req_open_order_end_lock, [] { return req_open_oder_cond_var_trigger_; });
  req_open_oder_cond_var_trigger_ = req_open_oder_cond_var_trigger_default_;

  std::cout << "Received open order trigger" << std::endl;

  //  std::unique_lock<std::mutex> lock_open_order(req_open_oder_end_mtx_);



  // m_pClient->reqAccountUpdates(false, ""); 
  auto account_summary_order_id = m_orderId++;

  std::cout << "requesting account summary:" << std::endl;
  account_summary_.clear();
  m_pClient->reqAccountSummary(account_summary_order_id, "All", AccountSummaryTags::getAllTags());

  std::cout << "stop requesting account summary:" << std::endl;






  //request_mutex_.lock();

  //  std::cout << "making request" << std::endl;
  m_pClient->reqPositions();

  // std::cout << "waiting..." << std::endl;


  // 1. Acquire the lock using std::unique_lock
  //
  // req_position_end_cond_var_.notify_one();

  std::unique_lock<std::mutex> lock(req_position_end_mtx_);
  // 2. Wait until the predicate condition evaluates to true
  req_position_end_cond_var_.wait(lock, [] { return req_position_end_cond_var_trigger_; });

  req_position_end_cond_var_trigger_ = req_position_end_cond_var_trigger_default_;

  //  fmt::print(fmt::emphasis::bold | fg(fmt::color::red) | bg(fmt::color::black),"{}\n","Position request ends!");




  //m_pClient->reqAccountUpdates(true, ""); 

  // fmt::print("{}","Printing position details!");

  //  m_pClient->reqOpenOrders();



  // m_pClient->reqAccountUpdates(false, ""); 

  // print_position_details();






  std::set<std::string> long_positions;
  std::set<std::string> short_positions;
  std::set<std::string> open_positions;
  std::set<std::string> open_filled_positions;

  current_account_state_.print();

  std::get<0>(price_request_counter_).clear();
  std::get<0>(std::get<1>(std::get<1>(price_request_counter_))) = {0};
  std::get<1>(std::get<1>(std::get<1>(price_request_counter_))) = {0};
  //

  std::get<0>(std::get<1>(price_request_counter_)) = 0;
  if(position_details_.size() > 0) {
    //    std::map<int, std::map<std::set<int>, std::atomic<int> > > price_request_counter;
    //    std::tuple<std::set<int>, std::tuple<int, std::tuple<std::atomic<int>,std::atomic<int>>> >  price_request_counter_;

    for(auto position:position_details_){
      if(::fabs(position.second.num_positions_) > 0) 
        std::get<0>(std::get<1>(price_request_counter_)) += 1;
    }


    std::get<0>(std::get<1>(std::get<1>(price_request_counter_))) = 0;
    std::get<1>(std::get<1>(std::get<1>(price_request_counter_))) = 0;

    // get the prices
    std::map<int,std::string> req_id_to_ticker_map;

    current_price_list_.clear();
    for(const auto& position:position_details_) {
      if(::fabs(position.second.num_positions_) > 0 ) 
      {
        const auto& ticker = position.first;
        int counter = m_orderId++;
        std::get<0>(price_request_counter_)[counter] = ticker;
        // std::cout << "counter:" << counter << " " << "ticker:" << ticker << std::endl;

        current_price_list_[counter].last_price_ = {-1};
        current_price_list_[counter].bid_price_ = {-1};
        current_price_list_[counter].ask_price_ = {-1};


      }
    }




    //   std::cout << "requesting prices for assets" << std::endl;

    std::unique_lock<std::mutex> lock_request_price(req_position_price_end_mtx_);

    for(const auto& request:std::get<0>(price_request_counter_)) {

      //       std::cout << "request id:" << request.first << " ticker:" << request.second << std::endl;
      const std::string& symbol = request.second;
      Contract contract;
      contract.symbol = symbol;
      contract.secType = "STK";
      contract.exchange = "SMART";
      contract.currency = "USD";
      m_pClient->reqMktData(request.first, contract, "", true, false, TagValueListSPtr());

    }

    //    std::cout << "waiting on the price request completion" << std::endl;

    req_position_price_end_cond_var_.wait(lock_request_price, [] { return req_position_price_end_cond_var_trigger_; });

    req_position_price_end_cond_var_trigger_ = req_position_price_end_cond_var_trigger_default_;

    fmt::print(fmt::emphasis::bold | fg(fmt::color::red) | bg(fmt::color::black),"{}\n","Position price request ends!");


    std::map<std::string, AssetPrice> ticker_price_map;

    for(auto current_price:current_price_list_) {
      auto ticker = std::get<0>(price_request_counter_)[current_price.first];
      ticker_price_map[ticker] = current_price.second;
      std::cout  << ticker << ":l:" << current_price.second.last_price_ << " b:" << current_price.second.bid_price_ << " a:" << current_price.second.ask_price_ << std::endl;
    }




    float total_VaR = {0.0}; // value at risk! 
    int request_counter = {0};
    for(const auto& position:position_details_) {

      const auto& ticker = position.first;

      if(position.second.num_positions_ > 0) {
        long_positions.insert(ticker);

      }else if(::fabs(position.second.num_positions_)) {
        short_positions.insert(ticker);
      }

      total_VaR += ::fabs(position.second.num_positions_ * position.second.average_cost_);
    }

    std::cout << "############################################################"<< std::endl;
    //
    //    fmt::print(fg(fmt::color::violet), "Positions:\n");

    // printf( "Position. %s - Symbol: %s, SecType: %s, Currency: %s, Position: %s, Avg Cost: %s\n", account.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), DecimalFunctions::decimalStringToDisplay(position).c_str(), Utils::doubleMaxString(avgCost).c_str());
    if(short_positions.size() > 0){
      fmt::print(fmt::emphasis::bold | fg(fmt::color::red) | bg(fmt::color::black), "SHORTS:\n");

      for(const auto& position:short_positions) {


        const std::string& symbol = position;
        const auto position_float = position_details_[symbol].num_positions_ ;
        const auto average_cost_float = position_details_[symbol].average_cost_;
        auto total_position_cost = ::fabs(position_float*average_cost_float);
        const auto& last_price = ticker_price_map[symbol].last_price_;
        auto profit = ::fabs(position_float)*(average_cost_float  - last_price);


        //Contract contract;
        //contract.symbol = symbol;
        //contract.secType = "STK";
        //contract.exchange = "SMART";
        //contract.currency = "USD";


        //auto it = std::next(std::get<0>(price_request_counter_).begin(), request_counter++);
        //const int tickerId = *it;

        // Request market data. The last two arguments are for regulatory snapshots and API generic tags.
        // Setting snapshot to false gives a continuous live stream.

        //        m_pClient->reqMktData(tickerId, contract, "", true, false, TagValueListSPtr());




        if(::fabs(position_float) > 0) {
          // fmt::print(fmt::emphasis::bold | fg(fmt::color::orange) | bg(fmt::color::black), fmt::runtime("{}:({},{},{},{})\n"), symbol, static_cast<int>(position_float), average_cost_float,ticker_price_map[symbol].last_price_, total_position_cost); 
          fmt::print(fmt::emphasis::bold | fg(fmt::color::orange) | bg(fmt::color::black), fmt::runtime("{:>6}: "), symbol);
          float percent_deviation = 100.0*(last_price - average_cost_float)/average_cost_float;
          if(percent_deviation > 0){

            std::cout << BRIGHT_BRICK_RED << BOLD << std::fixed << std::setprecision(2) << "(" << percent_deviation << "%," << profit << "):" << RESET << PURPLE << BOLD << "("  << ticker_price_map[symbol].last_price_ << RESET << "," <<  average_cost_float << RESET << "):" << BLUE  << BOLD << "(" <<::fabs(position_float) << ","<<  total_position_cost << ")" << RESET << std::endl;
            // std::cout << BRICK_RED << BOLD << std::fixed<< std::setprecision(2) <<  ::fabs(position_float) << " (" <<  average_cost_float << "," << ticker_price_map[symbol].last_price_ << "," << percent_deviation << "%):("<< profit << "):"<<  total_position_cost << RESET << std::endl;
          }
          else {
            std::cout <<  GREEN << BOLD << std::fixed << std::setprecision(2) << "(" << percent_deviation << "%," << profit << "):" << RESET << PURPLE << BOLD << "("  << ticker_price_map[symbol].last_price_ << RESET << "," <<  average_cost_float << RESET << "):" << BLUE  << BOLD << "(" <<::fabs(position_float) << ","<<  total_position_cost << ")" << RESET << std::endl;

          }
        }

      }
    }

    if(long_positions.size() > 0){
      fmt::print(fg(fmt::color::red) | bg(fmt::color::black), "LONGS:\n");

      //  float num_positions_;
      //  float average_cost_;
      //
      for(const auto& position:long_positions) {
        const std::string& symbol = position;
        const auto position_float = position_details_[symbol].num_positions_ ;
        const auto average_cost_float = position_details_[symbol].average_cost_;
        auto total_position_cost = position_float*average_cost_float;

        Contract contract;
        contract.symbol = symbol;
        contract.secType = "STK";
        contract.exchange = "SMART";
        contract.currency = "USD";


        //        auto it = std::next(std::get<0>(price_request_counter_).begin(), request_counter++);
        //        const int tickerId = *it;

        //        m_pClient->reqMktData(tickerId, contract, "", true, false, TagValueListSPtr());


        if(position_float > 0){


          fmt::print(fmt::emphasis::bold | fg(fmt::color::green) | bg(fmt::color::black), fmt::runtime("{}:"), symbol);
          float percent_deviation = 100.0*(ticker_price_map[symbol].last_price_ - average_cost_float)/average_cost_float;

          if(percent_deviation < 0){
            std::cout << RED << std::fixed<< std::setprecision(2) <<  static_cast<int>(position_float) << " (" <<  average_cost_float << "," << ticker_price_map[symbol].last_price_ << "," << percent_deviation << "):" <<  total_position_cost << RESET << std::endl;
          } else{
            std::cout << GREEN << std::fixed<< std::setprecision(2) <<  static_cast<int>(position_float) << " (" <<  average_cost_float << "," << ticker_price_map[symbol].last_price_ << "," << percent_deviation << "):" <<  total_position_cost << RESET << std::endl;

          }


        }
      }
    }
  }else {
    fmt::print(fg(fmt::color::red),"SORRY THERE IS NO AVAILABLE POSIITON TO PRINT\n");
  }

  //  std::cout << std::endl;
  //  fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "VaR:{}\n",total_VaR);
  //  fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "Buying Power:{}\n", buying_power_);

  std::cout << "waiting for account update to finish" << std::endl;

  std::unique_lock<std::mutex> account_update_lock(account_update_end_mtx_);
  // 2. Wait until the predicate condition evaluates to true
  account_update_end_cond_var_.wait(account_update_lock, [] { return account_update_end_cond_var_trigger_; });




  std::cout << "ACCOUNT UPDATE FINISHED! " << std::endl;

  //
  // reset the game!
  account_update_end_cond_var_trigger_ = account_update_end_cond_var_trigger_default_;

  std::cout << account_summary_ << std::endl;



  m_pClient->cancelAccountSummary(account_summary_order_id);

}
//! [position]
void TestCppClient::position( const std::string& account, const Contract& contract, Decimal position, double avgCost) {

  //std::cout << "getting positions:" << std::endl;
  //printf("%f\n",position);

  std::string position_str = DecimalFunctions::decimalStringToDisplay(position);

  //  std::cout << "position_str:" << position_str << std::endl;

  std::string symbol =  contract.symbol;
  //   std::cout << "symbol:" << symbol << std::endl;
  //   std::cout << "symbol size:" << symbol.size() << std::endl;
  if(!symbol.empty()) {
    std::string average_cost_str = Utils::doubleMaxString(avgCost);
    float position_float = ::atof(position_str.c_str());
    float average_cost_float = ::atof(average_cost_str.c_str());
    float total_position_cost = position_float*average_cost_float;

    // contract.size;

    position_details_[symbol].num_positions_ = position_float;
    position_details_[symbol].average_cost_ = average_cost_float;



    //    if(!print_once_positions_) {
    //      fmt::print(fg(fmt::color::violet)|bg(fmt::color::black), "Positions:\n");
    //      print_once_positions_ = !print_once_positions_default_;
    //    }
    //
    // fmt::print(bg(fmt::color::black),"{}","");

    //     if(::fabs(position_float) > 0) {
    //       // printf( "Position. %s - Symbol: %s, SecType: %s, Currency: %s, Position: %s, Avg Cost: %s\n", account.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), DecimalFunctions::decimalStringToDisplay(position).c_str(), Utils::doubleMaxString(avgCost).c_str());
    //       if(position_float < 0){
    //   //      if(!print_once_positions_)
    //   //        fmt::print(fg(fmt::color::violet) | bg(fmt::color::black), "\tSHORTS:\n");
    //
    //         fmt::print(fg(fmt::color::orange) | bg(fmt::color::black) , "{}:({},{},{})\n",symbol,position_float,average_cost_float, total_position_cost);
    //       }else {
    //         //if(!print_once_positions_)
    //         //  fmt::print(fg(fmt::color::violet) | bg(fmt::color::black), "\tLONGS:\n");
    //         fmt::print(fg(fmt::color::olive) | bg(fmt::color::black) , "{}:({},{},{})\n",symbol,position_float,average_cost_float, total_position_cost);
    //       }
    //     }
    //


  }

}
//! [position]

//! [positionend]
void TestCppClient::positionEnd() {
  auto now = std::chrono::system_clock::now();
  auto local_time = std::chrono::zoned_time{std::chrono::current_zone(), now};
  std::cout << std::format("Local Time: {:%F %T}\n", local_time);
  std::cout << "############################################################"<< std::endl;
  req_position_end_cond_var_trigger_ = true;

  req_position_end_cond_var_.notify_one();

  //  print_once_positions_ = !print_once_positions_default_;
  //  std::cout << "position End" << std::endl;
}
//! [positionend]

//! [accountsummary]
void TestCppClient::accountSummary( int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& currency) {
  //  printf( "Acct Summary. ReqId: %d, Account: %s, Tag: %s, Value: %s, Currency: %s\n", reqId, account.c_str(), tag.c_str(), value.c_str(), currency.c_str());
  account_summary_[tag] = ::atof(value.c_str()); 

}
//! [accountsummary]

//! [accountsummaryend]
void TestCppClient::accountSummaryEnd( int reqId) {
  //  printf( "AccountSummaryEnd. Req Id: %d\n", reqId);
  account_update_end_cond_var_trigger_ = true;
  account_update_end_cond_var_.notify_one();

}
//! [accountsummaryend]

void TestCppClient::verifyMessageAPI( const std::string& apiData) {
  printf("verifyMessageAPI: %s\n", apiData.c_str());
}

void TestCppClient::verifyCompleted( bool isSuccessful, const std::string& errorText) {
  printf("verifyCompleted. IsSuccessful: %d - Error: %s\n", isSuccessful, errorText.c_str());
}

void TestCppClient::verifyAndAuthMessageAPI( const std::string& apiDatai, const std::string& xyzChallenge) {
  printf("verifyAndAuthMessageAPI: %s %s\n", apiDatai.c_str(), xyzChallenge.c_str());
}

void TestCppClient::verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) {
  printf("verifyAndAuthCompleted. IsSuccessful: %d - Error: %s\n", isSuccessful, errorText.c_str());
  if (isSuccessful)
    m_pClient->startApi();
}

//! [displaygrouplist]
void TestCppClient::displayGroupList( int reqId, const std::string& groups) {
  printf("Display Group List. ReqId: %d, Groups: %s\n", reqId, groups.c_str());
}
//! [displaygrouplist]

//! [displaygroupupdated]
void TestCppClient::displayGroupUpdated( int reqId, const std::string& contractInfo) {
  std::cout << "Display Group Updated. ReqId: " << reqId << ", Contract Info: " << contractInfo << std::endl;
}
//! [displaygroupupdated]

//! [positionmulti]
void TestCppClient::positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, Decimal pos, double avgCost) {
  printf("Position Multi. Request: %d, Account: %s, ModelCode: %s, Symbol: %s, SecType: %s, Currency: %s, Position: %s, Avg Cost: %s\n", reqId, account.c_str(), modelCode.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), DecimalFunctions::decimalStringToDisplay(pos).c_str(), Utils::doubleMaxString(avgCost).c_str());
}
//! [positionmulti]

//! [positionmultiend]
void TestCppClient::positionMultiEnd( int reqId) {
  printf("Position Multi End. Request: %d\n", reqId);
}
//! [positionmultiend]

//! [accountupdatemulti]
void TestCppClient::accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) {
  printf("AccountUpdate Multi. Request: %d, Account: %s, ModelCode: %s, Key, %s, Value: %s, Currency: %s\n", reqId, account.c_str(), modelCode.c_str(), key.c_str(), value.c_str(), currency.c_str());
}
//! [accountupdatemulti]

//! [accountupdatemultiend]
void TestCppClient::accountUpdateMultiEnd( int reqId) {
  printf("Account Update Multi End. Request: %d\n", reqId);
}
//! [accountupdatemultiend]

//! [securityDefinitionOptionParameter]
void TestCppClient::securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass,
    const std::string& multiplier, const std::set<std::string>& expirations, const std::set<double>& strikes) {
  printf("Security Definition Optional Parameter. Request: %d, Exchange: %s, UnderlyingConId: %d, Trading Class: %s, Multiplier: %s, Expirations (%zu): ",
      reqId, exchange.c_str(), underlyingConId, tradingClass.c_str(), multiplier.c_str(), expirations.size());

  bool first = true;
  for (const auto& exp : expirations) {
    printf("%s%s", first ? "" : ", ", exp.c_str());
    first = false;
  }

  printf(", Strikes (%zu): ", strikes.size());
  first = true;
  for (const auto& strike : strikes) {
    printf("%s%.2f", first ? "" : ", ", strike);
    first = false;
  }
  printf("\n");
}
//! [securityDefinitionOptionParameter]

//! [securityDefinitionOptionParameterEnd]
void TestCppClient::securityDefinitionOptionalParameterEnd(int reqId) {
  printf("Security Definition Optional Parameter End. Request: %d\n", reqId);
}
//! [securityDefinitionOptionParameterEnd]

//! [softDollarTiers]
void TestCppClient::softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) {
  printf("Soft dollar tiers (%zu):", tiers.size());

  for (unsigned int i = 0; i < tiers.size(); i++) {
    printSoftDollarTier(tiers[i]);
  }
}
//! [softDollarTiers]

//! [familyCodes]
void TestCppClient::familyCodes(const std::vector<FamilyCode> &familyCodes) {
  printf("Family codes (%zu):\n", familyCodes.size());

  for (unsigned int i = 0; i < familyCodes.size(); i++) {
    printf("Family code [%d] - accountID: %s familyCodeStr: %s\n", i, familyCodes[i].accountID.c_str(), familyCodes[i].familyCodeStr.c_str());
  }
}
//! [familyCodes]

//! [symbolSamples]
void TestCppClient::symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions) {
  printf("Symbol Samples (total=%zu) reqId: %d\n", contractDescriptions.size(), reqId);

  for (unsigned int i = 0; i < contractDescriptions.size(); i++) {
    Contract contract = contractDescriptions[i].contract;
    std::vector<std::string> derivativeSecTypes = contractDescriptions[i].derivativeSecTypes;
    printf("Contract (%u): conId: %d, symbol: %s, secType: %s, primaryExchange: %s, currency: %s, ", i, contract.conId, contract.symbol.c_str(), contract.secType.c_str(), contract.primaryExchange.c_str(), contract.currency.c_str());
    printf("Derivative Sec-types (%zu):", derivativeSecTypes.size());
    for (unsigned int j = 0; j < derivativeSecTypes.size(); j++) {
      printf(" %s", derivativeSecTypes[j].c_str());
    }
    printf(", description: %s, issuerId: %s", contract.description.c_str(), contract.issuerId.c_str());
    printf("\n");
  }
}
//! [symbolSamples]

//! [mktDepthExchanges]
void TestCppClient::mktDepthExchanges(const std::vector<DepthMktDataDescription> &depthMktDataDescriptions) {
  printf("Mkt Depth Exchanges (%zu):\n", depthMktDataDescriptions.size());

  for (unsigned int i = 0; i < depthMktDataDescriptions.size(); i++) {
    printf("Depth Mkt Data Description [%d] - exchange: %s secType: %s listingExch: %s serviceDataType: %s aggGroup: %s\n", i,
        depthMktDataDescriptions[i].exchange.c_str(),
        depthMktDataDescriptions[i].secType.c_str(),
        depthMktDataDescriptions[i].listingExch.c_str(),
        depthMktDataDescriptions[i].serviceDataType.c_str(),
        Utils::intMaxString(depthMktDataDescriptions[i].aggGroup).c_str());
  }
}
//! [mktDepthExchanges]

//! [tickNews]
void TestCppClient::tickNews(int tickerId, time_t timeStamp, const std::string& providerCode, const std::string& articleId, const std::string& headline, const std::string& extraData) {
  char timeStampStr[80];
#if defined(IB_WIN32)
  ctime_s(timeStampStr, sizeof(timeStampStr), &(timeStamp /= 1000));
#else
  ctime_r(&(timeStamp /= 1000), timeStampStr);
#endif
  printf("News Tick. TickerId: %d, TimeStamp: %s, ProviderCode: %s, ArticleId: %s, Headline: %s, ExtraData: %s\n", tickerId, timeStampStr, providerCode.c_str(), articleId.c_str(), headline.c_str(), extraData.c_str());
}
//! [tickNews]

//! [smartcomponents]]
void TestCppClient::smartComponents(int reqId, const SmartComponentsMap& theMap) {
  printf("Smart components: (%zu):\n", theMap.size());

  for (SmartComponentsMap::const_iterator i = theMap.begin(); i != theMap.end(); i++) {
    printf(" bit number: %d exchange: %s exchange letter: %c\n", i->first, std::get<0>(i->second).c_str(), std::get<1>(i->second));
  }
}
//! [smartcomponents]

//! [tickReqParams]
void TestCppClient::tickReqParams(int tickerId, double minTick, const std::string& bboExchange, int snapshotPermissions) { }
//! [tickReqParams]

//! [newsProviders]
void TestCppClient::newsProviders(const std::vector<NewsProvider> &newsProviders) {
  printf("News providers (%zu):\n", newsProviders.size());

  for (unsigned int i = 0; i < newsProviders.size(); i++) {
    printf("News provider [%d] - providerCode: %s providerName: %s\n", i, newsProviders[i].providerCode.c_str(), newsProviders[i].providerName.c_str());
  }
}
//! [newsProviders]

//! [newsArticle]
void TestCppClient::newsArticle(int requestId, int articleType, const std::string& articleText) {
  printf("News Article. Request Id: %d, Article Type: %d\n", requestId, articleType);
  if (articleType == 0) {
    printf("News Article Text (text or html): %s\n", articleText.c_str());
  } else if (articleType == 1) {
#if defined(IB_WIN32)
    std::wstring path;
    WCHAR s[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, s) == 0) {
      printf("GetCurrentDirectoryW error\n");
      return;
    }
    path = s + std::wstring(L"\\MST$06f53098.pdf");
#elif defined(IB_POSIX)
    std::string path;
    char s[1024];
    if (getcwd(s, sizeof(s)) == NULL) {
      printf("getcwd() error\n");
      return;
    }
    path = s + std::string("/MST$06f53098.pdf");
#endif
    std::vector<std::uint8_t> bytes = Utils::base64_decode(articleText);
    std::ofstream outfile(path, std::ios::out | std::ios::binary);
    outfile.write((const char*)bytes.data(), bytes.size());
#if defined(IB_WIN32)
    printf("Binary/pdf article was saved to: %ls\n", path.c_str());
#else
    printf("Binary/pdf article was saved to: %s\n", path.c_str());
#endif
  }
}
//! [newsArticle]

void TestCppClient::historicalNews(int requestId, const std::string& time, const std::string& providerCode, const std::string& articleId, const std::string& headline) {}
void TestCppClient::historicalNewsEnd(int requestId, bool hasMore) {}

//! [headTimestamp]
void TestCppClient::headTimestamp(int reqId, const std::string& headTimestamp) {
  printf( "Head time stamp. ReqId: %d - Head time stamp: %s,\n", reqId, headTimestamp.c_str());

}
//! [headTimestamp]

//! [histogramData]
void TestCppClient::histogramData(int reqId, const HistogramDataVector& data) {
  printf("Histogram. ReqId: %d, data length: %zu\n", reqId, data.size());

  for (const HistogramEntry& entry : data) {
    printf("\t price: %s, size: %s\n", Utils::doubleMaxString(entry.price).c_str(), DecimalFunctions::decimalStringToDisplay(entry.size).c_str());
  }
}
//! [histogramData]

//! [historicalDataUpdate]
void TestCppClient::historicalDataUpdate(int reqId, const Bar& bar) {
  printf( "HistoricalDataUpdate. ReqId: %d - Date: %s, Open: %s, High: %s, Low: %s, Close: %s, Volume: %s, Count: %s, WAP: %s\n", reqId, bar.time.c_str(),
      Utils::doubleMaxString(bar.open).c_str(), Utils::doubleMaxString(bar.high).c_str(), Utils::doubleMaxString(bar.low).c_str(), Utils::doubleMaxString(bar.close).c_str(),
      DecimalFunctions::decimalStringToDisplay(bar.volume).c_str(), Utils::intMaxString(bar.count).c_str(), DecimalFunctions::decimalStringToDisplay(bar.wap).c_str());
}
//! [historicalDataUpdate]

//! [rerouteMktDataReq]
void TestCppClient::rerouteMktDataReq(int reqId, int conid, const std::string& exchange) {
  printf( "Re-route market data request. ReqId: %d, ConId: %d, Exchange: %s\n", reqId, conid, exchange.c_str());
}
//! [rerouteMktDataReq]

//! [rerouteMktDepthReq]
void TestCppClient::rerouteMktDepthReq(int reqId, int conid, const std::string& exchange) {
  printf( "Re-route market depth request. ReqId: %d, ConId: %d, Exchange: %s\n", reqId, conid, exchange.c_str());
}
//! [rerouteMktDepthReq]

//! [marketRule]
void TestCppClient::marketRule(int marketRuleId, const std::vector<PriceIncrement> &priceIncrements) {
  printf("Market Rule Id: %s\n", Utils::intMaxString(marketRuleId).c_str());
  for (unsigned int i = 0; i < priceIncrements.size(); i++) {
    printf("Low Edge: %s, Increment: %s\n", Utils::doubleMaxString(priceIncrements[i].lowEdge).c_str(), Utils::doubleMaxString(priceIncrements[i].increment).c_str());
  }
}
//! [marketRule]

//! [pnl]
void TestCppClient::pnl(int reqId, double dailyPnL, double unrealizedPnL, double realizedPnL) {
  printf("PnL. ReqId: %d, daily PnL: %s, unrealized PnL: %s, realized PnL: %s\n", reqId, Utils::doubleMaxString(dailyPnL).c_str(), Utils::doubleMaxString(unrealizedPnL).c_str(),
      Utils::doubleMaxString(realizedPnL).c_str());
}
//! [pnl]

//! [pnlsingle]
void TestCppClient::pnlSingle(int reqId, Decimal pos, double dailyPnL, double unrealizedPnL, double realizedPnL, double value) {
  printf("PnL Single. ReqId: %d, pos: %s, daily PnL: %s, unrealized PnL: %s, realized PnL: %s, value: %s\n", reqId, DecimalFunctions::decimalStringToDisplay(pos).c_str(), Utils::doubleMaxString(dailyPnL).c_str(),
      Utils::doubleMaxString(unrealizedPnL).c_str(), Utils::doubleMaxString(realizedPnL).c_str(), Utils::doubleMaxString(value).c_str());
}
//! [pnlsingle]

//! [historicalticks]
void TestCppClient::historicalTicks(int reqId, const std::vector<HistoricalTick>& ticks, bool done) {
  for (const HistoricalTick& tick : ticks) {
    std::time_t t = tick.time;
    char timeStr[80];
#if defined(IB_WIN32)
    ctime_s(timeStr, sizeof(timeStr), &t);
#else
    ctime_r(&t, timeStr);
#endif
    std::cout << "Historical tick. ReqId: " << reqId << ", time: " << timeStr << ", price: "<< Utils::doubleMaxString(tick.price).c_str()	<< ", size: " << DecimalFunctions::decimalStringToDisplay(tick.size).c_str() << std::endl;
  }
}
//! [historicalticks]

//! [historicalticksbidask]
void TestCppClient::historicalTicksBidAsk(int reqId, const std::vector<HistoricalTickBidAsk>& ticks, bool done) {
  for (const HistoricalTickBidAsk& tick : ticks) {
    std::time_t t = tick.time;
    char timeStr[80];
#if defined(IB_WIN32)
    ctime_s(timeStr, sizeof(timeStr), &t);
#else
    ctime_r(&t, timeStr);
#endif
    std::cout << "Historical tick bid/ask. ReqId: " << reqId << ", time: " << timeStr << ", price bid: "<< Utils::doubleMaxString(tick.priceBid).c_str()	<<
      ", price ask: "<< Utils::doubleMaxString(tick.priceAsk).c_str() << ", size bid: " << DecimalFunctions::decimalStringToDisplay(tick.sizeBid).c_str() << ", size ask: " << DecimalFunctions::decimalStringToDisplay(tick.sizeAsk).c_str() <<
      ", bidPastLow: " << tick.tickAttribBidAsk.bidPastLow << ", askPastHigh: " << tick.tickAttribBidAsk.askPastHigh << std::endl;
  }
}
//! [historicalticksbidask]

//! [historicaltickslast]
void TestCppClient::historicalTicksLast(int reqId, const std::vector<HistoricalTickLast>& ticks, bool done) {
  for (HistoricalTickLast tick : ticks) {
    std::time_t t = tick.time;
    char timeStr[80];
#if defined(IB_WIN32)
    ctime_s(timeStr, sizeof(timeStr), &t);
#else
    ctime_r(&t, timeStr);
#endif
    std::cout << "Historical tick last. ReqId: " << reqId << ", time: " << timeStr << ", price: "<< Utils::doubleMaxString(tick.price).c_str() <<
      ", size: " << DecimalFunctions::decimalStringToDisplay(tick.size).c_str() << ", exchange: " << tick.exchange << ", special conditions: " << tick.specialConditions <<
      ", unreported: " << tick.tickAttribLast.unreported << ", pastLimit: " << tick.tickAttribLast.pastLimit << std::endl;
  }
}
//! [historicaltickslast]

//! [tickbytickalllast]
void TestCppClient::tickByTickAllLast(int reqId, int tickType, time_t time, double price, Decimal size, const TickAttribLast& tickAttribLast, const std::string& exchange, const std::string& specialConditions) {
  char timeStr[80];
#if defined(IB_WIN32)
  ctime_s(timeStr, sizeof(timeStr), &time);
#else
  ctime_r(&time, timeStr);
#endif
  printf("Tick-By-Tick. ReqId: %d, TickType: %s, Time: %s, Price: %s, Size: %s, PastLimit: %d, Unreported: %d, Exchange: %s, SpecialConditions:%s\n",
      reqId, (tickType == 1 ? "Last" : "AllLast"), timeStr, Utils::doubleMaxString(price).c_str(), DecimalFunctions::decimalStringToDisplay(size).c_str(), tickAttribLast.pastLimit, tickAttribLast.unreported, exchange.c_str(), specialConditions.c_str());
}
//! [tickbytickalllast]

//! [tickbytickbidask]
void TestCppClient::tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, Decimal bidSize, Decimal askSize, const TickAttribBidAsk& tickAttribBidAsk) {
  char timeStr[80];
#if defined(IB_WIN32)
  ctime_s(timeStr, sizeof(timeStr), &time);
#else
  ctime_r(&time, timeStr);
#endif
  printf("Tick-By-Tick. ReqId: %d, TickType: BidAsk, Time: %s, BidPrice: %s, AskPrice: %s, BidSize: %s, AskSize: %s, BidPastLow: %d, AskPastHigh: %d\n",
      reqId, timeStr, Utils::doubleMaxString(bidPrice).c_str(), Utils::doubleMaxString(askPrice).c_str(), DecimalFunctions::decimalStringToDisplay(bidSize).c_str(), DecimalFunctions::decimalStringToDisplay(askSize).c_str(), tickAttribBidAsk.bidPastLow, tickAttribBidAsk.askPastHigh);
}
//! [tickbytickbidask]

//! [tickbytickmidpoint]
void TestCppClient::tickByTickMidPoint(int reqId, time_t time, double midPoint) {
  char timeStr[80];
#if defined(IB_WIN32)
  ctime_s(timeStr, sizeof(timeStr), &time);
#else
  ctime_r(&time, timeStr);
#endif
  printf("Tick-By-Tick. ReqId: %d, TickType: MidPoint, Time: %s, MidPoint: %s\n", reqId, timeStr, Utils::doubleMaxString(midPoint).c_str());
}
//! [tickbytickmidpoint]

//! [orderbound]
void TestCppClient::orderBound(long long permId, int clientId, int orderId) {
  printf("Order bound. PermId: %s, clientId: %s, orderId: %s\n", Utils::llongMaxString(permId).c_str(), Utils::intMaxString(clientId).c_str(), Utils::intMaxString(orderId).c_str());
}
//! [orderbound]

void TestCppClient::completedOrder(const Contract& contract, const Order& order, const OrderState& orderState) {}
void TestCppClient::completedOrdersEnd() {}

//! [replacefaend]
void TestCppClient::replaceFAEnd(int reqId, const std::string& text) {
  printf("Replace FA End. Request: %d, Text:%s\n", reqId, text.c_str());
}
//! [replacefaend]

//! [wshMetaData]
void TestCppClient::wshMetaData(int reqId, const std::string& dataJson) {
  printf("WSH Meta Data. ReqId: %d, dataJson: %s\n", reqId, dataJson.c_str());
}
//! [wshMetaData]

//! [wshEventData]
void TestCppClient::wshEventData(int reqId, const std::string& dataJson) {
  printf("WSH Event Data. ReqId: %d, dataJson: %s\n", reqId, dataJson.c_str());
}
//! [wshEventData]

//! [historicalSchedule]
void TestCppClient::historicalSchedule(int reqId, const std::string& startDateTime, const std::string& endDateTime, const std::string& timeZone, const std::vector<HistoricalSession>& sessions) {
  printf("Historical Schedule. ReqId: %d, Start: %s, End: %s, TimeZone: %s\n", reqId, startDateTime.c_str(), endDateTime.c_str(), timeZone.c_str());
  for (unsigned int i = 0; i < sessions.size(); i++) {
    printf("\tSession. Start: %s, End: %s, RefDate: %s\n", sessions[i].startDateTime.c_str(), sessions[i].endDateTime.c_str(), sessions[i].refDate.c_str());
  }
}
//! [historicalSchedule]

//! [userInfo]
void TestCppClient::userInfo(int reqId, const std::string& whiteBrandingId) {
  printf("User Info. ReqId: %d, WhiteBrandingId: %s\n", reqId, whiteBrandingId.c_str());
}
//! [userInfo]

//! [currenttimeinmillis]
void TestCppClient::currentTimeInMillis(time_t timeInMillis) {
  struct tm timeinfo;
  char currentTimeInMillis[80];
  time_t time = timeInMillis / 1000;
  time_t millis = timeInMillis - (time * 1000);
#if defined(IB_WIN32)
  localtime_s(&timeinfo, &time);
#else
  localtime_r(&time, &timeinfo);
#endif
  strftime(currentTimeInMillis, sizeof(currentTimeInMillis), "%b %d, %Y %H:%M:%S", &timeinfo);
  printf("The current date/time in millis is %llu : %s.%03llu\n", static_cast<unsigned long long>(timeInMillis), currentTimeInMillis, static_cast<unsigned long long>(millis));
}
//! [currenttimeinmillis]

// protobuf
#if !defined(USE_WIN_DLL)
void TestCppClient::execDetailsProtoBuf(const protobuf::ExecutionDetails& executionDetailsProto) {}
void TestCppClient::execDetailsEndProtoBuf(const protobuf::ExecutionDetailsEnd& executionDetailsEndProto) {}
void TestCppClient::orderStatusProtoBuf(const protobuf::OrderStatus& orderStatusProto) {
  //printf("Order Status: %s\n", orderStatusProto.ShortDebugString().c_str());

  // std::cout << " now printing account summary:" << std::endl;
  //  m_pClient->reqAccountSummary(++m_orderId, "All", AccountSummaryTags::getAllTags());



}
void TestCppClient::openOrderProtoBuf(const protobuf::OpenOrder& openOrderProto) {
  printf("Open Order: %s\n", openOrderProto.ShortDebugString().c_str());
}
void TestCppClient::openOrdersEndProtoBuf(const protobuf::OpenOrdersEnd& openOrdersEndProto) {
  printf("Open Orders End: %s\n", openOrdersEndProto.ShortDebugString().c_str());


}

//void TestCppClient::openOrder(OrderId orderId, const Contract& contract, 
//                          const Order& order, const OrderState& orderState) {
//    // Process each open order here
//}

//void TestCppClient::orderStatus(OrderId orderId, const std::string& status, 
//                            double filled, double remaining, double avgFillPrice, 
//                            int permId, int parentId, double lastFillPrice, 
//                            int clientId, const std::string& whyHeld) {
//    // Track order status updates
//}
//




void TestCppClient::errorProtoBuf(const protobuf::ErrorMessage& errorProto) {}
void TestCppClient::completedOrderProtoBuf(const protobuf::CompletedOrder& completedOrderProto) {
  printf("Completed Order: %s\n", completedOrderProto.ShortDebugString().c_str());
}
void TestCppClient::completedOrdersEndProtoBuf(const protobuf::CompletedOrdersEnd& completedOrdersEndProto) {
  printf("Completed Orders End: %s\n", completedOrdersEndProto.ShortDebugString().c_str());
}
void TestCppClient::orderBoundProtoBuf(const protobuf::OrderBound& orderBoundProto) {}
void TestCppClient::contractDataProtoBuf(const protobuf::ContractData& contractDataProto) {}
void TestCppClient::bondContractDataProtoBuf(const protobuf::ContractData& contractDataProto) {}
void TestCppClient::contractDataEndProtoBuf(const protobuf::ContractDataEnd& contractDataEndProto) {}
void TestCppClient::tickPriceProtoBuf(const protobuf::TickPrice& tickPriceProto) {
  printf("Tick Price: %s\n", tickPriceProto.ShortDebugString().c_str());
}
void TestCppClient::tickSizeProtoBuf(const protobuf::TickSize& tickSizeProto) {
  printf("Tick Size: %s\n", tickSizeProto.ShortDebugString().c_str());
}
void TestCppClient::tickOptionComputationProtoBuf(const protobuf::TickOptionComputation& tickOptionComputationProto) {
  printf("Tick Option Computation: %s\n", tickOptionComputationProto.ShortDebugString().c_str());
}
void TestCppClient::tickGenericProtoBuf(const protobuf::TickGeneric& tickGenericProto) {
  printf("Tick Generic: %s\n", tickGenericProto.ShortDebugString().c_str());
}
void TestCppClient::tickStringProtoBuf(const protobuf::TickString& tickStringProto) {
  printf("Tick String: %s\n", tickStringProto.ShortDebugString().c_str());
}
void TestCppClient::tickSnapshotEndProtoBuf(const protobuf::TickSnapshotEnd& tickSnapshotEndProto) {
  printf("Tick Snapshot End: %s\n", tickSnapshotEndProto.ShortDebugString().c_str());
}
void TestCppClient::updateMarketDepthProtoBuf(const protobuf::MarketDepth& marketDepthProto) {}
void TestCppClient::updateMarketDepthL2ProtoBuf(const protobuf::MarketDepthL2& marketDepthL2Proto) {}
void TestCppClient::marketDataTypeProtoBuf(const protobuf::MarketDataType& marketDataTypeProto) {}
void TestCppClient::tickReqParamsProtoBuf(const protobuf::TickReqParams& tickReqParamsProto) {
  std::ostringstream oss;
  if (tickReqParamsProto.has_reqid()) oss << "Ticker Id: " << tickReqParamsProto.reqid() << ", ";
  if (tickReqParamsProto.has_mintick()) oss << " MinTick: " << tickReqParamsProto.mintick() << ", ";
  if (tickReqParamsProto.has_bboexchange()) {
    oss << "BboExchange: " << tickReqParamsProto.bboexchange() << ", ";
    m_bboExchange = tickReqParamsProto.bboexchange();
  }
  if (tickReqParamsProto.has_snapshotpermissions()) oss << "SnapshotPermissions: " << tickReqParamsProto.snapshotpermissions() << ", ";
  if (tickReqParamsProto.has_lastpriceprecision()) oss << "LastPricePrecision: " << tickReqParamsProto.lastpriceprecision() << ", ";
  if (tickReqParamsProto.has_lastsizeprecision()) oss << "lastSizePrecision: " << tickReqParamsProto.lastsizeprecision();
  printf("Tick Req Params. %s\n", oss.str().c_str());
}
void TestCppClient::updateAccountValueProtoBuf(const protobuf::AccountValue& accountValueProto) {}
void TestCppClient::updatePortfolioProtoBuf(const protobuf::PortfolioValue& portfolioValueProto) {}
void TestCppClient::updateAccountTimeProtoBuf(const protobuf::AccountUpdateTime& accountUpdateTimeProto) {}
void TestCppClient::accountDataEndProtoBuf(const protobuf::AccountDataEnd& accountDataEndProto) {}
void TestCppClient::managedAccountsProtoBuf(const protobuf::ManagedAccounts& managedAccountsProto) {}
void TestCppClient::positionProtoBuf(const protobuf::Position& positionProto) {}
void TestCppClient::positionEndProtoBuf(const protobuf::PositionEnd& positionEndProto) {}
void TestCppClient::accountSummaryProtoBuf(const protobuf::AccountSummary& accountSummaryProto) {}
void TestCppClient::accountSummaryEndProtoBuf(const protobuf::AccountSummaryEnd& accountSummaryEndProto) {}
void TestCppClient::positionMultiProtoBuf(const protobuf::PositionMulti& positionMultiProto) {}
void TestCppClient::positionMultiEndProtoBuf(const protobuf::PositionMultiEnd& positionMultiEndProto) {}
void TestCppClient::accountUpdateMultiProtoBuf(const protobuf::AccountUpdateMulti& accountUpdateMultiProto) {}
void TestCppClient::accountUpdateMultiEndProtoBuf(const protobuf::AccountUpdateMultiEnd& accountUpdateMultiEndProto) {}
void TestCppClient::historicalDataProtoBuf(const protobuf::HistoricalData& historicalDataProto) {}
void TestCppClient::historicalDataUpdateProtoBuf(const protobuf::HistoricalDataUpdate& historicalDataUpdateProto) {}
void TestCppClient::historicalDataEndProtoBuf(const protobuf::HistoricalDataEnd& historicalDataEndProto) {}
void TestCppClient::realTimeBarTickProtoBuf(const protobuf::RealTimeBarTick& realTimeBarTickProto) {}
void TestCppClient::headTimestampProtoBuf(const protobuf::HeadTimestamp& headTimestampProto) {}
void TestCppClient::histogramDataProtoBuf(const protobuf::HistogramData& histogramDataProto) {}
void TestCppClient::historicalTicksProtoBuf(const protobuf::HistoricalTicks& historicalTicksProto) {}
void TestCppClient::historicalTicksBidAskProtoBuf(const protobuf::HistoricalTicksBidAsk& historicalTicksBidAskProto) {}
void TestCppClient::historicalTicksLastProtoBuf(const protobuf::HistoricalTicksLast& historicalTicksLastProto) {}
void TestCppClient::tickByTickDataProtoBuf(const protobuf::TickByTickData& tickByTickDataProto) {}
void TestCppClient::updateNewsBulletinProtoBuf(const protobuf::NewsBulletin& newsBulletinProto) {}
void TestCppClient::newsArticleProtoBuf(const protobuf::NewsArticle& newsArticleProto) {}
void TestCppClient::newsProvidersProtoBuf(const protobuf::NewsProviders& newsProvidersProto) {}
void TestCppClient::historicalNewsProtoBuf(const protobuf::HistoricalNews& historicalNewsProto) {
  printf("Historical News: %s\n", historicalNewsProto.ShortDebugString().c_str());
}
void TestCppClient::historicalNewsEndProtoBuf(const protobuf::HistoricalNewsEnd& historicalNewsEndProto) {
  printf("Historical News End: %s\n", historicalNewsEndProto.ShortDebugString().c_str());
}
void TestCppClient::wshMetaDataProtoBuf(const protobuf::WshMetaData& wshMetaDataProto) {}
void TestCppClient::wshEventDataProtoBuf(const protobuf::WshEventData& wshEventDataProto) {}
void TestCppClient::tickNewsProtoBuf(const protobuf::TickNews& tickNewsProto) {}
void TestCppClient::scannerParametersProtoBuf(const protobuf::ScannerParameters& scannerParametersProto) {}
void TestCppClient::scannerDataProtoBuf(const protobuf::ScannerData& scannerDataProto) {}
void TestCppClient::fundamentalsDataProtoBuf(const protobuf::FundamentalsData& fundamentalsDataProto) {}
void TestCppClient::pnlProtoBuf(const protobuf::PnL& pnlProto) {}
void TestCppClient::pnlSingleProtoBuf(const protobuf::PnLSingle& pnlSingleProto) {}
void TestCppClient::receiveFAProtoBuf(const protobuf::ReceiveFA& receiveFAProto) {}
void TestCppClient::replaceFAEndProtoBuf(const protobuf::ReplaceFAEnd& replaceFAEndProto) {}
void TestCppClient::commissionAndFeesReportProtoBuf(const protobuf::CommissionAndFeesReport& commissionAndFeesReportProto) {}
void TestCppClient::historicalScheduleProtoBuf(const protobuf::HistoricalSchedule& historicalScheduleProto) {}
void TestCppClient::rerouteMarketDataRequestProtoBuf(const protobuf::RerouteMarketDataRequest& rerouteMarketDataRequestProto) {}
void TestCppClient::rerouteMarketDepthRequestProtoBuf(const protobuf::RerouteMarketDepthRequest& rerouteMarketDepthRequestProto) {}
void TestCppClient::secDefOptParameterProtoBuf(const protobuf::SecDefOptParameter& secDefOptParameterProto) {}
void TestCppClient::secDefOptParameterEndProtoBuf(const protobuf::SecDefOptParameterEnd& secDefOptParameterEndProto) {}
void TestCppClient::softDollarTiersProtoBuf(const protobuf::SoftDollarTiers& softDollarTiersProto) {}
void TestCppClient::familyCodesProtoBuf(const protobuf::FamilyCodes& familyCodesProto) {}
void TestCppClient::symbolSamplesProtoBuf(const protobuf::SymbolSamples& symbolSamplesProto) {}
void TestCppClient::smartComponentsProtoBuf(const protobuf::SmartComponents& smartComponentsProto) {}
void TestCppClient::marketRuleProtoBuf(const protobuf::MarketRule& marketRuleProto) {}
void TestCppClient::userInfoProtoBuf(const protobuf::UserInfo& userInfoProto) {}
void TestCppClient::nextValidIdProtoBuf(const protobuf::NextValidId& nextValidIdProto) {}
void TestCppClient::currentTimeProtoBuf(const protobuf::CurrentTime& currentTimeProto) {}
void TestCppClient::currentTimeInMillisProtoBuf(const protobuf::CurrentTimeInMillis& currentTimeInMillisProto) {}
void TestCppClient::verifyMessageApiProtoBuf(const protobuf::VerifyMessageApi& verifyMessageApiProto) {}
void TestCppClient::verifyCompletedProtoBuf(const protobuf::VerifyCompleted& verifyCompletedProto) {}
void TestCppClient::displayGroupListProtoBuf(const protobuf::DisplayGroupList& displayGroupListProto) {}
void TestCppClient::displayGroupUpdatedProtoBuf(const protobuf::DisplayGroupUpdated& displayGroupUpdatedProto) {}
void TestCppClient::marketDepthExchangesProtoBuf(const protobuf::MarketDepthExchanges& marketDepthExchangesProto) {}
void TestCppClient::configResponseProtoBuf(const protobuf::ConfigResponse& configResponseProto) {
  printf("==== Config Response Begin ====\n");
  printf("%s\n", configResponseProto.DebugString().c_str());
  printf("==== Config Response End ====\n");
}
void TestCppClient::updateConfigResponseProtoBuf(const protobuf::UpdateConfigResponse& updateConfigResponseProto) {
  printf("==== Update Config Response Begin ====\n");
  printf("%s\n", updateConfigResponseProto.DebugString().c_str());
  printf("==== Update Config Response End ====\n");
}

void TestCppClient::ctor_helpers() {
  contract_template.symbol = {"LLY"};
  contract_template.secType =  {"STK"};
  contract_template.exchange = {"SMART"};
  contract_template.currency = {"USD"};
  contract_template.exchange = {"SMART"};
  contract_template.primaryExchange = {"NASDAQ"};

  print_once_positions_ = print_once_positions_default_;

  account_value_ = account_value_default_;

  buying_power_ = buying_power_default_;
}


bool TestCppClient::req_position_end_cond_var_trigger_ = {req_position_end_cond_var_trigger_default_};
std::mutex TestCppClient::req_position_end_mtx_;
std::condition_variable TestCppClient::req_position_end_cond_var_;


std::condition_variable TestCppClient::req_position_price_end_cond_var_;
std::mutex TestCppClient::req_position_price_end_mtx_;
bool TestCppClient::req_position_price_end_cond_var_trigger_ = {req_position_price_end_cond_var_trigger_default_};

std::condition_variable TestCppClient::account_update_end_cond_var_;
std::mutex TestCppClient::account_update_end_mtx_;
bool TestCppClient::account_update_end_cond_var_trigger_ = {account_update_end_cond_var_trigger_default_};



std::condition_variable TestCppClient::req_open_oder_end_cond_var_;
std::mutex TestCppClient::req_open_oder_end_mtx_;
bool TestCppClient::req_open_oder_cond_var_trigger_ = {req_open_oder_cond_var_trigger_default_};






std::map<int, AssetPrice> TestCppClient::current_price_list_;

Json::Value CurrentAccountState::account_update_state_;
Json::Value TestCppClient::account_summary_;


TmuxManagement TestCppClient::tmux_management;

template<typename T>
void TmuxManagement::send_msg_to_last_ttys(const T&& msg) {

  auto the_last_TMUX_warrior = tmux_ttys_.at(tmux_ttys_.size()-1);

  std::string&& command = std::string("echo ") + std::string(msg)  + std::string(" > ") + the_last_TMUX_warrior;

  //   std::cout << "command:" << command << std::endl;
  std::erase(command, '\n');
  std::erase(command, '\r');


  ::exec(command);


  //  command = std::string("echo ") + std::string("\n") +   std::string(" > ") + the_last_TMUX_warrior;
  //
  //  ::exec(command);
}

void TmuxManagement::populate_tmux_ttys() {

  tmux_ttys_ = get_tmux_ttys();

  std::cout << "tmux_result:" << std::endl;

  fmt::print(fg(fmt::color::green),"{}\n",tmux_ttys_);

  //  std::sort(tmux_ttys_.begin(),tmux_ttys_.end());
  //
  //  fmt::print(fg(fmt::color::green),"{}\n",tmux_ttys_);


  auto the_last_TMUX_warrior = tmux_ttys_.at(tmux_ttys_.size()-1);

  std::string command = std::string("echo ") + std::string("hello tty:") +  the_last_TMUX_warrior  + std::string(" > ") + the_last_TMUX_warrior;

  std::cout << command << std::endl;

  ::exec(command);


  command = std::string("echo ") + std::string("\n") +   std::string(" > ") + the_last_TMUX_warrior;

  ::exec(command);
}

void TmuxManagement::ctor_helpers() {
  populate_tmux_ttys();
}

void TmuxManagement::dtor_helpers() {
}

TmuxManagement::TmuxManagement() {
  ctor_helpers();

}
TmuxManagement::~TmuxManagement() {

  dtor_helpers();
}


template<typename T>
void TmuxManagement::send_msg_to_last_ttys(const T&& msg, int index) {


  auto the_TMUX_warrior = tmux_ttys_.at(index);

  std::string&& command = std::string("echo ") + std::string(msg)  + std::string(" > ") + the_TMUX_warrior;
  std::cout << command << std::endl;
  ::exec(command);
}
#endif
