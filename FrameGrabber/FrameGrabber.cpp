// FrameGrabber.cpp : Defines the entry point for the console application.
// Andy Thai

#include "stdafx.h"
#include "Interval.h"

using namespace std;

// Constant variables
const string AIR_ACTIVE = "Air Active";
const string AIR_PASSIVE = "Air Passive";
const string INTERIM = "Interim";
const bool DEBUG = true;

// Variables
int num_intervals = 0; // Number of intervals read from .csv file
vector<Interval> intervals;

int main()
{
	cout << "Starting FrameGrabber program..." << endl;

	// Get video file location
	cout << "Input video file: ";
	std::string video_filename;
	getline(cin, video_filename);
	std::cout << std::endl;
	// Default test file
	if (video_filename == "") {
		video_filename = "input/CSD2122sub-111615.mp4";
	}

	// Get .csv file location
	cout << "Input .csv file: ";
	std::string csv_filename;
	getline(cin, csv_filename);
	std::cout << std::endl;
	// Default test file
	if (csv_filename == "") {
		csv_filename = "input/CSD2122sub-111615.csv";
	}

	// TODO: Loading video
	cout << "Loading " << video_filename << "..." << endl;

	// Loading csv
	cout << "Loading " << csv_filename << "..." << endl;
	ifstream in_stream(csv_filename, ios::binary);

	// Check if stream opens a valid file
	if (in_stream.fail())
	{
		cout << "Could not open file to read.\n"; // if the open file fails.
		cout << "Press ENTER to exit the program." << endl;
		cin.ignore();
		return 1;
	}

	// Skip first line
	string line;
	getline(in_stream, line);

	// Read rest of .csv
	while (getline(in_stream, line)) {
		// Look for the three keywords
		if (line.find(AIR_ACTIVE) != string::npos || 
			line.find(AIR_PASSIVE) != string::npos || 
			line.find(INTERIM) != string::npos) {

			num_intervals++;

			// Find start time
			string delimiter = ",";
			int start_find = line.find(delimiter);
			string start_time = line.substr(0, start_find);
			if (DEBUG) {
				cout << start_time << endl;
			}

			// Find end time
			string delimiter2 = ",,,";
			int end_find = line.find(delimiter2);
			string end_time = line.substr(start_find + 1, end_find - 8);
			if (DEBUG) {
				cout << end_time << endl;
			}

			// Find label / name
			string name = line.substr(end_find + 3, line.length() - 1);
			if (DEBUG) {
				cout << name << endl;
			}

			// Create interval
			Interval current_interval = Interval(name, start_time, end_time);
			intervals.push_back(current_interval);

			if (DEBUG) {
				cout << line << endl << endl;
			}
		}
	}
	cout << "Number of valid intervals: " << num_intervals << endl;

	if (DEBUG) {
		for (int i = 0; i < intervals.size(); i++) {
			cout << intervals[i].getName() << endl;
		}
	}

	in_stream.close();
	cin.ignore();
    return 0;
}

