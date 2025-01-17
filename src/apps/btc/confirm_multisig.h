// Copyright 2019 Shift Cryptosecurity AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _APPS_BTC_CONFIRM_MULTISIG_H_
#define _APPS_BTC_CONFIRM_MULTISIG_H_

#include "btc_params.h"

#include <btc.pb.h>
#include <compiler_util.h>
#include <stdbool.h>

/**
 * Confirms a multisig setup with the user during send/receive.
 * Verified are:
 * - coin
 * - multisig type (m-of-n)
 * - name given by the user
 * @param[in] title the title shown in each confirmation screen
 * @param[in] params Coin params of the coin to be confirmed.
 * @param[in] name User given name of the multisig account.
 * @param[in] multisig multisig details
 */
USE_RESULT bool apps_btc_confirm_multisig_basic(
    const char* title,
    const app_btc_coin_params_t* params,
    const char* name,
    const BTCScriptConfig_Multisig* multisig);

/**
 * Confirms a multisig setup with the user during account registration.
 * Verified are:
 * - coin
 * - multisig type (m-of-n)
 * - name given by the user
 * - script type (e.g. p2wsh, p2wsh-p2sh)
 * - account keypath
 * - all xpubs (formatted according to `xpub_type`).
 * @param[in] title the title shown in each confirmation screen
 * @param[in] params Coin params of the coin to be confirmed.
 * @param[in] name User given name of the multisig account.
 * @param[in] multisig multisig details
 * @param[in] xpub_type: if AUTO_ELECTRUM, will automatically format xpubs as `Zpub/Vpub`,
 * `Ypub/UPub` depending on the script type, to match Electrum's formatting. If AUTO_XPUB_TPUB,
 * format as xpub (mainnets) or tpub (testnets). Only applies if `verify_xpubs` is true.
 * @return true if the user accepts all confirmation screens, false otherwise.
 */
USE_RESULT bool apps_btc_confirm_multisig_extended(
    const char* title,
    const app_btc_coin_params_t* params,
    const char* name,
    const BTCScriptConfig_Multisig* multisig,
    BTCRegisterScriptConfigRequest_XPubType xpub_type,
    const uint32_t* keypath,
    size_t keypath_len);

#endif
