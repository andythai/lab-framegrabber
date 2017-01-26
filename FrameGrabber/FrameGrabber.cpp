// FrameGrabber.cpp : Defines the entry point for the console application.
// Andy Thai
// Dependencies:
//		QT
//		OpenCV

#include "stdafx.h"
#include "Interval.h"
#define USE_FFMPEG

using namespace std;
using namespace cv;

// Constant variables
const string AIR_ACTIVE = "Air Active";
const string AIR_PASSIVE = "Air Passive";
const string INTERIM = "Interim";
const bool DEBUG = false;
const string TEST_MP4 = "input/CSD2122sub-111615.mp4";
const string TEST_CSV = "input/CSD2122sub-111615.csv";

// Variables
vector<Interval> intervals;		// Container for intervals
VideoCapture video;				// Loads the video

// Forward declarations
static void load_video();
static void load_csv();
static void filter();

// TODO: Main method
int main()
{
	cout << "FrameGrabber\nAuthor: Andy Thai" << endl;
	cout << "\nUsing OpenCV version " << CV_VERSION << endl;
	if (DEBUG) {
		cout << getBuildInformation();
	}

	// Load files
	load_video();
	load_csv();

	// Filter prompt
	filter();
	if (intervals.size() == 0) {
		cout << "\nERROR: No intervals to grab frames from!" << endl;
		cout << "Press ENTER to close program." << endl;
		cin.ignore();
		return 1;
	}

	// Print controls
	cout << "\nThis program will grab a frame from the previously loaded video." << endl << 
		"CONTROLS:" << endl << 
		"\ta: Accept" << endl << 
		"\t\tThe program will accept the frame and save it in the output folder." << 
		"\n\tr: Reject" << endl << 
		"\t\tThe program will reject the frame and will select another one." << endl <<
		"\tS: Skip" << endl << 
		"\t\tThe program will skip the current interval and go to the next one." << endl <<
		"\t\tNOTE: To skip, you must press capital S (Shift + S)." << endl;

	// TEST
	Mat image = Mat();
	// Make resized image copy for view window size
	Mat small_image = Mat();
	Size small_image_size = Size(720, 540); // Original ratio: 1440 x 1080
	video.read(image);
	resize(image, small_image, small_image_size);

	namedWindow("Test frame", WINDOW_AUTOSIZE);
	imshow("Test frame", small_image);

	// Check for valid key input
	while (1) {
		char key_pressed = waitKey(0);
		if (key_pressed == 'a' || key_pressed == 'r' || key_pressed == 'S') {
			break;
		}
	}
	string write_image = "output/frame.jpg";
	cout << "Closing window" << endl;
	destroyAllWindows();
	
	
	//string jpg_extension = ".jpg";
	/*
	for (int i = 0; i < 10; i++) { // Need QT framework
		video.read(image);
		Mat small = Mat();
		Size small_size(640, 480);
		resize(image, small, small_size);
		namedWindow("Frame " + to_string(i), WINDOW_NORMAL);
		//resizeWindow("Frame" + to_string(i), 600, 600); //720 540
		imshow("Frame " + to_string(i), small);
		waitKey(0);
		imwrite(write_image + to_string(i) + jpg_extension, image);
	}
	*/

	// Exit behavior
	cout << "\nPress ENTER to exit program." << endl;
	cin.ignore();
    return 0;
}

// Loads mp4 file
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

	// Initialize and load video into VideoCapture class
	video = VideoCapture();
	video.set(CV_CAP_PROP_FOURCC, CV_FOURCC('H', '2', '6', '4'));
	video.open(video_filename, CAP_FFMPEG);

	// Check if stream opens a valid file
	while (!video.isOpened())
	{
		//csv_load_first_try = false;
		cout << "Could not open file " << video_filename << " to read.\n"; // if the open file fails.
		cout << "Please re-enter the video file name: ";
		getline(cin, video_filename);
		if (video_filename == "") {
			video_filename = TEST_MP4;
		}
		video.release();
		video.open(video_filename, CAP_FFMPEG);
	}
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
			int start_find = (int)line.find(delimiter);
			string start_time_string = line.substr(0, start_find);
			// Pull data from start time string
			int start_time_minutes = stoi(start_time_string.substr(0, 2));
			int start_time_seconds = stoi(start_time_string.substr(3, 5));
			double start_time_milliseconds_to_seconds = stod("0." + start_time_string.substr(6, 7));
			double start_time = start_time_minutes * 60 + start_time_seconds + start_time_milliseconds_to_seconds;

			// Find end time
			string delimiter2 = ",,,";
			int end_find = (int)line.find(delimiter2);
			string end_time_string = line.substr(start_find + 1, end_find - 8);
			// Pull data from end time string
			int end_time_minutes = stoi(end_time_string.substr(0, 2));
			int end_time_seconds = stoi(end_time_string.substr(3, 5));
			double end_time_milliseconds_to_seconds = stod("0." + end_time_string.substr(6, 7));
			double end_time = end_time_minutes * 60 + end_time_seconds + end_time_milliseconds_to_seconds;

			// Find label / name
			string name = line.substr(end_find + 3, line.length() - 1);

			// Create interval
			Interval current_interval = Interval(name, start_time, end_time);
			intervals.push_back(current_interval);
		}
	}

	cout << "Total amount of csv intervals: " << intervals.size() << endl << endl;
	in_stream.close();
}

static void filter() {
	cout << "You have the option to enable a filter. Doing so will remove\n" <<
		"all intervals less than the specified value. Enable filter? (y/n): ";

	// Variables
	string filter_input_string;			// User input
	bool filter_on;						// Determines if user picked filter on or off
	bool valid_input = false;			// Loop user input in case invalid answer

	// Get user input
	while (!valid_input) {
		getline(cin, filter_input_string);
		if (filter_input_string == "yes" || filter_input_string == "y") {
			filter_on = true;
			valid_input = true;
		}
		else if (filter_input_string == "no" || filter_input_string == "n") {
			filter_on = false;
			return;
		}
		else {
			valid_input = false;
			cout << "Invalid input. Please try again. (y/n): ";
		}
	}

	// Ask for filter threshold
	cout << "\nFilter activated. All intervals less than the filter in seconds will be\n"
		<< "pruned from the list and ignored. Please input the filter threshold: ";
	string threshold_string;
	double threshold = 0;
	bool filter_valid_input = false;

	// Loop check for validity in input
	while (!filter_valid_input) {
		getline(cin, threshold_string);
		// Check if input is a number
		if (!threshold_string.empty() && threshold_string.find_first_not_of("0123456789.") == string::npos) {
			cout << "\nPruning all intervals less than " << threshold_string << " seconds." << endl;
			filter_valid_input = true;
		}
		else {
			cout << "Invalid input. Please input a number: ";
			filter_valid_input = false;
		}
	}
	threshold = stod(threshold_string);


	// Prune intervals
	int num_pruned = 0;
	vector<Interval>::iterator it;
	for (it = intervals.begin(); it != intervals.end();) {
		
		if (it->getLength() < threshold) {
			num_pruned++;
			it = intervals.erase(it);
		}
		else {
			++it;
		}
		
	}

	for (int i = 0; i < intervals.size(); i++) {
		cout << intervals[i].getLength() << endl;
	}

	cout << "Pruned " << num_pruned << " intervals." << endl;
	cout << "Total amount of csv intervals: " << intervals.size() << endl;
	return;
}