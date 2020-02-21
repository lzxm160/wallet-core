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

using namespace TW;

TWData* _Nonnull TWIoTeXStakingCreate(TWData* _Nonnull candidate, TWData* _Nonnull amount,
                                      uint32_t duration, bool autoStake,
                                      TWData* _Nonnull payload) {
    auto stakeAction = IoTeX::Proto::StakeCreate();
    stakeAction.set_candidatename(candidate); 
    stakeAction.set_stakedamount(amount);
    stakeAction.set_stakedduration(duration); 
    stakeAction.set_autostake(autoStake);
    stakeAction.set_payload(data);
    return TWDataCreateWithHexString(stakeAction.SerializeAsString());
}

/// Function to generate Unstake message
TWData* _Nonnull TWIoTeXStakingUnstake(uint64_t pyggIndex, TWData* _Nonnull data) {
    // TWEthereumAbiFunction* func =
    // TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("unstake"));
    // assert(func != nullptr);

    // auto v = store(uint256_t(pyggIndex));
    // TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    // TWEthereumAbiFunctionAddParamBytes(func, data, false);

    // auto ret = TWEthereumAbiEncoderEncode(func);
    // TWEthereumAbiEncoderDeleteFunction(func);
    // return ret;
    return 0;
}

/// Function to generate Withdraw message
TWData* _Nonnull TWIoTeXStakingWithdraw(uint64_t pyggIndex, TWData* _Nonnull data) {
    // TWEthereumAbiFunction* func =
    // TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("withdraw"));
    // assert(func != nullptr);

    // auto v = store(uint256_t(pyggIndex));
    // TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    // TWEthereumAbiFunctionAddParamBytes(func, data, false);

    // auto ret = TWEthereumAbiEncoderEncode(func);
    // TWEthereumAbiEncoderDeleteFunction(func);
    // return ret;
    return 0;
}

/// Function to generate AddStake message
TWData* _Nonnull TWIoTeXStakingAddStake(uint64_t pyggIndex, TWData* _Nonnull data) {
    // TWEthereumAbiFunction* func =
    // TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("storeToPygg"));
    // assert(func != nullptr);

    // auto v = store(uint256_t(pyggIndex));
    // TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    // TWEthereumAbiFunctionAddParamBytes(func, data, false);

    // auto ret = TWEthereumAbiEncoderEncode(func);
    // TWEthereumAbiEncoderDeleteFunction(func);
    // return ret;
    return 0;
}

/// Function to generate MoveStake message
TWData* _Nonnull TWIoTeXStakingMoveStake(uint64_t pyggIndex, TWData* _Nonnull candidate, TWData* _Nonnull data) {
    // TWEthereumAbiFunction* func =
    // TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("revote"));
    // assert(func != nullptr);

    // auto v = store(uint256_t(pyggIndex));
    // TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    // TWEthereumAbiFunctionAddParamBytesFix(func, 12, candidate, false);
    // TWEthereumAbiFunctionAddParamBytes(func, data, false);

    // auto ret = TWEthereumAbiEncoderEncode(func);
    // TWEthereumAbiEncoderDeleteFunction(func);
    // return ret;
    return 0;
}
