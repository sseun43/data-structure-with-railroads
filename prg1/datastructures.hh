// Datastructures.hh
//
// Student name: Oluwaseun Akangbe
// Student email: oluwaseun.akangbe@tuni.fi
// Student number: 150474395

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <vector>
#include <utility>

#include "customtypes.hh"

// Add your own STL includes below this comment

// my code
#include <string>
#include <tuple>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <cmath>

struct Station_struct { 
  StationID stationid;
  Name name;
  Coord location;
};

class Region {
public:
  Region(RegionID id_in, Name title_in, std::vector<Coord> regionShapeCoords_in, std::vector<StationID> stations_in = {}) {
    std::unordered_set<StationID> myset(stations_in.begin(), stations_in.end());
    id = id_in;
    regionShapeCoords = regionShapeCoords_in;
    title = title_in;
    region_stations = myset;
  }

  RegionID getId() {
    return id;
  }

  Name getTitle() {
    return title;
  }

  std::vector<Coord> getCoords() {
    return regionShapeCoords;
  }

  void addReference(Region* reference) {
    references.push_back(reference);
  }

  void add_station(StationID station_id) {
    region_stations.insert(station_id);
  }

  void remove_station(StationID station_id) {
    region_stations.erase(station_id);
  }

  void addParent(Region* parentToAdd){
      parent = parentToAdd;
  }

  std::vector<Region*> getReferences() {
    return references;
  }

  std::vector<StationID> getRegStations() {
      std::vector<StationID> vec(region_stations.begin(), region_stations.end());
      return vec;
  }

  Region* getParent(){
      return parent;
  }

  void clearReference(){
      for (auto p : references) {
          (*p).removeParent();
      }
      references.clear();
  }

  void removeParent(){
      parent = nullptr;
  }

private:
  RegionID id;
  Name title;
  std::vector<Coord> regionShapeCoords;
  std::unordered_set<StationID> region_stations;
  std::vector<Region*> references;
  Region* parent = nullptr;
};

struct CompareCoordinates {
   bool operator()(Coord c1, Coord c2) const {
       double distanceA = std::abs(c1.x) + std::abs(c1.y);
       double distanceB = std::abs(c2.x) + std::abs(c2.y);

       if (distanceA == distanceB) {
           return c1.y < c2.y;
       }

       return distanceA < distanceB;
   }
};

struct CompareCoordinatesForSet {
   bool operator()(std::pair<Coord, StationID> c1, std::pair<Coord, StationID> c2) const {
      if (c1.first == c2.first) {
          return c1.second < c2.second;
      }
       double distanceA = std::abs(c1.first.x) + std::abs(c1.first.y);
       double distanceB = std::abs(c2.first.x) + std::abs(c2.first.y);

       if (distanceA == distanceB) {
           return c1.first.y < c2.first.y;
       }

       return distanceA < distanceB;
   }
};

// Add your own STL includes above this comment

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Just a short call to get size
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: clearing container depends on it size
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: converting a map into a vector
    std::vector<StationID> all_stations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: worst insertion is that of map
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find worst case for unordered map is linear
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find worst case for unordered map is linear
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: converting map to vector 
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: converting map to vector
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is logarithmic for map
    std::vector<StationID> find_stations_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: the most expensive operation is find
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Equal range of station is main operation here
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Equal range of station is main operation here
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Sorting of a vector
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: worst insertion is that of map
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: conversion of map to vector
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find operation on a map
    Name get_region_name(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find operation on a map
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find operation on a map
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find operation on a map
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: find operation in a loop
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: It is just collecting data into a list
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: 0(n)
    // Short rationale for estimate: partial sort is constant O(3log(3)) so then the most expensive operation is linear
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: there is a find operation in a loop
    bool remove_station(StationID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: there is a find operation in a loop
    RegionID common_ancestor_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here
    std::multiset<std::pair<Name, StationID>> set_of_station_names;
    std::multimap<Coord, StationID, CompareCoordinates> multimap_of_station_coord;

    std::set<std::pair<Coord, StationID>, CompareCoordinatesForSet> set_of_station_coord;

    std::unordered_map<StationID, Station_struct> map_of_stationID;
    std::vector<StationID> sorted_Id_Alphabetically;
    std::vector<StationID> sorted_Id_Distance;
    std::vector<std::pair<Coord, StationID>> vector_of_station_coord;
    bool is_latest_station_sortedValid = false;
    bool is_latest_station_sortedAlphabeticallyValid = false;

    std::unordered_map<RegionID, Region> map_of_regionID;
    std::unordered_map<StationID, RegionID> map_of_station_region_id;
    std::unordered_multimap<StationID, std::pair<Time, TrainID>> multimap_of_station_train_id;
    void get_all_references_helper(Region node, std::vector<RegionID>& referenceListToUpdate);
    RegionID get_parent(RegionID id);
    std::vector<RegionID> getAllParentsOfRegions(RegionID id);
};

#endif // DATASTRUCTURES_HH
