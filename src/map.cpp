/***************************************************************************
 *   Copyright (C) 2008 Philipp Nordhus                                    *
 *   pnordhus@users.sourceforge.net                                        *
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


#include "bouncer.h"
#include "map.h"
#include "mapdef.h"
#include "node.h"
#include "scene.h"
#include "physics/world.h"


Map::Map(const MapDef& def, Scene& scene) :
	m_scene(scene)
{
	m_world = new World;
	
	const float width = def.width();
	const float height = def.height();
	
	m_rect.setWidth(width);
	m_rect.setHeight(height);
	
	m_scene.setField(width, height);
	
	m_world->setGravity(def.gravity());
	
	foreach (const Vector& pos, def.nodes()) {
		Node* node = new Node(m_scene);
		node->setPosition(pos);
		m_nodes.append(node);
	}
	
	foreach (const BouncerDef& def, def.bouncers()) {
		Bouncer* bouncer = new Bouncer(m_scene, m_world);
		bouncer->setDef(def);
		m_bouncers.append(bouncer);
	}
}


Map::~Map()
{
	qDeleteAll(m_bouncers);
	qDeleteAll(m_nodes);
	
	delete m_world;
}
