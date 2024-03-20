// The main program (provided by the course), TIE-20100/TIE-20106
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do so
//  (for debugging, for example), DO NOT commit any changes to git, but revert all
//  changes later. Otherwise you won't be able to get any updates/fixes to this
//  file from git!)

#include <string>
using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;
using std::noskipws;

#include <iomanip>
using std::setfill;
using std::setw;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;

#include <iomanip>
using std::setw;

#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;

#include <regex>
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::regex;
using std::sregex_token_iterator;

#include <algorithm>
using std::find_if;
using std::find;
using std::binary_search;
using std::max_element;
using std::max;
using std::min;
using std::shuffle;
using std::sort;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

#include <chrono>

#include <functional>
using std::function;
using std::equal_to;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <array>
using std::array;

#include <bitset>
using std::bitset;

#include <iterator>
using std::next;

#include <ctime>
using std::time;

#include <memory>
using std::move;

#include <utility>
using std::pair;
using std::make_pair;

#include <cmath>
using std::abs;

#include <cstdlib>
using std::div;

#include <algorithm>
using std::transform;

#include <iterator>
using std::back_inserter;

#include <cstddef>
#include <cassert>


#include "mainprogram.hh"

#include "datastructures.hh"

#ifdef GRAPHICAL_GUI
#include "mainwindow.hh"
#endif

string const MainProgram::PROMPT = "> ";

template<typename C, typename F> size_t remove_if_from(C& container, F fun){
    auto old_size = container.size();
    for (auto first = container.begin();first!=container.end();){
        if (fun(*first)){
            first=container.erase(first);
        }else{
            ++first;
        }
    }
    return old_size-container.size();
}

template<typename T> void unstable_vector_erase(std::vector<T>& vector, const T& to_remove){
    auto findit = std::find(vector.begin(),vector.end(),to_remove);
    if(findit == vector.end()){
        return;
    }
    std::swap(*findit,vector.back());
    vector.pop_back();
}

void MainProgram::test_get_functions(StationID id)
{
    ds_.get_station_name(id);
    ds_.get_station_coordinates(id);
}

MainProgram::CmdResult MainProgram::cmd_add_station(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    StationID id = *begin++;
    string name = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    bool success = ds_.add_station(id, name, {x, y});

    view_dirty = true;
    return {ResultType::IDLIST, CmdResultIDs{{}, {success ? id : NO_STATION}}};
}

MainProgram::CmdResult MainProgram::cmd_station_info(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    StationID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");
    return {ResultType::IDLIST, CmdResultIDs{{}, {id}}};
}

MainProgram::CmdResult MainProgram::cmd_get_station_name(std::ostream &/*output*/, MatchIter begin, MatchIter end)
{
    StationID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");
    Name station_name = ds_.get_station_name(id);
    return {ResultType::NAMELIST, CmdResultNames{{id,NO_REGION},station_name}};
}

MainProgram::CmdResult MainProgram::cmd_get_region_name(std::ostream &/*output*/, MatchIter begin, MatchIter end)
{
    RegionID id = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");
    Name region_name = ds_.get_region_name(id);
    return {ResultType::NAMELIST, CmdResultNames{{NO_STATION,id},region_name}};
}

MainProgram::CmdResult MainProgram::cmd_get_station_coordinates(std::ostream &/*output*/, MatchIter begin, MatchIter end)
{
    StationID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");
    Coord station_coordinates = ds_.get_station_coordinates(id);
    return {ResultType::COORDLIST, CmdResultCoords{{id,NO_REGION},{station_coordinates}}};
}

MainProgram::CmdResult MainProgram::cmd_get_region_coords(std::ostream &/*output*/, MatchIter begin, MatchIter end)
{
    RegionID id = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");
    std::vector<Coord> region_coordinates = ds_.get_region_coords(id);
    return {ResultType::COORDLIST, CmdResultCoords{{NO_STATION,id},region_coordinates}};
}

bool MainProgram::test_station_info(Stopwatch &stopwatch)
{
    if(!test_get_station_name(stopwatch)){
        return false;
    }
    return test_get_station_coordinates(stopwatch);
}

MainProgram::CmdResult MainProgram::cmd_change_station_coord(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    StationID id = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    bool success = ds_.change_station_coord(id, {x,y});

    view_dirty = true;
    return {ResultType::IDLIST, CmdResultIDs{{}, {success ? id : NO_STATION}}};
}

MainProgram::CmdResult MainProgram::cmd_add_departure(std::ostream& output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    TrainID trainid = *begin++;
    Time time = convert_string_to<Time>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool success = ds_.add_departure(stationid, trainid, time);

    if (success)
    {
        output << "Train " << trainid << " leaves from station ";
        print_station_brief(stationid, output, false);
        output << " at " << setw(4) << setfill('0') << time << endl;
        //        return {ResultType::IDLIST, CmdResultIDs{{}, {stationid}}};
        return {};
    }
    else
    {
        output << "Adding departure failed!" << endl;
        return {};
    }
}

bool MainProgram::test_add_departure(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }
    std::tuple<StationID,TrainID,Time> departure;
    for(departure=random_departure();
         valid_departures_.find(departure)!=valid_departures_.end();
         departure=random_departure()){}
    stopwatch.start();
    bool retval = ds_.add_departure(std::get<0>(departure),std::get<1>(departure),std::get<2>(departure));
    stopwatch.stop();
    valid_departures_.insert(departure);
    departures_.push_back(departure);
    ++random_departures_added_;
    return retval;
}

MainProgram::CmdResult MainProgram::cmd_remove_departure(std::ostream &output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    TrainID trainid = *begin++;
    Time time = convert_string_to<Time>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool success = ds_.remove_departure(stationid, trainid, time);

    if (success)
    {
        output << "Removed departure of train " << trainid << " from station ";
        print_station_brief(stationid, output, false);
        output << " at " << setw(4) << setfill('0') << time << endl;
        //        return {ResultType::IDLIST, CmdResultIDs{{}, {stationid}}};
        return {};
    }
    else
    {
        output << "Removing departure failed!" << endl;
        return {};
    }
}

bool MainProgram::test_remove_departure(Stopwatch &stopwatch)
{
    if(random_departures_added_<1){
        return true;
    }
    auto tuple = random_valid_departure();

    stopwatch.start();
    bool result = ds_.remove_departure(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
    stopwatch.stop();

    auto findit = valid_departures_.find(tuple);
    auto vecit = std::find(departures_.begin(),departures_.end(),tuple);
    if(findit==valid_departures_.end() || vecit==departures_.end()){
        return false;
    }
    valid_departures_.erase(findit);
    departures_.erase(vecit);
    --random_departures_added_;
    return result;
}

MainProgram::CmdResult MainProgram::cmd_station_departures_after(std::ostream &output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    Time time = convert_string_to<Time>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    auto departures = ds_.station_departures_after(stationid, time);

    if (departures.size() == 1 && departures.front() == std::make_pair(NO_TIME, NO_TRAIN))
    {
        output << "No such station (NO_TIME, NO_TRAIN returned)" << endl;
        return {};
    }

    if (!departures.empty())
    {
        output << "Departures from station ";
        print_station_brief(stationid, output, false);
        output << " after " << setw(4) << setfill('0') << time << ":" << endl;
        for (auto& [deptime, trainid] : departures)
        {
            output << " " << trainid << " at " << setw(4) << setfill('0') << deptime << endl;
        }
    }
    else
    {
        output << "No departures from station ";
        print_station_brief(stationid, output, false);
        output << " after " << time << endl;
    }

    //    return {ResultType::IDLIST, CmdResultIDs{{}, {stationid}}};
    return {};
}

bool MainProgram::test_station_departures_after(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }

    auto id = random_valid_station();
    auto time = random_time();
    std::vector<std::pair<Time,TrainID>> result;
    stopwatch.start();
    result = ds_.station_departures_after(id, time);
    stopwatch.stop();

    if(random_departures_added_<1 && result.size()!=0){
        return false;
    }
    return !(result.size()==1 && result.front()==std::pair<Time,TrainID>{NO_TIME, NO_TRAIN});
}

bool MainProgram::test_change_station_coord(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }

    auto id = random_valid_station();
    Coord oldcoord=ds_.get_station_coordinates(id);
    if(oldcoord == NO_COORD){
        return false;
    }
    Coord newcoord=random_coord();
    stopwatch.start();
    bool result = ds_.change_station_coord(id, newcoord);
    stopwatch.stop();
    auto find_iter = valid_coords_.find(oldcoord);
    auto vec_it = std::find(coords_.begin(),coords_.end(),oldcoord);
    if(find_iter==valid_coords_.end() || vec_it==coords_.end()){
        return false;
    }
    valid_coords_.erase(find_iter);
    valid_coords_.insert(newcoord);
    *vec_it=newcoord;

    return result;
}

MainProgram::CmdResult MainProgram::cmd_add_subregion_to_region(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    RegionID subregionid = convert_string_to<RegionID>(*begin++);
    RegionID parentid = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.add_subregion_to_region(subregionid, parentid);
    if (ok)
    {
        try
        {
            auto subregionname = ds_.get_region_name(subregionid);
            auto parentname = ds_.get_region_name(parentid);
            output << "Added '" << subregionname << "' as a subregion of '" << parentname << "'" << endl;
        }
        catch (NotImplemented&)
        {
            output << "Added a subregion to region." << endl;
        }
        return {ResultType::IDLIST, CmdResultIDs{{subregionid, parentid}, {}}};
    }
    else
    {
        output << "Adding a subregion failed!" << endl;
        return {};
    }
}

MainProgram::CmdResult MainProgram::cmd_add_station_to_region(std::ostream &output, MatchIter begin, MatchIter end)
{
    StationID stationid = *begin++;
    RegionID regionid = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.add_station_to_region(stationid, regionid);
    if (ok)
    {
        try
        {
            auto stationname = ds_.get_station_name(stationid);
            auto regionname = ds_.get_region_name(regionid);
            output << "Added '" << stationname << "' to region '" << regionname << "'" << endl;
        }
        catch (NotImplemented&)
        {
            output << "Added a station to region." << endl;
        }
        return {ResultType::IDLIST, CmdResultIDs{{regionid}, {stationid}}};
    }
    else
    {
        output << "Adding a station to region failed!" << endl;
        return {};
    }
}

MainProgram::CmdResult MainProgram::cmd_stations_closest_to(std::ostream &output, MatchIter begin, MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    auto stations = ds_.stations_closest_to({x,y});
    if (stations.empty())
    {
        output << "No stations!" << endl;
    }

    return {ResultType::IDLIST, CmdResultIDs{{}, stations}};
}

MainProgram::CmdResult MainProgram::cmd_common_ancestor_of_regions(std::ostream &output, MatchIter begin, MatchIter end)
{
    RegionID regionid1 = convert_string_to<RegionID>(*begin++);
    RegionID regionid2 = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");


    auto regionid = ds_.common_ancestor_of_regions(regionid1, regionid2);
    if (regionid == NO_REGION)
    {
        output << "No common parent region found." << endl;
    }

    return {ResultType::IDLIST, CmdResultIDs{{regionid1, regionid2, regionid}, {}}};
}

