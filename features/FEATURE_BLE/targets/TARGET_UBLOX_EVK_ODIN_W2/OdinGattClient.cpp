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

#include "ble/GattClient.h"
#include "OdinGap.h"
#include "OdinGattClient.h"
#include "mbed.h"

namespace ble {
namespace vendor {
namespace odin {

GattClient &GattClient::getInstance()
{
    static GattClient m_instance;
    return m_instance;
}

GattClient::GattClient()
{
    // TODO, empty?
}
    
    
ble_error_t GattClient::launchServiceDiscovery(Gap::Handle_t connectionHandle,
        ServiceDiscovery::ServiceCallback_t         sc,
        ServiceDiscovery::CharacteristicCallback_t  cc,
        const UUID                                 &matchingServiceUUID,
        const UUID                                 &matchingCharacteristicUUIDIn) {
    // TODO
    return BLE_ERROR_NONE;
}

void GattClient::onServiceDiscoveryTermination(ServiceDiscovery::TerminationCallback_t callback) {
    // TODO
}

bool GattClient::isServiceDiscoveryActive(void) const {
    // TODO
    return true;
}

void GattClient::terminateServiceDiscovery(void) {
    // TODO
}

ble_error_t GattClient::discoverCharacteristicDescriptors(
        const DiscoveredCharacteristic& characteristic,
        const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
        const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback) {
    // TODO
    return BLE_ERROR_NONE;
}

bool GattClient::isCharacteristicDescriptorsDiscoveryActive(const DiscoveredCharacteristic& characteristic) const {
    // TODO
    return true;
}
    
void GattClient::terminateCharacteristicDescriptorsDiscovery(const DiscoveredCharacteristic& characteristic) {
    // TODO
}

ble_error_t GattClient::read(Gap::Handle_t connHandle, GattAttribute::Handle_t attributeHandle, uint16_t offset) const {
    // TODO
    return BLE_ERROR_NONE;
}
    
ble_error_t GattClient::write(GattClient::WriteOp_t cmd, Gap::Handle_t connHandle, GattAttribute::Handle_t attributeHandle, size_t length, const uint8_t *value) const {
    // TODO
    return BLE_ERROR_NONE;
}
    
ble_error_t GattClient::reset(void) {
    // TODO
    return BLE_ERROR_NONE;
}


} // namespace odin
} // namespace vendor
} // namespace ble
