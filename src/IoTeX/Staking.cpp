// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Staking.h"

// #include "Ethereum/ABI/Function.h"
#include "Data.h"
// #include "uint256.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWString.h>
using namespace TW;

namespace TW::IoTeX {

const char* charFromTWData(TWData* _Nonnull data) {
    // return (char*)data;
    // auto v = const_cast<std::vector<uint8_t>*>(reinterpret_cast<const
    // std::vector<uint8_t>*>(data)); auto bytes=v->data(); auto s = new std::string(bytes, bytes +
    // v->size());
    // // append null terminator
    // s->append(size, '\0');
    // return s->data();
    return TWStringUTF8Bytes(TWStringCreateWithRawBytes(TWDataBytes(data), TWDataSize(data)));
    // return TWStringUTF8Bytes(TWStringCreateWithHexData(data));
}

TWData* _Nullable stakingCreate(TWData* _Nonnull candidate, TWData* _Nonnull amount,
                                uint32_t duration, bool autoStake, TWData* payload) {
    auto action = IoTeX::Proto::StakeCreate();
    action.set_candidatename(charFromTWData(candidate));
    action.set_stakedamount(charFromTWData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable stakingAddDeposit(uint64_t index, TWData* _Nonnull amount, TWData* payload) {
    auto action = IoTeX::Proto::StakeAddDeposit();
    action.set_bucketindex(index);
    action.set_amount(charFromTWData(amount));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable stakingUnstake(uint64_t index, TWData* payload) {
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable stakingWithdraw(uint64_t index, TWData* payload) {
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable stakingRestake(uint64_t index, uint32_t duration, bool autoStake,
                                 TWData* payload) {
    auto action = IoTeX::Proto::StakeRestake();
    action.set_bucketindex(index);
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable stakingChangeCandidate(uint64_t index, TWData* _Nonnull candidate,
                                         TWData* payload) {
    auto action = IoTeX::Proto::StakeChangeCandidate();
    action.set_bucketindex(index);
    action.set_candidatename(charFromTWData(candidate));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable stakingTransfer(uint64_t index, TWData* _Nonnull voterAddress, TWData* payload) {
    auto action = IoTeX::Proto::StakeTransferOwnership();
    action.set_bucketindex(index);
    action.set_voteraddress(charFromTWData(voterAddress));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable candidateRegister(TWData* _Nonnull name, TWData* _Nonnull operatorAddress,
                                    TWData* _Nonnull rewardAddress, TWData* _Nonnull amount,
                                    uint32_t duration, bool autoStake,
                                    TWData* _Nonnull ownerAddress, TWData* payload) {
    auto cbi = IoTeX::Proto::CandidateBasicInfo();
    cbi.set_name(charFromTWData(name));
    cbi.set_operatoraddress(charFromTWData(operatorAddress));
    cbi.set_rewardaddress(charFromTWData(rewardAddress));

    auto action = IoTeX::Proto::CandidateRegister();
    action.set_allocated_candidate(&cbi);
    action.set_stakedamount(charFromTWData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_owneraddress(charFromTWData(ownerAddress));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

TWData* _Nullable candidateUpdate(TWData* _Nonnull name, TWData* _Nonnull operatorAddress,
                                  TWData* _Nonnull rewardAddress) {
    auto action = IoTeX::Proto::CandidateBasicInfo();
    action.set_name(charFromTWData(name));
    action.set_operatoraddress(charFromTWData(operatorAddress));
    action.set_rewardaddress(charFromTWData(rewardAddress));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}
}