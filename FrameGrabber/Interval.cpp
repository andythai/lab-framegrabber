#include "Interval.h"

// No-arg constructor
Interval::Interval()
{
}

// One-arg constructor with name
Interval::Interval(string intervalName)
{
	name = intervalName;
}

// Three-arg constructor for full data
Interval::Interval(string intervalName, string intervalStart, string intervalEnd)
{
	name = intervalName;
	startTime = intervalStart;
	endTime = intervalEnd;
	calculateElapsed();
}

// Destructor
Interval::~Interval()
{
}

// Getter method for starting time
string Interval::getStartTime()
{
	return startTime;
}

// Getter method for ending time
string Interval::getEndTime()
{
	return endTime;
}

// Getter method for interval length
string Interval::getElapsedTime()
{
	return elapsedTime;
}

// Getter method for name
string Interval::getName()
{
	return name;
}

// Setter method for start time
void Interval::setStartTime(string newStartTime)
{
	startTime = newStartTime;
	calculateElapsed();
}

// Setter method for end time
void Interval::setEndTime(string newEndTime)
{
	endTime = newEndTime;
	calculateElapsed();
}

// Setter method for both times
void Interval::setTimes(string newStartTime, string newEndTime)
{
	startTime = newStartTime;
	endTime = newEndTime;
	calculateElapsed();
}

// Setter method for name
void Interval::setName(string newName)
{
	name = newName;
}

// TODO: Setter method for interval length
void Interval::calculateElapsed()
{

}