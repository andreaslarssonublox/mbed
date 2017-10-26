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

#include "mbed.h"
#include "OdinGap.h"
#include "OdinGattServer.h"

/**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
#define BLE_GAP_ADV_INTERVAL_MIN        0x0020 // TODO check

/**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 // TODO check

/**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */
#define BLE_GAP_ADV_INTERVAL_MAX        0x4000 // TODO check

namespace ble {
namespace vendor {
namespace odin {

Gap &Gap::getInstance()
{
    static Gap m_instance;
    return m_instance;
}

void Gap::initialize()
{
}

ble_error_t Gap::setAddress(AddressType_t type, const Address_t address)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::getAddress(AddressType_t *typeP, Address_t address)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::setAdvertisingData(const GapAdvertisingData &advData, const GapAdvertisingData &scanResponse)
{
    /* Make sure we don't exceed the advertising payload length */
    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* Make sure we have a payload! */
    if (advData.getPayloadLen() == 0) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    // TODO
    
    return BLE_ERROR_NONE;
}

ble_error_t Gap::connect(
    const BLEProtocol::AddressBytes_t peerAddr,
    BLEProtocol::AddressType_t peerAddrType,
    const ConnectionParams_t* connectionParams,
    const GapScanningParams* scanParams
) {
    // prepare the scan interval
    if (scanParams != NULL) {
        // TODO
    }

    if (connectionParams != NULL) {
        // TODO
    }

    // TODO connect
    return BLE_ERROR_NONE;
}

uint16_t Gap::getMinAdvertisingInterval() const
{
    return BLE_GAP_ADV_INTERVAL_MIN;
}

uint16_t Gap::getMinNonConnectableAdvertisingInterval() const
{
    return BLE_GAP_ADV_NONCON_INTERVAL_MIN;
}

uint16_t Gap::getMaxAdvertisingInterval() const
{
    return BLE_GAP_ADV_INTERVAL_MAX;
}

ble_error_t Gap::startAdvertising(const GapAdvertisingParams &params)
{
    /* Make sure we support the advertising type */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) {
        // TODO
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* Check interval range */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED) {
        /* Min delay is slightly longer for unconnectable devices */
        if ((params.getIntervalInADVUnits() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) ||
            (params.getIntervalInADVUnits() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    } else {
        if ((params.getIntervalInADVUnits() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN) ||
            (params.getIntervalInADVUnits() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    }

    /* Check timeout is zero for Connectable Directed */
    if ((params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) && (params.getTimeout() != 0)) {
        /* Timeout must be 0 with this type, although we'll never get here */
        /* since this isn't implemented yet anyway */
        // TODO
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Check timeout for other advertising types */
    if ((params.getAdvertisingType() != GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) &&
        (params.getTimeout() > GapAdvertisingParams::GAP_ADV_PARAMS_TIMEOUT_MAX)) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    // TODO check this behavior from cordio
    uint16_t adv_interval_min = params.getIntervalInADVUnits();
    if (adv_interval_min == GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX) {
        --adv_interval_min;
    }
    uint16_t adv_interval_max = adv_interval_min + 1;

    // TODO start advertising
    
    return BLE_ERROR_NONE;
}

ble_error_t Gap::stopAdvertising(void)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::disconnect(DisconnectionReason_t reason)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::setDeviceName(const uint8_t *deviceName)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::getDeviceName(uint8_t *deviceName, unsigned *lengthP)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::setAppearance(GapAdvertisingData::Appearance appearance)
{
    //GattServer::getInstance().setAppearance(appearance);
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::getAppearance(GapAdvertisingData::Appearance *appearanceP)
{
    //*appearanceP = GattServer::getInstance().getAppearance();
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setTxPower(int8_t txPower)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

void Gap::getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP)
{
    *valueArrayPP = NULL;
    *countP = 0;
}

void Gap::setConnectionHandle(uint16_t connectionHandle)
{
    // TODO how is this used?
    m_connectionHandle = connectionHandle;
}

uint16_t Gap::getConnectionHandle(void)
{
    // TODO how is this used?
    return m_connectionHandle;
}

ble_error_t Gap::getPreferredConnectionParams(ConnectionParams_t *params)
{
    //*params = GattServer::getInstance().getPreferredConnectionParams();
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::setPreferredConnectionParams(const ConnectionParams_t *params)
{
    // ensure that parameters are correct
    // see BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part C]
    // section 12.3 PERIPHERAL PREFERRED CONNECTION PARAMETERS CHARACTERISTIC
 /*
        if (((0x0006 > params->minConnectionInterval) || (params->minConnectionInterval > 0x0C80)) &&
        params->minConnectionInterval != 0xFFFF) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (((params->minConnectionInterval > params->maxConnectionInterval) || (params->maxConnectionInterval > 0x0C80)) &&
        params->maxConnectionInterval != 0xFFFF) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (params->slaveLatency > 0x01F3) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (((0x000A > params->connectionSupervisionTimeout) || (params->connectionSupervisionTimeout > 0x0C80)) &&
        params->connectionSupervisionTimeout != 0xFFFF) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    GattServer::getInstance().setPreferredConnectionParams(*params);*/
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::updateConnectionParams(Handle_t handle, const ConnectionParams_t *newParams)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::startRadioScan(const GapScanningParams &scanningParams)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::stopScan(void)
{
    // TODO
    return BLE_ERROR_NONE;
}

ble_error_t Gap::reset(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

Gap::Gap() :
    ::Gap(),
    m_connectionHandle(0), // TODO invalid connection handle here?
    m_type(BLEProtocol::AddressType::PUBLIC),
    m_addr()
{
}

} // namespace odin
} // namespace vendor
} // namespace ble
