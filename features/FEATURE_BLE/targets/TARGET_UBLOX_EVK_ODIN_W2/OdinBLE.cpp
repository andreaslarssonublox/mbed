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

namespace ble {
namespace vendor {
namespace odin {

void handle_controller_startup_complete_stack(void)
{
    BLE& ble = BLE::deviceInstance();
    ble.handle_controller_startup_complete();
}
    
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

ble_error_t BLE::init(FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> initCallback,
        struct BLE::InitParams_t params)
{   
    switch (initialization_status) {
    case NOT_INITIALIZED:
        {
            cbMAIN_BtInitParams initParams;
            _init_callback = initCallback;

            if (params.role == Gap::CENTRAL) {
                initParams.leRole = cbBM_LE_ROLE_CENTRAL;
            } else {
                initParams.leRole = cbBM_LE_ROLE_PERIPHERAL;
            }
            cbBT_UTILS_setInvalidBdAddr(&initParams.address);
            initParams.maxOutputPower = params.max_output_pwr;
            initParams.maxLinkKeysLe = params.max_linkkeys;
            initParams.maxLinkKeysClassic = params.max_linkkeys; // TODO we need to document this or? Should 0 be used instead?
    
            //cbMAIN_driverLock(); // TODO should we add protection in cbMAIN instead?
            cbMAIN_initBt(&initParams, handle_controller_startup_complete_stack);
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

ble_error_t BLE::init(
    ::BLE::InstanceID_t instanceID,
    FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext *> initCallback)
{

    switch (initialization_status) {
        case NOT_INITIALIZED:
        {
            cb_int32 result;
            _init_callback = initCallback;

            cbMAIN_BtInitParams initParams;
            cbBT_UTILS_setInvalidBdAddr(&initParams.address);
            initParams.leRole = cbBM_LE_ROLE_CENTRAL;
            initParams.maxOutputPower = cbBM_MAX_OUTPUT_POWER;
            initParams.maxLinkKeysClassic = 25;
            initParams.maxLinkKeysLe = 25;
    
            //cbMAIN_driverLock(); // TODO should we add protection in cbMAIN instead?
            cbMAIN_initBt(&initParams, handle_controller_startup_complete_stack);
            //cbMAIN_driverUnlock();
            cbMAIN_startOS(); // TODO fix startOS so it can only be called once
            result = cbBCM_setMaxLinksLE(7);
            
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
    return cbBM_getStackVersionString();
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

void BLE::handle_controller_startup_complete()
{
    ::BLE::InitializationCompleteCallbackContext context = {
        ::BLE::Instance(::BLE::DEFAULT_INSTANCE),
        BLE_ERROR_NONE
    };
    
    _init_callback.call(&context);
}
    
FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> BLE::_init_callback;

} // namespace odin
} // namespace vendor
} // namespace ble
