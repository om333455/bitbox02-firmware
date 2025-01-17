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

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include <cipher/cipher.h>
#include <keystore.h>
#include <memory/bitbox02_smarteeprom.h>
#include <memory/memory.h>
#include <memory/smarteeprom.h>
#include <secp256k1_ecdsa_s2c.h>
#include <secp256k1_recovery.h>
#include <securechip/securechip.h>
#include <util.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PASSWORD ("password")

static uint8_t _mock_seed[32] = {
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
};

static uint8_t _mock_seed_2[32] = {
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
};

static uint8_t _mock_bip39_seed[64] = {
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
};

const uint8_t _aes_iv[32] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

static const uint32_t _keypath[] = {
    44 + BIP32_INITIAL_HARDENED_CHILD,
    0 + BIP32_INITIAL_HARDENED_CHILD,
    0 + BIP32_INITIAL_HARDENED_CHILD,
    0,
    5,
};
// seckey at the above keypath with the above bip39 seed.
static const uint8_t _expected_seckey[32] = {
    0x4e, 0x64, 0xdf, 0xd3, 0x3a, 0xae, 0x66, 0xc4, 0xc7, 0x52, 0x6c, 0xf0, 0x2e, 0xe8, 0xae, 0x3f,
    0x58, 0x92, 0x32, 0x9d, 0x67, 0xdf, 0xd4, 0xad, 0x05, 0xe9, 0xc3, 0xd0, 0x6e, 0xdf, 0x74, 0xfb,
};

static uint8_t _password_salted_hashed_stretch_in[32] = {
    0x5e, 0x88, 0x48, 0x98, 0xda, 0x28, 0x04, 0x71, 0x51, 0xd0, 0xe5, 0x6f, 0x8d, 0xc6, 0x29, 0x27,
    0x73, 0x60, 0x3d, 0x0d, 0x6a, 0xab, 0xbd, 0xd6, 0x2a, 0x11, 0xef, 0x72, 0x1d, 0x15, 0x42, 0xd8,
};

static uint8_t _password_salted_hashed_stretch_out[32] = {
    0x73, 0x60, 0x3d, 0x0d, 0x6a, 0xab, 0xbd, 0xd6, 0x2a, 0x11, 0xef, 0x72, 0x1d, 0x15, 0x42, 0xd8,
    0x5e, 0x88, 0x48, 0x98, 0xda, 0x28, 0x04, 0x71, 0x51, 0xd0, 0xe5, 0x6f, 0x8d, 0xc6, 0x29, 0x27,
};

static uint8_t _password_salted_hashed_stretch_out_invalid[32] = {
    0x72, 0x60, 0x3d, 0x0d, 0x6a, 0xab, 0xbd, 0xd6, 0x2a, 0x11, 0xef, 0x72, 0x1d, 0x15, 0x42, 0xd8,
    0x5e, 0x88, 0x48, 0x98, 0xda, 0x28, 0x04, 0x71, 0x51, 0xd0, 0xe5, 0x6f, 0x8d, 0xc6, 0x29, 0x27,
};

static uint8_t _kdf_out_1[32] = {
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
};

static uint8_t _kdf_out_2[32] = {
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
};

static uint8_t _kdf_out_3[32] = {
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
};

// Fixture: hmac.new(_password_salted_hashed_stretch_out, _kdf_out_3,
// hashlib.sha256).hexdigest()
static uint8_t _expected_secret[32] = {
    0x39, 0xa7, 0x4f, 0x75, 0xb6, 0x9d, 0x6c, 0x84, 0x5e, 0x18, 0x91, 0x5b, 0xae, 0x29, 0xd1, 0x06,
    0x12, 0x12, 0x40, 0x37, 0x7a, 0x79, 0x97, 0x55, 0xd7, 0xcc, 0xe9, 0x26, 0x1e, 0x16, 0x91, 0x71,
};

int __real_secp256k1_anti_exfil_sign(
    const secp256k1_context* ctx,
    secp256k1_ecdsa_signature* sig,
    const unsigned char* msg32,
    const unsigned char* seckey,
    const unsigned char* host_data32,
    int* recid);

