// Datastructures.cc

#include "datastructures.hh"

#include <random>

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
    throw NotImplemented("station_count()");
}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    throw NotImplemented("clear_all()");
}

std::vector<StationID> Datastructures::all_stations()
{
    // Replace the line below with your implementation
    throw NotImplemented("all_stations()");
}

bool Datastructures::add_station(StationID /*id*/, const Name& /*name*/, Coord /*xy*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("add_station()");
}

Name Datastructures::get_station_name(StationID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_station_name()");
}

Coord Datastructures::get_station_coordinates(StationID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_station_coordinates()");
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    // Replace the line below with your implementation
    throw NotImplemented("stations_alphabetically()");
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    // Replace the line below with your implementation
    throw NotImplemented("stations_distance_increasing()");
}

std::vector<StationID> Datastructures::find_stations_with_coord(Coord /*xy*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("find_stations_with_coord()");
}

bool Datastructures::change_station_coord(StationID /*id*/, Coord /*newcoord*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("change_station_coord()");
}

bool Datastructures::add_departure(StationID /*stationid*/, TrainID /*trainid*/, Time /*time*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("add_departure()");
}

bool Datastructures::remove_departure(StationID /*stationid*/, TrainID /*trainid*/, Time /*time*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("remove_departure()");
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID /*stationid*/, Time /*time*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("station_departures_after()");
}

bool Datastructures::add_region(RegionID /*id*/, const Name &/*name*/, std::vector<Coord> /*coords*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("add_region()");
}

std::vector<RegionID> Datastructures::all_regions()
{
    // Replace the line below with your implementation
    throw NotImplemented("all_regions()");
}

Name Datastructures::get_region_name(RegionID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_region_name()");
}

std::vector<Coord> Datastructures::get_region_coords(RegionID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_region_coords()");
}

bool Datastructures::add_subregion_to_region(RegionID /*id*/, RegionID /*parentid*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("add_subregion_to_region()");
}

bool Datastructures::add_station_to_region(StationID /*id*/, RegionID /*parentid*/)
{
    // Replace the line below with your implementation
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

// prg2 functions below

bool Datastructures::add_train(TrainID /*trainid*/, std::vector<std::pair<StationID, Time> > /*stationtimes*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("add_train()");
}

std::vector<StationID> Datastructures::next_stations_from(StationID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("next_stations_from()");
}

std::vector<StationID> Datastructures::train_stations_from(StationID /*stationid*/, TrainID /*trainid*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("train_stations_from()");
}

void Datastructures::clear_trains()
{
    // Replace the line below with your implementation
    throw NotImplemented("clear_trains()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("route_any()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("route_least_stations()");
}

std::vector<StationID> Datastructures::route_with_cycle(StationID /*fromid*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("route_with_cycle()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("route_shortest_distance()");
}

std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID /*fromid*/, StationID /*toid*/, Time /*starttime*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("route_earliest_arrival()");
}
