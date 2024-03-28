// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)
// You may add your own functions here

bool compare_time_TrainID(const std::pair<Time, TrainID>& p1, const std::pair<Time, TrainID>& p2) {
    if (p1.first < p2.first) return true;
    else if (p1.first > p2.first) return false;
    else return p1.second < p2.second;
}

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::station_count()
{
    // Replace the line below with your implementation
    return map_of_stationID.size();
    // throw NotImplemented("station_count()");
}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    map_of_stationID.clear();
    map_of_regionID.clear();
    set_of_station_names.clear();
    map_of_station_coord.clear();

    sorted_Id_Alphabetically.clear();
    sorted_Id_Distance.clear();
    multimap_of_station_train_id.clear();
    // throw NotImplemented("clear_all()");
}

std::vector<StationID> Datastructures::all_stations()
{
    // Replace the line below with your implementation
    std::vector<StationID> vectorOfId;
    vectorOfId.reserve(map_of_stationID.size());
    std::transform(map_of_stationID.begin(), map_of_stationID.end(), std::back_inserter(vectorOfId),
              [](const std::pair<StationID, Station_struct>& pair) {
                 return pair.first;
              });
    return vectorOfId;
    // throw NotImplemented("all_stations()");
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // Replace the line below with your implementation
    if (map_of_stationID.find(id) == map_of_stationID.end()) {
        // not found
        Station_struct temp_aff;
        temp_aff.stationid = id;
        temp_aff.name = name;
        temp_aff.location = xy;

        map_of_stationID.insert({id, temp_aff});

        set_of_station_names.insert({name, id});
        map_of_station_coord.insert({xy,id});
        is_latest_station_sortedValid = false;
        is_latest_station_sortedAlphabeticallyValid = false;
        return true;
    } else {
        // found
        return false;
    }
    //throw NotImplemented("add_station()");
}

Name Datastructures::get_station_name(StationID id)
{
    // Replace the line below with your implementation
    auto search = map_of_stationID.find(id);
    if (search == map_of_stationID.end()) {
        // not found
        return NO_NAME;
    } else {
        // found
        return search->second.name;
    }
    // throw NotImplemented("get_station_name()");
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    // Replace the line below with your implementation
    auto search = map_of_stationID.find(id);
    if (search == map_of_stationID.end()) {
        // not found
        return NO_COORD;
    } else {
        // when it is not found
        return search->second.location;
    }
    // throw NotImplemented("get_station_coordinates()");
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    // Replace the line below with your implementation
    unsigned int latest_station_count = map_of_stationID.size();
    if (is_latest_station_sortedAlphabeticallyValid == false){
        std::vector<StationID> vectorOfId;
        vectorOfId.reserve(latest_station_count);
        std::transform(set_of_station_names.begin(), set_of_station_names.end(), std::back_inserter(vectorOfId), 
        [](const std::pair<Name, StationID>& pair) { return pair.second; });

        is_latest_station_sortedAlphabeticallyValid = true;
        sorted_Id_Alphabetically = vectorOfId;
    }

    return sorted_Id_Alphabetically;
    //throw NotImplemented("stations_alphabetically()");
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    // Replace the line below with your implementation
    unsigned int latest_station_count = map_of_stationID.size();
    if (is_latest_station_sortedValid == false){
        std::vector<StationID> vectorOfId;
        vectorOfId.reserve(latest_station_count);
        std::transform(map_of_station_coord.begin(), map_of_station_coord.end(), std::back_inserter(vectorOfId),
        [](const std::pair<Coord, StationID>& p) { return p.second; });

        sorted_Id_Distance = vectorOfId;
        is_latest_station_sortedValid = true;
    }

    return sorted_Id_Distance;
    //throw NotImplemented("stations_distance_increasing()");
}