static const unsigned char* _sign_expected_msg = NULL;
static const unsigned char* _sign_expected_seckey = NULL;
int __wrap_secp256k1_anti_exfil_sign(
    const secp256k1_context* ctx,
    secp256k1_ecdsa_signature* sig,
    const unsigned char* msg32,
    const unsigned char* seckey,
    const unsigned char* host_data32,
    int* recid)
{
    if (_sign_expected_msg != NULL) {
        assert_memory_equal(_sign_expected_msg, msg32, 32);
        _sign_expected_msg = NULL;
    }
    if (_sign_expected_seckey != NULL) {
        assert_memory_equal(_sign_expected_seckey, seckey, 32);
        _sign_expected_seckey = NULL;
    }
    return __real_secp256k1_anti_exfil_sign(ctx, sig, msg32, seckey, host_data32, recid);
}

bool __wrap_salt_hash_data(
    const uint8_t* data,
    size_t data_len,
    const char* purpose,
    uint8_t* hash_out)
{
    check_expected(data);
    check_expected(data_len);
    check_expected(purpose);
    memcpy(hash_out, (const void*)mock(), 32);
    return true;
}

bool __real_cipher_aes_hmac_encrypt(
    const unsigned char* in,
    int in_len,
    uint8_t* out,
    int* out_len,
    const uint8_t* secret);

bool __wrap_cipher_aes_hmac_encrypt(
    const unsigned char* in,
    int in_len,
    uint8_t* out,
    int* out_len,
    const uint8_t* secret)
{
    check_expected(secret);
    return __real_cipher_aes_hmac_encrypt(in, in_len, out, out_len, secret);
}

/** Reset the SmartEEPROM configuration. */
static void _smarteeprom_reset(void)
{
    if (smarteeprom_is_enabled()) {
        smarteeprom_disable();
    }
    smarteeprom_bb02_config();
    bitbox02_smarteeprom_init();
}

static bool _reset_reset_called = false;
void __wrap_reset_reset(void)
{
    _reset_reset_called = true;
}

void __wrap_random_32_bytes(uint8_t* buf)
{
    memcpy(buf, (const void*)mock(), 32);
}

static bool _pubkeys_equal(
    const secp256k1_context* ctx,
    const secp256k1_pubkey* pubkey1,
    const secp256k1_pubkey* pubkey2)
{
    uint8_t pubkey1_bytes[33];
    uint8_t pubkey2_bytes[33];
    size_t len = 33;
    assert_true(
        secp256k1_ec_pubkey_serialize(ctx, pubkey1_bytes, &len, pubkey1, SECP256K1_EC_COMPRESSED));
    assert_true(
        secp256k1_ec_pubkey_serialize(ctx, pubkey2_bytes, &len, pubkey2, SECP256K1_EC_COMPRESSED));
    return memcmp(pubkey1_bytes, pubkey2_bytes, len) == 0;
}

static void _test_keystore_get_xpub(void** state)
{
    secp256k1_context* ctx = wally_get_secp_context();

    struct ext_key xpub = {0};

    mock_state(NULL, NULL);
    // fails because keystore is locked
    assert_false(keystore_get_xpub(_keypath, sizeof(_keypath) / sizeof(uint32_t), &xpub));

    mock_state(_mock_seed, _mock_bip39_seed);
    assert_true(keystore_get_xpub(_keypath, sizeof(_keypath) / sizeof(uint32_t), &xpub));

    secp256k1_pubkey expected_pubkey;
    assert_true(secp256k1_ec_pubkey_create(ctx, &expected_pubkey, _expected_seckey));

    secp256k1_pubkey pubkey;
    assert_true(secp256k1_ec_pubkey_parse(ctx, &pubkey, xpub.pub_key, sizeof(xpub.pub_key)));

    assert_true(_pubkeys_equal(ctx, &pubkey, &expected_pubkey));

    char* xpub_string;
    // Make sure it's a public key, no
    assert_false(bip32_key_to_base58(&xpub, BIP32_FLAG_KEY_PRIVATE, &xpub_string) == WALLY_OK);
    assert_true(bip32_key_to_base58(&xpub, BIP32_FLAG_KEY_PUBLIC, &xpub_string) == WALLY_OK);
    assert_string_equal(
        xpub_string,
        "xpub6Gmp9vKrJrVbU5JDcPRm6UmJPjTBurWfqow6w3BoK46E6mVyScMfTXd66WFeLfRa7Ug4iGMWDpWLpZAYcuUHyz"
        "cWZCqh8393rbuMoerRK1p");
    wally_free_string(xpub_string);
}