MainProgram::CmdResult MainProgram::cmd_station_in_regions(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    StationID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.station_in_regions(id);
    if (result.empty()) { output << "Station does not belong to any region." << std::endl; }
    return {ResultType::IDLIST, CmdResultIDs{result, {id}}};
}

bool MainProgram::test_station_in_regions(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }

    auto id = random_valid_station();
    stopwatch.start();
    auto result = ds_.station_in_regions(id);
    stopwatch.stop();
    std::unordered_set<RegionID> unique(result.begin(),result.end());
    if(unique.size()!=result.size()){
        return false;
    }
    for(const auto& rid : result){
        if(valid_region_ids_.find(rid)==valid_region_ids_.end()){
            return false;
        }
    }

    return true;
}

bool MainProgram::test_all_subregions_of_region(Stopwatch &stopwatch)
{
    if(random_regions_added_<1){
        return true;
    }

    auto id = random_root_region();
    stopwatch.start();
    auto result = ds_.all_subregions_of_region(id);
    stopwatch.stop();
    if(random_regions_added_ > 1 && (result.size()+1)*(pow(2,ROOT_BIAS_MULTIPLIER*random_regions_added_))<random_regions_added_){
        return false;
    }
    std::unordered_set<RegionID> unique(result.begin(),result.end());
    if(result.size()!=unique.size()){
        return false;
    }
    // check validity of each id
    for(const auto& rid : result){
        if(valid_region_ids_.find(rid)==valid_region_ids_.end()){
            return false;
        }
    }

    return true;
}

MainProgram::CmdResult MainProgram::cmd_remove_station(ostream& output, MatchIter begin, MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto name = ds_.get_station_name(id);
    bool success = ds_.remove_station(id);
    if (success)
    {
        output << name << " removed." << endl;
        view_dirty = true;
        return {};
    }
    else
    {
        return {ResultType::IDLIST, CmdResultIDs{{}, {NO_STATION}}};
    }
}

bool MainProgram::test_remove_station(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }

    StationID stationid = random_valid_station();
    Name name = ds_.get_station_name(stationid);
    if(name==NO_NAME){
        return false;
    }
    Coord coords = ds_.get_station_coordinates(stationid);
    if(coords==NO_COORD){
        return false;
    }
    stopwatch.start();
    ds_.remove_station(stationid);
    stopwatch.stop();

    // remove from caches
    valid_station_ids_.erase(stationid);

    unstable_vector_erase(station_ids_,stationid);
    departures_.erase(std::remove_if(departures_.begin(),departures_.end(),[&stationid](auto& tuple){
                          return std::get<0>(tuple)==stationid;
                      }),departures_.end());

    remove_if_from(valid_departures_,[&stationid](auto& tuple){
        return std::get<0>(tuple)==stationid;
    });
    --random_stations_added_;
    return true;
}



bool MainProgram::add_random_stations_regions_trains(Stopwatch &stopwatch, unsigned int size)
{
    // first add regions so that there are #_of_stations/10 regions
    unsigned long long int regions_added_this_time = 0;
    for (; random_regions_added_<(random_stations_added_+size)/10; ++regions_added_this_time){
        if (!test_add_region(stopwatch)){
            return false;
        }
    }
    // create a binary tree from the regions
    for (unsigned int i=(random_regions_added_-regions_added_this_time); i<random_regions_added_; ++i){
        auto regionid = n_to_regionid(i);
        auto parentid = n_to_regionid(i/ 2);
        if(i==(i/2)){
            continue;
        }
        stopwatch.start();
        bool retval=ds_.add_subregion_to_region(regionid, parentid);
        stopwatch.stop();
        if (!retval){
            return false;
        }
    }

    // Add the wanted number of stations
    for (unsigned int i = 0; i < size; ++i)
    {
        bool retval = test_add_station(stopwatch);
        if (!retval){
            return false;
        }
    }
    if(random_regions_added_>0){
        // only for new stations
        for (auto it = std::prev(station_ids_.end(),size);it!=station_ids_.end();++it){
            const auto& id = *it;
            // With a 50 % chance, add station to random region
            if (random(0,2) == 0)
            {
                auto rid = random_valid_region();
                stopwatch.start();
                bool retval = ds_.add_station_to_region(id, rid);
                stopwatch.stop();
                if (!retval){
                    return false;
                }
            }
        }
    }

    // add n departures to random stations with random time (NOT USED IN PRG2)

    // for (unsigned int i = 0; i< size; ++i){
    //     if(!test_add_departure(stopwatch)){
    //         return false;
    //     }
    // }


    // add trains so that there are #_of_stations/1000 trains

    if (random_stations_added_ > 0) // Don't do anything if there's no stations
    {
        for (; random_trains_added_<random_stations_added_/1000; )
        {
            if(!test_add_train(stopwatch)){
                return false;
            }
        }
    }
    return true;
}

MainProgram::CmdResult MainProgram::cmd_random_stations(ostream& output, MatchIter begin, MatchIter end)
{
    string sizestr = *begin++;
    string minxstr = *begin++;
    string minystr = *begin++;
    string maxxstr = *begin++;
    string maxystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int size = convert_string_to<unsigned int>(sizestr);

    if (!minxstr.empty() && !minystr.empty() && !maxxstr.empty() && !maxystr.empty())
    {
        min.x = convert_string_to<unsigned int>(minxstr);
        min.y = convert_string_to<unsigned int>(minystr);
        max.x = convert_string_to<unsigned int>(maxxstr);
        max.y = convert_string_to<unsigned int>(maxystr);
    }
    else
    {
        auto stations = ds_.all_stations();
        if (!stations.empty())
        {
            // Find out bounding box
            min = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
            max = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
            for (auto const& station : stations)
            {
                auto [x,y] = ds_.get_station_coordinates(station);
                if (x < min.x) { min.x = x; }
                if (y < min.y) { min.y = y; }
                if (x > max.x) { max.x = x; }
                if (y > max.y) { max.y = y; }
            }
        }
    }
    Stopwatch dummy;
    if(!add_random_stations_regions_trains(dummy, size)){
        output << "Failed to add! There might be some leftover data in Datastructures" <<endl;
        // no need to make the view dirty, there is some
        return {};
    }

    output << "Added: " << size << " stations." << endl;

    view_dirty = true;

    return {};
}

bool MainProgram::test_random_stations(Stopwatch &stopwatch)
{
    // TODO checks and possible forwarding stopwatch to called function
    return add_random_stations_regions_trains(stopwatch,1);
}

bool MainProgram::test_station_count(Stopwatch &stopwatch)
{
    stopwatch.start();
    auto result = ds_.station_count();
    stopwatch.stop();
    return result==random_stations_added_;
}

bool MainProgram::test_clear_all(Stopwatch &stopwatch)
{
    stopwatch.start();
    ds_.clear_all();
    stopwatch.stop();
    clear_test_cache();
    return true;
}

bool MainProgram::test_add_station(Stopwatch &stopwatch)
{
    StationID stationid = n_to_stationid(random_stations_added_);
    Name stationname = n_to_name(random_stations_added_);
    Coord newcoord = n_to_coord(random_stations_added_);
    stopwatch.start();
    bool retval = ds_.add_station(stationid, stationname, newcoord);
    stopwatch.stop();
    ++random_stations_added_;
    return retval;
}

bool MainProgram::test_get_station_name(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }
    StationID id = random_valid_station();
    stopwatch.start();
    Name retval = ds_.get_station_name(id);
    stopwatch.stop();
    return retval != NO_NAME;
}

bool MainProgram::test_get_station_coordinates(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }
    StationID id = random_valid_station();
    stopwatch.start();
    Coord retval = ds_.get_station_coordinates(id);
    stopwatch.stop();
    return retval != NO_COORD;
}

bool MainProgram::test_add_region(Stopwatch &stopwatch)
{
    auto regionid = n_to_regionid(random_regions_added_);
    vector<Coord> coords;
    // possible to change the number of coords by random chance
    for (int j=0; j<3; ++j)
    {
        coords.push_back(random_coord());
    }
    Name regionname=convert_to_string(regionid);
    stopwatch.start();
    bool retval=ds_.add_region(regionid, regionname, std::move(coords));
    stopwatch.stop();
    ++random_regions_added_;
    return retval;
}

bool MainProgram::test_all_regions(Stopwatch &stopwatch)
{
    stopwatch.start();
    auto result = ds_.all_regions();
    stopwatch.stop();
    if(result.size()!=random_regions_added_){
        return false;
    }
    std::unordered_set<RegionID> unique(result.begin(),result.end());
    if(unique.size()!=result.size()){
        return false;
    }
    for(const auto& id : result){
        if(valid_region_ids_.find(id)==valid_region_ids_.end()){
            return false;
        }
    }
    return true;
}

bool MainProgram::test_get_region_name(Stopwatch &stopwatch)
{
    RegionID regionid = random_valid_region();
    Name regionname=convert_to_string(regionid);

    stopwatch.start();
    auto result = ds_.get_region_name(regionid);
    stopwatch.stop();
    return result == regionname;
}

bool MainProgram::test_get_region_coords(Stopwatch &stopwatch)
{
    RegionID regionid = random_valid_region();

    stopwatch.start();
    auto result = ds_.get_region_coords(regionid);
    stopwatch.stop();

    if (result.size()!=3){
        return false;
    }
    for (const auto& coord : result){
        if (coord==NO_COORD){
            return false;
        }
    }
    return true;
}

bool MainProgram::test_add_subregion_to_region(Stopwatch &stopwatch)
{
    Stopwatch dummy;
    // add a new subregion to continue making binary tree with
    unsigned int i = random_regions_added_;
    if (!test_add_region(dummy)){
        return false;
    }

    auto regionid = n_to_regionid(i);
    auto parentid = n_to_regionid(i/ 2);
    stopwatch.start();
    bool retval=ds_.add_subregion_to_region(regionid, parentid);
    stopwatch.stop();
    return retval;
}

bool MainProgram::test_add_station_to_region(Stopwatch &stopwatch)
{
    Stopwatch dummy;
    if(random_regions_added_<1){
        if(!test_add_region(dummy)){
            return false;
        }
    }
    auto regionid = random_valid_region();
    int i = random_stations_added_;
    if(!test_add_station(dummy)){
        return false;
    }
    auto stationid = n_to_stationid(i);
    stopwatch.start();
    bool retval = ds_.add_station_to_region(stationid,regionid);
    stopwatch.stop();
    return retval;
}

bool MainProgram::test_stations_alphabetically(Stopwatch &stopwatch)
{
    // get the ordered thing

    stopwatch.start();
    auto result = ds_.stations_alphabetically();
    stopwatch.stop();
    if (result.size() != random_stations_added_){
        return false;
    }
    std::unordered_set<StationID> unique(result.begin(),result.end());
    if(unique.size()!=result.size()){
        return false;
    }
    std::vector<Name> names(result.size());
    size_t i=0;
    for(const auto& id : result){
        auto name = ds_.get_station_name(id);
        if(name == NO_NAME || (valid_station_names_.find(name)==valid_station_names_.end())){
            return false;
        }
        names[i++]=name;
    }
    // check for the order
    return std::is_sorted(names.begin(),names.end());

}

