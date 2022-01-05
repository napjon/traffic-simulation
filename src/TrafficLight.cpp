#include <iostream>
#include <random>
#include "TrafficLight.h"

template <typename T>
T MessageQueue<T>::receive()
{
    // method should use unique_locks and wait for and receive message
    // and pull from queue using move semantics

    std::unique_lock<std::mutex> lck(_mutex);
    _condition.wait(lk, [this] {return !_queue.empty();});

    T v = std::move(_queue.back());

    std::cout << "receiving" << std::endl;
    _queue.pop_back();

    return v; // will not be compied due RVO in c++
}

template <typename T>
void MessageQueue<T>::send(T && msg)
{
    // send will mutex lock the queue until it send
    // condition.notify_one() will add a new message and send notifications
    std::lock_guard<std::mutex> lck(_mutex);
    _queue.push_back(std::move(msg));
    _condition.notify_one();
}

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    std::unique_lock<std::mutex> lck(_mutex);
    while (true)
    {
        std::cout << "Waiting for green" << std::endl;
        // Sleeping to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        auto _trafficLight = _trafficLightQueue.receive();
        if (_trafficLight == TrafficLightPhase::green)
        {
            return;

        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // Finally simulate should be used in the same thread to do simulation
    // Use th thread from the base class
    // threads queue belong to TrafficObject base class, to run all of
    // TrafficObject simulation
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));

}

void TrafficLight::cycleThroughPhases()
{
    // Cycle method of trafficlight, where the green and red should toggles 4-6 seconds
    // For each toggle, it will send traffic status to the MessageQueue.
    // Sleep for 1 seconds for each toggle, and send the traffic status message
    // using move semantics
    double cycleDuration = 1;
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    lastUpdate = std::chrono::system_clock::now;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        std::random_device rd;
        std::mt19937 eng(rd);

        std::uniform_int_distribution<> distr(4,6);

        auto cycle = distr(eng) * 1000;
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate);
        if (timeSinceLastUpdate >= cycle) {
            if (_currentPhase != TrafficLightPhase::red)
            {
                _trafficLightQueue.send(std::move(TrafficLightPhase::red));
                _currentPhase = TrafficLightPhase::red;
            } else {
                _trafficLightQueue.send(std::move(TrafficLightPhase::green));
                _currentPhase = TrafficLightPhase::green:
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            lastUpdate = std::chrono::system_clock::now();
        }

        



    }
}