/* Odin BLE implementation
 * Copyright (c) 2017 u-blox Malmö AB
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "OdinGattServer.h"
#include "OdinGap.h"
#include "mbed.h"

namespace ble {
namespace vendor {
namespace odin {

GattServer &GattServer::getInstance()
{
    static GattServer m_instance;
    return m_instance;
}

void GattServer::initialize()
{
    // TODO add GAP service
    // TODO add GATT service
}

ble_error_t GattServer::addService(GattService &service)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t GattServer::read(GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t * lengthP)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t GattServer::read(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t *lengthP)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t GattServer::write(GattAttribute::Handle_t attributeHandle, const uint8_t buffer[], uint16_t len, bool localOnly)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t GattServer::write(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, const uint8_t buffer[], uint16_t len, bool localOnly)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t GattServer::areUpdatesEnabled(const GattCharacteristic &characteristic, bool *enabledP)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t GattServer::areUpdatesEnabled(Gap::Handle_t connectionHandle, const GattCharacteristic &characteristic, bool *enabledP)
{
    // TODO
    return BLE_ERROR_NONE;
}

bool GattServer::isOnDataReadAvailable() const
{
    // TODO
    return true;
}

ble_error_t GattServer::reset(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

GattServer::GattServer() :
    ::GattServer()
{
}

} // namespace odin
} // namespace vendor
} // namespace ble