bool MainProgram::test_stations_distance_increasing(Stopwatch &stopwatch)
{
    // get the ordered thing

    stopwatch.start();
    auto result = ds_.stations_distance_increasing();
    stopwatch.stop();
    if (result.size() != random_stations_added_){
        return false;
    }
    std::unordered_set<StationID> unique(result.begin(),result.end());
    if(unique.size()!=result.size()){
        return false;
    }
    std::vector<unsigned long long int> something(result.size());
    size_t i=0;
    for(const auto& id : result){
        auto coord = ds_.get_station_coordinates(id);
        if(coord == NO_COORD || (valid_coords_.find(coord)==valid_coords_.end())){
            return false;
        }

        something[i++]=abs(coord.x)+abs(coord.y);
    }

    // check for the order
    return std::is_sorted(something.begin(),something.end());
}

bool MainProgram::test_add_train(Stopwatch &stopwatch)
{
    auto trainid = n_to_trainid(random_trains_added_++);
    // random number of stations for each train
    auto stations = random<unsigned int>(2,10);
    std::vector<std::pair<StationID, Time>> departures;
    departures.reserve(stations);
    auto time = random_time();
    for (unsigned int j=0; j<stations; ++j)
    {
        // can go throught the same station twice, but not at the same timestamp (due to max time for train being 10*60min < 24h)
        auto stationid = random_valid_station();
        departures.emplace_back(stationid, time);
        time += random(0,60);
        if (time % 100 > 59) // Fix if minutes exceed 59
        {
            time += 40;
        }
        if (time > 2359)
        {
            time -= 2400;
        }
    }
    stopwatch.start();
    bool retval = ds_.add_train(trainid, departures);
    stopwatch.stop();
    // add also to valid departures
    for(auto it = departures.begin();it!=std::prev(departures.end());++it){
        const auto& d_s_t = *it;
        std::tuple<StationID,TrainID,Time> departure{d_s_t.first,trainid,d_s_t.second};
        valid_departures_.insert(departure);
        departures_.push_back(departure);
        ++random_departures_added_;
    }
    return retval;
}

bool MainProgram::test_next_stations_from(Stopwatch &stopwatch)
{
    auto id = random_valid_station();
    stopwatch.start();
    auto retval = ds_.next_stations_from(id);
    stopwatch.stop();
    auto unique = std::unordered_set<StationID>(retval.begin(),retval.end());
    if(retval.size()!=unique.size()){
        return false;
    }
    auto enditer = valid_station_ids_.end();
    for(const auto& searchid : retval){
        if(valid_station_ids_.find(searchid)==enditer){
            return false;
        }
    }
    return true;
}

bool MainProgram::test_train_stations_from(Stopwatch &stopwatch)
{
    if(random_trains_added_<1){
        return true;
    }
    auto stationid = random_valid_station();
    auto trainid = random_valid_train();
    stopwatch.start();
    auto result = ds_.train_stations_from(stationid,trainid);
    stopwatch.stop();

    if(result.empty()){
        return false;
    }
    if(result.size()==1 && result[0]==NO_STATION){
        return true;
    }
    auto endit = valid_station_ids_.end();
    for(const auto& sid: result){
        if(valid_station_ids_.find(sid)==endit){
            return false;
        }
    }

    return true;
}

bool MainProgram::test_clear_trains(Stopwatch &stopwatch)
{
    stopwatch.start();
    ds_.clear_trains();
    stopwatch.stop();
    random_trains_added_=0;
    trains_.clear();
    valid_train_ids_.clear();
    valid_departures_.clear();
    departures_.clear();

    for(const auto& sid : station_ids_){
        auto retval = ds_.station_departures_after(sid,0000);
        if(!retval.empty()){
            return false;
        }
    }
    return true;

}

bool MainProgram::test_route_any(Stopwatch &stopwatch)
{
    if(random_stations_added_<2){
        return true;
    }
    auto sid1 = random_valid_station();
    auto sid2 = sid1;
    for(;sid2==sid1;sid2=random_valid_station()){}
    stopwatch.start();
    auto result = ds_.route_any(sid1,sid2);
    stopwatch.stop();

    if(result.empty()){
        return true;
    }
    if(result.size()==1){
        return false;
    }
    if(result.front().first!=sid1 && result.back().first!=sid2){
        return false;
    }

    auto endit = valid_station_ids_.end();
    for(const auto& pair : result){
        if(valid_station_ids_.find(pair.first)==endit){
            return false;
        }
    }
    return std::is_sorted(result.begin(),result.end(),[](const auto& a, const auto& b){
        return a.second<b.second;
    });
}

bool MainProgram::test_route_least_stations(Stopwatch &stopwatch)
{
    if(random_stations_added_<2){
        return true;
    }
    auto sid1 = random_valid_station();
    auto sid2 = sid1;
    for(;sid2==sid1;sid2=random_valid_station()){}
    stopwatch.start();
    auto result = ds_.route_least_stations(sid1,sid2);
    stopwatch.stop();

    if(result.empty()){
        return true;
    }
    if(result.size()==1){
        return false;
    }
    if(result.front().first!=sid1 && result.back().first!=sid2){
        return false;
    }

    auto endit = valid_station_ids_.end();
    for(const auto& pair : result){
        if(valid_station_ids_.find(pair.first)==endit){
            return false;
        }
    }
    return std::is_sorted(result.begin(),result.end(),[](const auto& a, const auto& b){
        return a.second<b.second;
    });
}

bool MainProgram::test_route_with_cycle(Stopwatch &stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }
    auto stationid = random_valid_station();
    stopwatch.start();
    auto result = ds_.route_with_cycle(stationid);
    stopwatch.stop();

    if(result.empty()){
        return true;
    }
    if(result.size()==1){
        return false;
    }
    if(result.size()==2){
        return (result.front()==stationid && result.back()==stationid);
    }
    std::unordered_set<StationID> unique(result.begin(),result.end());
    // no invalidation on next line due to limitations before
    std::unordered_set<StationID> unique2(result.begin(),--result.end());
    if(unique.size()!=unique2.size()){
        return false;
    }

    auto endit = valid_station_ids_.end();
    for(const auto& sid : unique){
        if(valid_station_ids_.find(sid)==endit){
            return false;
        }
    }
    return true;
}

bool MainProgram::test_route_shortest_distance(Stopwatch &stopwatch)
{
    if(random_stations_added_<2){
        return true;
    }
    auto sid1 = random_valid_station();
    auto sid2 = sid1;
    for(;sid2==sid1;sid2=random_valid_station()){}
    stopwatch.start();
    auto result = ds_.route_shortest_distance(sid1,sid2);
    stopwatch.stop();

    if(result.empty()){
        return true;
    }
    if(result.size()==1){
        return false;
    }
    if(result.front().first!=sid1 && result.back().first!=sid2){
        return false;
    }

    auto endit = valid_station_ids_.end();
    for(const auto& pair : result){
        if(valid_station_ids_.find(pair.first)==endit){
            return false;
        }
    }
    return std::is_sorted(result.begin(),result.end(),[](const auto& a, const auto& b){
        return a.second<b.second;
    });
}

bool MainProgram::test_route_earliest_arrival(Stopwatch &stopwatch)
{
    if(random_stations_added_<2){
        return true;
    }
    auto sid1 = random_valid_station();
    auto sid2 = sid1;
    auto starttime =random_time();
    for(;sid2==sid1;sid2=random_valid_station()){}
    stopwatch.start();
    auto result = ds_.route_earliest_arrival(sid1,sid2,starttime);
    stopwatch.stop();

    if(result.empty()){
        return true;
    }
    if(result.size()==1){
        return false;
    }
    if(result.front().first!=sid1 && result.back().first!=sid2){
        return false;
    }

    auto endit = valid_station_ids_.end();
    for(const auto& pair : result){
        if(valid_station_ids_.find(pair.first)==endit){
            return false;
        }
    }
    return true;
}

MainProgram::CmdResult MainProgram::cmd_station_count(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    output << "Number of stations: " << ds_.station_count() << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_all_stations(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto stations = ds_.all_stations();
    if (stations.empty())
    {
        output << "No stations!" << endl;
    }

    std::sort(stations.begin(), stations.end());
    return {ResultType::IDLIST, CmdResultIDs{{}, stations}};
}

MainProgram::CmdResult MainProgram::cmd_add_region(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    RegionID id = convert_string_to<RegionID>(*begin++);
    string name = *begin++;
    string coordsstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    vector<Coord> coords;
    smatch coord;
    auto sbeg = coordsstr.cbegin();
    auto send = coordsstr.cend();
    for ( ; regex_search(sbeg, send, coord, coords_regex_); sbeg = coord.suffix().first)
    {
        coords.push_back({convert_string_to<int>(coord[1]),convert_string_to<int>(coord[2])});
    }

    assert(coords.size() >= 3 && "Region with <3 coords");

    bool success = ds_.add_region(id, name, coords);

    view_dirty = true;
    return {ResultType::IDLIST, CmdResultIDs{{success ? id : NO_REGION}, {}}};
}

MainProgram::CmdResult MainProgram::cmd_all_regions(std::ostream &output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto regions = ds_.all_regions();
    if (regions.empty())
    {
        output << "No regions!" << endl;
    }

    std::sort(regions.begin(), regions.end());
    return {ResultType::IDLIST, CmdResultIDs{regions, {}}};
}

MainProgram::CmdResult MainProgram::cmd_region_info(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    RegionID id = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    return {ResultType::IDLIST, CmdResultIDs{{id}, {}}};
}

bool MainProgram::test_all_stations(Stopwatch& stopwatch)
{
    stopwatch.start();
    auto result = ds_.all_stations();
    stopwatch.stop();

    // checks for result validity:
    if (result.size() != random_stations_added_){
        return false;
    }
    std::unordered_set<StationID> unique(result.begin(),result.end());
    if(unique.size()!=result.size()){
        return false;
    }
    for(const auto& id : result){
        if(valid_station_ids_.find(id)==valid_station_ids_.end()){
            return false;
        }
    }
    return true;
}

MainProgram::CmdResult MainProgram::cmd_all_subregions_of_region(std::ostream &output, MatchIter begin, MatchIter end)
{
    RegionID regionid = convert_string_to<RegionID>(*begin++);
    assert( begin == end && "Impossible number of parameters!");

    auto regions = ds_.all_subregions_of_region(regionid);
    if (regions.empty())
    {
        output << "No regions!" << endl;
    }

    std::sort(regions.begin(), regions.end());
    regions.insert(regions.begin(), regionid); // Add parameter as the first region
    return {ResultType::IDLIST, CmdResultIDs{regions, {}}};
}

Distance MainProgram::calc_distance(Coord c1, Coord c2)
{
    if (c1 == NO_COORD || c2 == NO_COORD) { return NO_DISTANCE; }

    long long int deltax = c1.x - c2.x;
    long long int deltay = c1.y - c2.y;
    return static_cast<Distance>(std::sqrt(deltax*deltax + deltay*deltay));
}

MainProgram::CmdResult MainProgram::cmd_clear_all(ostream& output, MatchIter begin, MatchIter end)
{
    assert(begin == end && "Invalid number of parameters");

    ds_.clear_all();
    init_primes();

    output << "Cleared all stations" << endl;

    view_dirty = true;

    return {};
}

string MainProgram::print_station(StationID id, ostream& output, bool nl)
{
    try
    {
        if (id != NO_STATION)
        {
            auto name = ds_.get_station_name(id);
            auto xy = ds_.get_station_coordinates(id);
            if (!name.empty())
            {
                output << name << ": ";
            }
            else
            {
                output << "*: ";
            }

            output << "pos=";
            print_coord(xy, output, false);
            output << ", id=" << id;
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_STATION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing station : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing station : " << e.what() << endl;
        return "";
    }
}

string MainProgram::print_station_brief(StationID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_STATION)
        {
            auto name = ds_.get_station_name(id);
            if (!name.empty())
            {
                output << name << " ";
            }
            else
            {
                output << "* ";
            }

            output << "(" << id << ")";
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_STATION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing station : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing station : " << e.what() << endl;
        return "";
    }
}

string MainProgram::print_region(RegionID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_REGION)
        {
            auto name = ds_.get_region_name(id);
            //            auto xy = ds_.get_region_coords(id);
            if (!name.empty())
            {
                output << name << ": ";
            }
            else
            {
                output << "*: ";
            }

            output << "id=" << id;
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_REGION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing region : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing region : " << e.what() << endl;
        return "";
    }
}

