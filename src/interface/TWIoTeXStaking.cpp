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
    auto ret = const_cast<std::vector<uint8_t>*>(reinterpret_cast<const std::vector<uint8_t>*>(data));
    std::string str;
    str.assign(ret->begin(), ret->end());
    return str;
}
inline std::vector<uint8_t>* dataFromTWData(TWData* data) {
    return const_cast<std::vector<uint8_t>*>(reinterpret_cast<const std::vector<uint8_t>*>(data));
}

/// Function to generate Create message
TWData* _Nonnull TWIoTeXStakingCreate(TWData* _Nonnull candidate, TWData* _Nonnull amount,uint32_t duration, bool autoStake,TWData* _Nonnull payload) {
    auto action = IoTeX::Proto::StakeCreate();
    action.set_candidatename(stringFromTWData(candidate)); 
    action.set_stakedamount(stringFromTWData(amount));
    action.set_stakedduration(duration); 
    action.set_autostake(autoStake);
    action.set_payload(stringFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end()); 
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate Unstake message
TWData* _Nonnull TWIoTeXStakingUnstake(uint64_t index,TWData* _Nonnull payload) {
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(stringFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate withdraw message
TWData* _Nonnull TWIoTeXStakingWithdraw(uint64_t index, TWData* _Nonnull payload) {
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(stringFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate AddDeposit message
TWData* _Nonnull TWIoTeXStakingAddDeposit(uint64_t index, TWData* _Nonnull amount,TWData* _Nonnull payload) {
    auto action = IoTeX::Proto::StakeAddDeposit();
    action.set_bucketindex(index);
    action.set_amount(stringFromTWData(amount));
    action.set_payload(stringFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate Restake message
TWData* _Nonnull TWIoTeXStakingRestake(uint64_t index, uint32_t duration,bool autoStake,TWData* _Nonnull payload) {
    auto action = IoTeX::Proto::StakeRestake();
    action.set_bucketindex(index);
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(stringFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate ChangeCandidate message
TWData* _Nonnull TWIoTeXStakingChangeCandidate(TWData* _Nonnull candidate, uint64_t index,TWData* _Nonnull payload) {
    auto action = IoTeX::Proto::StakeMove();
    action.set_name(stringFromTWData(candidate));
    action.set_bucketindex(index);
    action.set_payload(stringFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

/// Function to generate Transfer message
TWData* _Nonnull TWIoTeXStakingTransfer(TWData* _Nonnull candidate, uint64_t index,
                                        TWData* _Nonnull payload) {
    auto action = IoTeX::Proto::StakeMove();
    action.set_name(stringFromTWData(candidate));
    action.set_bucketindex(index);
    action.set_payload(stringFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}