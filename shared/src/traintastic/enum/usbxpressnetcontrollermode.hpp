/**
 * shared/src/enum/usbxpressnetcontrollermode.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019-2020 Reinder Feenstra
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

#ifndef TRAINTASTIC_SHARED_TRAINTASTIC_ENUM_USBXPRESSNETCONROLLERMODE_HPP
#define TRAINTASTIC_SHARED_TRAINTASTIC_ENUM_USBXPRESSNETCONROLLERMODE_HPP

#include <cstdint>
#include "enum.hpp"
#include <frozen/map.h>

enum class USBXpressNetControllerMode : uint8_t
{
  Direct = 0,
  Virtual = 1,
};

ENUM_NAME(USBXpressNetControllerMode, "usb_xpressnet_controller_mode")

ENUM_VALUES(USBXpressNetControllerMode, 2,
{
  {USBXpressNetControllerMode::Direct, "direct"},
  {USBXpressNetControllerMode::Virtual, "virtual"},
})

#endif