MainProgram::CmdResult MainProgram::cmd_find_stations_with_coord(ostream& output , MatchIter begin, MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    auto result = ds_.find_stations_with_coord({x,y});
    std::sort(result.begin(), result.end());

    if (result.empty())
    {
        output << "No stations found!" << std::endl;
    }

    return {ResultType::IDLIST, CmdResultIDs{{}, result}};
}

bool MainProgram::test_find_stations_with_coord(Stopwatch& stopwatch)
{
    if(random_stations_added_<1){
        return true;
    }
    Coord testcoord = random_valid_coord();
    stopwatch.start();
    auto result = ds_.find_stations_with_coord(testcoord);
    stopwatch.stop();
    auto correct_count = valid_coords_.count(testcoord);
    if(result.size()!=correct_count){
        return false;
    }
    std::unordered_set<StationID> unique(result.begin(),result.end());
    if(unique.size()!=result.size()){
        return false;
    }
    for(const auto& id: result){
        if(valid_station_ids_.find(id)==valid_station_ids_.end()){
            return false;
        }
    }
    return true;
}

bool MainProgram::test_region_info(Stopwatch &stopwatch)
{
    // TODO tests
    if (random_regions_added_ > 0) // Don't do anything if there's no regions
    {
        auto id = random_valid_region();
        stopwatch.start();
        ds_.get_region_name(id);
        ds_.get_region_coords(id);
        stopwatch.stop();
    }
    return true;
}

bool MainProgram::test_stations_closest_to(Stopwatch &stopwatch)
{
    auto searchcoord = random_coord();
    stopwatch.start();
    auto result = ds_.stations_closest_to(searchcoord);
    stopwatch.stop();

    unsigned int size = std::min((unsigned long int)3,random_stations_added_);
    if(result.size()!=size){
        return false;
    }
    std::unordered_set<StationID> unique(result.begin(),result.end());
    if(unique.size()!=result.size()){
        return false;
    }
    for (const auto& id : result){
        if(valid_station_ids_.find(id)==valid_station_ids_.end()){
            return false;
        }
    }
    return true;
}

bool MainProgram::test_common_ancestor_of_regions(Stopwatch &stopwatch)
{
    if(random_regions_added_<1){
        return true;
    }

    auto id1 = random_valid_region();
    auto id2 = random_valid_region();
    stopwatch.start();
    auto result = ds_.common_ancestor_of_regions(id1, id2);
    stopwatch.stop();

    return result==NO_REGION || (valid_region_ids_.find(result)!=valid_region_ids_.end());
}