static void _test_keystore_get_root_fingerprint(void** state)
{
    mock_state(_mock_seed, _mock_bip39_seed);
    uint8_t fingerprint[4];
    assert_true(keystore_get_root_fingerprint(fingerprint));
    uint8_t expected_fingerprint[4] = {0x9e, 0x1b, 0x2d, 0x1e};
    assert_memory_equal(fingerprint, expected_fingerprint, 4);
}

static void _test_keystore_secp256k1_nonce_commit(void** state)
{
    uint8_t msg[32] = {0};
    memset(msg, 0x88, sizeof(msg));
    uint8_t commitment[EC_PUBLIC_KEY_LEN] = {0};
    uint8_t host_nonce_commitment[32] = {0};
    memset(host_nonce_commitment, 0xAB, sizeof(host_nonce_commitment));

    {
        mock_state(NULL, NULL);
        // fails because keystore is locked
        assert_false(keystore_secp256k1_nonce_commit(
            _keypath, sizeof(_keypath) / sizeof(uint32_t), msg, host_nonce_commitment, commitment));
    }
    {
        mock_state(_mock_seed, _mock_bip39_seed);
        assert_true(keystore_secp256k1_nonce_commit(
            _keypath, sizeof(_keypath) / sizeof(uint32_t), msg, host_nonce_commitment, commitment));
        const uint8_t expected_commitment[EC_PUBLIC_KEY_LEN] =
            "\x02\xfd\xcf\x79\xf9\xc0\x3f\x6a\xcc\xc6\x56\x95\xa1\x90\x82\xe3\x0b\xfb\x9e\xdc\x93"
            "\x04\x5a\x03\x05\x8a\x99\x09\xe4\x9b\x1a\x37\x7b";
        assert_memory_equal(expected_commitment, commitment, sizeof(commitment));
    }
}

static void _test_keystore_secp256k1_sign(void** state)
{
    secp256k1_context* ctx = wally_get_secp_context();

    secp256k1_pubkey expected_pubkey;
    assert_true(secp256k1_ec_pubkey_create(ctx, &expected_pubkey, _expected_seckey));

    uint8_t msg[32] = {0};
    memset(msg, 0x88, sizeof(msg));
    uint8_t sig[64] = {0};

    uint8_t host_nonce[32] = {0};
    memset(host_nonce, 0x56, sizeof(host_nonce));

    {
        mock_state(NULL, NULL);
        // fails because keystore is locked
        assert_false(keystore_secp256k1_sign(
            _keypath, sizeof(_keypath) / sizeof(uint32_t), msg, host_nonce, sig, NULL));
    }
    {
        mock_state(_mock_seed, _mock_bip39_seed);

        _sign_expected_seckey = _expected_seckey;
        _sign_expected_msg = msg;
        // check sig by verifying it against the msg.
        assert_true(keystore_secp256k1_sign(
            _keypath, sizeof(_keypath) / sizeof(uint32_t), msg, host_nonce, sig, NULL));
        secp256k1_ecdsa_signature secp256k1_sig = {0};
        assert_true(secp256k1_ecdsa_signature_parse_compact(ctx, &secp256k1_sig, sig));
        assert_true(secp256k1_ecdsa_verify(ctx, &secp256k1_sig, msg, &expected_pubkey));
    }
    { // test recoverable id (recid)
        int recid;
        assert_true(keystore_secp256k1_sign(
            _keypath, sizeof(_keypath) / sizeof(uint32_t), msg, host_nonce, sig, &recid));
        assert_int_equal(recid, 1);

        // Test recid by recovering the public key from the signature and checking against the
        // expected puklic key.
        secp256k1_ecdsa_recoverable_signature recoverable_sig;
        assert_true(
            secp256k1_ecdsa_recoverable_signature_parse_compact(ctx, &recoverable_sig, sig, recid));

        secp256k1_pubkey recovered_pubkey;
        assert_true(secp256k1_ecdsa_recover(ctx, &recovered_pubkey, &recoverable_sig, msg));

        assert_true(_pubkeys_equal(ctx, &recovered_pubkey, &expected_pubkey));
    }
}

