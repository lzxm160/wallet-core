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

TEST(TWIoTeXStaking, Restake) {
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingRestake(10, 1000, true, payload.get()));

    auto result = dataFromTWData(stake.get());

    ASSERT_EQ(hex(*result), "080a10e807180122077061796c6f6164");
}

TEST(TWIoTeXStaking, ChangeCandidate) {
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CANDIDATE, 41));
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingChangeCandidate(candidate.get(), 10, payload.get()));

    auto result = dataFromTWData(stake.get());

    ASSERT_EQ(hex(*result), "080a1229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a077061796c6f6164");
}

TEST(TWIoTeXStaking, Transfer) {
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CANDIDATE, 41));
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingTransfer(candidate.get(), 10, payload.get()));

    auto result = dataFromTWData(stake.get());

    ASSERT_EQ(hex(*result), "080a1229696f3178707136326177383575717a72636367397935686e727976386c6432"
                            "6e6b7079636333677a611a077061796c6f6164");
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

TEST(TWIoTeXStaking, SignWithdraw) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());

    auto staking = input.mutable_stakewithdraw();
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingWithdraw(10, payload.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()),
              "9f7845013c46a387a0b8832d757a9f21aab054fa73c045ad3a66b52f2cad3627");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()),"0a18080118c0843d22023130d2020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4152644d102186be6640d46b517331f3402e24424b0d85129595421d28503d75340b2922f5a0d4f667bbd6f576d9816770286b2ce032ba22eaec3952e24da4756b00");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()),"28049348cf34f1aa927caa250e7a1b08778c44efaf73b565b6fa9abe843871b4");
}

TEST(TWIoTeXStaking, SignRestake) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the stakerestake message
    auto staking = input.mutable_stakerestake();
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingRestake(10, 1000, true, payload.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()),
              "f3e36f74dfe53c39c66a827244fc20f44b8f22db23e84776c4b1d2123a72c63a");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(
        hex(encoded.begin(), encoded.end()),"0a1d080118c0843d22023130e20210080a10e807180122077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41e2e763aed5b1fd1a8601de0f0ae34eb05162e34b0389ae3418eedbf762f64959634a968313a6516dba3a97b34efba4753bbed3a33d409ecbd45ac75007cd8e9101");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()),
              "8816e8f784a1fce40b54d1cd172bb6976fd9552f1570c73d1d9fcdc5635424a9");
}

TEST(TWIoTeXStaking, SignChangeCandidate) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the stakecreate message
    auto staking = input.mutable_stakechangecandidate();
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CANDIDATE, 41));
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingChangeCandidate(candidate.get(), 10, payload.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()),
              "4fe20be23e3de2fd81cacdebb04d4b69b0f80404fd0193a92d6478ae38602d9f");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(
        hex(encoded.begin(), encoded.end()),"0a43080118c0843d22023130ea0236080a1229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41d519eb3747163b945b862989b7e82a7f8468001e9683757cb88d5ddd95f81895047429e858bd48f7d59a88bfec92de231d216293aeba1e4fbe11461d9c9fc99801");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()),
              "186526b5b9fe74e25beb52c83c41780a69108160bef2ddaf3bffb9f1f1e5e73a");
}

TEST(TWIoTeXStaking, SignTransfer) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the stakecreate message
    auto staking = input.mutable_staketransferownership();
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CANDIDATE, 41));
    auto payload = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_PAYLOAD, 7));
    auto stake = WRAPD(TWIoTeXStakingTransfer(candidate.get(), 10, payload.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()),
              "d22b4b3e630e1d494951e9041a983608232cf64629262296b6ef1f57fa748fd2");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()),"0a43080118c0843d22023130f20236080a1229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41fa26db427ab87a56a129196c1604f2e22c4dd2a1f99b2217bc916260757d00093d9e6dccdf53e3b0b64e41a69d71c238fbf9281625164694a74dfbeba075d0ce01");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()),
              "74b2e1d6a09ba5d1298fa422d5850991ae516865077282196295a38f93c78b85");
}