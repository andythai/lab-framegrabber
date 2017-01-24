// FrameGrabber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;


int main()
{
	cout << "Starting FrameGrabber program..." << endl;

	// Get video file location
	cout << "Input video file: ";
	std::string video_filename;
	getline(cin, video_filename);
	std::cout << std::endl;

	// Get .csv file location
	cout << "Input .csv file: ";
	std::string csv_filename;
	getline(cin, csv_filename);
	std::cout << std::endl;

	cout << "Loading " << video_filename << "..." << endl;

	cout << "Loading " << csv_filename << "..." << endl;


    return 0;
}