static void _expect_stretch(bool valid)
{
    expect_memory(__wrap_salt_hash_data, data, PASSWORD, strlen(PASSWORD));
    expect_value(__wrap_salt_hash_data, data_len, strlen(PASSWORD));
    expect_string(__wrap_salt_hash_data, purpose, "keystore_seed_access_in");
    will_return(__wrap_salt_hash_data, _password_salted_hashed_stretch_in);

    // KDF 1
    expect_value(securechip_kdf, slot, SECURECHIP_SLOT_ROLLKEY);
    expect_memory(securechip_kdf, msg, _password_salted_hashed_stretch_in, 32);
    will_return(securechip_kdf, _kdf_out_1);

    // KDF 2
    expect_value(securechip_kdf, slot, SECURECHIP_SLOT_KDF);
    expect_memory(securechip_kdf, msg, _kdf_out_1, 32);
    will_return(securechip_kdf, _kdf_out_2);

    // KDF 3
    expect_value(securechip_kdf, slot, SECURECHIP_SLOT_KDF);
    expect_memory(securechip_kdf, msg, _kdf_out_2, 32);
    will_return(securechip_kdf, _kdf_out_3);

    expect_memory(__wrap_salt_hash_data, data, PASSWORD, strlen(PASSWORD));
    expect_value(__wrap_salt_hash_data, data_len, strlen(PASSWORD));
    expect_string(__wrap_salt_hash_data, purpose, "keystore_seed_access_out");
    will_return(
        __wrap_salt_hash_data,
        valid ? _password_salted_hashed_stretch_out : _password_salted_hashed_stretch_out_invalid);
}

static void _expect_encrypt_and_store_seed(void)
{
    will_return(__wrap_memory_is_initialized, false);

    _expect_stretch(true); // first stretch to encrypt
    _expect_stretch(true); // second stretch to verify

    expect_memory(__wrap_cipher_aes_hmac_encrypt, secret, _expected_secret, 32);
    // For the AES IV:
    will_return(__wrap_random_32_bytes, _aes_iv);
}

static void _test_keystore_encrypt_and_store_seed(void** state)
{
    _expect_encrypt_and_store_seed();
    assert_true(keystore_encrypt_and_store_seed(_mock_seed, 32, PASSWORD));
}

// this tests that you can create a keystore, unlock it, and then do this again. This is an expected
// workflow for when the wallet setup process is restarted after seeding and unlocking, but before
// creating a backup, in which case a new seed is created.
static void _test_keystore_create_and_unlock_twice(void** state)
{
    _expect_encrypt_and_store_seed();
    assert_true(keystore_encrypt_and_store_seed(_mock_seed, 32, PASSWORD));

    uint8_t remaining_attempts;
    _smarteeprom_reset();

    will_return(__wrap_memory_is_seeded, true);
    _expect_stretch(true);
    assert_int_equal(KEYSTORE_OK, keystore_unlock(PASSWORD, &remaining_attempts, NULL));

    // Create new (different) seed.
    _expect_encrypt_and_store_seed();
    assert_true(keystore_encrypt_and_store_seed(_mock_seed_2, 32, PASSWORD));

    will_return(__wrap_memory_is_seeded, true);
    _expect_stretch(true);
    assert_int_equal(KEYSTORE_OK, keystore_unlock(PASSWORD, &remaining_attempts, NULL));
}

static void _expect_seeded(bool seeded)
{
    uint8_t seed[KEYSTORE_MAX_SEED_LENGTH];
    uint32_t len;
    assert_int_equal(seeded, keystore_copy_seed(seed, &len));
}

