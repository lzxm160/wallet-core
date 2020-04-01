// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/IoTeX.pb.h"
#include <TrustWalletCore/TWData.h>
namespace TW::IoTeX {

// /// Function to generate Stake message
// void stakingStake(const Data& candidate, uint64_t stakeDuration, bool nonDecay, const Data& dataIn, Data& dataOut);

// /// Function to generate Unstake message
// void stakingUnstake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

// /// Function to generate Withdraw message
// void stakingWithdraw(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

// /// Function to generate AddStake message
// void stakingAddStake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

// /// Function to generate MoveStake message
// void stakingMoveStake(uint64_t pyggIndex, const Data& candidate, const Data& dataIn, Data& dataOut);
// struct TWIoTeXStaking;

/// Function to generate Create message
TWData* _Nonnull stakingCreate(const Data& candidate, const Data& amount,uint32_t duration, bool autoStake, const Data& payload);

/// Function to generate AddDeposit message
TWData* _Nonnull stakingAddDeposit(uint64_t index, const Data& amount,const Data& payload);

/// Function to generate Unstake message
TWData* _Nonnull stakingUnstake(uint64_t index, const Data& payload);

/// Function to generate Withdraw message
TWData* _Nonnull stakingWithdraw(uint64_t index, const Data& payload);

/// Function to generate Restake message
TWData* _Nonnull stakingRestake(uint64_t index, uint32_t duration, bool autoStake,const Data& payload);

/// Function to generate ChangeCandidate message
TWData* _Nonnull stakingChangeCandidate(uint64_t index,const Data& candidate, const Data& payload);

/// Function to generate Transfer message
TWData* _Nonnull stakingTransfer(uint64_t index,const Data& voterAddress, const Data& payload);

/// Function to generate candidate register message
TWData* _Nonnull candidateRegister(const Data& name,const Data& operatorAddress,const Data& rewardAddress,const Data& amount,uint32_t duration, bool autoStake, const Data& ownerAddress,const Data& payload);

/// Function to generate candidate update message
TWData* _Nonnull candidateUpdate(const Data& name,const Data& operatorAddress,const Data& rewardAddress);

} // namespace TW::IoTeX
