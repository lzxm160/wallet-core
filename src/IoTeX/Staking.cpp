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
namespace TW::IoTeX {

// using namespace TW::Ethereum::ABI;
using namespace TW;
const char* charFromTWData(const char* data) {
    // return (char*)data;
    // auto v = const_cast<std::vector<uint8_t>*>(reinterpret_cast<const
    // std::vector<uint8_t>*>(data)); auto bytes=v->data(); auto s = new std::string(bytes, bytes +
    // v->size());
    // // append null terminator
    // s->append(size, '\0');
    // return s->data();
    //     return TWStringUTF8Bytes(TWStringCreateWithRawBytes(TWDataBytes(data),
    //     TWDataSize(data)));
    //     // return TWStringUTF8Bytes(TWStringCreateWithHexData(data));
    return data;
}
const char* stakingCreate(const char* candidate, const char* amount, uint32_t duration,
                          bool autoStake, const char* payload) {
    auto action = IoTeX::Proto::StakeCreate();
    action.set_candidatename(charFromTWData(candidate));
    action.set_stakedamount(charFromTWData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    // auto actionHex = hex(s.begin(), s.end());
    // return TWDataCreateWithHexString(&actionHex);
    return s.get();
}

const char* stakingAddDeposit(uint64_t index, const char* amount, const char* payload) {
    auto action = IoTeX::Proto::StakeAddDeposit();
    action.set_bucketindex(index);
    action.set_amount(charFromTWData(amount));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

const char* stakingUnstake(uint64_t index, const char* payload) {
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

const char* stakingWithdraw(uint64_t index, const char* payload) {
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

const char* stakingRestake(uint64_t index, uint32_t duration, bool autoStake, const char* payload) {
    auto action = IoTeX::Proto::StakeRestake();
    action.set_bucketindex(index);
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

const char* stakingChangeCandidate(uint64_t index, const char* candidate, const char* payload) {
    auto action = IoTeX::Proto::StakeChangeCandidate();
    action.set_bucketindex(index);
    action.set_candidatename(charFromTWData(candidate));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

const char* stakingTransfer(uint64_t index, const char* voterAddress, const char* payload) {
    auto action = IoTeX::Proto::StakeTransferOwnership();
    action.set_bucketindex(index);
    action.set_voteraddress(charFromTWData(voterAddress));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

const char* candidateRegister(const char* name, const char* operatorAddress,
                              const char* rewardAddress, const char* amount, uint32_t duration,
                              bool autoStake, const char* ownerAddress, const char* payload) {
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

const char* candidateUpdate(const char* name, const char* operatorAddress,
                            const char* rewardAddress) {
    auto action = IoTeX::Proto::CandidateBasicInfo();
    action.set_name(charFromTWData(name));
    action.set_operatoraddress(charFromTWData(operatorAddress));
    action.set_rewardaddress(charFromTWData(rewardAddress));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}
// void stakingStake(const Data& candidate, uint64_t stakeDuration, bool nonDecay, const Data&
// dataIn, Data& dataOut) {
//     Function func("createPygg");
//     func.addInParam(std::make_shared<ParamByteArrayFix>(12, candidate));
//     func.addInParam(std::make_shared<ParamUInt256>(uint256_t(stakeDuration)));
//     func.addInParam(std::make_shared<ParamBool>(nonDecay));
//     func.addInParam(std::make_shared<ParamByteArray>(dataIn));

//     func.encode(dataOut);
// }

// void stakingUnstake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut) {
//     Function func("unstake");
//     func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
//     func.addInParam(std::make_shared<ParamByteArray>(dataIn));

//     func.encode(dataOut);
// }

// void stakingWithdraw(uint64_t pyggIndex, const Data& dataIn, Data& dataOut) {
//     Function func("withdraw");
//     func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
//     func.addInParam(std::make_shared<ParamByteArray>(dataIn));

//     func.encode(dataOut);
// }

// void stakingAddStake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut) {
//     Function func("storeToPygg");
//     func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
//     func.addInParam(std::make_shared<ParamByteArray>(dataIn));

//     func.encode(dataOut);
// }

// void stakingMoveStake(uint64_t pyggIndex, const Data& candidate, const Data& dataIn, Data&
// dataOut) {
//     Function func("revote");
//     func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
//     func.addInParam(std::make_shared<ParamByteArrayFix>(12, candidate));
//     func.addInParam(std::make_shared<ParamByteArray>(dataIn));

//     func.encode(dataOut);
// }

} // namespace TW::IoTeX