MainProgram::CmdResult MainProgram::cmd_randseed(std::ostream& output, MatchIter begin, MatchIter end)
{
    string seedstr = *begin++;
    assert(begin == end && "Invalid number of parameters");

    unsigned long int seed = convert_string_to<unsigned long int>(seedstr);

    rand_engine_.seed(seed);
    init_primes();

    output << "Random seed set to " << seed << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_read(std::ostream& output, MatchIter begin, MatchIter end)
{
    string filename = *begin++;
    string silentstr =  *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool silent = !silentstr.empty();
    ostream* new_output = &output;

    ostringstream dummystr; // Given as output if "silent" is specified, the output is discarded
    if (silent)
    {
        new_output = &dummystr;
    }

    ifstream input(filename);
    if (input)
    {
        output << "** Commands from '" << filename << "'" << endl;
        command_parser(input, *new_output, PromptStyle::NORMAL);
        if (silent) { output << "...(output discarded in silent mode)..." << endl; }
        output << "** End of commands from '" << filename << "'" << endl;
    }
    else
    {
        output << "Cannot open file '" << filename << "'!" << endl;
    }

    return {};
}


MainProgram::CmdResult MainProgram::cmd_testread(std::ostream& output, MatchIter begin, MatchIter end)
{
    string infilename = *begin++;
    string outfilename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(infilename);
    if (input)
    {
        ifstream expected_output(outfilename);
        if (output)
        {
            stringstream actual_output;
            command_parser(input, actual_output, PromptStyle::NO_NESTING);

            vector<string> actual_lines;
            while (actual_output)
            {
                string line;
                getline(actual_output, line);
                if (!actual_output) { break; }
                actual_lines.push_back(line);
            }

            vector<string> expected_lines;
            while (expected_output)
            {
                string line;
                getline(expected_output, line);
                if (!expected_output) { break; }
                expected_lines.push_back(line);
            }

            string heading_actual = "Actual output";
            unsigned int actual_max_length = heading_actual.length();
            auto actual_max_iter = max_element(actual_lines.cbegin(), actual_lines.cend(),
                                               [](string s1, string s2){ return s1.length() < s2.length(); });
            if (actual_max_iter != actual_lines.cend())
            {
                actual_max_length = actual_max_iter->length();
            }

            string heading_expected = "Expected output";
            unsigned int expected_max_length = heading_expected.length();
            auto expected_max_iter = std::max_element(expected_lines.cbegin(), expected_lines.cend(),
                                                      [](string s1, string s2){ return s1.length() < s2.length(); });
            if (expected_max_iter != expected_lines.cend())
            {
                expected_max_length = expected_max_iter->length();
            }

            auto pos_actual = actual_lines.cbegin();
            auto pos_expected = expected_lines.cbegin();
            output << "  " << heading_actual << string(actual_max_length - heading_actual.length(), ' ') << " | " << heading_expected << endl;
            output << "--" << string(actual_max_length, '-') << "-|-" << string(expected_max_length, '-') << endl;

            bool lines_ok = true;
            while (pos_expected != expected_lines.cend() || pos_actual != actual_lines.cend())
            {
                if (pos_expected != expected_lines.cend())
                {
                    if (pos_actual != actual_lines.cend())
                    {
                        bool ok = (*pos_expected == *pos_actual);
                        output << (ok ? ' ' : '?') << ' ' << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = lines_ok && ok;
                        ++pos_actual;
                    }
                    else
                    { // Actual output was too short
                        output << "? " << string(actual_max_length, ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = false;
                    }
                    ++pos_expected;
                }
                else
                { // Actual output was too long
                    output << "? " << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                           << " | " << endl;
                    lines_ok = false;
                    ++pos_actual;
                }
            }
            if (lines_ok)
            {
                output << "**No differences in output.**" << endl;
                if (test_status_ == TestStatus::NOT_RUN)
                {
                    test_status_ = TestStatus::NO_DIFFS;
                }
            }
            else
            {
                output << "**Differences found! (Lines beginning with '?')**" << endl;
                test_status_ = TestStatus::DIFFS_FOUND;
            }

        }
        else
        {
            output << "Cannot open file '" << outfilename << "'!" << endl;
        }
    }
    else
    {
        output << "Cannot open file '" << infilename << "'!" << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_stopwatch(std::ostream& output, MatchIter begin, MatchIter end)
{
    string on = *begin++;
    string off = *begin++;
    string next = *begin++;
    assert(begin == end && "Invalid number of parameters");

    if (!on.empty())
    {
        stopwatch_mode = StopwatchMode::ON;
        output << "Stopwatch: on" << endl;
    }
    else if (!off.empty())
    {
        stopwatch_mode = StopwatchMode::OFF;
        output << "Stopwatch: off" << endl;
    }
    else if (!next.empty())
    {
        stopwatch_mode = StopwatchMode::NEXT;
        output << "Stopwatch: on for the next command" << endl;
    }
    else
    {
        assert(!"Impossible stopwatch mode!");
    }

    return {};
}

std::string MainProgram::print_station_name(StationID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_STATION)
        {
            auto name = ds_.get_station_name(id);
            if (!name.empty())
            {
                output << name;
            }
            else
            {
                output << "*";
            }

            ostringstream retstream;
            retstream << name;
            if (nl) { output << endl; }
            return retstream.str();
        }
        else
        {
            output << "--NO_STATION--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing station name : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing station name : " << e.what() << endl;
        return "";
    }
}

std::string MainProgram::print_coord(Coord coord, std::ostream& output, bool nl)
{
    if (coord != NO_COORD)
    {
        output << "(" << coord.x << "," << coord.y << ")";
        ostringstream retstream;
        retstream << "(" << coord.x << "," << coord.y << ")";
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "(--NO_COORD--)";
        if (nl) { output << endl; }
        return "";
    }
}

string MainProgram::print_train(TrainID id, std::ostream &output, bool nl)
{
    try
    {
        if (id != NO_TRAIN)
        {
            output << id;
            if (nl) { output << endl; }

            ostringstream retstream;
            retstream << id;
            return retstream.str();
        }
        else
        {
            output << "--NO_TRAIN--";
            if (nl) { output << endl; }
            return "";
        }
    }
    catch (NotImplemented const& e)
    {
        output << endl << "NotImplemented while printing train : " << e.what() << endl;
        std::cerr << endl << "NotImplemented while printing train : " << e.what() << endl;
        return "";
    }
}

string MainProgram::print_time(Time time, std::ostream &output, bool nl)
{
    if (time != NO_TIME)
    {
        output << setw(4) << setfill('0') << time;
        ostringstream retstream;
        retstream << setw(4) << setfill('0') << time;
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "(--NO_TIME--)";
        if (nl) { output << endl; }
        return "";
    }
}

string const stationidx = "([a-zA-Z0-9-]+)";
string const trainidx = "([a-zA-Z0-9-]+)";
string const regionidx = "([0-9]+)";
string const namex = "([ a-zA-Z0-9-]+)";
string const timex = "((?:[01][0-9][0-5][0-9])|(?:2[0-3][0-5][0-9]))";
string const numx = "([0-9]+)";
string const optcoordx = "\\([[:space:]]*[0-9]+[[:space:]]*,[[:space:]]*[0-9]+[[:space:]]*\\)";
string const coordx = "\\([[:space:]]*([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)[[:space:]]*\\)";
string const wsx = "[[:space:]]+";
string const optstationtimeidx = "[a-zA-Z0-9-]+:(?:(?:[01][0-9][0-5][0-9])|(?:2[0-3][0-5][0-9]))";

string const perftest_cmds_listx = "((?:[0-9]+\\*)?[0-9a-zA-Z_]+(?:;(?:[0-9]+\\*)?[0-9a-zA-Z_]+)*(?:(?::(?:[0-9]+\\*)?[0-9a-zA-Z_]+)(?:;(?:[0-9]+\\*)?[0-9a-zA-Z_]+)*(?::[0-9]+)?)?)";
string const perftest_timeoutx = "([0-9]+(?::[0-9]+)?)";
string const perftest_n_values_or_rangex = "([0-9]+(?:(?::[0-9]+){1,2}|(?:;[0-9]+)+)?)";
string const perftest_logx = "(?:[[:space:]]+(log))?";

vector<MainProgram::CmdInfo> MainProgram::cmds_ =
    {
        {"station_count", "", "", &MainProgram::cmd_station_count, &MainProgram::test_station_count },
        {"clear_all", "", "", &MainProgram::cmd_clear_all, &MainProgram::test_clear_all },
        {"all_stations", "", "", &MainProgram::cmd_all_stations, &MainProgram::test_all_stations },
        {"add_station", "StationID \"Name\" (x,y)", stationidx+wsx+'"'+namex+'"'+wsx+coordx, &MainProgram::cmd_add_station, &MainProgram::test_add_station },
        {"station_info", "StationID", stationidx, &MainProgram::cmd_station_info, &MainProgram::test_station_info },
        {"get_station_name", "StationID",stationidx,&MainProgram::cmd_get_station_name,&MainProgram::test_get_station_name},
        {"get_station_coordinates", "StationID",stationidx,&MainProgram::cmd_get_station_coordinates,&MainProgram::test_get_station_coordinates},
        {"stations_alphabetically", "", "", &MainProgram::NoParListCmd<&Datastructures::stations_alphabetically>, &MainProgram::test_stations_alphabetically },
        {"stations_distance_increasing", "", "", &MainProgram::NoParListCmd<&Datastructures::stations_distance_increasing>,
         &MainProgram::test_stations_distance_increasing },
        {"find_stations_with_coord", "(x,y)", coordx, &MainProgram::cmd_find_stations_with_coord, &MainProgram::test_find_stations_with_coord },
        {"change_station_coord", "StationID (x,y)", stationidx+wsx+coordx, &MainProgram::cmd_change_station_coord, &MainProgram::test_change_station_coord },
        {"add_departure", "StationID TrainID Time", stationidx+wsx+trainidx+wsx+timex, &MainProgram::cmd_add_departure, &MainProgram::test_add_departure },
        {"remove_departure", "StationID TrainID Time", stationidx+wsx+trainidx+wsx+timex, &MainProgram::cmd_remove_departure, &MainProgram::test_remove_departure },
        {"station_departures_after", "StationID Time", stationidx+wsx+timex, &MainProgram::cmd_station_departures_after, &MainProgram::test_station_departures_after },
        //    {"mindist", "", "", &MainProgram::NoParstationCmd<&Datastructures::min_distance>, &MainProgram::NoParstationTestCmd<&Datastructures::min_distance> },
        //    {"maxdist", "", "", &MainProgram::NoParstationCmd<&Datastructures::max_distance>, &MainProgram::NoParstationTestCmd<&Datastructures::max_distance> },
        {"add_region", "RegionID \"Name\" (x,y) (x,y)...", regionidx+wsx+'"'+namex+'"'+"((?:"+wsx+optcoordx+")+)", &MainProgram::cmd_add_region, &MainProgram::test_add_region },
        {"all_regions", "", "", &MainProgram::cmd_all_regions, &MainProgram::test_all_regions },
        {"region_info", "RegionID", regionidx, &MainProgram::cmd_region_info, &MainProgram::test_region_info },
        {"get_region_name", "RegionID",regionidx,&MainProgram::cmd_get_region_name,&MainProgram::test_get_region_name},
        {"get_region_coords", "RegionID",regionidx,&MainProgram::cmd_get_region_coords,&MainProgram::test_get_region_coords},
        {"add_subregion_to_region", "SubregionID RegionID", regionidx+wsx+regionidx, &MainProgram::cmd_add_subregion_to_region, &MainProgram::test_add_subregion_to_region},
        {"add_station_to_region", "StationID RegionID", stationidx+wsx+regionidx, &MainProgram::cmd_add_station_to_region, &MainProgram::test_add_station_to_region },
        {"station_in_regions", "StationID", stationidx, &MainProgram::cmd_station_in_regions, &MainProgram::test_station_in_regions },
        {"all_subregions_of_region", "RegionID", regionidx, &MainProgram::cmd_all_subregions_of_region, &MainProgram::test_all_subregions_of_region },
        {"stations_closest_to", "(x,y)", coordx, &MainProgram::cmd_stations_closest_to, &MainProgram::test_stations_closest_to },
        {"remove_station", "StationID", stationidx, &MainProgram::cmd_remove_station, &MainProgram::test_remove_station },
        {"common_ancestor_of_regions", "RegionID1 RegionID2", regionidx+wsx+regionidx, &MainProgram::cmd_common_ancestor_of_regions, &MainProgram::test_common_ancestor_of_regions },
        {"quit", "", "", nullptr, nullptr },
        {"help", "", "", &MainProgram::help_command, nullptr },
        {"random_add", "number_of_stations_to_add  (minx,miny) (maxx,maxy) (coordinates optional)",
         numx+"(?:"+wsx+coordx+wsx+coordx+")?", &MainProgram::cmd_random_stations, &MainProgram::test_random_stations },
        {"read", "\"in-filename\" [silent]", "\"([-a-zA-Z0-9 ./:_]+)\"(?:"+wsx+"(silent))?", &MainProgram::cmd_read, nullptr },
        {"testread", "\"in-filename\" \"out-filename\"", "\"([-a-zA-Z0-9 ./:_]+)\""+wsx+"\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_testread, nullptr },
        {"perftest", "[REPEAT*]cmd1[;[REPEAT*]cmd2...][:[REPEAT*]extracmd1[;[REPEAT*]extracmd2...]:extra_every_n_test_entry] timeout[:extra_timeout] range_start:range_end[:step]|n1[;n2...] [log] (parts in [] are optional)",
         perftest_cmds_listx+wsx+perftest_timeoutx+wsx+perftest_n_values_or_rangex+perftest_logx, &MainProgram::cmd_perftest, nullptr },
        {"stopwatch", "on|off|next (alternatives separated by |)", "(?:(on)|(off)|(next))", &MainProgram::cmd_stopwatch, nullptr },
        {"random_seed", "new-random-seed-integer", numx, &MainProgram::cmd_randseed, nullptr },
        {"#", "comment text", ".*", &MainProgram::cmd_comment, nullptr },
        // prg2 commands
        {"add_train", "TrainID StationID1:Time1 ... StationIDlast:Timelast", trainidx+"((?:"+wsx+optstationtimeidx+")+)", &MainProgram::cmd_add_train, &MainProgram::test_add_train },
        {"next_stations_from", "StationID", stationidx, &MainProgram::cmd_next_stations_from, &MainProgram::test_next_stations_from },
        {"train_stations_from", "StationID TrainID", stationidx+wsx+trainidx, &MainProgram::cmd_train_stations_from, &MainProgram::test_train_stations_from},
        {"clear_trains", "", "", &MainProgram::cmd_clear_trains, &MainProgram::test_clear_trains },
        {"route_any", "StationID StationID", stationidx+wsx+stationidx, &MainProgram::cmd_route_any, &MainProgram::test_route_any},
        {"route_least_stations", "StationID StationID", stationidx+wsx+stationidx, &MainProgram::cmd_route_least_stations, &MainProgram::test_route_least_stations},
        {"route_with_cycle", "StationID", stationidx, &MainProgram::cmd_route_with_cycle, &MainProgram::test_route_with_cycle },
        {"route_shortest_distance", "StationID StationID", stationidx+wsx+stationidx, &MainProgram::cmd_route_shortest_distance, &MainProgram::test_route_shortest_distance},
        {"route_earliest_arrival", "StationID StationID StartTime", stationidx+wsx+stationidx+wsx+timex, &MainProgram::cmd_route_earliest_arrival, &MainProgram::test_route_earliest_arrival},

        };
/*
 * regex cmds;cmds:cmds;cmds
 * ([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*(?:(?::[0-9a-zA-Z_]+)(?:;[0-9a-zA-Z_]+)*)?)
 *
 *
 * timeoutit
 * ([0-9]+(?::[0-9]+)?)
 *
 * extra_cmds_after
 * ([0-9]+)
 *
 * N ja ranget siihen
 * ([0-9]+(?::[0-9]+){0,2}(?:;[0-9]+(?::[0-9]+){0,2})*)
 *
 * log
 * ((?:log)?)
 */

MainProgram::CmdResult MainProgram::help_command(std::ostream& output, MatchIter /*begin*/, MatchIter /*end*/)
{
    output << "Commands:" << endl;
    for (auto& i : cmds_)
    {
        output << "  " << i.cmd << " " << i.info << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_perftest(std::ostream& output, MatchIter begin, MatchIter end)
{
#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    string commandstr = *begin++;
    string timeoutstr = *begin++;
    string n_rangestr = *begin++;
    string logstr = *begin++;
    assert(begin == end && "Invalid number of parameters");

    // parse the commands and extra commands
    vector<pair<unsigned int,string>> testcmds;
    vector<pair<unsigned int,string>> extracmds;
    unsigned int n_extra = 1;
    // to reduce the number of unnecessary data in ram
    {
        vector<string> test_extrastr={"","",""};
        auto cbeg = commandstr.cbegin();
        auto cend = commandstr.cend();
        size_t i = 0;
        for (smatch sma;regex_search(cbeg,cend,sma,commands_divide_regex_);cbeg = sma.suffix().first){
            test_extrastr[i++]=sma[1];
        }
        // first find the regular commands
        cbeg = test_extrastr[0].cbegin();
        cend = test_extrastr[0].cend();
        for (smatch scmd; regex_search(cbeg, cend, scmd, commands_regex_); cbeg = scmd.suffix().first)
        {
            unsigned int repeat_count = scmd[1]==""?1:convert_string_to<unsigned int>(scmd[1]);
            testcmds.push_back({repeat_count,scmd[2]});
        }
        // then add the extra commands
        cbeg = test_extrastr[1].cbegin();
        cend = test_extrastr[1].cend();
        for (smatch scmd; regex_search(cbeg, cend, scmd, commands_regex_); cbeg = scmd.suffix().first)
        {
            unsigned int repeat_count = scmd[1]==""?1:convert_string_to<unsigned int>(scmd[1]);
            extracmds.push_back({repeat_count,scmd[2]});
        }
        if(test_extrastr[1]!="" && test_extrastr[2]==""){
            output <<"using extra commands requires every n parameter"<<endl;
            return {};
        }
        n_extra=test_extrastr[2]==""?1:convert_string_to<unsigned int>(test_extrastr[2]);
        if(n_extra < 1){
            output << "extra commands can not be run every 0 normal command listing"<<endl;
            return {};
        }
    }
    vector<unsigned int> timeouts;
    {
        auto cbeg = timeoutstr.cbegin();
        auto cend = timeoutstr.cend();
        for (smatch timeoutsmatch; regex_search(cbeg,cend,timeoutsmatch,timeouts_regex_);cbeg=timeoutsmatch.suffix().first){
            timeouts.push_back(convert_string_to<unsigned int>(timeoutsmatch[1]));
        }
    }
    if(!extracmds.empty() && timeouts.size()==1){
        output << "Using extra commands requires timeout for extra commands"<<endl;
        return {};
    }

    bool is_range=false;
    {
        auto cbeg = n_rangestr.cbegin();
        auto cend = n_rangestr.cend();
        smatch isrange;
        regex_search(cbeg,cend,isrange,is_range_notation_regex_);
        is_range = isrange[1]!="";
    }
    vector<unsigned int> init_ns;
    if (is_range){
        vector<unsigned int> range_limits;
        auto sbeg = n_rangestr.cbegin();
        auto send = n_rangestr.cend();
        for (smatch range; regex_search(sbeg, send, range, range_regex_); sbeg = range.suffix().first)
        {
            range_limits.push_back(convert_string_to<unsigned int>(range[1]));
        }
        if(range_limits[0]>=range_limits[1]){
            output <<  "Lower bound of range must be less than upper bound"<<endl;
            return {};
        }
        if(logstr.empty()){
            unsigned int step = 1;
            if (range_limits.size()==3){
                step=range_limits[2];
            }
            if(step<1){
                output << "Step for linear range can not be < 1"<<endl;
                return{};
            }
            // linear range
            for(unsigned int n=range_limits[0];n<=range_limits[1];n+=step){
                init_ns.push_back(n);
            }
        } else{
            unsigned int step = 10;
            if (range_limits.size()==3){
                step=range_limits[2];
            }
            if(step<2){
                output << "Base for log range can not be < 2"<<endl;
                return{};
            }
            unsigned int overflow_boundary_check = std::numeric_limits<unsigned int>::max()/step;

            for(unsigned int n=range_limits[0];n<=range_limits[1];n*=step){
                init_ns.push_back(n);
                if(overflow_boundary_check<n){
                    break;
                }
            }
        }
    }else{
        auto sbeg = n_rangestr.cbegin();
        auto send = n_rangestr.cend();
        for (smatch size; regex_search(sbeg, send, size, sizes_regex_); sbeg = size.suffix().first)
        {
            init_ns.push_back(convert_string_to<unsigned int>(size[1]));
        }
    }

    try {


        output << "Timeout for each N is " << timeouts[0] <<" sec (for ADD and test functions)";
        //perftest stations_distance_increasing;stations_distance_increasing:stations_distance_increasing 1:1 1 1000
        if(!extracmds.empty()){
            output <<" + " <<timeouts[1]<<" sec (for extra functions)";
        }
        output << ". " << endl;
        output << "For each N perform command(s):" << endl;

        // Initialize test functions
        vector<pair<unsigned int,bool(MainProgram::*)(Stopwatch&)>> testfuncs;
        vector<pair<unsigned int,bool(MainProgram::*)(Stopwatch&)>> extrafuncs;

        for (auto& i : testcmds)
        {
            auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto const& cmd){ return cmd.cmd == i.second; });
            if (pos != cmds_.end() && pos->testfunc)
            {
                output << i.second << " ";
                testfuncs.push_back({i.first,pos->testfunc});
            }
            else
            {
                output << "(cannot test " << i.second << ") ";
            }
        }
        if(!extracmds.empty()){
            output << endl << "with extra functions"<<endl;

            for (auto& i : extracmds)
            {
                auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto const& cmd){ return cmd.cmd == i.second; });
                if (pos != cmds_.end() && pos->testfunc)
                {
                    output << i.second << " ";
                    extrafuncs.push_back({i.first,pos->testfunc});
                }
                else
                {
                    output << "(cannot add extra function " << i.second << ") ";
                }
            }
            output<< endl<<"after every " <<n_extra << " entries in test functions"<<endl;
        }
        output << endl<<endl;

        if (testfuncs.empty())
        {
            output << "No commands to test!" << endl;
            return {};
        }

#ifdef USE_PERF_EVENT
        output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "add (count)" << " , " << setw(12) << "cmds (sec)" << " , "
               << setw(12) << "cmds (count)"  << " , " << setw(12) << "total (sec)" << " , " << setw(12) << "total (count)" << endl;
#else
        output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "cmds (sec)" << " , "
               << setw(12) << "total (sec)" << endl;
#endif
        flush_output(output);

        auto stop = false;
        for (unsigned int n : init_ns)
        {
            if (stop) { break; }

            output << setw(7) << n << " , " << flush;
            ds_.clear_all();
            init_primes();

            Stopwatch stopwatch(true); // Use also instruction counting, if enabled
            Stopwatch extrasw;

            // Add random stations
            // modify the add to work and add correct number of things also with lower n values
            for (unsigned int i = 0; i < n / 1000; ++i)
            {
                if(!add_random_stations_regions_trains(stopwatch,1000)){
                    output << "Adding stations, regions and trains failed!" << endl;
                    stop=true;
                    break;
                }


                if (stopwatch.elapsed() >= timeouts[0])
                {
                    output << "ADD Timeout!" << endl;
                    stop = true;
                    break;
                }
                if (check_stop())
                {
                    output << "Stopped!" << endl;
                    stop = true;
                    break;
                }
            }
            if (stop) { break; }

            if (n % 1000 != 0)
            {
                if(!add_random_stations_regions_trains(stopwatch,n % 1000)){
                    output << "Adding stations, regions and trains failed!" << endl;
                    stop=true;
                    break;
                }

            }

#ifdef USE_PERF_EVENT
            auto addcount = stopwatch.count();
#endif
            auto addsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
            output << setw(12) << addsec << " , " << setw(12) << addcount << " , " << flush;
#else
            output << setw(12) << addsec << " , " << flush;
#endif

            if (addsec >= timeouts[0])
            {
                output << "ADD Timeout!" << endl;
                stop = true;
                break;
            }

            // TODO reformat this to its own function -> return instead of stop and unnecessary checks

            unsigned int extra=0;
            for (auto pair : testfuncs)
            {
                auto cmdpos = pair.second;
                for (unsigned int i = 0; i<pair.first;++i){
                    if (!(this->*cmdpos)(stopwatch)){
                        output <<"Failed check!"<<endl;
                        stop=true;
                        break;
                    }
                    if (stopwatch.elapsed() >= timeouts[0])
                    {
                        output << "Timeout!" << endl;
                        stop = true;
                        break;
                    }
                    if (check_stop())
                    {
                        output << "Stopped!" << endl;
                        stop = true;
                        break;
                    }
                }
                if(stop){
                    break;
                }
                // run extra functions every "n_extra" elements in the command list
                if((++extra)%(n_extra<1?1:n_extra) == 0 && extra<testfuncs.size()){
                    for (auto extrapair : extrafuncs){
                        auto cmdpos = extrapair.second;
                        for (unsigned int i = 0; i<extrapair.first;++i){
                            if (!(this->*cmdpos)(extrasw)){
                                output <<"Extrafunc Failed check!"<<endl;
                                stop=true;
                                break;
                            }
                            if (extrasw.elapsed() >= timeouts[1])
                            {
                                output << "Extrafunc Timeout!" << endl;
                                stop = true;
                                break;
                            }
                            if (check_stop())
                            {
                                output << "Stopped!" << endl;
                                stop = true;
                                break;
                            }
                        }
                    }
                }
                if(stop){
                    break;
                }
            }
            if (stop) { break; }

#ifdef USE_PERF_EVENT
            auto totalcount = stopwatch.count();
#endif
            auto totalsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
            output << setw(12) << totalsec-addsec << " , " << setw(12) << totalcount-addcount << " , " << setw(12) << totalsec << " , " << setw(12) << totalcount;
#else
            output << setw(12) << totalsec-addsec << " , " << setw(12) << totalsec;
#endif

            //        unsigned long int maxmem;
            //        string unit;
            //        tie(maxmem, unit) = mempeak();
            //        maxmem -=  startmem;
            //        if (maxmem != 0)
            //        {
            //            output << ", memory " << maxmem << " " << unit;
            //        }
            output << endl;
            flush_output(output);
        }

        ds_.clear_all();
        init_primes();

    }
    catch (NotImplemented const&)
    {
        // Clean up after NotImplemented
        ds_.clear_all();
        init_primes();
        throw;
    }

#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    return {};
}

MainProgram::CmdResult MainProgram::cmd_comment(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    return {};
}

MainProgram::CmdResult MainProgram::cmd_add_train(std::ostream &output, MatchIter begin, MatchIter end)
{
    TrainID trainid = *begin++;
    string stationtimesstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    CmdResultTrains result;
    std::vector<std::pair<StationID,Time>> stationtimes;
    smatch stationtime;
    auto sbeg = stationtimesstr.cbegin();
    auto send = stationtimesstr.cend();
    StationID prevstation = NO_STATION;
    Time prevtime = NO_TIME;
    for ( ; regex_search(sbeg, send, stationtime, stationtimes_regex_); sbeg = stationtime.suffix().first)
    {
        StationID stationid = stationtime[1];
        Time time = convert_string_to<Time>(stationtime[2]);
        if (prevstation != NO_STATION)
        {
            result.push_back({trainid, prevstation, stationid, prevtime});
        }
        prevstation = stationid;
        prevtime = time;
        stationtimes.push_back({stationid, time});
    }
    result.push_back({trainid, prevstation, NO_STATION, prevtime});

    assert(stationtimes.size() >= 2 && "Train with <2 stations");

    bool success = ds_.add_train(trainid, stationtimes);

    if (success)
    {
        view_dirty = true;
        return {ResultType::TRAINS, result};
    }
    else
    {
        output << "Adding train failed!" << endl;
        return {};
    }
}

MainProgram::CmdResult MainProgram::cmd_next_stations_from(std::ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto opresult = ds_.next_stations_from(id);
    std::sort(opresult.begin(), opresult.end());

    CmdResultTrains result;
    result.reserve(opresult.size());
    std::transform(opresult.begin(), opresult.end(), std::back_inserter(result),
                   [id](auto& id2){ return std::make_tuple(NO_TRAIN, id, id2, NO_TIME) ;});
    return {ResultType::TRAINS, result};
}

MainProgram::CmdResult MainProgram::cmd_train_stations_from(std::ostream& output, MatchIter begin, MatchIter end)
{
    string stationid = *begin++;
    string trainid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto opresult = ds_.train_stations_from(stationid, trainid);

    if (opresult.size() == 1 && opresult.front() == NO_STATION)
    {
        output << "Unsuccessful, operation returned {NO_STATION}." << endl;
        return {};
    }

    CmdResultRoute result;
    result.reserve(opresult.size());
    auto previd = stationid;
    std::transform(opresult.begin(), opresult.end(), std::back_inserter(result),
                   [&previd](auto& id){ auto res=std::make_tuple(NO_TRAIN, previd, id, NO_TIME, NO_DISTANCE); previd=id; return res; });
    return {ResultType::ROUTE, result};
}

MainProgram::CmdResult MainProgram::cmd_clear_trains(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    ds_.clear_trains();
    output << "All trains removed." << std::endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_route_any(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_any(fromid, toid);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_DISTANCE))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ndist] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, NO_TIME, dist);
            id = nid; dist = ndist;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, dist);
    }
    return {ResultType::ROUTE, result};
}

