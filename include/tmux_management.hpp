#ifndef TMUX_MANAGEMENT_HPP
#define TMUX_MANAGEMENT_HPP
class TmuxManagement {
  public:
    TmuxManagement();
    ~TmuxManagement();
    void ctor_helpers();
    void dtor_helpers();
    void populate_tmux_ttys();
    template<typename T>
      void send_msg_to_last_ttys(const T&& msg);
    template<typename T>
      void send_msg_to_last_ttys(const T&& msg, int index);


        void reset_ttys();
        void print_ttys() const;

        private:
        std::vector<std::string>   tmux_ttys_;

      };

    template<typename T>
      void TmuxManagement::send_msg_to_last_ttys(const T&& msg, int index) {



        auto the_TMUX_warrior = tmux_ttys_.at(index);

        std::string&& command = std::string("echo ") + std::string(msg)  + std::string(" > ") + the_TMUX_warrior;
        std::cout << command << std::endl;
        ::exec(command);
      }



    template<typename T>
      void TmuxManagement::send_msg_to_last_ttys(const T&& msg) {



        auto the_last_TMUX_warrior = tmux_ttys_.at(tmux_ttys_.size()-1);

        std::string&& command = std::string("echo ") + std::string(msg)  + std::string(" > ") + the_last_TMUX_warrior;

        std::cout << command << std::endl;

        ::exec(command);


        command = std::string("echo ") + std::string("\n") +   std::string(" > ") + the_last_TMUX_warrior;

        ::exec(command);
      }

    void TmuxManagement::populate_tmux_ttys() {

      tmux_ttys_ = get_tmux_ttys();

      std::cout << "tmux_result:" << std::endl;

      fmt::print(fg(fmt::color::green),"{}\n",tmux_ttys_);

      std::sort(tmux_ttys_.begin(),tmux_ttys_.end());

      fmt::print(fg(fmt::color::green),"{}\n",tmux_ttys_);


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

#endif
