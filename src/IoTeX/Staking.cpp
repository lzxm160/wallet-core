// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Staking.h"
#include "Data.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWString.h>
using namespace TW;

namespace TW::IoTeX {

const char* charFromTWData(const Data& data) {
    // return TWStringUTF8Bytes(TWStringCreateWithRawBytes(TWDataBytes(data), TWDataSize(data)));
    std::stringstream oss;
    for (unsigned int i = 0; i < data.size(); i++) {
        oss << data[i];
    }
    string temp = oss.str();
    return temp.c_str();
}

const Data& stakingCreate(const Data& candidate, const Data& amount, uint32_t duration,
                          bool autoStake, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeCreate();
    action.set_candidatename(charFromTWData(candidate));
    action.set_stakedamount(charFromTWData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& stakingAddDeposit(uint64_t index, const Data& amount, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeAddDeposit();
    action.set_bucketindex(index);
    action.set_amount(charFromTWData(amount));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& stakingUnstake(uint64_t index, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& stakingWithdraw(uint64_t index, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& stakingRestake(uint64_t index, uint32_t duration, bool autoStake, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeRestake();
    action.set_bucketindex(index);
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& stakingChangeCandidate(uint64_t index, const Data& candidate, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeChangeCandidate();
    action.set_bucketindex(index);
    action.set_candidatename(charFromTWData(candidate));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& stakingTransfer(uint64_t index, const Data& voterAddress, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeTransferOwnership();
    action.set_bucketindex(index);
    action.set_voteraddress(charFromTWData(voterAddress));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& candidateRegister(const Data& name, const Data& operatorAddress,
                              const Data& rewardAddress, const Data& amount, uint32_t duration,
                              bool autoStake, const Data& ownerAddress, const Data& payload) {
    auto cbi = new IoTeX::Proto::Staking_CandidateBasicInfo();
    cbi->set_name(charFromTWData(name));
    cbi->set_operatoraddress(charFromTWData(operatorAddress));
    cbi->set_rewardaddress(charFromTWData(rewardAddress));

    auto action = IoTeX::Proto::Staking_CandidateRegister();
    action.set_allocated_candidate(cbi);
    action.set_stakedamount(charFromTWData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_owneraddress(charFromTWData(ownerAddress));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}

const Data& candidateUpdate(const Data& name, const Data& operatorAddress,
                            const Data& rewardAddress) {
    auto action = IoTeX::Proto::Staking_CandidateBasicInfo();
    action.set_name(charFromTWData(name));
    action.set_operatoraddress(charFromTWData(operatorAddress));
    action.set_rewardaddress(charFromTWData(rewardAddress));
    auto s = action.SerializeAsString();
    return Data(s.begin(), s.end());
}
} // namespace TW::IoTeX
