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

static const char *_Nonnull IOTEX_STAKING_CONTRACT = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";
static const char* _Nonnull IOTEX_STAKING_DATA = "payload";
static const char* _Nonnull IOTEX_STAKING_AMOUNT = "10";

TEST(TWIoTeXStaking, Create) {
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CONTRACT, 41));
    auto data = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_DATA, 7));
    auto amount = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_AMOUNT, 2));
    auto stake = WRAPD(TWIoTeXStakingCreate(candidate.get(), amount.get(), 1000, true, data.get()));

    auto result = dataFromTWData(stake.get());

    ASSERT_EQ(hex(*result), "0a29696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611202313018e80720012a077061796c6f6164");
}
/*
TEST(TWIoTeXStaking, Reclaim) {
    auto test = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    auto reclaim = WRAPD(TWIoTeXStakingReclaim(1001, test.get()));

    auto result = dataFromTWData(reclaim.get());

    ASSERT_EQ(hex(*result), "08e907121c37343638363937333230363937333230363132303734363537333734");
}

TEST(TWIoTeXStaking, AddDeposit) {

    auto test = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    byte num[2] = {1, 2};
    auto amount = WRAPD(TWDataCreateWithBytes(num, 2));
    auto ad = WRAPD(TWIoTeXStakingAddDeposit(1000, amount.get(), test.get()));

    auto result = dataFromTWData(ad.get());

    ASSERT_EQ(hex(*result), "08e8071204303130321a1c37343638363937333230363937333230363132303734363537333734");
}

TEST(TWIoTeXStaking, Restake) {

    auto test = WRAPD(TWDataCreateWithSize(0));
    auto rs = WRAPD(TWIoTeXStakingRestake(1001, 1000, true, test.get()));

    auto result = dataFromTWData(rs.get());

    ASSERT_EQ(hex(*result), "08e90710e8071801");
}

TEST(TWIoTeXStaking, Move) {

    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto candidate = TWDataCreateWithBytes(name, 12);
    auto test = WRAPD(TWDataCreateWithSize(0));
    auto mv = WRAPD(TWIoTeXStakingMove(1001, candidate, test.get()));

    auto result = dataFromTWData(mv.get());

    ASSERT_EQ(hex(*result), "08e9071218303130323033303430353036303730383039306130623063");
}
*/
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

    // staking is implemented using the Execution message
    auto staking = input.mutable_stakecreate();
    auto candidate = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_CONTRACT, 41));
    auto data = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_DATA, 7));
    auto amount = WRAPD(TWDataCreateWithBytes((uint8_t*)IOTEX_STAKING_AMOUNT, 2));
    auto stake = WRAPD(TWIoTeXStakingCreate(candidate.get(), amount.get(), 1000, true, data.get()));
    //staking->set_data(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    staking->ParseFromArray(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    //const Data ser = parse_hex("0b6d6172696f40747275737402313003424e42044d656d6f001768747470733a2f2"
      //                         "f747275737477616c6c65742e636f6d");
    //size_t index = 0;
    //const auto newFunds = NewFundsContent::deserialize(ser, index);
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "219483a7309db9f1c41ac3fa0aadecfbdbeb0448b0dfaee54daec4ec178aa9f1");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto outputser = output.SerializeAsString();
    std::cout << hex(outputser.begin(), outputser.end()) << std::endl;
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()), "0a4a080118c0843d22023130c2023d0a29696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611202313018e80720012a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a415db41c974bc1d8edd59fad54c4eac41250981640c44183c1c3ed9e45873bf15c02f3575de59233aefd7ec6eecfa7254bf4b67501e96bea8a4d54a18b4e0e4fec01");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "a324d56f5b50e86aab27c0c6d33f9699f36d3ed8e27967a56e644f582bbd5e2d");
}

/*
TEST(TWIoTeXStaking, SignReclaim) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(123);
    input.set_gaslimit(888);
    input.set_gasprice("999");
    auto keyhex = parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the Execution message
    auto staking = input.mutable_execution();
    staking->set_amount("456");
    staking->set_contract(IOTEX_STAKING_CONTRACT);
    // call staking API to generate calldata
    auto data = WRAPD(TWDataCreateWithSize(0));
    auto reclaim = WRAPD(TWIoTeXStakingReclaim(1001, data.get()));
    staking->set_data(TWDataBytes(reclaim.get()), TWDataSize(reclaim.get()));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "03a0fd1eff29765741f950d574cece2eeff538def70f3c6a6755e1e4622f5963");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "b93a2874a72ce4eb8a41a20c209cf3fd188671ed8be8239a57960cbed887e962");
}

TEST(TWIoTeXStaking, SignAddDeposit) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(123);
    input.set_gaslimit(888);
    input.set_gasprice("999");
    auto keyhex = parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the Execution message
    auto staking = input.mutable_execution();
    staking->set_amount("456");
    staking->set_contract(IOTEX_STAKING_CONTRACT);
    // call staking API to generate calldata
    auto data = WRAPD(TWDataCreateWithSize(0));
    byte num[2] = {1, 2};
    auto amount = WRAPD(TWDataCreateWithBytes(num, 2));
    auto addDeposit = WRAPD(TWIoTeXStakingAddDeposit(1001, amount.get(), data.get()));
    staking->set_data(TWDataBytes(addDeposit.get()), TWDataSize(addDeposit.get()));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "b2e443e9b0e83f21008c7fecb20d64859e08bc2eb317e7935478b063f5361abf");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "2b2657247a72cb262de214b4e793c7a01fa2139fd5d12a46d43c24f87f9e2396");
}

TEST(TWIoTeXStaking, SignReStake) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(123);
    input.set_gaslimit(888);
    input.set_gasprice("999");
    auto keyhex = parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the Execution message
    auto staking = input.mutable_execution();
    staking->set_amount("456");
    staking->set_contract(IOTEX_STAKING_CONTRACT);
    // call staking API to generate calldata
    auto data = WRAPD(TWDataCreateWithSize(0));
    auto reStake = WRAPD(TWIoTeXStakingRestake(1001, 1000, true, data.get()));
    staking->set_data(TWDataBytes(reStake.get()), TWDataSize(reStake.get()));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "597f5a63df273016a0aa851b0e0bd325ee95c693d6ffc0e9389b7dbf2909418a");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "c71058812a5febe5cdcdaf9499ba0b2c895f88d1acd3203e5097b307c2a5f1d1");
}

TEST(TWIoTeXStaking, SignMove) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(123);
    input.set_gaslimit(888);
    input.set_gasprice("999");
    auto keyhex = parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the Execution message
    auto staking = input.mutable_execution();
    staking->set_amount("456");
    staking->set_contract(IOTEX_STAKING_CONTRACT);
    // call staking API to generate calldata
    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto candidate = WRAPD(TWDataCreateWithBytes(name, 12));
    auto data = WRAPD(TWDataCreateWithSize(0));
    auto moveStake = WRAPD(TWIoTeXStakingMove(1001, candidate.get(), data.get()));
    staking->set_data(TWDataBytes(moveStake.get()), TWDataSize(moveStake.get()));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "d921d62673de5935480f9c375a2278168b5a5d9e62e4789f8b923e98d74dd4de");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "33290ded342efaebf795855be73d34cbac149a2415ff9558de10303e6126f30d");
}
*/