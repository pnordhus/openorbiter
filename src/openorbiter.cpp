/***************************************************************************
 *   Copyright (C) 2004 by Philipp Nordhus                                 *
 *   philipp.nordhus@web.de                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <iostream>
#include <stdlib.h>
#include "SDL.h"
#include <GL/glu.h>

#include "input.h"
#include "match.h"
#include "renderer.h"
#include "timer/Timer.h"


using namespace Mercury;


static unsigned int res_x = 800;
static unsigned int res_y = 600;
static bool         fullscreen = false;



void printUsage(const char *file)
{
        cerr << endl;
        cerr << "Usage: " << file << " [options]" << endl;
        cerr << endl;
        cerr << "Options:" << endl;
        cerr << "  --fullscreen      run in fullscreen mode" << endl;
        cerr << "  --window          run in window mode (default)" << endl;
        cerr << "  --resolution      specify resolution" << endl;
        cerr << endl;
        cerr << "Example: " << file << " --resolution=800x600" << endl;
        cerr << endl;

        exit(EXIT_FAILURE);
}


void getResolution(int c, char *argv[])
{
        int pos = 1;

        while (pos < c) {
                if (strncmp(argv[pos], "--resolution=", 13) == 0) {
                        if (sscanf(&argv[pos][13], "%ux%u", &res_x, &res_y) != 2)
                                printUsage(argv[0]);
                } else if (strcmp(argv[pos], "--fullscreen") == 0) {
                        fullscreen = true;
                } else if (strcmp(argv[pos], "--window") == 0) {
                        fullscreen = false;
                } else {
                        printUsage(argv[0]);
                }
                ++pos;
        }	
}



int main(int argc, char *argv[])
{
	srand((unsigned int) time(NULL));
	
	Map map_5nodes(0.0f, 0.0f, 4000.0f, 2500.0f);
	
	map_5nodes.addDeathZone(DeathZone(0.0f, 0.0f, 4000.0f, 150.0f));
	map_5nodes.addDeathZone(DeathZone(0.0f, 150.0f, 150.0f, 2500.0f));
	map_5nodes.addDeathZone(DeathZone(0.0f, 2500.0f, 4000.0f, 150.0f));
	map_5nodes.addDeathZone(DeathZone(3850.0f, 150.0f, 150.0f, 2500.0f));
	
	map_5nodes.addNode(Node(2000.0f, 1250.0f));
	map_5nodes.addNode(Node(1000.0f, 750.0f));
	map_5nodes.addNode(Node(3000.0f, 750.0f));
	map_5nodes.addNode(Node(1000.0f, 1750.0f));
	map_5nodes.addNode(Node(3000.0f, 1750.0f));
	
	Map map_slow(0.0f, 0.0f, 4000.0f, 2500.0f);
	
	map_slow.addDeathZone(DeathZone(0.0f, 0.0f, 4000.0f, 150.0f));
	map_slow.addDeathZone(DeathZone(0.0f, 150.0f, 150.0f, 2500.0f));
	map_slow.addDeathZone(DeathZone(0.0f, 2500.0f, 4000.0f, 150.0f));
	map_slow.addDeathZone(DeathZone(3850.0f, 150.0f, 150.0f, 2500.0f));
	
	map_slow.addNode(Node(2000.0f, 1000.0f));
	map_slow.addNode(Node(2500.0f, 1500.0f));
	map_slow.addNode(Node(1500.0f, 1500.0f));

	
	Map map_slow2(0.0f, 0.0f, 4000.0f, 2500.0f);
	
	map_slow2.addDeathZone(DeathZone(0.0f, 0.0f, 4000.0f, 150.0f));
	map_slow2.addDeathZone(DeathZone(0.0f, 150.0f, 150.0f, 2500.0f));
	map_slow2.addDeathZone(DeathZone(0.0f, 2500.0f, 4000.0f, 150.0f));
	map_slow2.addDeathZone(DeathZone(3850.0f, 150.0f, 150.0f, 2500.0f));
	
	map_slow2.addNode(Node(1000.0f, 2500.0 / 3.0));
	//map_slow2.addNode(Node(2000.0f, 2500.0 / 3.0));
	map_slow2.addNode(Node(3000.0f, 2500.0 / 3.0));
	
	map_slow2.addNode(Node(1500.0f, 2500.0 * 2.0 / 3.0));
	map_slow2.addNode(Node(2500.0f, 2500.0 * 2.0 / 3.0));
	
	Solid sol1;
	sol1.addVertex(1500.0f, 2450.0f);
	sol1.addVertex(1500.0f, 2500.0f);
	sol1.addVertex(2500.0f, 2450.0f);
	sol1.addVertex(2500.0f, 2500.0f);
	
	map_5nodes.addSolid(sol1);
	
	//map_5nodes.addStart(500.0f, 200.0f);
	map_5nodes.addStart(1100.0f, 200.0f);
	map_5nodes.addStart(1700.0f, 200.0f);
	map_5nodes.addStart(2300.0f, 200.0f);
	map_5nodes.addStart(2900.0f, 200.0f);
	//map_5nodes.addStart(3500.0f, 200.0f);

	map_5nodes.addStart(800.0f, 600.0f);
	map_5nodes.addStart(1400.0f, 600.0f);
	//map_5nodes.addStart(2000.0f, 600.0f);
	map_5nodes.addStart(2600.0f, 600.0f);
	map_5nodes.addStart(3200.0f, 600.0f);
	
	
	
	map_slow.addStart(1100.0f, 200.0f);
	map_slow.addStart(1700.0f, 200.0f);
	map_slow.addStart(2300.0f, 200.0f);
	map_slow.addStart(2900.0f, 200.0f);
	//map_5nodes.addStart(3500.0f, 200.0f);

	map_slow.addStart(800.0f, 600.0f);
	map_slow.addStart(1400.0f, 600.0f);
	//map_5nodes.addStart(2000.0f, 600.0f);
	map_slow.addStart(2600.0f, 600.0f);
	map_slow.addStart(3200.0f, 600.0f);
	
	
	map_slow2.addStart(1100.0f, 200.0f);
	map_slow2.addStart(1700.0f, 200.0f);
	map_slow2.addStart(2300.0f, 200.0f);
	map_slow2.addStart(2900.0f, 200.0f);
	//map_5nodes.addStart(3500.0f, 200.0f);

	map_slow2.addStart(800.0f, 600.0f);
	map_slow2.addStart(1400.0f, 600.0f);
	//map_5nodes.addStart(2000.0f, 600.0f);
	map_slow2.addStart(2600.0f, 600.0f);
	map_slow2.addStart(3200.0f, 600.0f);
	
	
	//Match::setMap(map_5nodes);
	Match::setMap(map_slow);

//	Match::addPlayer(Player(Color(1.0f, 0.0f, 0.0f), SDLK_RIGHT));
	//
	Match::addPlayer(Player(Color(0.0f, 1.0f, 0.0f), SDLK_1));
	Match::addPlayer(Player(Color(0.0f, 0.0f, 1.0f), SDLK_v));
	
	Match::addPlayer(Player(Color(1.0f, 1.0f, 0.0f), SDLK_m));
	Match::addPlayer(Player(Color(0.0f, 1.0f, 1.0f), SDLK_HOME));
	Match::addPlayer(Player(Color(1.0f, 0.0f, 1.0f), SDLK_RIGHT));

	getResolution(argc, argv);

	try {	
		if (!Renderer::init(res_x, res_y, 0, fullscreen))
			return EXIT_FAILURE;
	
		Match::reset();
		while (Input::query()) {
			Match::process();
			Renderer::endFrame();
			Timer::update(50.0);
			Renderer::beginFrame();
			Match::render();
		}
	} catch (char const* c) {
		cerr << "Exception: " << c << endl;
	}
	
	return EXIT_SUCCESS;
}
