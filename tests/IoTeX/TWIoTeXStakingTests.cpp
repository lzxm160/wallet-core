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
static const char * IOTEX_STAKING_TEST = "this is a test";

TEST(TWIoTeXStaking, Create) {
    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto candidate = WRAPD(TWDataCreateWithBytes(name, 12));

    auto test = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    byte num[2] = {1, 2};
    auto amount = WRAPD(TWDataCreateWithBytes(num, 2));
    auto stake = WRAPD(TWIoTeXStakingCreate(candidate.get(), amount.get(), 1000, true, test.get()));

    auto result = dataFromTWData(stake.get());

    ASSERT_EQ(hex(*result), "0a1830313032303330343035303630373038303930613062306312043031303218e80720012a1c37343638363937333230363937333230363132303734363537333734");
}

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

TEST(TWIoTeXStaking, SignCreate) {
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
    auto data = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    // data = "this is a test" here, it could be null (user leaves data empty when signing the tx)
    byte num[2] = {1, 2};
    auto amount = WRAPD(TWDataCreateWithBytes(num, 2));
    auto stake = WRAPD(TWIoTeXStakingCreate(candidate.get(), amount.get(), 1000, true, data.get()));
    staking->set_data(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "c28f514c0e45833fa17b7a6e028831f91da041059b1ecb60c5c2eff1757c3580");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()), "0a86020801107b18f806220339393962f7010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611ac40107c35fc00102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003e900000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000e74686973206973206120746573740000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a41a558bc9a4bfba920242ccd4d5c5da363ec534d4dd5eb67f88e9db7aaad5c50ad62dfe298c0e54e311ebba045f48cea1136e42a123a8e6b03d3e6ed82d4ec2b9401");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "41b1f8be5f6b884c06556fba2611716e8e514b507f5a653fc02ac50ba13fbd6c");
}

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