MainProgram::CmdResult MainProgram::cmd_route_least_stations(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_least_stations(fromid, toid);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_DISTANCE))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ndist] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, NO_TIME, dist);
            id = nid; dist = ndist;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, dist);
    }

    return {ResultType::ROUTE, result};
}

MainProgram::CmdResult MainProgram::cmd_route_with_cycle(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_with_cycle(fromid);
    if (steps.empty())
    {
        output << "No route found." << std::endl;
        return {};
    }

    if (steps.front() == NO_STATION)
    {
        output << "Starting station not found!" << endl;
        return {};
    }

    if (steps.size() < 2)
    {
        output << "Too short route (" << steps.size() << ") to contain cycles!" << endl;
        return {};
    }

    auto laststation = steps.back();
    auto cycbeg = std::find_if(steps.begin(), steps.end()-1, [laststation](auto const& e){ return e == laststation; });
    if (cycbeg == steps.end())
    {
        output << "No cycle found in returned route!";
        return {};
    }

    //    // If necessary, swap cycle so that it starts with smaller coord
    //    if ((cycbeg+1) < (steps.end()-2))
    //    {
    //        auto stationfirst = *cycbeg;
    //        auto stationlast = *(steps.end()-2);
    //        if (stationlast < stationfirst)
    //        {
    //           std::reverse(cycbeg+1, steps.end()-1);
    //        }
    //    }

    CmdResultRoute result;
    auto id = steps.front();
    for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
    {
        auto& nid = *iter;
        result.emplace_back(NO_TRAIN, id, nid, NO_TIME, NO_DISTANCE);
        id = nid;
    }
    result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, NO_DISTANCE);

    return {ResultType::ROUTE, result};
}


