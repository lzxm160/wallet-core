// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIoTeXStaking.h>

#include "Data.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "IoTeX/Signer.h"
#include "proto/IoTeX.pb.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::IoTeX;

static const char* _Nonnull IOTEX_STAKING_CANDIDATE = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";
static const char* _Nonnull IOTEX_STAKING_PAYLOAD = "payload";
static const char* _Nonnull IOTEX_STAKING_AMOUNT = "10";

TEST(TWIoTeXStaking, Create) {
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CANDIDATE, 41));
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto amount = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_AMOUNT, 2));
    auto stake = WRAPD(TWIoTeXStakingCreate(candidate.get(), amount.get(), 1000, true, payload.get()));

    auto result = dataFromTWData(stake.get());

    ASSERT_EQ(hex(*result), "0a29696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611202313018e80720012a077061796c6f6164");
}

TEST(TWIoTeXStaking, AddDeposit) {
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto amount = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_AMOUNT, 2));
    auto ad = WRAPD(TWIoTeXStakingAddDeposit(10, amount.get(), payload.get()));

    auto result = dataFromTWData(ad.get());

    ASSERT_EQ(hex(*result),"080a120231301a077061796c6f6164");
}

TEST(TWIoTeXStaking, Unstake) {
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto ad = WRAPD(TWIoTeXStakingUnstake(10, payload.get()));

    auto result = dataFromTWData(ad.get());

    ASSERT_EQ(hex(*result), "080a12077061796c6f6164");
}

TEST(TWIoTeXStaking, Withdraw) {
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto ad = WRAPD(TWIoTeXStakingWithdraw(10, payload.get()));

    auto result = dataFromTWData(ad.get());

    ASSERT_EQ(hex(*result), "080a12077061796c6f6164");
}

inline std::string stringFromTWData(TWData* data) {
    auto ret =
        const_cast<std::vector<uint8_t>*>(reinterpret_cast<const std::vector<uint8_t>*>(data));
    std::string str;
    str.assign(ret->begin(), ret->end());
    return str;
}

TEST(TWIoTeXStaking, SignCreate) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the stakecreate message
    auto staking = input.mutable_stakecreate();
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CANDIDATE, 41));
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto amount = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_AMOUNT, 2));
    auto stake = WRAPD(TWIoTeXStakingCreate(candidate.get(), amount.get(), 1000, true, payload.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "219483a7309db9f1c41ac3fa0aadecfbdbeb0448b0dfaee54daec4ec178aa9f1");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()), "0a4a080118c0843d22023130c2023d0a29696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611202313018e80720012a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a415db41c974bc1d8edd59fad54c4eac41250981640c44183c1c3ed9e45873bf15c02f3575de59233aefd7ec6eecfa7254bf4b67501e96bea8a4d54a18b4e0e4fec01");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "a324d56f5b50e86aab27c0c6d33f9699f36d3ed8e27967a56e644f582bbd5e2d");
}

TEST(TWIoTeXStaking, SignAddDeposit) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());

    auto staking = input.mutable_stakeadddeposit();
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto amount = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_AMOUNT, 2));
    auto stake = WRAPD(TWIoTeXStakingAddDeposit(10, amount.get(), payload.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()),
              "9089e7eb1afed64fcdbd3c7ee29a6cedab9aa59cf3f7881dfaa3d19f99f09338");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(
        hex(encoded.begin(), encoded.end()),"0a1c080118c0843d22023130da020f080a120231301a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41a48ab1feba8181d760de946aefed7d815a89fd9b1ab503d2392bb55e1bb75eec42dddc8bd642f89accc3a37b3cf15a103a95d66695fdf0647b202869fdd66bcb01");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()),
              "ca8937d6f224a4e4bf93cb5605581de2d26fb0481e1dfc1eef384ee7ccf94b73");
}

TEST(TWIoTeXStaking, SignUnstake) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());

    auto staking = input.mutable_stakeunstake();
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingUnstake(10, payload.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()),
              "9c806c793d5e452ecf944aa18b07fb8ee0b07fa37807b6480d1208bd591c5c92");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()),"0a18080118c0843d22023130ca020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4100adee39b48e1d3dbbd65298a57c7889709fc4df39987130da306f6997374a184b7e7c232a42f21e89b06e6e7ceab81303c6b7483152d08d19ac829b22eb81e601");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()),
              "bed58b64a6c4e959eca60a86f0b2149ce0e1dd527ac5fd26aef725ebf7c22a7d");
}