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
#include "BLE.h"
#include "OdinBLE.h"
#include "OdinGap.h"
#include "OdinGattServer.h"
#include "OdinGattClient.h"
#include "OdinSecurityManager.h"
#include "mbed_assert.h"

#include "ublox-odin-w2-drivers/bt_types.h"
#include "ublox-odin-w2-drivers/cb_bt_utils.h"
#include "ublox-odin-w2-drivers/cb_bt_man.h"
#include "ublox-odin-w2-drivers/cb_bt_conn_man.h"
#include "ublox-odin-w2-drivers/cb_main.h"

BLEInstanceBase *createBLEInstance()
{
    return (&(ble::vendor::odin::BLE::deviceInstance()));
}

static void controllerStartupComplete(void)
{
    
}

namespace ble {
namespace vendor {
namespace odin {

BLE::BLE() :
    initialization_status(NOT_INITIALIZED),
    instanceID(::BLE::DEFAULT_INSTANCE)
{
    // Nothing more to do
}

BLE::~BLE() { }

/**
 * The singleton which represents the BLE transport for the BLE.
 */
BLE& BLE::deviceInstance()
{
    static BLE instance;
    return instance;
}

ble_error_t BLE::init(
    ::BLE::InstanceID_t instanceID,
    FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext *> initCallback)
{

    switch (initialization_status) {
        case NOT_INITIALIZED:
        {
            _init_callback = initCallback;

            cbMAIN_BtInitParams initParams;
            cbBT_UTILS_setInvalidBdAddr(&initParams.address);
            initParams.leRole = cbBM_LE_ROLE_CENTRAL;
            initParams.maxOutputPower = cbBM_MAX_OUTPUT_POWER;
            initParams.maxLinkKeysClassic = 25;
            initParams.maxLinkKeysLe = 25;
    
            //cbMAIN_driverLock(); // TODO should we add protection in cbMAIN instead?
            cbMAIN_initBt(&initParams, controllerStartupComplete);
            //cbMAIN_driverUnlock();
            cbMAIN_startOS(); // TODO fix startOS so it can only be called once
         }
         return BLE_ERROR_NONE;

        case INITIALIZING:
            return BLE_ERROR_INITIALIZATION_INCOMPLETE;

        case INITIALIZED:
            return BLE_ERROR_NONE;

        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

bool BLE::hasInitialized() const
{
    return initialization_status == INITIALIZED;
}

ble_error_t BLE::shutdown()
{
    if (initialization_status != INITIALIZED) {
        return BLE_ERROR_INITIALIZATION_INCOMPLETE;
    }

    return BLE_ERROR_NOT_IMPLEMENTED;
}

const char* BLE::getVersion()
{
    //static const char version[] = "generic-cordio";
    // TODO
    return "Not implemented";
}

Gap& BLE::getGap()
{
    return odin::Gap::getInstance();
}

const Gap& BLE::getGap() const
{
    return odin::Gap::getInstance();
}

GattServer& BLE::getGattServer()
{
    return odin::GattServer::getInstance();
}

const GattServer& BLE::getGattServer() const
{
    return odin::GattServer::getInstance();
}

GattClient& BLE::getGattClient()
{
    return odin::GattClient::getInstance();
}

SecurityManager& BLE::getSecurityManager()
{
    return odin::SecurityManager::getInstance();
}

const SecurityManager& BLE::getSecurityManager() const
{
    return odin::SecurityManager::getInstance();
}

void BLE::waitForEvent()
{
    // Do nothing, handled in the driver thread
}

void BLE::processEvents()
{
    // Do nothing, handled in the driver thread
 }

FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> BLE::_init_callback;

} // namespace odin
} // namespace vendor
} // namespace ble
