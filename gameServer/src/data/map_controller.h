#ifndef gameServer_data_map_controllerh
#define gameServer_data_map_controllerh

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "src/util/binary.h"
#include "src/data/map.h"

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
            m_startID = startID;
            m_endID = endID;

            mainThreadHandler = std::thread(mainThread, this);

            return true; // TODO
        }

        bool split() {
            mapID_t numMaps = m_endID - m_startID + 1;
            mapID_t maxSquare = util::binary::getMSB32(numMaps);
            mapID_t newEnd;

            if (numMaps == maxSquare) {
                // no heading or trailing non-regular maps
                newEnd = m_startID + numMaps / 2 - 1;
            } else {
                mapID_t uniformedMapOffset = (maxSquare - (m_startID & (maxSquare - 1))) & (maxSquare - 1);
                uniformedMapOffset += (uniformedMapOffset == 0) * maxSquare;
                newEnd = m_startID + uniformedMapOffset - 1;
            }

            int numFirst = 0, numSecond = 0;

            for (int i = 0; i < m_maps.size(); i++) {
                numFirst += m_maps[i].mapID <= newEnd;
                numSecond += m_maps[i].mapID > newEnd;
            }

            map_controller* newController = new map_controller();
            std::vector<data::m_maphandle_t> newMaps(numFirst);
            newController->m_maps.resize(numSecond);

            for (int i = 0; i < m_maps.size(); i++) {
                if (m_maps[i].mapID <= newEnd) {
                    newMaps[numFirst] = m_maps[i];
                    numFirst++;
                } else {
                    newController->m_maps[numSecond] = m_maps[i];
                    numSecond++;
                }
            }

            m_maps = newMaps;


            //TODO finish newController

            m_endID = newEnd;
        }

        void load(mapID_t mapID) {
            data::m_maphandle_t link;
            link.map = nullptr; //TODO
            link.mapID = mapID;
            m_maps.push_back(link);
        }


        void printDebug() {
            std::vector<std::string> output;

            mapID_t start = m_startID - ((util::binary::getMSB32(m_endID - m_startID + 1) - 1) & m_startID);
            mapID_t end = m_endID - ((util::binary::getMSB32(m_endID - m_startID + 1) - 1) & m_endID) + util::binary::getMSB32(m_endID - m_startID + 1);

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

                if (i >= m_startID && i <= m_endID) {
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

        std::thread m_mainThreadHandler;

        mapID_t m_startID;
        mapID_t m_endID;
        std::vector<data::m_maphandle_t> m_maps;
    };
}

#endif

