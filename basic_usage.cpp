#include <algorithm>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "loader.h"

using json = nlohmann_loader::json;
using namespace visuser;

const std::string USAGE =
    "./mini_vistool <config.json> [options]\n"
    "Options:\n"
    "  -h                   Print this help.";

int main(int argc, char **argv)
{
    if (argc < 2) std::cout << USAGE<< std::endl;

    // get input from command line
    std::vector<std::string> args(argv, argv + argc);
    std::string config_str;
    for (int i = 1; i < argc; ++i) {
        if (args[i] == "-h") {
            std::cout << USAGE << "\n";
            return 0;
        } else {
            config_str = args[1];
        }
    }
    
    // load key frames into the loader data structure
    AniObjHandler h(config_str.c_str());

    // loop through key frames
    for (int kf_idx=0; kf_idx<h.widgets.size(); kf_idx++){
	std::cout << "On key frame ["<< kf_idx <<"] \n";
	// load data
	std::cout <<"Loading volume "<<h.widgets[kf_idx].file_name
		  <<" with dim "<<h.widgets[kf_idx].dims[0]
		  <<" "<<h.widgets[kf_idx].dims[1]
		  <<" "<<h.widgets[kf_idx].dims[2]<<"\n";
	std::fstream file;
	file.open(h.widgets[kf_idx].file_name, std::fstream::in | std::fstream::binary);
	if (!file) {
                std::cerr << "[error]: Failed to open data file "
			  << h.widgets[kf_idx].file_name << "\n";
                throw std::runtime_error("Failed to open input data file");
            }
	else{
	    for (int z = 0; z < h.widgets[kf_idx].dims[2]; ++z)
		for (int y = 0; y < h.widgets[kf_idx].dims[1]; ++y)
		    for (int x = 0; x < h.widgets[kf_idx].dims[0]; ++x)
			{
			    float buff;
			    file.read((char*)(&buff), sizeof(buff));
			    // TODO: read into your own data structure
		    
			}
	}
	file.close();
	std::cout<< "End loading volume\n";
	
	// get transfer function
	std::vector<float> colors;
	std::vector<float> opacities;
	h.widgets[kf_idx].getFrameTF(colors, opacities);
	std::cout << "Load tf size="<< colors.size()/3 << "\n";

	// get current frame by key frame
    	for (int f = h.widgets[kf_idx].frameRange[0]; f <= h.widgets[kf_idx].frameRange[1]; f++){
	    if (f < h.widgets[kf_idx].frameRange[1]){
		// advance frame 
		h.widgets[kf_idx].advanceFrame();
		std::cout << "frame: "<< f <<"\n";

		// get current camera
		auto c = Camera();
		h.widgets[kf_idx].getFrameCam(c); 
		c.print();

		// TODO render

		// TODO save image
	    }
	}

	std::cout << "\n\n";
    }
    return 0;
}
