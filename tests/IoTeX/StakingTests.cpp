// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "HexCoding.h"
#include "IoTeX/Signer.h"
#include "IoTeX/Staking.h"
#include "PrivateKey.h"
#include "proto/IoTeX.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::IoTeX;

TEST(TWIoTeXStaking, Create) {
    std::string IOTEX_STAKING_CANDIDATE = "io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    std::string IOTEX_STAKING_AMOUNT = "100";
    Data candidate(IOTEX_STAKING_CANDIDATE.begin(), IOTEX_STAKING_CANDIDATE.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());
    Data amount(IOTEX_STAKING_AMOUNT.begin(), IOTEX_STAKING_AMOUNT.end());

    auto stake = stakingCreate(candidate, amount, 10000, true, payload);
    ASSERT_EQ(hex(stake), "0a29696f313964307033616834673877773964376b63786671383779786537666e723872"
                          "7074683573686a120331303018904e20012a077061796c6f6164");
}

TEST(TWIoTeXStaking, AddDeposit) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    std::string IOTEX_STAKING_AMOUNT = "10";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());
    Data amount(IOTEX_STAKING_AMOUNT.begin(), IOTEX_STAKING_AMOUNT.end());

    auto stake = stakingAddDeposit(10, amount, payload);

    ASSERT_EQ(hex(stake), "080a120231301a077061796c6f6164");
}

TEST(TWIoTeXStaking, Unstake) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingUnstake(10, payload);

    ASSERT_EQ(hex(stake), "080a12077061796c6f6164");
}

TEST(TWIoTeXStaking, Withdraw) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingWithdraw(10, payload);

    ASSERT_EQ(hex(stake), "080a12077061796c6f6164");
}

TEST(TWIoTeXStaking, Restake) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingRestake(10, 1000, true, payload);

    ASSERT_EQ(hex(stake), "080a10e807180122077061796c6f6164");
}

