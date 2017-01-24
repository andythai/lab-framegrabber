#pragma once

#include "stdafx.h"
using namespace std;

// Interval to hold data and intervals to grab frames from in the video
class Interval
{
public:
	// Constructors
	Interval();
	Interval(string intervalName);
	Interval(string intervalName, string intervalStart, string intervalEnd);
	~Interval();

	// Getter methods
	string getStartTime();
	string getEndTime();
	string getElapsedTime();
	string getName();

	// Setter methods
	void setStartTime(string newStartTime);
	void setEndTime(string newEndTime);
	void setTimes(string newStartTime, string newEndTime);
	void setName(string newName);

	// Calculations
	void calculateElapsed();

private:
	// Variables
	string name;
	string startTime;
	string endTime;
	string elapsedTime;
};