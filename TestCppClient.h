/* Copyright (C) 2026 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef TWS_API_SAMPLES_TESTCPPCLIENT_TESTCPPCLIENT_H
#define TWS_API_SAMPLES_TESTCPPCLIENT_TESTCPPCLIENT_H
#define FMT_HEADER_ONLY
#include <iostream>
#include <fmt/core.h>
#include <memory>
#include <fmt/color.h>
#include <fmt/ranges.h>
#include <numeric>
#include <execution>
#include <vector>
#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Decimal.h>
#include <cxxopts.hpp>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <utility>

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <fmt/std.h>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <fmt/std.h>
#include <fmt/compile.h>
#include <fmt/ostream.h>
#include <fmt/printf.h>
#include <iostream>
#include <memory>
#include <string>
#include <json/json.h>

#include <iostream>
#include <chrono>
#include <cmath>
#include <chrono>
#include <cmath>

#include "ThreadPool.hpp"

#include "ThreadpoolSessionScheduler.hpp"

#include <boost/circular_buffer.hpp>
#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include "OrderState.h"
std::vector<std::string> get_tmux_ttys();
bool has_only_spaces(const std::string& str);
std::string exec(std::string cmd);

constexpr size_t _size = 10UL;
constexpr size_t TASKS = 10000UL;
constexpr size_t ITERATIONS = 100000UL;


#include "tmux_management.hpp"
#include <memory>
#include <vector>
#include <cstring>

#include <iostream>
#include <chrono>
#include <cmath>



#if THREADPOOL_ENABLE_SINGLETON
#include "ThreadPoolManager.hpp"
#endif

//constexpr size_t _size = 10UL;
//constexpr size_t TASKS = 10000UL;
//constexpr size_t ITERATIONS = 100000UL;





#if THREADPOOL_ENABLE_SINGLETON
void runThreadPoolManager(void) {
  auto& _thread_pool = ThreadPool::ThreadPoolManager::get_instance(_size).get_thread_pool();

  std::cout   << "Testing ThreadPoolManager Singleton"
    << " | ThreadMode mode: " << ThreadPool::ThreadMode_name(_thread_pool.mode()) 
    << " | Adoptive: " << std::boolalpha << _thread_pool.adoptive()
    << " | Adoptive Tick: " << _thread_pool.adoptive_tick_size() << std::endl;

}// end void runThreadPoolManager(void)
#endif




struct OrderStatus {

  int orderId;
  std::string status;
  double remaining;
  double filled;
  double avgFillPrice;
  double lastFillPrice;
  std::string whyHeld;
  double mktCapPrice;


};

struct OpenOrderInfo {
  int orderId;
  Contract contract;
  Order order;
  OrderState orderState;
};

struct AssetPrice {
  float last_price_;
  float bid_price_;
  float ask_price_;
  std::vector<int> tick_size_;
  static short int constexpr tick_size_field_max_length_ = {10};
};

struct PositionDetails {

  float num_positions_;
  float average_cost_;
};



struct CurrentAccountState {
  CurrentAccountState();
  ~CurrentAccountState();

  void ctor_helpers();
  void dtor_helpers();
  ///////////////////
  float SMA_;
  static float constexpr SMA_default_ = {0.0};

  float buying_power_;
  static float constexpr buying_power_default_ = {0.0};

  static float FullInitMarginReq_;
  static float FullMaintMarginReq_;
  static float FullAvailableFunds_;
  static float FullExcessLiquidity_;
  static float Cushion_;
  static float EquityWithLoanValue_;
  static float NetLiquidation_;
  static float BuyingPower_;
  static float GrossPositionValue_;
  static float InitMarginReq_;
  static float MaintMarginReq_;
  static float AvailableFunds_;
  static float ExcessLiquidity_;
  static float LookAheadInitMarginReq_;
  static float LookAheadMaintMarginReq_;
  static float LookAheadAvailableFunds_;
  static float LookAheadExcessLiquidity_;


  static Json::Value account_update_state_;

  void print() const;

};




enum State {
  ST_CONNECT,
  ST_TICKDATAOPERATION,
  ST_TICKDATAOPERATION_ACK,
  ST_TICKOPTIONCOMPUTATIONOPERATION,
  ST_TICKOPTIONCOMPUTATIONOPERATION_ACK,
  ST_DELAYEDTICKDATAOPERATION,
  ST_DELAYEDTICKDATAOPERATION_ACK,
  ST_MARKETDEPTHOPERATION,
  ST_MARKETDEPTHOPERATION_ACK,
  ST_REALTIMEBARS,
  ST_REALTIMEBARS_ACK,
  ST_MARKETDATATYPE,
  ST_MARKETDATATYPE_ACK,
  ST_HISTORICALDATAREQUESTS,
  ST_HISTORICALDATAREQUESTS_ACK,
  ST_OPTIONSOPERATIONS,
  ST_OPTIONSOPERATIONS_ACK,
  ST_CONTRACTOPERATION,
  ST_CONTRACTOPERATION_ACK,
  ST_MARKETSCANNERS,
  ST_MARKETSCANNERS_ACK,
  ST_FUNDAMENTALS,
  ST_FUNDAMENTALS_ACK,
  ST_BULLETINS,
  ST_BULLETINS_ACK,
  ST_ACCOUNTOPERATIONS,
  ST_ACCOUNTOPERATIONS_ACK,
  ST_ORDEROPERATIONS,
  ST_ORDEROPERATIONS_ACK,
  ST_OCASAMPLES,
  ST_OCASAMPLES_ACK,
  ST_CONDITIONSAMPLES,
  ST_CONDITIONSAMPLES_ACK,
  ST_BRACKETSAMPLES,
  ST_BRACKETSAMPLES_ACK,
  ST_HEDGESAMPLES,
  ST_HEDGESAMPLES_ACK,
  ST_TESTALGOSAMPLES,
  ST_TESTALGOSAMPLES_ACK,
  ST_FAORDERSAMPLES,
  ST_FAORDERSAMPLES_ACK,
  ST_FAOPERATIONS,
  ST_FAOPERATIONS_ACK,
  ST_DISPLAYGROUPS,
  ST_DISPLAYGROUPS_ACK,
  ST_MISCELANEOUS,
  ST_MISCELANEOUS_ACK,
  ST_CANCELORDER,
  ST_CANCELORDER_ACK,
  ST_FAMILYCODES,
  ST_FAMILYCODES_ACK,
  ST_SYMBOLSAMPLES,
  ST_SYMBOLSAMPLES_ACK,
  ST_REQMKTDEPTHEXCHANGES,
  ST_REQMKTDEPTHEXCHANGES_ACK,
  ST_REQNEWSTICKS,
  ST_REQNEWSTICKS_ACK,
  ST_REQSMARTCOMPONENTS,
  ST_REQSMARTCOMPONENTS_ACK,
  ST_NEWSPROVIDERS,
  ST_NEWSPROVIDERS_ACK,
  ST_REQNEWSARTICLE,
  ST_REQNEWSARTICLE_ACK,
  ST_REQHISTORICALNEWS,
  ST_REQHISTORICALNEWS_ACK,
  ST_REQHEADTIMESTAMP,
  ST_REQHEADTIMESTAMP_ACK,
  ST_REQHISTOGRAMDATA,
  ST_REQHISTOGRAMDATA_ACK,
  ST_REROUTECFD,
  ST_REROUTECFD_ACK,
  ST_MARKETRULE,
  ST_MARKETRULE_ACK,
  ST_PNL,
  ST_PNL_ACK,
  ST_PNLSINGLE,
  ST_PNLSINGLE_ACK,
  ST_CONTFUT,
  ST_CONTFUT_ACK,
  ST_PING,
  ST_PING_ACK,
  ST_REQHISTORICALTICKS,
  ST_REQHISTORICALTICKS_ACK,
  ST_REQTICKBYTICKDATA,
  ST_REQTICKBYTICKDATA_ACK,
  ST_WHATIFSAMPLES,
  ST_WHATIFSAMPLES_ACK,
  ST_IDLE,
  ST_IBKRATSSAMPLE,
  ST_IBKRATSSAMPLE_ACK,
  ST_WSH,
  ST_WSH_ACK,
  ST_CONFIG,
  ST_CONFIG_ACK,
  ST_ORDER_PARENTCHILD_OPERATIONS,
  ST_ORDER_PARENTCHILD_OPERATIONS_ACK,
  ST_NEWS_OPERATIONS_PROTO,
  ST_NEWS_OPERATIONS_PROTO_ACK,
  ST_TICK_DATA_OPERATIONS_PROTO,
  ST_TICK_DATA_OPERATIONS_PROTO_ACK
};

// Intent --> action
// e.g. at 3:15 PM a thread spins off to do EOD balancing
// e.g. at 3:15 PM a thread spins off to maximize profit taking
enum CurrentPositionsIntentList {
  ProfitTaking,
  EOD_balancing,
  SMA_balancing,
  EXCESS_LIQUIDITY_balancing
};

enum CurrentPositionsActionsList {
  CurrentAsset_SAMPLE,
  CurrentAsset_SELL
};

struct CurrentPositionsActions {
  CurrentPositionsActions();
  ~CurrentPositionsActions();
  int action;
  float priority;
  static float constexpr priority_default = {100.0};
  static float constexpr action_default = {100.0};
};

//! [ewrapperimpl]
class TestCppClient : public EWrapper {
  //! [ewrapperimpl]
  public:

    TestCppClient();
    ~TestCppClient();

    double get_ticker_price(std::string ticker);
    void ctor_helpers();
    void setConnectOptions(const std::string&);
    void setOptionalCapabilities(const std::string&);
    void processMessages();

  public:
    void print_position_details();
    bool connect(const char * host, int port, int clientId = 0);
    void disconnect() const;
    bool isConnected() const;
    void placeOrder() ;

    void placeOrderCustom(std::string order_action_type_string, std::string symbol, double limit_price, int quantity, std::string order_type_string);
    void printAccountSummary();

  private:
    void pnlOperation();
    void pnlSingleOperation();
    void tickDataOperation();
    void tickOptionComputationOperation();
    void delayedTickDataOperation();
    void marketDepthOperations();
    void realTimeBars();
    void marketDataType();
    void historicalDataRequests();
    void optionsOperations();
    void accountOperations();
    void orderOperations();
    void ocaSamples();
    void conditionSamples();
    void bracketSample();
    void hedgeSample();
    void contractOperations();
    void marketScanners();
    void fundamentals();
    void bulletins();
    void testAlgoSamples();
    void financialAdvisorOrderSamples();
    void financialAdvisorOperations();
    void testDisplayGroups();
    void miscellaneous();
    void reqFamilyCodes();
    void reqMatchingSymbols();
    void reqMktDepthExchanges();
    void reqNewsTicks();
    void reqSmartComponents();
    void reqNewsProviders();
    void reqNewsArticle();
    void reqHistoricalNews();
    void reqHeadTimestamp();
    void reqHistogramData();
    void rerouteCFDOperations();
    void marketRuleOperations();
    void continuousFuturesOperations();
    void reqHistoricalTicks();
    void reqTickByTickData();
    void whatIfSamples();
    void ibkratsSample();
    void wshCalendarOperations();
    void configOperations();
    void orderParentChildOperations();
    void newsOperationsProto();
    void tickDataOperationsProto();

    void reqCurrentTime();

  public:
    // events
#include "EWrapper_prototypes.h"


  private:
    void printContractMsg(const Contract& contract);
    void printContractDetailsMsg(const ContractDetails& contractDetails);
    void printContractDetailsSecIdList(const TagValueListSPtr &secIdList);
    void printBondContractDetailsMsg(const ContractDetails& contractDetails);
    void printContractDetailsIneligibilityReasonList(const IneligibilityReasonListSPtr &ineligibilityReasonList);
    void printSoftDollarTier(const SoftDollarTier& softDollarTier);
    //void TestCppClient::placeOrder(std::string symbol, double limit_price, int quantity) {

    // 1. Define the Contract (e.g., AAPL Stock)
    Contract contract_template;
    //  contract.symbol = {"LLY"};
    //  contract.secType =  {"STK"};
    //  contract.exchange = {"SMART"};
    //  contract.currency = {"USD"};
    //  contract.exchange = {"SMART"};
    //  contract.primaryExchange = {"NASDAQ"};
    //




  private:
    //! [socket_declare]
    EReaderOSSignal m_osSignal;
    EClientSocket * const m_pClient;
    //! [socket_declare]
    State m_state;
    time_t m_sleepDeadline;

    int m_orderId;
    std::unique_ptr<EReader> m_pReader;
    bool m_extraAuth;
    std::string m_bboExchange;

    std::unique_ptr<std::jthread> reader_thread;
    bool print_once_positions_;
    std::mutex account_update_mutex_;

    std::mutex request_mutex_;
    static bool constexpr print_once_positions_default_ = {false};

    std::map<std::string, PositionDetails> position_details_;


    std::multimap<std::string, std::tuple<int, Contract, Order, OrderState>> open_orders_;

    double account_value_;
    static double constexpr account_value_default_  = {0.0};

    double buying_power_;
    static double constexpr buying_power_default_  = {0.0};
    CurrentAccountState current_account_state_;

    /////////////////////////////////////////////////////////////////////
    static std::map<int, AssetPrice> current_price_list_;

    static std::condition_variable req_position_end_cond_var_;
    static bool req_position_end_cond_var_trigger_;
    static bool constexpr req_position_end_cond_var_trigger_default_ = {false};
    static std::mutex req_position_end_mtx_;





    static std::condition_variable req_position_price_end_cond_var_;
    static bool req_position_price_end_cond_var_trigger_;
    static bool constexpr req_position_price_end_cond_var_trigger_default_ = {false};
    static std::mutex req_position_price_end_mtx_;

    static std::condition_variable account_update_end_cond_var_;
    static std::mutex account_update_end_mtx_;
    static bool account_update_end_cond_var_trigger_;
    static bool constexpr account_update_end_cond_var_trigger_default_ = {false};


    static std::condition_variable req_open_oder_end_cond_var_;
    static std::mutex req_open_oder_end_mtx_;
    static bool req_open_oder_cond_var_trigger_;
    static bool constexpr req_open_oder_cond_var_trigger_default_ = {false};

    static std::mutex open_order_update_lock_mtx_;


    static std::mutex req_open_oder_status_update_mtx_;
    static std::map<int,OrderStatus> open_order_status_update_;

    static std::mutex req_open_oder_status_update_proto_mtx_;
    static std::map<std::string,std::map<int,OrderStatus>> open_order_status_proto_update_;

    //    std::tuple<std::map<int,std::string>, std::tuple<int,std::atomic<int>> >  price_request_counter_;
    // ID, price, volume
    std::tuple<std::map<int,std::string>, std::tuple<int, std::tuple<std::atomic<int>,std::atomic<int>>> >  price_request_counter_;

    static Json::Value account_summary_;

    static TmuxManagement tmux_management;

    std::map<std::string, double> percent_deviation_threshold_map_;
    static constexpr double percent_deviation_threshold_map_default_threshold_ = {10.0};


    /// AUTO ACTION MODES
    //   std::set<CurrentPositionsActions> current_positions_action_modes_; 
    //    ThreadPool::ThreadPool<ThreadPool::ThreadMode::PRIORITY> threadpool_priority_(12);

    static std::unique_ptr<ThreadPool::ThreadPool<ThreadPool::ThreadMode::PRIORITY>> threadpool_priority_;
    static int threadpool_size_;
    static int constexpr threadpool_size_default_ = {16};

    std::set<std::string> streaming_data_candidates_;
    static float constexpr bid_ask_lowest_denominator_ = {0.01};
    static  int constexpr thread_pool_session_scheduler_size_ = {4};
    static std::unique_ptr<ThreadPoolSessionScheduler> scheduler_threadpool_;


    static bool constexpr should_initiate_task_schedulers_default_ = {true};
    static bool should_initiate_task_schedulers_;
  public:
    void  initiate_task_schedulers();
  private:


    static bool should_take_profit_;
    static bool constexpr should_take_profit_default_ = {true};

    static float profit_minimum_threshold_;
    static float constexpr profit_minimum_threshold_default_ = {50.0};

    static float profit_percent_threshold_;
    static float constexpr profit_percent_threshold_default_ = {0.05};

    std::map<std::string,std::atomic<bool>> profit_taking_engagements_activation_;

    std::map<std::string,std::atomic<bool>> profit_taking_activity_finished_;



    std::map<std::string, std::unique_ptr<boost::circular_buffer<float>>> tracked_circular_buffer_tick_value_;
    std::map<std::string, std::unique_ptr<boost::circular_buffer<float>>> tracked_circular_buffer_tick_size_;

    std::set<std::string> tracked_assets_;
    std::vector<int> tracked_assets_IDs_;

    std::vector<std::future<void>> tracked_assets_IDs_futures_;

    std::map<std::string, int> tracked_assets_IDs_symbol_map_;
    std::map<int, std::string> tracked_assets_IDs_symbol_map_inverse_;

      static float constexpr limit_price_percent_above_current_level_ = {0.005};

};


#endif

