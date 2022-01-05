#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>

#include "TrafficObject.h"

class Vehicle;

enum TrafficLightPhase
{
    red,
    green,
    yellow
};

template <class T>
class MessageQueue
{
    public:

    T receive();
    void send(T &&message);

    private:
    std::condition_variable _condition;
    std::mutex _mutex;
    std::deque<T> _queue;

};

class TrafficLight: public TrafficObject
{
    public:
    TrafficLight();

    void waitForGreen();
    void simulate();

    TrafficLightPhase getCurrentPhase();

private:
    void cycleThroughPhases();

    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _trafficLightQueue;

};

#endif