MainProgram::CmdResult MainProgram::cmd_route_shortest_distance(ostream& output, MatchIter begin, MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto steps = ds_.route_shortest_distance(fromid, toid);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_DISTANCE))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ndist] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, NO_TIME, dist);
            id = nid; dist = ndist;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, NO_TIME, dist);
    }

    return {ResultType::ROUTE, result};
}

MainProgram::CmdResult MainProgram::cmd_route_earliest_arrival(std::ostream &output, MatchIter begin, MatchIter end)
{
    string fromid = *begin++;
    string toid = *begin++;
    string starttimestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Time starttime = convert_string_to<Time>(starttimestr);
    auto steps = ds_.route_earliest_arrival(fromid, toid, starttime);
    CmdResultRoute result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_pair(NO_STATION, NO_TIME))
    {
        output << "Starting or destination station not found!" << endl;
    }
    else
    {
        auto [id, time] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [nid, ntime] = *iter;
            result.emplace_back(NO_TRAIN, id, nid, time, NO_DISTANCE);
            id = nid; time = ntime;
        }
        result.emplace_back(NO_TRAIN, id, NO_STATION, time, NO_DISTANCE);
        // calculate the total time taken for the path
        std::vector<long long int> times(steps.size());
        long long int* first = &times[0];
        for(const auto& item : steps){
            *(first++)=item.second;
        }
        if(steps.size()<2){
            output << "There should be at least two steps in route"<<endl;
            return {ResultType::ROUTE,result};
        }
        auto prevtime = starttime;
        for(auto it=times.begin();it!=times.end();++it){
            for(auto& time = *it;time<prevtime;time+=2400){}
            prevtime = *it;
        }
        // calculate the total time
        if (prevtime%100 < starttime%100){
            prevtime-=40;
        }
        auto timetaken = prevtime-starttime;
        // print the time
        output << "Total time for the route " << timetaken/100 << " hours " << timetaken%100 << " minutes." << endl;
    }

    return {ResultType::ROUTE, result};
}


bool MainProgram::command_parse_line(string inputline, ostream& output)
{
    //    static unsigned int nesting_level = 0; // UGLY! Remember nesting level to print correct amount of >:s.
    //    if (promptstyle != PromptStyle::NO_NESTING) { ++nesting_level; }

    if (inputline.empty()) { return true; }

    smatch match;
    bool matched = regex_match(inputline, match, cmds_regex_);
    if (matched)
    {
        assert(match.size() == 3);
        string cmd = match[1];
        string params = match[2];

        auto pos = find_if(cmds_.begin(), cmds_.end(), [cmd](CmdInfo const& ci) { return ci.cmd == cmd; });
        assert(pos != cmds_.end());

        smatch match2;
        bool matched2 = regex_match(params, match2, pos->param_regex);
        if (matched2)
        {
            if (pos->func)
            {
                assert(!match2.empty());

                Stopwatch stopwatch;
                bool use_stopwatch = (stopwatch_mode != StopwatchMode::OFF);
                // Reset stopwatch mode if only for the next command
                if (stopwatch_mode == StopwatchMode::NEXT) { stopwatch_mode = StopwatchMode::OFF; }

                TestStatus initial_status = test_status_;
                test_status_ = TestStatus::NOT_RUN;

                if (use_stopwatch)
                {
                    stopwatch.start();
                }

                CmdResult result;
                try
                {
                    result = (this->*(pos->func))(output, ++(match2.begin()), match2.end());
                }
                catch (NotImplemented const& e)
                {
                    output << endl << "NotImplemented from cmd " << pos->cmd << " : " << e.what() << endl;
                    std::cerr << endl << "NotImplemented from cmd " << pos->cmd << " : " << e.what() << endl;
                }

                if (use_stopwatch)
                {
                    stopwatch.stop();
                }

                switch (result.first)
                {
                case ResultType::NOTHING:
                {
                    break;
                }
                case ResultType::IDLIST:
                {
                    auto& [regions, stations] = std::get<CmdResultIDs>(result.second);
                    if (stations.size() == 1 && stations.front() == NO_STATION)
                    {
                        output << "Failed (NO_STATION returned)!" << std::endl;
                    }
                    else
                    {
                        if (!stations.empty())
                        {
                            if (stations.size() == 1) { output << "Station:" << std::endl; }
                            else { output << "Stations:" << std::endl; }

                            unsigned int num = 0;
                            for (StationID id : stations)
                            {
                                ++num;
                                if (stations.size() > 1) { output << num << ". "; }
                                else { output << "   "; }
                                print_station(id, output);
                            }
                        }
                    }

                    if (regions.size() == 1 && regions.front() == NO_REGION)
                    {
                        output << "Failed (NO_REGION returned)!" << std::endl;
                    }
                    else
                    {
                        if (!regions.empty())
                        {
                            if (regions.size() == 1) { output << "Region:" << std::endl; }
                            else { output << "Regions:" << std::endl; }

                            unsigned int num = 0;
                            for (RegionID id : regions)
                            {
                                ++num;
                                if (regions.size() > 1) { output << num << ". "; }
                                else { output << "   "; }
                                print_region(id, output);
                            }
                        }
                    }
                    break;
                }
                case ResultType::NAMELIST: {
                    auto& resulttype = std::get<CmdResultNames>(result.second);
                    auto& sid = resulttype.first.first;
                    auto rid = convert_to_string(resulttype.first.second);
                    auto& name = resulttype.second;
                    std::string type = sid!=NO_STATION? "station":"region";
                    if (name == NO_NAME)
                    {
                        output << "Failed (NO_NAME returned)!" << std::endl;
                        break;
                    }
                    output << "Name for "<<type<<" with id" << (sid!=NO_STATION? sid:rid);
                    output <<" is " << name<<endl;
                    break;
                }
                case ResultType::COORDLIST: {
                    auto& resulttype = std::get<CmdResultCoords>(result.second);
                    auto& sid = resulttype.first.first;
                    auto rid = convert_to_string(resulttype.first.second);
                    auto& coordlist = resulttype.second;
                    std::string type = sid!=NO_STATION? "Station":"Region";
                    if (coordlist.size()==1 && coordlist.front()==NO_COORD){
                        output << "Failed (NO_COORD returned)!" << std::endl;
                        break;
                    }
                    output << type << (sid!=NO_STATION? sid:rid);
                    output <<" has coordinates"<<endl;
                    for (const auto& coord : coordlist){
                        print_coord(coord, output);
                    }
                    break;
                }
                case ResultType::ROUTE:
                {
                    auto& route = std::get<CmdResultRoute>(result.second);
                    if (!route.empty())
                    {
                        if (route.size() == 1 && get<1>(route.front()) == NO_STATION)
                        {
                            output << "Failed (...NO_STATION... returned)!" << std::endl;
                        }
                        else
                        {
                            unsigned int num = 1;
                            for (auto& r : route)
                            {
                                auto [trainid, stationid1, stationid2, time, dist] = r;
                                output << num << ". ";
                                if (stationid1 != NO_STATION)
                                {
                                    print_station_brief(stationid1, output, false);
                                }
                                if (stationid2 != NO_STATION)
                                {
                                    output << " -> ";
                                    print_station_brief(stationid2, output, false);
                                }
                                if (trainid != NO_TRAIN)
                                {
                                    output << ": ";
                                    print_train(trainid, output, false);
                                }
                                if (time != NO_TIME)
                                {
                                    output << " (at ";
                                    print_time(time, output, false);
                                    output << ")";
                                }
                                if (dist != NO_DISTANCE)
                                {
                                    output << " (distance " << dist << ")";
                                }
                                output << endl;

                                ++num;
                            }
                        }
                    }
                    break;
                }
                case ResultType::TRAINS:
                {
                    auto& route = std::get<CmdResultTrains>(result.second);
                    if (!route.empty())
                    {
                        if (route.size() == 1 && get<1>(route.front()) == NO_STATION)
                        {
                            output << "Failed (...NO_STATION... returned)!" << std::endl;
                        }
                        else
                        {
                            unsigned int num = 1;
                            for (auto& r : route)
                            {
                                auto [trainid, stationid1, stationid2, time] = r;
                                output << num << ". ";
                                if (stationid1 != NO_STATION)
                                {
                                    print_station_brief(stationid1, output, false);
                                }
                                if (stationid2 != NO_STATION)
                                {
                                    output << " -> ";
                                    print_station_brief(stationid2, output, false);
                                }
                                if (trainid != NO_TRAIN)
                                {
                                    output << ": ";
                                    print_train(trainid, output, false);
                                }
                                if (time != NO_TIME)
                                {
                                    output << " (at ";
                                    print_time(time, output, false);
                                    output << ")";
                                }
                                output << endl;

                                ++num;
                            }
                        }
                    }
                    break;
                }
                default:
                {
                    assert(false && "Unsupported result type!");
                }
                }

                if (result != prev_result)
                {
                    prev_result = move(result);
                    view_dirty = true;
                }

                if (use_stopwatch)
                {
                    output << "Command '" << cmd << "': " << stopwatch.elapsed() << " sec";
                    output << endl;
                }

                if (test_status_ != TestStatus::NOT_RUN)
                {
                    output << "Testread-tests have been run, " << ((test_status_ == TestStatus::DIFFS_FOUND) ? "differences found!" : "no differences found.") << endl;
                }
                if (test_status_ == TestStatus::NOT_RUN || (test_status_ == TestStatus::NO_DIFFS && initial_status == TestStatus::DIFFS_FOUND))
                {
                    test_status_ = initial_status;
                }
            }
            else
            { // No function to run = quit command
                return false;
            }
        }
        else
        {
            output << "Invalid parameters for command '" << cmd << "'!" << endl;
        }
    }
    else
    {
        output << "Unknown command!" << endl;
    }

    return true; // Signal continuing
}

