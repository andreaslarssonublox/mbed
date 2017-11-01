/* Odin BLE implementation
 * Copyright (c) 2017 u-blox Malm� AB
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

#ifndef ODIN_GAP_H_
#define ODIN_GAP_H_

#include <stdint.h>
#include "ble/blecommon.h"
#include "ble/GapAdvertisingParams.h"
#include "ble/GapAdvertisingData.h"
#include "ble/Gap.h"
#include "ble/GapScanningParams.h"
#include "EventFlags.h"

#include "cb_bt_conn_man.h"

namespace ble {
namespace vendor {
namespace odin {

/**
 * @see ::Gap
 */
class Gap : public ::Gap
{
public:
    /**
     * Return the Gap singleton implementing ::Gap for the Odin stack.
     */
    static Gap &getInstance();

    /**
     * This function shall be called once the stack has been initialized
     */
    void initialize();

    /**
     * @see ::Gap::setAddress
     */
    virtual ble_error_t setAddress(AddressType_t  type,  const Address_t address);

    /**
     * @see ::Gap::getAddress
     */
    virtual ble_error_t getAddress(AddressType_t *typeP, Address_t address);

    /**
     * @see ::Gap::setAdvertisingData
     */
    virtual ble_error_t setAdvertisingData(
        const GapAdvertisingData&, const GapAdvertisingData&
    );

    /**
     * @see ::Gap::connect
     */
    virtual ble_error_t connect(
        const BLEProtocol::AddressBytes_t  peerAddr,
        BLEProtocol::AddressType_t peerAddrType,
        const ConnectionParams_t* connectionParams,
        const GapScanningParams* scanParams
    );

    /**
     * @see ::Gap::getMinAdvertisingInterval
     */
    virtual uint16_t getMinAdvertisingInterval() const;

    /**
     * @see ::Gap::getMinNonConnectableAdvertisingInterval
     */
    virtual uint16_t getMinNonConnectableAdvertisingInterval() const;

    /**
     * @see ::Gap::getMaxAdvertisingInterval
     */
    virtual uint16_t getMaxAdvertisingInterval() const;

    /**
     * @see ::Gap::startAdvertising
     */
    virtual ble_error_t startAdvertising(const GapAdvertisingParams &);

    /**
     * @see ::Gap::stopAdvertising
     */
    virtual ble_error_t stopAdvertising();

    /**
     * @see ::Gap::disconnect
     */
    virtual ble_error_t disconnect(
        Handle_t connectionHandle,
        DisconnectionReason_t reason
    );

    /**
     * @see ::Gap::disconnect
     */
    virtual ble_error_t disconnect(DisconnectionReason_t reason);

    /**
     * @see ::Gap::setDeviceName
     */
    virtual ble_error_t setDeviceName(const uint8_t *deviceName);

    /**
     * @see ::Gap::getDeviceName
     */
    virtual ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP);

    /**
     * @see ::Gap::setAppearance
     */
    virtual ble_error_t setAppearance(GapAdvertisingData::Appearance appearance);

    /**
     * @see ::Gap::getAppearance
     */
    virtual ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP);

    /**
     * @see ::Gap::setTxPower
     */
    virtual ble_error_t setTxPower(int8_t txPower);

    /**
     * @see ::Gap::getPermittedTxPowerValues
     */
    virtual void getPermittedTxPowerValues(
        const int8_t **valueArrayPP, size_t *countP
    );

    /**
     * @see ::Gap::getPreferredConnectionParams
     */
    virtual ble_error_t getPreferredConnectionParams(ConnectionParams_t *params);

    /**
     * @see ::Gap::setPreferredConnectionParams
     */
    virtual ble_error_t setPreferredConnectionParams(
        const ConnectionParams_t *params
    );

    /**
     * @see ::Gap::updateConnectionParams
     */
    virtual ble_error_t updateConnectionParams(
        Handle_t handle, const ConnectionParams_t *params
    );

    /**
     * @see ::Gap::startRadioScan
     */
    virtual ble_error_t startRadioScan(const GapScanningParams &scanningParams);

    /**
     * @see ::Gap::stopScan
     */
    virtual ble_error_t stopScan();

    /**
     * @see ::Gap::reset
     */
    virtual ble_error_t reset(void);

private:
    
    enum ConnectState {
        S_CONN_IDLE               = 1,
        S_CONN_CONNECTING,
        S_CONN_CONNECTING_FOREVER
    };

    Gap();

    Gap(Gap const &);
    void operator=(Gap const &);

    friend void handle_bcm_connect_ind(cbBCM_Handle handle, cbBCM_ConnectionInfo info);
    friend void handle_bcm_connect_evt(cbBCM_Handle handle, cbBCM_ConnectionInfo info);
    friend void schedule_connect_acl_le(cbBCM_Handle bcm_handle, cbBCM_ConnectionInfo info, cb_int32 status);
    friend void schedule_bcm_disconnect_evt(cbBCM_Handle handle);
    cbBCM_ConnectionCallback _conn_callback;
    
    friend void handle_bm_dev_disc_evt(TBdAddr *addr, cb_int8 rssi, cb_char *name, TAdvData *adv_data);
    friend void handle_bm_dev_disc_complete_evt(cb_int32 status);
    
    addr_type_t _type;
    Address_t _addr;
    
    cbBCM_ConnectionParametersLe _ongoing_conn_params;
    cbBCM_Handle _ongoing_conn_bcm_handle; // TODO needed?
    ConnectState _connect_state;
    rtos::EventFlags _scan_completed_event;
};

} // namespace odin
} // namespace vendor
} // namespace ble

#endif /* ODIN_GAP_H_ */
