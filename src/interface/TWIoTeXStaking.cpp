// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiEncoder.h>
#include <TrustWalletCore/TWEthereumAbiFunction.h>
#include <TrustWalletCore/TWIoTeXStaking.h>

#include "Data.h"
#include "uint256.h"
#include "proto/IoTeX.pb.h"
#include "HexCoding.h"
using namespace TW;

TWData* _Nonnull TWIoTeXStakingCreate(TWData* _Nonnull candidate, TWData* _Nonnull amount,uint32_t duration, bool autoStake,TWData* _Nonnull payload) {
    auto stakeAction = IoTeX::Proto::StakeCreate();
    auto c=hex(*candidate)
    stakeAction.set_candidatename(c); 
    auto a=hex(*amount)
    stakeAction.set_stakedamount(a);
    stakeAction.set_stakedduration(duration); 
    stakeAction.set_autostake(autoStake);
    auto p=hex(*payload)
    stakeAction.set_payload(p);
    auto s = stakeAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end()); 
    return TWDataCreateWithHexString(actionHex);
}

/// Function to generate unstake or withdraw message
TWData* _Nonnull TWIoTeXStakingReclaim(uint64_t index, TWData* _Nonnull payload) {
    auto reclaimAction = IoTeX::Proto::StakeReclaim();
    reclaimAction.set_bucketindex(index);
    auto p = hex(*payload)
    reclaimAction.set_payload(p);
    auto s = reclaimAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(actionHex);
}

/// Function to generate AddDeposit message
TWData* _Nonnull TWIoTeXStakingAddDeposit(uint64_t index, TWData* _Nonnull amount,TWData* _Nonnull payload) {
    auto addDepositAction = IoTeX::Proto::StakeAddDeposit();
    addDepositAction.set_bucketindex(index);
    auto a = hex(*amount)
    addDepositAction.set_amount(a);
    auto p = hex(*payload)
    addDepositAction.set_payload(p);
    auto s = addDepositAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(actionHex);
}

/// Function to generate Restake message
TWData* _Nonnull TWIoTeXStakingRestake(uint64_t index, uint32_t duration,bool autoStake,TWData* _Nonnull payload) {
    auto restakeAction = IoTeX::Proto::StakeRestake();
    restakeAction.set_bucketindex(index);
    restakeAction.set_stakedduration(duration);
    restakeAction.set_autostake(autoStake);
    auto p = hex(*payload)
    restakeAction.set_payload(p);
    auto s = restakeAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(actionHex);
}

/// Function to generate Move message
TWData* _Nonnull TWIoTeXStakingMove(uint64_t index, TWData* _Nonnull name, TWData* _Nonnull payload) {
    auto moveAction = IoTeX::Proto::StakeMove();
    moveAction.set_bucketindex(index);
    auto n = hex(*name)
    moveAction.set_name(n);
    auto p = hex(*payload)
    moveAction.set_payload(p);
    auto s = moveAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(actionHex);
}