TEST(TWIoTeXStaking, ChangeCandidate) {
    std::string IOTEX_STAKING_CANDIDATE = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data candidate(IOTEX_STAKING_CANDIDATE.begin(), IOTEX_STAKING_CANDIDATE.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingChangeCandidate(10, candidate, payload);

    ASSERT_EQ(hex(stake), "080a1229696f3178707136326177383575717a72636367397935686e727976386c"
                          "64326e6b7079636333677a611a077061796c6f6164");
}

TEST(TWIoTeXStaking, Transfer) {
    std::string IOTEX_STAKING_CANDIDATE = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data candidate(IOTEX_STAKING_CANDIDATE.begin(), IOTEX_STAKING_CANDIDATE.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingTransfer(10, candidate, payload);

    ASSERT_EQ(hex(stake), "080a1229696f3178707136326177383575717a72636367397935686e727976386c6432"
                          "6e6b7079636333677a611a077061796c6f6164");
}

TEST(TWIoTeXStaking, CandidateRegister) {
    std::string IOTEX_STAKING_NAME = "test";
    std::string IOTEX_STAKING_OPERATOR = "io10a298zmzvrt4guq79a9f4x7qedj59y7ery84he";
    std::string IOTEX_STAKING_REWARD = "io13sj9mzpewn25ymheukte4v39hvjdtrfp00mlyv";
    std::string IOTEX_STAKING_OWNER = "io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj";
    std::string IOTEX_STAKING_AMOUNT = "100";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data name(IOTEX_STAKING_NAME.begin(), IOTEX_STAKING_NAME.end());
    Data operatorAddress(IOTEX_STAKING_OPERATOR.begin(), IOTEX_STAKING_OPERATOR.end());
    Data reward(IOTEX_STAKING_REWARD.begin(), IOTEX_STAKING_REWARD.end());
    Data amount(IOTEX_STAKING_AMOUNT.begin(), IOTEX_STAKING_AMOUNT.end());
    Data owner(IOTEX_STAKING_OWNER.begin(), IOTEX_STAKING_OWNER.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake =
        candidateRegister(name, operatorAddress, reward, amount, 10000, false, owner, payload);

    ASSERT_EQ(hex(stake),
              "0a5c0a04746573741229696f3130613239387a6d7a7672743467757137396139663478377165646a3539"
              "7937657279383468651a29696f3133736a396d7a7065776e3235796d6865756b74653476333968766a64"
              "7472667030306d6c7976120331303018904e2a29696f313964307033616834673877773964376b637866"
              "71383779786537666e7238727074683573686a32077061796c6f6164");
}

TEST(TWIoTeXStaking, CandidateUpdate) {
    std::string IOTEX_STAKING_NAME = "test";
    std::string IOTEX_STAKING_OPERATOR = "io1cl6rl2ev5dfa988qmgzg2x4hfazmp9vn2g66ng";
    std::string IOTEX_STAKING_REWARD = "io1juvx5g063eu4ts832nukp4vgcwk2gnc5cu9ayd";
    Data name(IOTEX_STAKING_NAME.begin(), IOTEX_STAKING_NAME.end());
    Data operatorAddress(IOTEX_STAKING_OPERATOR.begin(), IOTEX_STAKING_OPERATOR.end());
    Data reward(IOTEX_STAKING_REWARD.begin(), IOTEX_STAKING_REWARD.end());

    auto stake = candidateUpdate(name, operatorAddress, reward);

    ASSERT_EQ(hex(stake), "0a04746573741229696f31636c36726c32657635646661393838716d677a6732783468"
                          "66617a6d7039766e326736366e671a29696f316a757678356730363365753474733833"
                          "326e756b7034766763776b32676e6335637539617964");
}

TEST(TWIoTeXStaking, SignAll) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    input.set_privatekey(keyhex.data(), keyhex.size());
    Proto::SigningOutput output;

    {
        // sign stakecreate
        auto action = input.mutable_stakecreate();
        action->set_candidatename("io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj");
        action->set_stakedamount("100");
        action->set_stakedduration(10000);
        action->set_autostake(true);
        action->set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(hex(output.encoded()),
                  "0a4b080118c0843d22023130c2023e0a29696f313964307033616834673877773964376b63786671"
                  "3837797865"
                  "37666e7238727074683573686a120331303018904e20012a077061796c6f6164124104755ce6d890"
                  "3f6b3793bd"
                  "db4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c"
                  "0bc76ef30d"
                  "d6a1038ed9da8daf331a412e8bac421bab88dcd99c26ac8ffbf27f11ee57a41e7d2537891bfed5ae"
                  "d8e2e026d4"
                  "6e55d1b856787bc1cd7c1216a6e2534c5b5d1097c3afe8e657aa27cbbb0801");
        input.release_stakecreate();
        // output.release_encoded();
    }

    { // sign stakeadddeposit
        auto action = input.mutable_stakeadddeposit();
        action->set_bucketindex(10);
        action->set_amount("10");
        action->set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a1c080118c0843d22023130da020f080a120231301a077061796c6f6164124104755ce6d8903f6b3793"
            "bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0b"
            "c76ef30dd6a1038ed9da8daf331a41a48ab1feba8181d760de946aefed7d815a89fd9b1ab503d2392bb5"
            "5e1bb75eec42dddc8bd642f89accc3a37b3cf15a103a95d66695fdf0647b202869fdd66bcb01");
        input.release_stakeadddeposit();
    }

    { // sign stakeunstake
        auto action = input.mutable_stakeunstake();
        action->set_bucketindex(10);
        action->set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a18080118c0843d22023130ca020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5"
            "d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30d"
            "d6a1038ed9da8daf331a4100adee39b48e1d3dbbd65298a57c7889709fc4df39987130da306f6997374a"
            "184b7e7c232a42f21e89b06e6e7ceab81303c6b7483152d08d19ac829b22eb81e601");
        input.release_stakeunstake();
    }
    {
        auto action = input.mutable_stakewithdraw();
        action->set_bucketindex(10);
        action->set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a18080118c0843d22023130d2020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5"
            "d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30d"
            "d6a1038ed9da8daf331a4152644d102186be6640d46b517331f3402e24424b0d85129595421d28503d75"
            "340b2922f5a0d4f667bbd6f576d9816770286b2ce032ba22eaec3952e24da4756b00");
        input.release_stakewithdraw();
    }
}