std::vector<StationID> Datastructures::find_stations_with_coord(Coord xy)
{
    // Replace the line below with your implementation
    auto search = map_of_station_coord.find(xy);
    if (search == map_of_station_coord.end()) {
        // not found
        return NO_STATION;
    } else {
        // found
        return search->second;
    }
    //throw NotImplemented("find_stations_with_coord()");
}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    // Replace the line below with your implementation
    auto search = map_of_stationID.find(id);
    if (search == map_of_stationID.end()) {
        // not found
        return false;
    } else {
        // found
        Station_struct old_aff = search->second;
        map_of_station_coord.erase(old_aff.location); // removable to improve time efficiency
        map_of_station_coord[newcoord] = id;

        old_aff.location = newcoord;
        map_of_stationID[id] = old_aff;
        is_latest_station_sortedValid = false;
        return true;
    }    
    //throw NotImplemented("change_station_coord()");
}

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    if (map_of_stationID.find(stationid) == map_of_stationID.end()) {
        // not found
        return false;
    } else {
        // found
        // need to check if trainId is present and time is 
        auto range = multimap_of_station_train_id.equal_range(stationid);
        for (auto it = range.first; it != range.second; ++it){
            if(it->second == {time, trainid}){
                return false;
            }
        }
        multimap_of_station_train_id.insert({stationid, {time, trainid}});        
        return true;
        
    }
    // throw NotImplemented("add_departure()");
}

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    if (map_of_stationID.find(stationid) == map_of_stationID.end()) {
        // not found
        return false;
    } else {
        // found
        // need to check if trainId is present and time is 
        auto range = multimap_of_station_train_id.equal_range(stationid);
        for (auto it = range.first; it != range.second; ++it){
            if(it->second == {time, trainid}){
                //erase
                multimap_of_station_train_id.erase(it);
                return true;
            }
        }
        multimap_of_station_train_id.insert({stationid, {time, trainid}});        
        return true;
        
    }
    // throw NotImplemented("remove_departure()");
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    // Replace the line below with your implementation
    auto search = multimap_of_station_train_id.find(stationid);
    if (search == multimap_of_station_train_id.end()) {
        // not found
        return {{NO_TIME, NO_TRAIN}};
    } else {
        // found
        // use equal range
        auto range = multimap_of_station_train_id.equal_range(stationid);
        std::vector<std::pair<Time, TrainID>> vectorOfId;
        vectorOfId.reserve((std::distance(range.first, range.second)));

        for (auto it = range.first; it != range.second; ++it){
            // std::cout << it->first << ' ' << it->second.first << '\n';
            if(it->second.first >= time) {
                vectorOfId.push_back(it->second);
            }
        }
        std::sort(vectorOfId.begin(),vectorOfId.end(), compare_time_TrainID);
        return vectorOfId;
    }
    // throw NotImplemented("station_departures_after()");
}

bool Datastructures::add_region(RegionID id, const Name & name, std::vector<Coord> coords)
{
    // Replace the line below with your implementation
    if (map_of_regionID.find(id) == map_of_regionID.end()) {
        // not found
        Region newRegion(id, name, coords);
        map_of_regionID.insert({id, newRegion}); // i dont need to use new;
        // for (AffiliationID affilation_id : affiliations) {
        //     multimap_of_affilation_publication_id.insert({affilation_id, {id, year}});
        // }         
        return true;
    } else {
        // found
        return false;
    }
    // throw NotImplemented("add_region()");
}

std::vector<RegionID> Datastructures::all_regions()
{
    // Replace the line below with your implementation
    std::vector<RegionID> vectorOfId;
    vectorOfId.reserve(map_of_regionID.size());
    std::transform(map_of_regionID.begin(), map_of_regionID.end(), std::back_inserter(vectorOfId),
              [](const std::pair<RegionID, Region>& pair) {
                 return pair.first;
              });
    return vectorOfId;
    // throw NotImplemented("all_regions()");
}

Name Datastructures::get_region_name(RegionID id)
{
    // Replace the line below with your implementation
    auto search = map_of_regionID.find(id);
    if (search == map_of_regionID.end()) {
        // not found
        return NO_NAME;
    } else {
        // found
        return search->second.getTitle();
    }
    // throw NotImplemented("get_region_name()");
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    // Replace the line below with your implementation
    auto search = map_of_regionID.find(id);
    if (search == map_of_regionID.end()) {
        // not found
        return NO_COORD;
    } else {
        // found
        return search->second.getCoords();
    }
    // throw NotImplemented("get_region_coords()");
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    // Replace the line below with your implementation
    auto search_parent = map_of_regionID.find(parentid);
    auto search_child = map_of_regionID.find(id);
    if (search_parent == map_of_regionID.end() || search_child == map_of_regionID.end()) {
        // not found
        return false;
    } else {
        // found
        search_parent->second.addReference(&(search_child->second));
        search_child->second.addParent(&(search_parent->second));
        return true;
    }
    // throw NotImplemented("add_subregion_to_region()");
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    // Replace the line below with your implementation
    auto search = map_of_regionID.find(parentid);
    if (search == map_of_regionID.end()) {
        // not found
        return false;
    } else {
        // found
        search->second.add_station(id);
        // multimap_of_station_train_id.insert({id, {search->second.getYear(), search->second.getId()}});
        return true;
    }   
    throw NotImplemented("add_station_to_region()");
}

std::vector<RegionID> Datastructures::station_in_regions(StationID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("station_in_regions()");
}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("remove_station()");
}

RegionID Datastructures::common_ancestor_of_regions(RegionID /*id1*/, RegionID /*id2*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("common_ancestor_of_regions()");
}
