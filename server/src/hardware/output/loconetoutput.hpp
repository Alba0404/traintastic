/**
 * server/src/hardware/output/loconetoutput.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019-2021 Reinder Feenstra
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

#ifndef TRAINTASTIC_SERVER_HARDWARE_OUTPUT_LOCONETOUTPUT_HPP
#define TRAINTASTIC_SERVER_HARDWARE_OUTPUT_LOCONETOUTPUT_HPP

#include "output.hpp"
#include "../../core/objectproperty.hpp"
#include "../protocol/loconet/loconet.hpp"

class LocoNetOutput : public Output
{
  friend class LocoNet::LocoNet;

  protected:
    void loaded() final;
    void destroying() final;
    void worldEvent(WorldState state, WorldEvent event) final;
    void idChanged(const std::string& newId) final;
    void valueChanged(TriState _value) final;
    bool setValue(TriState& _value) final;

    inline void updateValue(TriState _value) { Output::updateValue(_value); }

  public:
    CLASS_ID("output.loconet")
    CREATE(LocoNetOutput)

    static constexpr uint16_t addressInvalid = 0;
    static constexpr uint16_t addressMin = 1;
    static constexpr uint16_t addressMax = 4096;

    ObjectProperty<LocoNet::LocoNet> loconet;
    Property<uint16_t> address;

    LocoNetOutput(const std::weak_ptr<World> world, std::string_view _id);
};

#endif