static void _perform_some_unlocks(void)
{
    uint8_t remaining_attempts;
    // Loop to check that unlocking unlocked works while unlocked.
    for (int i = 0; i < 3; i++) {
        _reset_reset_called = false;
        will_return(__wrap_memory_is_seeded, true);
        _expect_stretch(true);
        assert_int_equal(KEYSTORE_OK, keystore_unlock(PASSWORD, &remaining_attempts, NULL));
        assert_int_equal(remaining_attempts, MAX_UNLOCK_ATTEMPTS);
        assert_false(_reset_reset_called);
        _expect_seeded(true);
    }
}

static void _test_keystore_unlock(void** state)
{
    _smarteeprom_reset();
    mock_state(NULL, NULL); // reset to locked

    uint8_t remaining_attempts;

    will_return(__wrap_memory_is_seeded, false);
    assert_int_equal(KEYSTORE_ERR_UNSEEDED, keystore_unlock(PASSWORD, &remaining_attempts, NULL));
    _expect_encrypt_and_store_seed();
    assert_true(keystore_encrypt_and_store_seed(_mock_seed, 32, PASSWORD));
    _expect_seeded(false);

    _perform_some_unlocks();

    // Invalid passwords until we run out of attempts.
    for (int i = 1; i <= MAX_UNLOCK_ATTEMPTS; i++) {
        _reset_reset_called = false;
        will_return(__wrap_memory_is_seeded, true);
        _expect_stretch(false);
        assert_int_equal(
            i >= MAX_UNLOCK_ATTEMPTS ? KEYSTORE_ERR_MAX_ATTEMPTS_EXCEEDED
                                     : KEYSTORE_ERR_INCORRECT_PASSWORD,
            keystore_unlock(PASSWORD, &remaining_attempts, NULL));
        assert_int_equal(remaining_attempts, MAX_UNLOCK_ATTEMPTS - i);
        // Wrong password does not lock the keystore again if already unlocked.
        _expect_seeded(true);
        // reset_reset() called in last attempt
        assert_int_equal(i == MAX_UNLOCK_ATTEMPTS, _reset_reset_called);
    }

    // Trying again after max attempts is blocked immediately.
    _reset_reset_called = false;
    will_return(__wrap_memory_is_seeded, true);
    assert_int_equal(
        KEYSTORE_ERR_MAX_ATTEMPTS_EXCEEDED, keystore_unlock(PASSWORD, &remaining_attempts, NULL));
    assert_int_equal(remaining_attempts, 0);
    assert_true(_reset_reset_called);
}

static void _test_keystore_lock(void** state)
{
    mock_state(NULL, NULL);
    assert_true(keystore_is_locked());
    mock_state(_mock_seed, NULL);
    assert_true(keystore_is_locked());
    mock_state(_mock_seed, _mock_bip39_seed);
    assert_false(keystore_is_locked());
    keystore_lock();
    assert_true(keystore_is_locked());
}

static void _test_keystore_get_bip39_mnemonic(void** state)
{
    char mnemonic[300];
    mock_state(NULL, NULL);
    assert_false(keystore_get_bip39_mnemonic(mnemonic, sizeof(mnemonic)));

    mock_state(_mock_seed, NULL);
    assert_false(keystore_get_bip39_mnemonic(mnemonic, sizeof(mnemonic)));

    mock_state(_mock_seed, _mock_bip39_seed);
    assert_true(keystore_get_bip39_mnemonic(mnemonic, sizeof(mnemonic)));
    const char* expected_mnemonic =
        "baby mass dust captain baby mass mass dust captain baby mass dutch creek office smoke "
        "grid creek olive baby mass dust captain baby length";
    assert_string_equal(mnemonic, expected_mnemonic);

    // Output buffer too short.
    assert_false(keystore_get_bip39_mnemonic(mnemonic, strlen(expected_mnemonic)));
    // Just enough space to fit.
    assert_true(keystore_get_bip39_mnemonic(mnemonic, strlen(expected_mnemonic) + 1));
}

