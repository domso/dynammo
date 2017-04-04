#ifndef gameServer_data_map_controller_h
#define gameServer_data_map_controller_h

#include "include/util/binary.h"

#include <iostream>
#include <vector>
#include <string>

typedef uint32_t mapID_t;


class map_controller {
public:
    bool init(const mapID_t startID, const mapID_t endID) {
        startID_ = startID;
        endID_ = endID;

        return true; // TODO
    }


    bool split() {
        mapID_t numMaps = endID_ - startID_ + 1;
        mapID_t maxSquare = util::binary::getMSB32(numMaps);

        if (numMaps == maxSquare) {
            // no heading or trailing non-regular maps

            endID_ = startID_ + numMaps / 2 - 1;


        } else {
           
            mapID_t uniformedMapOffset = (maxSquare - (startID_ & (maxSquare - 1))) & (maxSquare - 1);
            
            uniformedMapOffset += (uniformedMapOffset == 0) * maxSquare;

            endID_ = startID_ + uniformedMapOffset - 1;
        }



    }


    void printDebug() {
        std::vector<std::string> output;

        mapID_t start = startID_ - ((util::binary::getMSB32(endID_ - startID_ + 1) - 1) & startID_);
        mapID_t end = endID_ - ((util::binary::getMSB32(endID_ - startID_ + 1) - 1) & endID_) + util::binary::getMSB32(endID_ - startID_ + 1);

        output.resize((end - start));


        for (mapID_t i = start; i != end; i++) {
            int id = 0;

            if ((i & 0xA) == 0) {
                id = 0;
            }

            if ((i & 0xA) == 2) {
                id = 1;
            }

            if ((i & 0xA) == 8) {
                id = 2;
            }

            if ((i & 0xA) == 10) {
                id = 3;
            }

            std::string tmp = std::to_string(i);

            if (tmp.length() != 2) {
                tmp = " " + tmp;
            }

            if (i >= startID_ && i <= endID_) {
                 output[id] += tmp + "x|";
            } else {
                 output[id] += tmp + " |";
            }

        }

        for (int i = 0; i < output.size(); i++) {
            std::cout << output[i] << std::endl;
        }
    }


    mapID_t startID_;
    mapID_t endID_;
};


#endif

