#ifndef gameServer_data_map_controller_h
#define gameServer_data_map_controller_h

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "include/util/binary.h"
#include "include/data/map.h"

namespace data {


    class map_controller {
    public:


        static void mainThread(map_controller* controller) {
            if (controller != nullptr) {

                //TODO add main logic

                delete controller;
            }
        }
        
        bool init(const mapID_t startID, const mapID_t endID) {
            startID_ = startID;
            endID_ = endID;
            
            mainThreadHandler = std::thread(mainThread, this);

            return true; // TODO
        }

        bool split() {
            mapID_t numMaps = endID_ - startID_ + 1;
            mapID_t maxSquare = util::binary::getMSB32(numMaps);
            mapID_t newEnd;

            if (numMaps == maxSquare) {
                // no heading or trailing non-regular maps
                newEnd = startID_ + numMaps / 2 - 1;
            } else {
                mapID_t uniformedMapOffset = (maxSquare - (startID_ & (maxSquare - 1))) & (maxSquare - 1);
                uniformedMapOffset += (uniformedMapOffset == 0) * maxSquare;
                newEnd = startID_ + uniformedMapOffset - 1;
            }

            int numFirst = 0, numSecond = 0;

            for (int i = 0; i < maps_.size(); i++) {
                numFirst += maps_[i].mapID <= newEnd;
                numSecond += maps_[i].mapID > newEnd;
            }

            map_controller* newController = new map_controller();
            std::vector<data::map_handle_t> newMaps(numFirst);
            newController->maps_.resize(numSecond);

            for (int i = 0; i < maps_.size(); i++) {
                if (maps_[i].mapID <= newEnd) {
                    newMaps[numFirst] = maps_[i];
                    numFirst++;
                } else {
                    newController->maps_[numSecond] = maps_[i];
                    numSecond++;
                }
            }

            maps_ = newMaps;


            //TODO finish newController

            endID_ = newEnd;
        }

        void load(mapID_t mapID) {
            data::map_handle_t link;
            link.map = nullptr; //TODO
            link.mapID = mapID;
            maps_.push_back(link);
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

    protected:

        std::thread mainThreadHandler;
        
        mapID_t startID_;
        mapID_t endID_;
        std::vector<data::map_handle_t> maps_;
    };
}

#endif

