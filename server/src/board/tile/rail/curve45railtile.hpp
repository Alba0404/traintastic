/**
 * server/src/board/tile/rail/curve45railtile.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2020,2022 Reinder Feenstra
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TRAINTASTIC_SERVER_BOARD_TILE_RAIL_CURVE45RAILTILE_HPP
#define TRAINTASTIC_SERVER_BOARD_TILE_RAIL_CURVE45RAILTILE_HPP

#include "railtile.hpp"

class Curve45RailTile : public RailTile
{
  CLASS_ID("board_tile.rail.curve_45")
  CREATE(Curve45RailTile)

  public:
    Curve45RailTile(World& world, std::string_view _id);

    void getConnectors(std::vector<Connector>& connectors) const final;
};

#endif
