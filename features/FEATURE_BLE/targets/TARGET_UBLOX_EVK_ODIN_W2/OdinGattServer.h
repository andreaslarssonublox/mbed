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

#ifndef ODIN_GATT_SERVER_H_
#define ODIN_GATT_SERVER_H_

#include <stddef.h>
#include "ble/blecommon.h"
#include "ble/GattServer.h"
#include "ble/Gap.h"

namespace ble {
namespace vendor {
namespace odin {

/**
 * Odin implementation of ::GattServer
 */
class GattServer : public ::GattServer
{
public:
    /**
     * Return the singleton of the Odin implementation of ::GattServer.
     */
    static GattServer &getInstance();

    /**
     * Initialize the GattServer and add mandatory services (generic access and
     * generic attribute service).
     */
    void initialize();

    /**
     * @see ::GattServer::addService
     */
    virtual ble_error_t addService(GattService &);

    /**
     * @see ::GattServer::read
     */
    virtual ble_error_t read(
        GattAttribute::Handle_t attributeHandle,
        uint8_t buffer[],
        uint16_t *lengthP
    );

    /**
     * @see ::GattServer::read
     */
    virtual ble_error_t read(
        Gap::Handle_t connectionHandle,
        GattAttribute::Handle_t attributeHandle,
        uint8_t buffer[], uint16_t *lengthP
    );

    /**
     * @see ::GattServer::write
     */
    virtual ble_error_t write(
        GattAttribute::Handle_t,
        const uint8_t[], uint16_t,
        bool localOnly = false
    );

    /**
     * @see ::GattServer::write
     */
    virtual ble_error_t write(
        Gap::Handle_t connectionHandle,
        GattAttribute::Handle_t,
        const uint8_t[],
        uint16_t,
        bool localOnly = false
    );

    /**
     * @see ::GattServer::areUpdatesEnabled
     */
    virtual ble_error_t areUpdatesEnabled(
        const GattCharacteristic &characteristic, bool *enabledP
    );

    /**
     * @see ::GattServer::areUpdatesEnabled
     */
    virtual ble_error_t areUpdatesEnabled(
        Gap::Handle_t connectionHandle,
        const GattCharacteristic &characteristic,
        bool *enabledP
    );

    /**
     * @see ::GattServer::isOnDataReadAvailable
     */
    virtual bool isOnDataReadAvailable() const;


    /**
     * @see ::GattServer::reset
     */
    virtual ble_error_t reset(void);

private:
    GattServer();

    GattServer(const GattServer &);
    const GattServer& operator=(const GattServer &);
};

} // namespace odin
} // namespace vendor
} // namespace ble

#endif /* ODIN_GATT_SERVER_H_ */
