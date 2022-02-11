/**
 * server/src/hardware/output/outputcontroller.cpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2021-2022 Reinder Feenstra
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

#include "outputcontroller.hpp"
#include "output.hpp"
#include "keyboard/outputkeyboard.hpp"
#include "../../utils/inrange.hpp"

bool OutputController::isOutputChannel(uint32_t channel) const
{
  const auto* channels = outputChannels();
  if(!channels || channels->empty())
    return channel == defaultOutputChannel;

  auto it = std::find(channels->begin(), channels->end(), channel);
  assert(it == channels->end() || *it != defaultOutputChannel);
  return it != channels->end();
}

bool OutputController::isOutputAddressAvailable(uint32_t channel, uint32_t address) const
{
  assert(isOutputChannel(channel));
  return
    inRange(address, outputAddressMinMax(channel)) &&
    m_outputs.find({channel, address}) == m_outputs.end();
}

uint32_t OutputController::getUnusedOutputAddress(uint32_t channel) const
{
  assert(isOutputChannel(channel));
  const auto end = m_outputs.cend();
  const auto range = outputAddressMinMax(channel);
  for(uint32_t address = range.first; address < range.second; address++)
    if(m_outputs.find({channel, address}) == end)
      return address;
  return Output::invalidAddress;
}

bool OutputController::changeOutputChannelAddress(Output& output, uint32_t newChannel, uint32_t newAddress)
{
  assert(output.interface.value().get() == this);
  assert(isOutputChannel(newChannel));

  if(!isOutputAddressAvailable(newChannel, newAddress))
    return false;

  auto node = m_outputs.extract({output.channel, output.address});
  node.key() = {newChannel, newAddress};
  m_outputs.insert(std::move(node));
  output.value.setValueInternal(TriState::Undefined);

  return true;
}

bool OutputController::addOutput(Output& output)
{
  if(isOutputChannel(output.channel) && isOutputAddressAvailable(output.channel, output.address))
  {
    m_outputs.insert({{output.channel, output.address}, output.shared_ptr<Output>()});
    output.value.setValueInternal(TriState::Undefined);
    return true;
  }
  return false;
}

bool OutputController::removeOutput(Output& output)
{
  assert(output.interface.value().get() == this);
  auto it = m_outputs.find({output.channel, output.address});
  if(it != m_outputs.end() && it->second.get() == &output)
  {
    m_outputs.erase(it);
    output.value.setValueInternal(TriState::Undefined);
    return true;
  }
  return false;
}

void OutputController::updateOutputValue(uint32_t channel, uint32_t address, TriState value)
{
  if(auto it = m_outputs.find({channel, address}); it != m_outputs.end())
    it->second->updateValue(value);
  if(auto keyboard = m_outputKeyboards[channel].lock())
    keyboard->outputValueChanged(*keyboard, address, value);
}

std::shared_ptr<OutputKeyboard> OutputController::outputKeyboard(uint32_t channel)
{
  assert(isOutputChannel(channel));
  auto keyboard = m_outputKeyboards[channel].lock();
  if(!keyboard)
  {
    keyboard = std::make_shared<OutputKeyboard>(*this, channel);
    m_outputKeyboards[channel] = keyboard;
  }
  return keyboard;
}
