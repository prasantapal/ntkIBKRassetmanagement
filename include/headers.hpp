#ifndef HEADERS_HPP
#define HEADERS_HPP

#define IS_MAD_CONVERSION_FACTOR_APPLIED 
// #undef IS_MAD_CONVERSION_FACTOR_APPLIED 
#define IS_MEAD_CONVERSION_FACTOR_APPLIED 
// #undef IS_MEAD_CONVERSION_FACTOR_APPLIED 

#define MEAD_CONVERSION_FACTOR 1.253314
#define MAD_CONVERSION_FACTOR 1.4826

std::vector<std::string> tokenize(const std::string& s, char c) {//taken from http://stackoverflow.com/questions/9435385/split-a-string-using-c11
  auto end = s.cend();
  auto start = end;
  std::vector<std::string> v;
  for( auto it = s.cbegin(); it != end; ++it ) {
    if( *it != c ) {
      if( start == end )
        start = it;
      continue;
    }
    if( start != end ) {
      v.emplace_back(start, it);
      start = end;
    }
  }
  if( start != end )
    v.emplace_back(start, end);
  return v;
}


std::vector<std::string> get_tmux_ttys() {
  std::vector<std::string> ttys;
  // -F specifies format for pane_tty, -t explicitly queries the current session
  std::string cmd = "tmux list-panes -t \"$(tmux display-message -p '#S')\" -F \"#{pane_tty}\"";

  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe) return ttys;

  char buffer[128];
  std::string result = "";
  while (fgets(buffer, 128, pipe.get()) != nullptr) {
    result += buffer;
  }

  std::stringstream ss(result);
  std::string tty;
  while (std::getline(ss, tty)) {
    // Remove potential trailing carriage returns
    if (!tty.empty() && tty.back() == '\r') {
      tty.pop_back();
    }
    if (!tty.empty()) {
      ttys.push_back(tty);
    }
  }

  return ttys;
}

/**
 * Calculate the median
 */
template<typename T>
double calculate_median(const T& vec){
  T local_copy_vec(vec);
  std::sort(std::execution::par, local_copy_vec.begin(),local_copy_vec.end());
  auto size = local_copy_vec.size();
  auto neighborhood_timeseries_data_half_size = size/2;
  double median = {std::numeric_limits<double>::quiet_NaN()}; ///@brief initialize with nan value so we know when it is replaced by actual timeseries_data
                                                              //assert(neighborhood_timeseries_data_half_size >= 1); ///@brief neighborhood_timeseries_data_half_size has to be within proper bounds
  if(size%2 == 0){
    median = (local_copy_vec[(neighborhood_timeseries_data_half_size-1)] + local_copy_vec[neighborhood_timeseries_data_half_size] )/2.0;
  }else {
    median = local_copy_vec[neighborhood_timeseries_data_half_size];
  }
  return median;
}

template<typename T>
std::tuple<double, double, double, double> calculate_median_mad_mean_mead(const T&  vec) {
  double median = std::numeric_limits<double>::quiet_NaN();
  double mad = std::numeric_limits<double>::quiet_NaN();
  double mean = std::numeric_limits<double>::quiet_NaN();
  double mead = std::numeric_limits<double>::quiet_NaN();
  static double seed_sum = {0.0};

  // if(vec.size()>0) { // should be checked from the caller
  // std::copy(vec.begin(), vec.end(),std::ostream_iterator<double>(std::cout, ","));
  // std::cout << std::endl;

  double sum = std::accumulate(vec.begin(), vec.end(), seed_sum);
  mean = sum/vec.size();

  //    std::cout << "sum:" << sum << std::endl;

  median = std::move(calculate_median(vec));
  std::vector<double> mad_vec;
  std::transform(vec.begin(),vec.end(), std::back_inserter(mad_vec),[median](const auto& x){
      return std::fabs(x - median);
      });
  mad = std::move(calculate_median(mad_vec));
  auto mead_sum = std::accumulate(mad_vec.begin(), mad_vec.end(), seed_sum);
  mead = mead_sum/vec.size();

  //    std::cout << "mead_sum:" << mead_sum << std::endl;
  //  double mad = is_mad_conversion_factor_applied_?calculated_mad*MAD_CONVERSION_FACTOR_:calculated_mad;
#ifdef IS_MAD_CONVERSION_FACTOR_APPLIED
  mad *= MAD_CONVERSION_FACTOR ;
#endif
#ifdef IS_MEAD_CONVERSION_FACTOR_APPLIED
  mead *= MEAD_CONVERSION_FACTOR;
#endif

  //  }else {
  //    std::cerr << "WARNING:" << " stat of zero sized vector..." << std::endl;
  //  }

  return std::tuple<double, double, double, double>(median, mad, mean, mead);
}




#endif