void MainProgram::command_parser(istream& input, ostream& output, PromptStyle promptstyle)
{
    string line;
    do
    {
        //        output << string(nesting_level, '>') << " ";
        output << PROMPT;
        getline(input, line, '\n');

        if (promptstyle != PromptStyle::NO_ECHO)
        {
            output << line << endl;
        }

        if (!input) { break; }

        bool cont = command_parse_line(line, output);
        view_dirty = false; // No need to keep track of individual result changes
        if (!cont) { break; }
    }
    while (input);
    //    if (promptstyle != PromptStyle::NO_NESTING) { --nesting_level; }

    view_dirty = true; // To be safe, assume that results have been changed
}

void MainProgram::setui(MainWindow* ui)
{
    ui_ = ui;
}

#ifdef GRAPHICAL_GUI
void MainProgram::flush_output(std::ostream& output)
{
    if (ui_)
    {
        if (auto soutput = dynamic_cast<ostringstream*>(&output))
        {
            ui_->output_text(*soutput);
        }
    }
}
#else
void MainProgram::flush_output(std::ostream& /*output*/)
{
}
#endif

bool MainProgram::check_stop() const
{
#ifdef GRAPHICAL_GUI
    if (ui_)
    {
        return ui_->check_stop_pressed();
    }
#endif
    return false;
}

std::array<unsigned long int, 20> const MainProgram::primes1{4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003,
                                                             5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087};
std::array<unsigned long int, 20> const MainProgram::primes2{81031,  81041,  81043,  81047,  81049,  81071,  81077,  81083,  81097,  81101,
                                                             81119,  81131,  81157,  81163,  81173,  81181,  81197,  81199,  81203,  81223};

MainProgram::MainProgram()
{
    rand_engine_.seed(time(nullptr));

    //    startmem = get<0>(mempeak());

    init_primes();
    init_regexs();
}

int MainProgram::mainprogram(int argc, char* argv[])
{
    vector<string> args(argv, argv+argc);

    if (args.size() < 1 || args.size() > 2)
    {
        cerr << "Usage: " + ((args.size() > 0) ? args[0] : "<program name>") + " [<command file>]" << endl;
        return EXIT_FAILURE;
    }

    MainProgram mainprg;

    if (args.size() == 2 && args[1] != "--console")
    {
        string filename = args[1];
        ifstream input(filename);
        if (input)
        {
            mainprg.command_parser(input, cout, MainProgram::PromptStyle::NORMAL);
        }
        else
        {
            cout << "Cannot open file '" << filename << "'!" << endl;
        }
    }
    else
    {
        mainprg.command_parser(cin, cout, MainProgram::PromptStyle::NO_ECHO);
    }

    cerr << "Program ended normally." << endl;
    if (mainprg.test_status_ == TestStatus::DIFFS_FOUND)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

void MainProgram::init_primes()
{
    // Initialize id generator
    prime1_ = primes1[random<int>(0, primes1.size())];
    prime2_ = primes2[random<int>(0, primes2.size())];
    random_stations_added_ = 0;
    random_regions_added_ = 0;
    random_trains_added_ = 0;
    random_departures_added_=0;
    clear_test_cache();
}

Name MainProgram::random_name(){
    Name id;
    // there might be changes in this function on grader

    // all perftest Names are 8 characters long -> (27*2)^20 possibilities
    // this results in pretty low chance of collisions when using <=10^6 ids

    // first get charcode
    auto chardistr = std::uniform_int_distribution<int>('A','Z'+('z'-'a'));
    for(unsigned int i = 0; i<NAME_LENGTH;++i){
        auto charint = chardistr(rand_engine_);
        // if charcode is above uppercase, add offset for lowercase charcodes
        id+=(char)(charint>'Z'?charint+('a'-'Z'):charint);
    }
    return id;
}

Coord MainProgram::random_coord()
{
    int x = random<int>(min.x, max.x);
    int y = random<int>(min.y, max.y);
    Coord newcoord{x,y};
    return newcoord;
}

RegionID MainProgram::random_region_id()
{
    return random<RegionID>(0,MAX_REGIONID);
}

TrainID MainProgram::random_train_id()
{
    TrainID id="T-";
    // there might be changes in this function on grader

    // first get charcode
    auto chardistr = std::uniform_int_distribution<int>('A','Z'+('z'-'a'));
    for(unsigned int i = 0; i<TRAINID_LENGTH;++i){
        auto charint = chardistr(rand_engine_);
        // if charcode is above uppercase, add offset for lowercase charcodes
        id+=(char)(charint>'Z'?charint+('a'-'Z'):charint);
    }
    return id;
}

Time MainProgram::random_time()
{
    return random<unsigned int>(0,24)*100+random<unsigned int>(0,60);
}

std::tuple<StationID, TrainID, Time> MainProgram::random_departure()
{
    return {
        random_valid_station(),
        random_train_id(),
        random_time()
    };
}

Name MainProgram::n_to_name(unsigned long n)
{
    if(n<valid_station_names_.size()){
        // this WILL result in different names for same n since valid_names_ is unordered set
        return *std::next(valid_station_names_.begin(),n);
    }
    Name name;
    for(name=random_name();valid_station_names_.find(name)!=valid_station_names_.end();name=random_name()){}
    valid_station_names_.insert(name);
    return name;
}

StationID MainProgram::random_station_id(){
    StationID id;
    // there might be changes in this function on grader

    // all perftest StationIDs are 20 characters long -> (27*2)^20 possibilities
    // this results in pretty low chance of collisions when using <=10^6 ids

    // first get charcode
    auto chardistr = std::uniform_int_distribution<int>('A','Z'+('z'-'a'));
    for(unsigned int i = 0; i<STATIONID_LENGTH;++i){
        auto charint = chardistr(rand_engine_);
        // if charcode is above uppercase, add offset for lowercase charcodes
        id+=(char)(charint>'Z'?charint+('a'-'Z'):charint);
    }
    return id;
}

StationID MainProgram::n_to_stationid(unsigned long n)
{
    if(n<station_ids_.size()){
        return *std::next(station_ids_.begin(),n);
    }
    StationID id;
    for(id=random_station_id();valid_station_ids_.find(id)!=valid_station_ids_.end();id=random_station_id()){}
    valid_station_ids_.insert(id);
    station_ids_.push_back(id);
    return id;


}

RegionID MainProgram::n_to_regionid(unsigned long n)
{
    if(n<region_ids_.size()){
        return *std::next(region_ids_.begin(),n);
    }
    RegionID id;
    for(id=random_region_id();valid_region_ids_.find(id)!=valid_region_ids_.end();id=random_region_id()){}
    valid_region_ids_.insert(id);
    region_ids_.push_back(id);
    return id;
}

TrainID MainProgram::n_to_trainid(unsigned long n)
{

    if(n<trains_.size()){
        return *std::next(trains_.begin(),n);
    }
    TrainID id;
    for(id=random_train_id();valid_train_ids_.find(id)!=valid_train_ids_.end();id=random_train_id()){}
    valid_train_ids_.insert(id);
    trains_.push_back(id);
    return id;
}



Coord MainProgram::n_to_coord(unsigned long n)
{
    if (n<coords_.size()){
        return *std::next(coords_.begin(),n);
    }
    Coord xy;
    for(xy=random_coord();valid_coords_.find(xy)!=valid_coords_.end();xy=random_coord()){}
    valid_coords_.insert(xy);
    coords_.push_back(xy);
    return xy;

}

void MainProgram::clear_test_cache()
{
    valid_station_ids_.clear();
    valid_region_ids_.clear();
    valid_train_ids_.clear();
    valid_coords_.clear();
    station_ids_.clear();
    region_ids_.clear();
    coords_.clear();
    trains_.clear();
    valid_departures_.clear();
    departures_.clear();
    valid_station_names_.clear();
}

RegionID MainProgram::random_root_region()
{
    if(random_regions_added_<2){
        return n_to_regionid(0);
    }
    unsigned long end = ROOT_BIAS_MULTIPLIER * random_regions_added_;
    if (end == 0 ) {
        return n_to_regionid(0);
    }
    return n_to_regionid(random<decltype(random_regions_added_)>(0, end));
}

RegionID MainProgram::random_leaf_region()
{
    if(random_regions_added_<2){
        return n_to_regionid(0);
    }
    unsigned long start = LEAF_BIAS_MULTIPLIER * random_regions_added_;
    if (start == random_regions_added_) {
        start = 0;
    }
    return n_to_regionid(random<decltype(random_regions_added_)>(start, random_regions_added_));
}

StationID MainProgram::random_valid_station()
{
    return n_to_stationid(random<decltype(random_stations_added_)>(0, random_stations_added_));
}

RegionID MainProgram::random_valid_region()
{
    return n_to_regionid(random<decltype(random_regions_added_)>(0, random_regions_added_));
}

Coord MainProgram::random_valid_coord()
{
    return *std::next(coords_.begin(),random<decltype(coords_.size())>(0,coords_.size()));
}

std::tuple<StationID, TrainID, Time> MainProgram::random_valid_departure()
{
    return *std::next(departures_.begin(),random<decltype(departures_.size())>(0,departures_.size()));
}

TrainID MainProgram::random_valid_train()
{
    return n_to_trainid(random<decltype(random_trains_added_)>(0,random_trains_added_));
}

void MainProgram::init_regexs()
{
    // Create regex <whitespace>(cmd1|cmd2|...)<whitespace>(.*)
    string cmds_regex_str = "[[:space:]]*(";
    bool first = true;
    for (auto& cmd : cmds_)
    {
        cmds_regex_str += (first ? "" : "|") + cmd.cmd;
        first = false;

        cmd.param_regex = regex(cmd.param_regex_str+"[[:space:]]*", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    }
    cmds_regex_str += ")(?:[[:space:]]*$|"+wsx+"(.*))";
    cmds_regex_ = regex(cmds_regex_str, std::regex_constants::ECMAScript | std::regex_constants::optimize);
    coords_regex_ = regex(coordx+"[[:space:]]?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    stationtimes_regex_ = regex(stationidx+":"+timex+"[[:space:]]?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    times_regex_ = regex(wsx+"([0-9][0-9]):([0-9][0-9]):([0-9][0-9])", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    commands_divide_regex_ = regex("([0-9a-zA-Z_;\\*]+):?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    timeouts_regex_=regex("([0-9]+):?",std::regex_constants::ECMAScript | std::regex_constants::optimize);
    commands_regex_ = regex("(?:([0-9]+)\\*)?([0-9a-zA-Z_]+);?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    sizes_regex_ = regex(numx+";?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    range_regex_=regex(numx+":?",std::regex_constants::ECMAScript | std::regex_constants::optimize);
    is_range_notation_regex_ =regex("([0-9]+(?::[0-9]+){1,2})",std::regex_constants::ECMAScript | std::regex_constants::optimize);
}
