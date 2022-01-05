#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>

#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"


int WaitingVehicles::getSize()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _vehicles.size();
}

void WaitingVehicles::pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _vehicles.push_back(vehicle);
    _promises.push_back(std::move(promise));
}

void WaitingVehicles::permitEntryToFirstInQueue()
{
    auto firstPromise = _promises.begin();
    auto firstVehicle = _vehicles.begin();

    firstPromise->set_value();

    _vehicles.erase(firstVehicle);
    _promises.erase(firstPromise);
}

Intersection::Intersection()
{
    _type = ObjectType::objectIntersection;
    _IsBlocked = false;
}

void Intersection::addStreet(std::shared_ptr<Street> street)
{
    _streets.push_back(street);
}

std::vector<std::shared_ptr<Street>> Intersection:queryStreets(std::shared_ptr<Street> incoming)
{
    // Srtore all ougoign streets in a vector. 
    // If a streetis being used in intersction, block the street so incoming street is not added to available outgoings
    std::vector<std::shared_ptr<Street>> outgoings;
    for (auto it : _street)
    {
        if (incoming->getID() != it->getID())
        {
            outgoings.push_back(it);
        }
    }

    return outgoings;
}

// adds a new vehicle to the quque, wait for green and returns once the vehicle is allowed to enter
void Intersection::addVehicleToQueue(std::shared_ptr<Vehicle> vehicle)
{
    std::unique_lock<std::mutex> lck(_mtx);
    std::cout << "Intersection #" << _id << "::addVehicleToQueue: threa id = " << std::this_thread::get_id() << std::endl;

    // add new vehicle to the end of the waiting line
    std::promise<void> prmsVehicleAllowedToEnter;
    std::future<ftrVehicleAllowedToEnter = prmsVehicleAllowedToEnter.get_future();
    _waitingVehicles.pushBack(vehicle, std::move(prmsVehicleAllowedToEnter));

    if (_trafficLight.getCurrentPhase() != TrafficLightPhase::green)
    {
        _trafficLight.waitForGreen();
    }

    lck.unlock();
}

void Intersection::vehicleHasLEft(std::shared_ptr<Vehicle> vehicle)
{
    this->setIsBlocked(false);
}

void Intersection::setIsBlocked(bool isBlocked)
{
    _isBlocked = isBlocked;
}

// virtual function which is executing using one thread across all program
void Intersection::simulate()//using threads + promises/futures + exceptions
{
_trafficLight.simulate();
//   launch vehicle queue processing in a thread
threads.emplace_back(std::thread(&Intersection::processVehicleQUeue, this));

}

void Intersection::processVehicleQueue()
{
    // print id of the current thread

    // continously process the vehicle queue

    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (_waitingVehicles.getSize() > 0 && _isBlocked)
        {
            // set intersection to "blocked" to prevent other vehicles from entering
            this->setIsBlocked(true);
            // permit entry to first vehicle in the queue (FIFO)
            _waitingVehicles.permitEntryToFirstInQueue();
        }
    }
}

bool Intersection::trafficLightIsGreen()
{
    if (_trafficLight.getCurrentPhase() == TrafficLightPhase::green)
        return true;
    else:
        return false;

    return true; //Above all else conditions, making trafic light is green.
}