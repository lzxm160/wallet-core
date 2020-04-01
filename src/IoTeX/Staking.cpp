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
#include "TWData.h"
namespace TW::IoTeX {

// using namespace TW::Ethereum::ABI;
using namespace TW;
const char* charFromTWData(const Data& data) {
    return reinterpret_cast<const char *>(&data[0]);
    // auto v = const_cast<std::vector<uint8_t>*>(reinterpret_cast<const std::vector<uint8_t>*>(data));
    // auto bytes=v->data();
    // auto s = new std::string(bytes, bytes + v->size());
    // // append null terminator
    // s->append(size, '\0');
    // return s->data();
    // return TWStringUTF8Bytes(TWStringCreateWithRawBytes(TWDataBytes(data), TWDataSize(data)));
}
void stakingCreate(const Data& candidate, const Data& amount,uint32_t duration, bool autoStake, const Data& payload){
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

void stakingAddDeposit(uint64_t index, const Data& amount,const Data& payload){
    auto action = IoTeX::Proto::StakeAddDeposit();
    action.set_bucketindex(index);
    action.set_amount(charFromTWData(amount));
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

void stakingUnstake(uint64_t index, const Data& payload){
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

void stakingWithdraw(uint64_t index, const Data& payload){
    auto action = IoTeX::Proto::StakeReclaim();
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

void stakingRestake(uint64_t index, uint32_t duration, bool autoStake,const Data& payload){
    auto action = IoTeX::Proto::StakeRestake();
    action.set_bucketindex(index);
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

void stakingChangeCandidate(const Data& candidate, uint64_t index,const Data& payload){
    auto action = IoTeX::Proto::StakeMove();
    action.set_name(charFromTWData(candidate));
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

void stakingTransfer(const Data& candidate, uint64_t index,const Data& payload){
    auto action = IoTeX::Proto::StakeMove();
    action.set_name(charFromTWData(candidate));
    action.set_bucketindex(index);
    action.set_payload(charFromTWData(payload));
    auto s = action.SerializeAsString();
    auto actionHex = hex(s.begin(), s.end());
    return TWDataCreateWithHexString(&actionHex);
}

void candidateRegister(const Data& name,const Data& operatorAddress,const Data& rewardAddress,const Data& amount,uint32_t duration, bool autoStake, const Data& ownerAddress,const Data& payload){
    
}

void candidateUpdate(const Data& name,const Data& operatorAddress,const Data& rewardAddress){
    
}
// void stakingStake(const Data& candidate, uint64_t stakeDuration, bool nonDecay, const Data& dataIn, Data& dataOut) {
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

// void stakingMoveStake(uint64_t pyggIndex, const Data& candidate, const Data& dataIn, Data& dataOut) {
//     Function func("revote");
//     func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
//     func.addInParam(std::make_shared<ParamByteArrayFix>(12, candidate));
//     func.addInParam(std::make_shared<ParamByteArray>(dataIn));

//     func.encode(dataOut);
// }

} // namespace TW::IoTeX
