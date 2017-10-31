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

#ifndef ODIN_BLE_H_
#define ODIN_BLE_H_

#include "ble/BLE.h"
#include "ble/blecommon.h"
#include "ble/BLEInstanceBase.h"
#include "OdinBLE.h"
#include "OdinGap.h"
#include "OdinGattServer.h"
#include "OdinGattClient.h"
#include "OdinSecurityManager.h"


//#include "OdinGap.h"
//#include "OdinGattServer.h"
//#include "OdinSecurityManager.h"
//#include "OdinPalAttClient.h"
//#include "ble/pal/AttClientToGattClientAdapter.h"
//#include "ble/generic/GenericGattClient.h"

namespace ble {
namespace vendor {
namespace odin {

/**
 * @see BLEInstanceBase
 */
class BLE : public ::BLEInstanceBase {

    BLE();

    /**
     * Destructor
     */
    virtual ~BLE();

public:

    static const uint8_t BLE_MAX_OUTPUT_PWR = 127;
    
    struct InitParams_t
    {
        Gap::Role_t role;
        uint32_t max_output_pwr;
        uint32_t max_linkkeys;
    };
    
/**
 * Access to the singleton containing the implementation of BLEInstanceBase
 * for the Odin stack.
 */
static BLE& deviceInstance();

    /**
    * @see BLEInstanceBase::init
    */
    virtual ble_error_t init(
        ::BLE::InstanceID_t instanceID,
        FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> initCallback
    );

    /**
    * @ Initialize with parameters
    */
    ble_error_t init(FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> initCallback,
        struct InitParams_t params);

    /**
     * @see BLEInstanceBase::hasInitialized
     */
    virtual bool hasInitialized() const;

    /**
     * @see BLEInstanceBase::shutdown
     */
    virtual ble_error_t shutdown();

    /**
     * @see BLEInstanceBase::getVersion
     */
    virtual const char *getVersion();

    /**
     * @see BLEInstanceBase::getGap
     */
    virtual Gap& getGap();

    /**
     * @see BLEInstanceBase::getGap
     */
    virtual const Gap& getGap() const;

    /**
     * @see BLEInstanceBase::getGattServer
     */
    virtual GattServer &getGattServer();

    /**
     * @see BLEInstanceBase::getGattServer
     */
    virtual const GattServer &getGattServer() const;

    /**
     * @see BLEInstanceBase::getGattClient
     */
    virtual GattClient &getGattClient();

    /**
     * @see BLEInstanceBase::getSecurityManager
     */
    virtual SecurityManager &getSecurityManager();

    /**
     * @see BLEInstanceBase::getSecurityManager
     */
    virtual const SecurityManager &getSecurityManager() const;

    /**
     * @see BLEInstanceBase::waitForEvent
     */
    virtual void waitForEvent();

    /**
     * @see BLEInstanceBase::processEvents
     */
    virtual void processEvents();

private:

    friend void handle_controller_startup_complete_stack();
    void handle_controller_startup_complete();
    static FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> _init_callback;

    enum {
        NOT_INITIALIZED,
        INITIALIZING,
        INITIALIZED
    } initialization_status;

    ::BLE::InstanceID_t instanceID;
};

} // namespace odin
} // namespace vendor
} // namespace ble


#endif /* ODIN_BLE_H_ */
