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
#include "cb_bt_man.h"
#include "cb_bt_conn_man.h"
#include "bt_types.h"
#include <string.h>


/**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
#define BLE_GAP_ADV_INTERVAL_MIN        0x0020 // TODO check

/**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 // TODO check

/**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */
#define BLE_GAP_ADV_INTERVAL_MAX        0x4000 // TODO check

namespace ble {
namespace vendor {
namespace odin {

    BLEProtocol::AddressType_t convert_odin_addr_type(TBdAddr *addr)
    {
        if (addr->AddrType == BT_PUBLIC_ADDRESS) return BLEProtocol::AddressType::PUBLIC;
        if (BD_ADDR_IS_NON_RESOLVABLE(addr->BdAddress)) return BLEProtocol::AddressType::RANDOM_PRIVATE_NON_RESOLVABLE;
        if (BD_ADDR_IS_RESOLVABLE(addr->BdAddress)) return BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE;
        return BLEProtocol::AddressType::RANDOM_STATIC;
    }
    
    void copy_from_odin_addr(BLEProtocol::AddressBytes_t addr, TBdAddr *odin_addr)
    {
        memcpy(addr, odin_addr->BdAddress, SIZE_OF_BD_ADDR);
    }
    
// Friend declared C-functions that calls corresponding Gap object member function
    struct c_callb_s {    

        static void handle_bcm_connect_ind(
                cbBCM_Handle handle,
            cbBCM_ConnectionInfo info)
        {
            Gap& gap = Gap::getInstance();
        };

        static void handle_bcm_connect_evt(
                cbBCM_Handle handle,
            cbBCM_ConnectionInfo info)
        {
            Gap& gap = Gap::getInstance();
        };

        static void handle_bcm_connect_cnf(
                cbBCM_Handle bcm_handle,
            cbBCM_ConnectionInfo info,
            cb_int32 status)
        {
            //MBED_ASSERT(info.type == cbBCM_ACL_LE_CONNECTION);
            
            Gap& gap = Gap::getInstance();
            if (status == cbBCM_OK) {
                Gap::Handle_t               handle = (Gap::Handle_t)bcm_handle;
                Gap::Role_t                 role = Gap::CENTRAL;
                BLEProtocol::AddressType_t  peer_addr_type = convert_odin_addr_type(&info.address);
                BLEProtocol::AddressBytes_t peer_addr;
                Gap::ConnectionParams_t     connection_params;
                connection_params.connectionSupervisionTimeout = gap._ongoing_conn_timeout;
                connection_params.maxConnectionInterval = gap._ongoing_conn_int_max;
                connection_params.minConnectionInterval = gap._ongoing_conn_int_min;
                connection_params.slaveLatency = gap._ongoing_conn_latency;
            
                copy_from_odin_addr(peer_addr, &info.address);
                gap.processConnectionEvent(handle, role, peer_addr_type, peer_addr, gap._type, gap._addr, &connection_params);
            }
            else {
                gap.processTimeoutEvent(Gap::TIMEOUT_SRC_CONN);
            }
        };

        static void handle_bcm_disconnect_evt(
                cbBCM_Handle handle)
        {
            Gap& gap = Gap::getInstance();
            gap.processDisconnectionEvent(handle, (Gap::DisconnectionReason_t)0x1f); // TODO pass HCI error code all the way instead see cb_bt_conn_man.c and cb_llc.c
        };
    };

    static cbBCM_ConnectionCallback _bcm_connection_cb = 
    {
        c_callb_s::handle_bcm_connect_ind,
        c_callb_s::handle_bcm_connect_evt,
        c_callb_s::handle_bcm_connect_cnf,
        c_callb_s::handle_bcm_disconnect_evt
    };

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
    MBED_ASSERT(typeP != NULL);
    
    TBdAddr odin_addr;
    cb_int32 result = cbBM_getLocalAddress(&odin_addr);
    MBED_ASSERT(result == cbBM_OK);
    copy_from_odin_addr(address, &odin_addr);
    *typeP = BLEProtocol::AddressType::PUBLIC; // This is the only local address type supported for now
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
    const GapScanningParams* scanParamsIn)
{
    cbBCM_ConnectionParametersLe conn_params;
    cb_int32 result;
    cb_int32 interval;
    cb_int32 window;
    
    if (connectionParams != NULL) {
        conn_params.connectionIntervalMin = connectionParams->minConnectionInterval;
        conn_params.connectionIntervalMax = connectionParams->maxConnectionInterval;
        conn_params.connectionLatency     = connectionParams->slaveLatency;
        conn_params.linkLossTimeout  = connectionParams->connectionSupervisionTimeout*10;
    }
    else {
        conn_params.connectionIntervalMin = 50;  // Use NRF52 default value
        conn_params.connectionIntervalMax = 100; // Use NRF52 default value
        conn_params.connectionLatency     = 0;   // Use NRF52 default value
        conn_params.linkLossTimeout       = 600; // Use NRF52 default value
    }
	
    if (scanParamsIn != NULL) {
        interval = scanParamsIn->getInterval();
        window = scanParamsIn->getWindow();
        conn_params.createConnectionTimeout = scanParamsIn->getTimeout()*1000;
    }
    else {
        interval = _scanningParams.getInterval();
        window = _scanningParams.getWindow();
        conn_params.createConnectionTimeout = _scanningParams.getTimeout()*1000;
    }

    result = cbBM_setConnectScanWindow(window); // This might fail, don't return error
    
    result = cbBM_setConnectScanInterval(interval);
    if (result != cbBM_OK) {
        return BLE_ERROR_INVALID_PARAM;
    }

    result = cbBM_setConnectScanWindow(window);
    if (result != cbBM_OK) {
        return BLE_ERROR_INVALID_PARAM;
    }
    
    // TODO handle timeout == 0 i.e. scan forever

    // Connect
    TBdAddr address;
    memcpy(&address.BdAddress, peerAddr, SIZE_OF_BD_ADDR);
    if (peerAddrType == BLEProtocol::AddressType::PUBLIC) {
        address.AddrType = BT_PUBLIC_ADDRESS;
    }
    else {
        address.AddrType = BT_RANDOM_ADDRESS;
    }
    _bcm_handle = cbBCM_reqConnectAclLe(&address, &conn_params, &_bcm_connection_cb);
    if (_bcm_handle == cbBCM_INVALID_CONNECTION_HANDLE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }
    _ongoing_conn_int_max = conn_params.connectionIntervalMax;
    _ongoing_conn_int_min = conn_params.connectionIntervalMin;
    _ongoing_conn_latency = conn_params.connectionLatency;
    _ongoing_conn_timeout = conn_params.linkLossTimeout / 10;
    
    // TODO Set state or send message to thread?
    
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
    (void)reason; // The ODIN C API does not support passing a reason
    cb_int32 ret_value = cbBCM_cmdDisconnect((cbBCM_Handle)connectionHandle);
    if (ret_value != cbBCM_OK)
    {
        return BLE_ERROR_UNSPECIFIED;
    }
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
    _connectionHandle = connectionHandle;
}

uint16_t Gap::getConnectionHandle(void)
{
    // TODO how is this used?
    return _connectionHandle;
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
    _connectionHandle(0) // TODO invalid connection handle here?
{
    ble_error_t status = getAddress(&_type, _addr);
    MBED_ASSERT(status == BLE_ERROR_NONE);
}

} // namespace odin
} // namespace vendor
} // namespace ble
