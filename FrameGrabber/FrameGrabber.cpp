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
const string TEST_MP4 = "input/CSD2122sub-111615.mp4";
const string TEST_CSV = "input/CSD2122sub-111615.csv";

// Variables
vector<Interval> intervals; // Container for intervals
bool csv_load_first_try = true; // Prevents premature closing

// Forward declarations
static void load_video();
static void load_csv();

// TODO: Main method
int main()
{
	cout << "Starting FrameGrabber program..." << endl;

	// Load files
	load_video();
	load_csv();

	// Exit behavior
	cout << "Press ENTER to exit program." << endl;
	cin.ignore();
	if (!csv_load_first_try) {
		cin.ignore();
	}
    return 0;
}

// TODO: Loads mp4 file
static void load_video()
{
	// Get video file location
	cout << "Input video file: ";
	std::string video_filename;
	getline(cin, video_filename);

	// Default test file if none specified
	if (video_filename == "") {
		video_filename = TEST_MP4;
	}

	// TODO: Loading video
	cout << "Loading " << video_filename << "..." << endl << endl;
}

// Loads csv file
static void load_csv()
{
	// Get .csv file location
	cout << "Input .csv file: ";
	std::string csv_filename;
	getline(cin, csv_filename);

	// Default test file if none specified
	if (csv_filename == "") {
		csv_filename = TEST_CSV;
	}

	// Loading csv
	ifstream in_stream(csv_filename, ios::binary);

	// Check if stream opens a valid file
	while (in_stream.fail())
	{
		csv_load_first_try = false;
		cout << "Could not open file " << csv_filename << " to read.\n"; // if the open file fails.
		cout << "Please re-enter the csv file name: ";
		getline(cin, csv_filename);
		if (csv_filename == "") {
			csv_filename = TEST_CSV;
		}
		in_stream.close();
		in_stream.open(csv_filename, ios::binary);
	}

	cout << "Loading " << csv_filename << "..." << endl;

	// Skip first line
	string line;
	getline(in_stream, line);

	// Read rest of .csv
	while (getline(in_stream, line)) {

		// Look for the three keywords
		if (line.find(AIR_ACTIVE) != string::npos ||
			line.find(AIR_PASSIVE) != string::npos ||
			line.find(INTERIM) != string::npos) {

			// Find start time
			string delimiter = ",";
			int start_find = line.find(delimiter);
			string start_time = line.substr(0, start_find);

			// Find end time
			string delimiter2 = ",,,";
			int end_find = line.find(delimiter2);
			string end_time = line.substr(start_find + 1, end_find - 8);

			// Find label / name
			string name = line.substr(end_find + 3, line.length() - 1);

			// Create interval
			Interval current_interval = Interval(name, start_time, end_time);
			intervals.push_back(current_interval);
		}
	}

	cout << "Amount of valid csv intervals: " << intervals.size() << endl << endl;
	in_stream.close();
}