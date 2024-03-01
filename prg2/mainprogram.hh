#ifndef MAINPROGRAM_HH
#define MAINPROGRAM_HH


#ifdef QT_CORE_LIB
#ifndef NO_GRAPHICAL_GUI
#define GRAPHICAL_GUI
#endif
#endif


#include <string>
#include <random>
#include <regex>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <utility>
#include <variant>
#include <bitset>
#include <cassert>
#include <unordered_set>

#include "datastructures.hh"

class MainWindow; // In case there's UI

struct TupleHash{
    size_t operator()(const std::tuple<StationID,TrainID,Time>& tuple) const{
        auto sidhash = std::hash<StationID>()(std::get<0>(tuple));
        auto tidhash = std::hash<TrainID>()(std::get<1>(tuple));
        // Combine hash values (magic!)
        auto newhash = sidhash ^ (tidhash + 0x9e3779b9 + (sidhash << 6) + (sidhash >> 2));
        auto thash = std::hash<Time>()(std::get<2>(tuple));
        return newhash ^ (thash + 0x9e3779b9 + (newhash << 6) + (newhash >> 2));
    }
};

const Coord DEFAULT_MIN_COORD={1,1};
const Coord DEFAULT_MAX_COORD={10000,10000};
const double ROOT_BIAS_MULTIPLIER = 0.05;
const double LEAF_BIAS_MULTIPLIER = 0.5;


class MainProgram
{
public:
    MainProgram();


    class Stopwatch;

    enum class PromptStyle { NORMAL, NO_ECHO, NO_NESTING };
    enum class TestStatus { NOT_RUN, NO_DIFFS, DIFFS_FOUND };

    bool command_parse_line(std::string input, std::ostream& output);
    void command_parser(std::istream& input, std::ostream& output, PromptStyle promptstyle);

    void setui(MainWindow* ui);

    void flush_output(std::ostream& output);
    bool check_stop() const;

    static int mainprogram(int argc, char* argv[]);

private:
    Datastructures ds_;
    MainWindow* ui_ = nullptr;

    static std::string const PROMPT;

    std::minstd_rand rand_engine_;

    static std::array<unsigned long int, 20> const primes1;
    static std::array<unsigned long int, 20> const primes2;
    unsigned long int prime1_ = 0; // Will be initialized to random value from above
    unsigned long int prime2_ = 0; // Will be initialized to random value from above
    unsigned long int random_stations_added_ = 0; // Counter for random stations added
    unsigned long int random_regions_added_ = 0; // Counter for random regions added
    unsigned long int random_trains_added_ = 0; // Counter for random trains added
    unsigned long int random_departures_added_=0;
    const unsigned int NAME_LENGTH = 8;
    const unsigned int STATIONID_LENGTH = 20;
    const unsigned int TRAINID_LENGTH = 5;
    const unsigned long long int MAX_REGIONID=std::numeric_limits<unsigned long int>::max();

    Coord min=DEFAULT_MIN_COORD;
    Coord max=DEFAULT_MAX_COORD;
    void init_primes();
    Name n_to_name(unsigned long int n);
    StationID n_to_stationid(unsigned long int n);
    RegionID n_to_regionid(unsigned long int n);
    TrainID n_to_trainid(unsigned long int n);
    Coord n_to_coord(unsigned long int n);


    std::unordered_set<StationID> valid_station_ids_;
    std::unordered_set<RegionID> valid_region_ids_;
    std::unordered_set<TrainID> valid_train_ids_;
    std::unordered_multiset<Coord> valid_coords_;
    std::vector<StationID> station_ids_;
    std::vector<RegionID> region_ids_;
    std::vector<Coord> coords_;
    std::vector<TrainID> trains_;

    std::unordered_set<std::tuple<StationID,TrainID,Time>,TupleHash> valid_departures_;
    std::vector<std::tuple<StationID,TrainID,Time>> departures_;

    std::unordered_multiset<Name> valid_station_names_;
    void clear_test_cache();

