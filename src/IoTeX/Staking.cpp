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

const char* charFromData(const Data& data) {
    // return TWStringUTF8Bytes(TWStringCreateWithRawBytes(TWDataBytes(data), TWDataSize(data)));
    // std::stringstream oss;
    // for (unsigned int i = 0; i < data.size(); i++) {
    //     oss << data[i];
    // }
    // std::string temp = oss.str();
    // return temp.c_str();
    // return (const char*)data.data();
    // std::string str = new std::string;
    // str.assign(data.begin(), data.end());
    // return str.c_str();
    char* ret = new char[data.size() + 1]; // init this with the correct size
    std::copy(data.begin(), data.end(), ret);
    ret[data.size()] = '\0';
    return ret;
}

const Data& stakingCreate(const Data& candidate, const Data& amount, uint32_t duration,
                          bool autoStake, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeCreate();
    action.set_candidatename(charFromData(candidate));
    action.set_stakedamount(charFromData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& stakingAddDeposit(uint64_t index, const Data& amount, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeAddDeposit();
    action.set_bucketindex(index);
    action.set_amount(charFromData(amount));
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& stakingUnstake(uint64_t index, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& stakingWithdraw(uint64_t index, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& stakingRestake(uint64_t index, uint32_t duration, bool autoStake, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeRestake();
    action.set_bucketindex(index);
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& stakingChangeCandidate(uint64_t index, const Data& candidate, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeChangeCandidate();
    action.set_bucketindex(index);
    action.set_candidatename(charFromData(candidate));
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& stakingTransfer(uint64_t index, const Data& voterAddress, const Data& payload) {
    auto action = IoTeX::Proto::Staking_StakeTransferOwnership();
    action.set_bucketindex(index);
    action.set_voteraddress(charFromData(voterAddress));
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& candidateRegister(const Data& name, const Data& operatorAddress,
                              const Data& rewardAddress, const Data& amount, uint32_t duration,
                              bool autoStake, const Data& ownerAddress, const Data& payload) {
    auto cbi = new IoTeX::Proto::Staking_CandidateBasicInfo();
    cbi->set_name(charFromData(name));
    cbi->set_operatoraddress(charFromData(operatorAddress));
    cbi->set_rewardaddress(charFromData(rewardAddress));

    auto action = IoTeX::Proto::Staking_CandidateRegister();
    action.set_allocated_candidate(cbi);
    action.set_stakedamount(charFromData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_owneraddress(charFromData(ownerAddress));
    action.set_payload(charFromData(payload));
    return data(action.SerializeAsString());
}

const Data& candidateUpdate(const Data& name, const Data& operatorAddress,
                            const Data& rewardAddress) {
    auto action = IoTeX::Proto::Staking_CandidateBasicInfo();
    action.set_name(charFromData(name));
    action.set_operatoraddress(charFromData(operatorAddress));
    action.set_rewardaddress(charFromData(rewardAddress));
    return data(action.SerializeAsString());
}
} // namespace TW::IoTeX
