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
    multimap_of_station_coord.clear();
    set_of_station_coord.clear();

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
        multimap_of_station_coord.insert({xy,id});
        set_of_station_coord.insert({xy,id});

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
        std::transform(set_of_station_coord.begin(), set_of_station_coord.end(), std::back_inserter(vectorOfId),
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
    auto search = multimap_of_station_coord.find(xy);
    if (search == multimap_of_station_coord.end()) {
        // not found
        return {};
    } else {
        // found
        // use equal range
        auto range = multimap_of_station_coord.equal_range(xy);
        std::vector<StationID> vectorOfId;
        vectorOfId.reserve((std::distance(range.first, range.second)));

        for (auto it = range.first; it != range.second; ++it){
            vectorOfId.push_back(it->second);
        }
        return vectorOfId;
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
        // use equal range
        Station_struct old_aff = search->second;
        auto range = multimap_of_station_coord.equal_range(old_aff.location);
        for (auto it = range.first; it != range.second; ++it){
            if(it->second == id){
                multimap_of_station_coord.erase(it);
                set_of_station_coord.erase({old_aff.location, id});
                multimap_of_station_coord.insert({newcoord, id});
                set_of_station_coord.insert({newcoord, id});

                old_aff.location = newcoord;
                map_of_stationID[id] = old_aff;
                is_latest_station_sortedValid = false;
                return true;
            }
        }
    }
    return false;    
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
            if(it->second.first == time && it->second.second == trainid){
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
            if(it->second.first == time && it->second.second == trainid){
                //erase
                multimap_of_station_train_id.erase(it);
                return true;
            }
        }
        // multimap_of_station_train_id.insert({stationid, {time, trainid}});        
        return false;
        
    }
    // throw NotImplemented("remove_departure()");
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    // Replace the line below with your implementation
    Name stationName = get_station_name(stationid);
    if(stationName == NO_NAME){
        return {{NO_TIME, NO_TRAIN}};
    }
    auto search = multimap_of_station_train_id.find(stationid);
    if (search == multimap_of_station_train_id.end()) {
        // not found s
        return {};
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
        // for (StationID affilation_id : affiliations) {
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
        return {NO_COORD};
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
        // check if id is a subregion of another, if not add new parent
        if (search_child->second.getParent() == nullptr){
            search_parent->second.addReference(&(search_child->second));
            search_child->second.addParent(&(search_parent->second));
            return true;
        }
        return false;
    }
    // throw NotImplemented("add_subregion_to_region()");
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    // Replace the line below with your implementation
    Name stationName = get_station_name(id);
    if(stationName == NO_NAME){
        return false;
    }
    auto search = map_of_regionID.find(parentid);
    if (search == map_of_regionID.end()) {
        // not found
        return false;
    } else {
        // found
        // check if station belongs to a region, if it does return false
        auto search_station_in_region = map_of_station_region_id.find(id);
        if (search_station_in_region == map_of_station_region_id.end()) {
            // not found
            search->second.add_station(id);
            map_of_station_region_id.insert({id, parentid});
            return true;
        }
        return false;
    }   
    throw NotImplemented("add_station_to_region()");
}

RegionID Datastructures::get_parent(RegionID id)
{
    // Replace the line below with your implementation
    auto search = map_of_regionID.find(id);
    if (search == map_of_regionID.end()) {
        // not found
        return NO_REGION;
    } else {
        // found
        if (search->second.getParent() == nullptr){
            return NO_REGION;
        }
        return (search->second.getParent())->getId();
    }
    // throw NotImplemented("get_parent()");
}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    // Replace the line below with your implementation
    Name stationName = get_station_name(id);
    if(stationName == NO_NAME){
        return {NO_REGION};
    }
    auto search = map_of_station_region_id.find(id);
    if (search == map_of_station_region_id.end()) {
        // not found
        return {};
    } else {
        // found
        // region should always be found
        // auto region_search = map_of_regionID.find(search->second.getId());

        // found
        std::vector<RegionID> vectorOfId;
        vectorOfId.reserve(map_of_regionID.size()); // try removing this line if there is problem
        vectorOfId.push_back(search->second);
        RegionID parentId = get_parent(search->second);
        while (parentId != NO_REGION) {
            vectorOfId.push_back(parentId);
            parentId = get_parent(parentId);
        }
        return vectorOfId;

    }
    // throw NotImplemented("station_in_regions()");
}