    RegionID random_root_region();
    RegionID random_leaf_region();
    StationID random_valid_station();
    RegionID random_valid_region();
    Coord random_valid_coord();
    std::tuple<StationID,TrainID,Time> random_valid_departure();
    TrainID random_valid_train();

    enum class StopwatchMode { OFF, ON, NEXT };
    StopwatchMode stopwatch_mode = StopwatchMode::OFF;

    enum class ResultType { NOTHING, IDLIST, NAMELIST, COORDLIST, ROUTE, TRAINS };
    using CmdResultIDs = std::pair<std::vector<RegionID>, std::vector<StationID>>;
    using CmdResultNames = std::pair<std::pair<StationID,RegionID>,Name>;
    using CmdResultCoords = std::pair<std::pair<StationID,RegionID>,std::vector<Coord>>;
    using CmdResultTrains = std::vector<std::tuple<TrainID, StationID, StationID, Time>>;
    using CmdResultRoute = std::vector<std::tuple<TrainID, StationID, StationID, Time, Distance>>;
    using CmdResult = std::pair<ResultType, std::variant<CmdResultIDs, CmdResultNames, CmdResultCoords, CmdResultTrains, CmdResultRoute>>;
    CmdResult prev_result;
    bool view_dirty = true;

    TestStatus test_status_ = TestStatus::NOT_RUN;

    using MatchIter = std::smatch::const_iterator;
    struct CmdInfo
    {
        std::string cmd;
        std::string info;
        std::string param_regex_str;
        CmdResult(MainProgram::*func)(std::ostream& output, MatchIter begin, MatchIter end);
        bool(MainProgram::*testfunc)(Stopwatch& stopwatch);
        std::regex param_regex = {};
    };
    static std::vector<CmdInfo> cmds_;
    // Regex objects and their initialization
    std::regex cmds_regex_;
    std::regex coords_regex_;
    std::regex stationtimes_regex_;
    std::regex times_regex_;
    std::regex commands_divide_regex_;
    std::regex timeouts_regex_;
    std::regex commands_regex_;
    std::regex sizes_regex_;
    std::regex range_regex_;
    std::regex is_range_notation_regex_;
    void init_regexs();