static void _test_keystore_encode_xpub(void** state)
{
    struct ext_key xpub = {0};
    assert_int_equal(
        bip32_key_from_seed(
            (const unsigned char*)"seedseedseedseed",
            BIP32_ENTROPY_LEN_128,
            BIP32_VER_MAIN_PRIVATE,
            BIP32_FLAG_SKIP_HASH,
            &xpub),
        WALLY_OK);
    assert_int_equal(bip32_key_strip_private_key(&xpub), WALLY_OK);
    char out[XPUB_ENCODED_LEN] = {0};
    assert_false(keystore_encode_xpub(&xpub, XPUB, out, 110));

    assert_true(keystore_encode_xpub(&xpub, TPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "tpubD6NzVbkrYhZ4X8SrpdvxUfkKsPg5iSLHQqmQ2e7MGczVsJskvL4uD62ckffe8zi4BVtbZXRCsVDythz1eNN3fN"
        "S2A14YakBkWLBbyJiVFQ9");
    assert_true(keystore_encode_xpub(&xpub, XPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "xpub661MyMwAqRbcFLG1NSwsGkQxYGaRj3qDsDB6g64CviEc82D3r7Dp4eMnWdarcVkpPbMgwwuLLPPwCXVQFWWomv"
        "yj6QKEuDXWvNbCDF98tgM");
    assert_true(keystore_encode_xpub(&xpub, YPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "ypub6QqdH2c5z7966dT8CojVUqWTiEisffpinKhKTUx6JicVB82H6mPNgi1vXqYScQQjoEUVhRVto3kV5p6xyCvpaA"
        "fKxk1fV8M1C6eqbq4wvip");
    assert_true(keystore_encode_xpub(&xpub, ZPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "zpub6jftahH18ngZwveF3AX7gvbxtCsKcHpDhSDYEsqygizNEDqWMRYwJmg4Z3W2cK4fCsbJSu6TFi72y6iXguLqNQ"
        "Lvq5i653AVTpiUzQXvTSr");
    assert_true(keystore_encode_xpub(&xpub, VPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "vpub5SLqN2bLY4WeYjsmhjNcraDxCLHXqorE2z8f7JGSAhUr1pabLntgpX3WUDfgcgSyaK85SziDR4gqRxGGp7gnBT"
        "cXMivPjPtYNvTuS6sWM3J");
    assert_true(keystore_encode_xpub(&xpub, UPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "upub57Wa4MvRPNyAhSgesNazeV8T2N95uBrj7scSKuNYnh6xximN68j8CTPNT1i6cmo4Ag1GhX7exQLHYfei6RGmPD"
        "vvVPDy9V547CQG3b2p2sZ");
    assert_true(keystore_encode_xpub(&xpub, CAPITAL_VPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "Vpub5dEvVGKn7251yK39ePqbgeZkv8Ko4AXpMFnL2ZXyYUKFe19W7CGxuduSGvdAB7fsonC4KaiLJH5LZ7t37LqjKw"
        "jCCC2o8oMYGejn221hXB7");
    assert_true(keystore_encode_xpub(&xpub, CAPITAL_ZPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "Zpub6vZyhw1ShkEwNVocypz6WzwmbzuapeVp1hsDA97X4VpmrQQR7pwDPtXzMkTWAkHZSLfHKV6a8vVY6GLHz8VnWt"
        "TbfYpVUSdVMYzMaJxms8u");
    assert_true(keystore_encode_xpub(&xpub, CAPITAL_UPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "Upub5JQfBberxLXY81r2p33yUZUFkABM7YYKS9G7FAe6ATwNauLGrY7QHaFJFifaBD1xQ95Fa77mqcinfqGUPeRiXi"
        "3bKrLNYtY3zvg8dWPdbfj");
    assert_true(keystore_encode_xpub(&xpub, CAPITAL_YPUB, out, sizeof(out)));
    assert_string_equal(
        out,
        "Ypub6bjiQGLXZ4hTXCcW9UCUJurGS2m8t2WK6bLzNkDdgVStoJbBsAmempsrLYVvAqde2hYUa1W1gG8zCyijGS5mie"
        "mzoD84tXp15pviBjgS4df");
}

static void _test_keystore_create_and_store_seed(void** state)
{
    const uint8_t seed_random[32] =
        "\x98\xef\xa1\xb6\x0a\x83\x39\x16\x61\xa2\x4d\xc7\x4a\x80\x4f\x34\x36\xe8\x33\xe0\xaa\xbe"
        "\x75\xe9\x71\x1e\x5d\xef\x3a\x8f\x9f\x7c";
    const uint8_t host_entropy[32] =
        "\x25\x56\x9b\x9a\x11\xf9\xdb\x65\x60\x45\x9e\x8e\x48\xb4\x72\x7a\x4c\x93\x53\x00\x14\x3d"
        "\x97\x89\x89\xed\x55\xdb\x1d\x1b\x9c\xbe";
    const uint8_t password_salted_hashed[32] =
        "\xad\xee\x84\x29\xf5\xb6\x70\xa9\xd7\x34\x17\x1b\x70\x87\xf3\x8f\x86\x6a\x7e\x26\x5f\x9d"
        "\x7d\x06\xf0\x0e\x6f\xa4\x17\x54\xac\x77";
    // expected_seed = seed_random ^ host_entropy ^ password_salted_hashed
    const uint8_t expected_seed[32] =
        "\x10\x57\xbe\x05\xee\xcc\x92\xda\xd6\xd3\xc4\x52\x72\xb3\xce\xc1\xfc\x11\x1e\xc6\xe1\x1e"
        "\x9f\x66\x08\xfd\x67\x90\x30\xc0\xaf\xb5";

    // Invalid seed lengths.
    assert_false(keystore_create_and_store_seed(PASSWORD, host_entropy, 8));
    assert_false(keystore_create_and_store_seed(PASSWORD, host_entropy, 24));
    assert_false(keystore_create_and_store_seed(PASSWORD, host_entropy, 40));

    size_t test_sizes[2] = {16, 32};
    for (size_t i = 0; i < sizeof(test_sizes) / sizeof(test_sizes[0]); i++) {
        size_t seed_len = test_sizes[i];
        // Seed random is xored with host entropy and the salted/hashed user password.
        will_return(__wrap_random_32_bytes, seed_random);
        expect_memory(__wrap_salt_hash_data, data, PASSWORD, strlen(PASSWORD));
        expect_value(__wrap_salt_hash_data, data_len, strlen(PASSWORD));
        expect_string(__wrap_salt_hash_data, purpose, "keystore_seed_generation");
        will_return(__wrap_salt_hash_data, password_salted_hashed);
        _expect_encrypt_and_store_seed();
        assert_true(keystore_create_and_store_seed(PASSWORD, host_entropy, seed_len));

        // Decrypt and check seed.
        uint8_t encrypted_seed_and_hmac[96] = {0};
        uint8_t len = 0;
        assert_true(memory_get_encrypted_seed_and_hmac(encrypted_seed_and_hmac, &len));
        size_t decrypted_len = len - 48;
        uint8_t out[decrypted_len];
        assert_true(cipher_aes_hmac_decrypt(
            encrypted_seed_and_hmac, len, out, &decrypted_len, _expected_secret));
        assert_true(decrypted_len == seed_len);

        assert_memory_equal(expected_seed, out, seed_len);
    }
}

static void _mock_with_mnemonic(const char* mnemonic, const char* passphrase)
{
    uint8_t seed[32] = {0};
    size_t seed_len;
    assert_true(keystore_bip39_mnemonic_to_seed(mnemonic, seed, &seed_len));

    mock_state(seed, NULL);
    assert_true(keystore_unlock_bip39(passphrase));
}

static void _test_keystore_get_ed25519_seed(void** state)
{
    // Test vectors taken from:
    // https://github.com/cardano-foundation/CIPs/blob/6c249ef48f8f5b32efc0ec768fadf4321f3173f2/CIP-0003/Ledger.md#test-vectors
    // See also: https://github.com/cardano-foundation/CIPs/pull/132

    _mock_with_mnemonic(
        "recall grace sport punch exhibit mad harbor stand obey short width stem awkward used "
        "stairs wool ugly trap season stove worth toward congress jaguar",
        "");

    uint8_t seed[96];
    assert_true(keystore_get_ed25519_seed(seed));
    assert_memory_equal(
        seed,
        "\xa0\x8c\xf8\x5b\x56\x4e\xcf\x3b\x94\x7d\x8d\x43\x21\xfb\x96\xd7\x0e\xe7\xbb\x76\x08\x77"
        "\xe3\x71\x89\x9b\x14\xe2\xcc\xf8\x86\x58\x10\x4b\x88\x46\x82\xb5\x7e\xfd\x97\xde\xcb\xb3"
        "\x18\xa4\x5c\x05\xa5\x27\xb9\xcc\x5c\x2f\x64\xf7\x35\x29\x35\xa0\x49\xce\xea\x60\x68\x0d"
        "\x52\x30\x81\x94\xcc\xef\x2a\x18\xe6\x81\x2b\x45\x2a\x58\x15\xfb\xd7\xf5\xba\xbc\x08\x38"
        "\x56\x91\x9a\xaf\x66\x8f\xe7\xe4",
        sizeof(seed));

    // Multiple loop iterations.
    _mock_with_mnemonic(
        "correct cherry mammal bubble want mandate polar hazard crater better craft exotic choice "
        "fun tourist census gap lottery neglect address glow carry old business",
        "");
    assert_true(keystore_get_ed25519_seed(seed));
    assert_memory_equal(
        seed,
        "\x58\x7c\x67\x74\x35\x7e\xcb\xf8\x40\xd4\xdb\x64\x04\xff\x7a\xf0\x16\xda\xce\x04\x00\x76"
        "\x97\x51\xad\x2a\xbf\xc7\x7b\x9a\x38\x44\xcc\x71\x70\x25\x20\xef\x1a\x4d\x1b\x68\xb9\x11"
        "\x87\x78\x7a\x9b\x8f\xaa\xb0\xa9\xbb\x6b\x16\x0d\xe5\x41\xb6\xee\x62\x46\x99\x01\xfc\x0b"
        "\xed\xa0\x97\x5f\xe4\x76\x3b\xea\xbd\x83\xb7\x05\x1a\x5f\xd5\xcb\xce\x5b\x88\xe8\x2c\x4b"
        "\xba\xca\x26\x50\x14\xe5\x24\xbd",
        sizeof(seed));

    _mock_with_mnemonic(
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon "
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon "
        "abandon art",
        "foo");
    assert_true(keystore_get_ed25519_seed(seed));
    assert_memory_equal(
        seed,
        "\xf0\x53\xa1\xe7\x52\xde\x5c\x26\x19\x7b\x60\xf0\x32\xa4\x80\x9f\x08\xbb\x3e\x5d\x90\x48"
        "\x4f\xe4\x20\x24\xbe\x31\xef\xcb\xa7\x57\x8d\x91\x4d\x3f\xf9\x92\xe2\x16\x52\xfe\xe6\xa4"
        "\xd9\x9f\x60\x91\x00\x69\x38\xfa\xc2\xc0\xc0\xf9\xd2\xde\x0b\xa6\x4b\x75\x4e\x92\xa4\xf3"
        "\x72\x3f\x23\x47\x20\x77\xaa\x4c\xd4\xdd\x8a\x8a\x17\x5d\xba\x07\xea\x18\x52\xda\xd1\xcf"
        "\x26\x8c\x61\xa2\x67\x9c\x38\x90",
        sizeof(seed));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(_test_keystore_get_xpub),
        cmocka_unit_test(_test_keystore_get_root_fingerprint),
        cmocka_unit_test(_test_keystore_secp256k1_nonce_commit),
        cmocka_unit_test(_test_keystore_secp256k1_sign),
        cmocka_unit_test(_test_keystore_encrypt_and_store_seed),
        cmocka_unit_test(_test_keystore_create_and_unlock_twice),
        cmocka_unit_test(_test_keystore_unlock),
        cmocka_unit_test(_test_keystore_lock),
        cmocka_unit_test(_test_keystore_get_bip39_mnemonic),
        cmocka_unit_test(_test_keystore_encode_xpub),
        cmocka_unit_test(_test_keystore_create_and_store_seed),
        cmocka_unit_test(_test_keystore_get_ed25519_seed),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
