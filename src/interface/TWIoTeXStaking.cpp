// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIoTeXStaking.h>

#include "Data.h"
#include "uint256.h"
#include "proto/IoTeX.pb.h"
#include "HexCoding.h"
using namespace TW;

inline std::string stringFromTWData(TWData* data) {
    auto ret = static_cast<std::string*>(data);
    return *ret;
}
inline std::vector<uint8_t>* dataFromTWData(TWData* data) {
    return const_cast<std::vector<uint8_t>*>(reinterpret_cast<const std::vector<uint8_t>*>(data));
}
TWData* _Nonnull TWIoTeXStakingCreate(TWData* _Nonnull candidate, TWData* _Nonnull amount,uint32_t duration, bool autoStake,TWData* _Nonnull payload) {
    auto stakeAction = IoTeX::Proto::StakeCreate();
    std::cout << stringFromTWData(candidate) << std::endl;
    stakeAction.set_candidatename(stringFromTWData(candidate)); 
    cout << stringFromTWData(amount) << endl;
    stakeAction.set_stakedamount(stringFromTWData(amount));
    stakeAction.set_stakedduration(duration); 
    stakeAction.set_autostake(autoStake);
    cout << stringFromTWData(payload) << endl;
    stakeAction.set_payload(stringFromTWData(payload));
    auto s = stakeAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end()); 
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate unstake or withdraw message
TWData* _Nonnull TWIoTeXStakingReclaim(uint64_t index, TWData* _Nonnull payload) {
    auto reclaimAction = IoTeX::Proto::StakeReclaim();
    reclaimAction.set_bucketindex(index);
    auto p = dataFromTWData(payload);
    reclaimAction.set_payload(hex(*p));
    auto s = reclaimAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate AddDeposit message
TWData* _Nonnull TWIoTeXStakingAddDeposit(uint64_t index, TWData* _Nonnull amount,TWData* _Nonnull payload) {
    auto addDepositAction = IoTeX::Proto::StakeAddDeposit();
    addDepositAction.set_bucketindex(index);
    auto a = dataFromTWData(amount);
    addDepositAction.set_amount(hex(*a));
    auto p = dataFromTWData(payload);
    addDepositAction.set_payload(hex(*p));
    auto s = addDepositAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate Restake message
TWData* _Nonnull TWIoTeXStakingRestake(uint64_t index, uint32_t duration,bool autoStake,TWData* _Nonnull payload) {
    auto restakeAction = IoTeX::Proto::StakeRestake();
    restakeAction.set_bucketindex(index);
    restakeAction.set_stakedduration(duration);
    restakeAction.set_autostake(autoStake);
    auto p = dataFromTWData(payload);
    restakeAction.set_payload(hex(*p));
    auto s = restakeAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate Move message
TWData* _Nonnull TWIoTeXStakingMove(uint64_t index, TWData* _Nonnull name, TWData* _Nonnull payload) {
    auto moveAction = IoTeX::Proto::StakeMove();
    moveAction.set_bucketindex(index);
    auto n = dataFromTWData(name);
    moveAction.set_name(hex(*n));
    auto p = dataFromTWData(payload);
    moveAction.set_payload(hex(*p));
    auto s = moveAction.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}