    CmdResult cmd_station_count(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_clear_all(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_all_stations(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_station(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_station_info(std::ostream& output, MatchIter begin, MatchIter end);

    CmdResult cmd_get_station_name(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_get_region_name(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_get_station_coordinates(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_get_region_coords(std::ostream& output, MatchIter begin, MatchIter end);


    CmdResult cmd_find_stations_with_coord(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_change_station_coord(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_departure(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_remove_departure(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_station_departures_after(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_all_regions(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_region_info(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_subregion_to_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_add_station_to_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_station_in_regions(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_all_subregions_of_region(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_stations_closest_to(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_remove_station(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_common_ancestor_of_regions(std::ostream& output, MatchIter begin, MatchIter end);

    CmdResult help_command(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_randseed(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_random_stations(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_random_trains(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_read(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_testread(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_stopwatch(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_perftest(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_comment(std::ostream& output, MatchIter begin, MatchIter end);

    // prg2 functions

    CmdResult cmd_add_train(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_next_stations_from(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_train_stations_from(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_clear_trains(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_any(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_least_stations(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_with_cycle(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_shortest_distance(std::ostream& output, MatchIter begin, MatchIter end);
    CmdResult cmd_route_earliest_arrival(std::ostream& output, MatchIter begin, MatchIter end);

    bool test_all_stations(Stopwatch &stopwatch);
    void test_get_functions(StationID id);
    bool test_station_info(Stopwatch& stopwatch);
    bool test_find_stations_with_coord(Stopwatch& stopwatch);
    bool test_change_station_coord(Stopwatch& stopwatch);
    bool test_add_departure(Stopwatch& stopwatch);
    bool test_remove_departure(Stopwatch& stopwatch);
    bool test_station_departures_after(Stopwatch& stopwatch);
    bool test_region_info(Stopwatch& stopwatch);
    bool test_station_in_regions(Stopwatch& stopwatch);
    bool test_all_subregions_of_region(Stopwatch& stopwatch);
    bool test_stations_closest_to(Stopwatch& stopwatch);
    bool test_remove_station(Stopwatch& stopwatch);
    bool test_common_ancestor_of_regions(Stopwatch& stopwatch);
    bool test_random_stations(Stopwatch& stopwatch);

    // new test functions with order checking
    bool test_station_count(Stopwatch& stopwatch);
    bool test_clear_all(Stopwatch& stopwatch);
    bool test_add_station(Stopwatch& stopwatch);
    bool test_get_station_name(Stopwatch& stopwatch);
    bool test_get_station_coordinates(Stopwatch& stopwatch);
    bool test_add_region(Stopwatch& stopwatch);
    bool test_all_regions(Stopwatch& stopwatch);
    bool test_get_region_name(Stopwatch& stopwatch);
    bool test_get_region_coords(Stopwatch& stopwatch);
    bool test_add_subregion_to_region(Stopwatch& stopwatch);
    bool test_add_station_to_region(Stopwatch& stopwatch);

    bool test_stations_alphabetically(Stopwatch &stopwatch);
    bool test_stations_distance_increasing(Stopwatch& stopwatch);

    // prg2

    bool test_add_train(Stopwatch &stopwatch);
    bool test_next_stations_from(Stopwatch &stopwatch);
    bool test_train_stations_from(Stopwatch &stopwatch);
    bool test_clear_trains(Stopwatch &stopwatch);
    bool test_route_any(Stopwatch &stopwatch);
    bool test_route_least_stations(Stopwatch &stopwatch);
    bool test_route_with_cycle(Stopwatch &stopwatch);
    bool test_route_shortest_distance(Stopwatch &stopwatch);
    bool test_route_earliest_arrival(Stopwatch &stopwatch);

    StationID random_station_id();
    Name random_name();
    Coord random_coord();
    RegionID random_region_id();
    TrainID random_train_id();
    Time random_time();
    std::tuple<StationID,TrainID,Time> random_departure();
    bool add_random_stations_regions_trains(Stopwatch & stopwatch,unsigned int size);
    Distance calc_distance(Coord c1, Coord c2);
    std::string print_station(StationID id, std::ostream& output, bool nl = true);
    std::string print_station_brief(StationID id, std::ostream& output, bool nl = true);
    std::string print_region(RegionID id, std::ostream& output, bool nl = true);
    std::string print_station_name(StationID id, std::ostream& output, bool nl = true);
    std::string print_coord(Coord coord, std::ostream& output, bool nl = true);
    std::string print_train(TrainID id, std::ostream& output, bool nl = true);
    std::string print_time(Time id, std::ostream& output, bool nl = true);
    template <typename Type>
    Type random(Type start, Type end);
    template <typename To>
    static To convert_string_to(std::string from);
    template <typename From>
    static std::string convert_to_string(From from);

    template<StationID(Datastructures::*MFUNC)()>
    CmdResult NoParStationCmd(std::ostream& output, MatchIter begin, MatchIter end);

    template<std::vector<StationID>(Datastructures::*MFUNC)()>
    CmdResult NoParListCmd(std::ostream& output, MatchIter begin, MatchIter end);

    template<StationID(Datastructures::*MFUNC)()>
    void NoParStationTestCmd();

    template<std::vector<StationID>(Datastructures::*MFUNC)()>
    void NoParListTestCmd();

    friend class MainWindow;
};

template <typename Type>
Type MainProgram::random(Type start, Type end)
{
    auto range = end-start;
    assert(range != 0 && "random() with zero range!");

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine_);

    return static_cast<Type>(start+num);
}

template <typename To>
To MainProgram::convert_string_to(std::string from)
{
    std::istringstream istr(from);
    To result;
    istr >> std::noskipws >> result;
    if (istr.fail() || !istr.eof())
    {
        throw std::invalid_argument("Cannot convert string to required type");
    }
    return result;
}

template <typename From>
std::string MainProgram::convert_to_string(From from)
{
    std::ostringstream ostr;
    ostr << from;
    if (ostr.fail())
    {
        throw std::invalid_argument("Cannot convert type to string");
    }
    return ostr.str();
}

template<StationID(Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParStationCmd(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    auto result = (ds_.*MFUNC)();
    return {ResultType::IDLIST, CmdResultIDs{{}, {result}}};
}

template<std::vector<StationID>(Datastructures::*MFUNC)()>
MainProgram::CmdResult MainProgram::NoParListCmd(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    auto result = (ds_.*MFUNC)();
    return {ResultType::IDLIST, CmdResultIDs{{}, result}};
}

template<StationID(Datastructures::*MFUNC)()>
void MainProgram::NoParStationTestCmd()
{
    (ds_.*MFUNC)();
}

template<std::vector<StationID>(Datastructures::*MFUNC)()>
void MainProgram::NoParListTestCmd()
{
    (ds_.*MFUNC)();
}


#ifdef USE_PERF_EVENT
extern "C"
{
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
                   group_fd, flags);
    return ret;
}
}
#endif

class MainProgram::Stopwatch
{
public:
    using Clock = std::chrono::high_resolution_clock;

    Stopwatch(bool use_counter = false) : use_counter_(use_counter)
    {
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            memset(&pe_, 0, sizeof(pe_));
            pe_.type = PERF_TYPE_HARDWARE;
            pe_.size = sizeof(pe_);
            pe_.config = PERF_COUNT_HW_INSTRUCTIONS;
            pe_.disabled = 1;
            pe_.exclude_kernel = 1;
            pe_.exclude_hv = 1;

            fd_ = perf_event_open(&pe_, 0, -1, -1, 0);
            if (fd_ == -1) {
                throw "Couldn't open perf events!";
            }
        }
#endif
        reset();
    }

    ~Stopwatch()
    {
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            close(fd_);
        }
#endif
    }

    void start()
    {
        running_ = true;
        starttime_ = Clock::now();
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            ioctl(fd_, PERF_EVENT_IOC_RESET, 0);
            read(fd_, &startcount_, sizeof(startcount_));
            ioctl(fd_, PERF_EVENT_IOC_ENABLE, 0);
        }
#endif
    }

    void stop()
    {
        running_ = false;
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0);
            long long c; read(fd_, &c, sizeof(c));
            counter_ += (c - startcount_);
        }
#endif
        elapsed_ += (Clock::now() - starttime_);
    }

    void reset()
    {
        running_ = false;
#ifdef USE_PERF_EVENT
        if (use_counter_)
        {
            ioctl(fd_, PERF_EVENT_IOC_DISABLE, 0);
            ioctl(fd_, PERF_EVENT_IOC_RESET, 0);
            counter_ = 0;
        }
#endif
        elapsed_ = elapsed_.zero();
    }

    double elapsed()
    {
        if (!running_)
        {
            return static_cast<std::chrono::duration<double>>(elapsed_).count();
        }
        else
        {
            auto total = elapsed_ + (Clock::now() - starttime_);
            return static_cast<std::chrono::duration<double>>(total).count();
        }
    }

#ifdef USE_PERF_EVENT
    long long count()
    {
        if (use_counter_)
        {
            if (!running_)
            {
                return counter_;
            }
            else
            {
                long long c;
                read(fd_, &c, sizeof(c));
                return counter_ + (c - startcount_);
            }
        }
        else
        {
            assert(!"perf_event not enabled during StopWatch creation!");
        }
    }
#endif

private:
    std::chrono::time_point<Clock> starttime_;
    Clock::duration elapsed_ = Clock::duration::zero();
    bool running_ = false;

    bool use_counter_;
#ifdef USE_PERF_EVENT
    struct perf_event_attr pe_;
    long long startcount_ = 0;
    long long counter_ = 0;
    int fd_ = 0;
#endif
};


#endif // MAINPROGRAM_HH