// Optional
void Datastructures::get_all_references_helper(Region node, std::vector<RegionID>& referenceListToUpdate)
{   
    std::vector<Region*> children;
//    if (referenceListToUpdate.empty() == false){
//        referenceListToUpdate.push_back(node.getId()); // only push the parent node if node is not first searched node
//    }
    children = node.getReferences();
    if (node.getReferences().empty()) {
        return;
    }
    for (auto it = children.begin(); it != children.end(); ++it){
        referenceListToUpdate.push_back((*it)->getId());
        get_all_references_helper(*(*(it)), referenceListToUpdate); // ugly dereferencing
    }   
    // throw NotImplemented("get_all_references()");
}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    // Replace the line below with your implementation
    auto search = map_of_regionID.find(id);
    if (search == map_of_regionID.end()) {
        // not found
        return {NO_REGION};
    } else {
        // found
        std::vector<RegionID> vectorOfId;
        vectorOfId.reserve(map_of_regionID.size()); // try removing this line if there is problem
        get_all_references_helper(search->second, vectorOfId);
        return vectorOfId;       
    }
    // throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    // Replace the line below with your implementation
    if(map_of_stationID.size() == 0){
        return {};
    }
    auto comp = [&xy](const std::pair<Coord, StationID>& a, const std::pair<Coord, StationID>& b) {
        double distanceA = std::abs(a.first.x - xy.x) + std::abs(a.first.y - xy.y);
        double distanceB = std::abs(b.first.x - xy.x) + std::abs(b.first.y - xy.y);

        if (distanceA == distanceB) {
           return a.first.y < b.first.y;
       }

        return distanceA < distanceB;
    };

    std::vector<StationID> resultVector;
    resultVector.reserve(3);

    if(vector_of_station_coord.size() == multimap_of_station_coord.size()){
        std::partial_sort(vector_of_station_coord.begin(), vector_of_station_coord.begin() + 3, vector_of_station_coord.end(), comp);
        if(vector_of_station_coord.size() > 2){
            for(int i = 0; i < 3; i++) { resultVector.push_back(vector_of_station_coord[i].second);}
            return resultVector;
        }
        std::transform(vector_of_station_coord.begin(), vector_of_station_coord.end(), std::back_inserter(resultVector),[](const std::pair<Coord, StationID>& p) {
            return p.second;
        });
        return resultVector;
    } else {
        std::vector<std::pair<Coord, StationID>> vectorOfId;
        vectorOfId.reserve(multimap_of_station_coord.size());
        std::transform(multimap_of_station_coord.begin(), multimap_of_station_coord.end(), std::back_inserter(vectorOfId),
                    [](const std::pair<Coord, StationID>& pair) {
                        return pair;
                    });
        std::partial_sort(vectorOfId.begin(), vectorOfId.begin() + 3, vectorOfId.end(), comp);
        vector_of_station_coord = vectorOfId;
        if(vectorOfId.size() > 2){
            for(int i = 0; i < 3; i++) { resultVector.push_back(vectorOfId[i].second);}
            return resultVector;
        }
        std::transform(vectorOfId.begin(), vectorOfId.end(), std::back_inserter(resultVector),[](const std::pair<Coord, StationID>& p) {
            return p.second;
        });
        return resultVector;
    }
    //throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID id)
{
    // Replace the line below with your implementation remove_affiliation
    // std::vector<RegionID> vectorOfId = get_publications(id);
    Name stationName = get_station_name(id);
    if(stationName == NO_NAME){
        return false;
    }
    // get direct region the station belongs to
    auto region_search = map_of_station_region_id.find(id);

    set_of_station_names.erase({get_station_name(id), id});
    Coord stationLocation = get_station_coordinates(id);
    // should only remove one station not everything
    auto range = multimap_of_station_coord.equal_range(stationLocation);
    for (auto it = range.first; it != range.second; ++it){
        if(it->second == id){
            it = multimap_of_station_coord.erase(it);
        }
    }

    set_of_station_coord.erase({stationLocation, id});

    multimap_of_station_train_id.erase(id);
    map_of_station_region_id.erase(id);
    map_of_stationID.erase(id);

    is_latest_station_sortedValid = false;
    is_latest_station_sortedAlphabeticallyValid = false;
    if(region_search == map_of_station_region_id.end()){
        // not found
        return true;
    } 

    RegionID stationRegionID = region_search->second;
    Region foundRegion = map_of_regionID.find(stationRegionID)->second;
    foundRegion.remove_station(id);

    return true;
    //throw NotImplemented("remove_station()");
}

std::vector<RegionID> Datastructures::getAllParentsOfRegions(RegionID id)
{
    auto search = map_of_regionID.find(id);
    if (search == map_of_regionID.end()) {
        // not found
        return {NO_REGION};
    } else {
        // found
        // region should always be found
        // auto region_search = map_of_regionID.find(search->second.getId());

        // found
        std::vector<RegionID> vectorOfId;
        vectorOfId.reserve(map_of_regionID.size()); // try removing this line if there is problem
        // vectorOfId.push_back(search->second.getId());
        RegionID parentId = get_parent(search->second.getId());
        while (parentId != NO_REGION) {
            vectorOfId.push_back(parentId);
            parentId = get_parent(parentId);
        }
        return vectorOfId;

    }
}

RegionID Datastructures::common_ancestor_of_regions(RegionID id1, RegionID id2)
{
    // Replace the line below with your implementation get_closest_common_parent
    std::vector<RegionID> list_of_reference_1 = getAllParentsOfRegions(id1);
    std::vector<RegionID> list_of_reference_2 = getAllParentsOfRegions(id2);

    if(list_of_reference_1[0] == NO_REGION || list_of_reference_2[0] == NO_REGION) {
        return NO_REGION;
    }

    std::vector<int> v_intersection;
    if(list_of_reference_1.size() < list_of_reference_2.size()){
        for(auto &id : list_of_reference_1) {
            if(std::find(list_of_reference_2.begin(), list_of_reference_2.end(), id) != list_of_reference_2.end()) {
                return id;
            }
        }
    } else {
        for(auto &id : list_of_reference_2) {
            if(std::find(list_of_reference_1.begin(), list_of_reference_1.end(), id) != list_of_reference_1.end()) {
                return id;
            }
        }
    }

    return NO_REGION;
    // throw NotImplemented("common_ancestor_of_regions()");
}
