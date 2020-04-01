// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

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
void stakingCreate(const Data& candidate, const Data& amount,uint32_t duration, bool autoStake, const Data& payload);

/// Function to generate AddDeposit message
void stakingAddDeposit(uint64_t index, const Data& amount,const Data& payload);

/// Function to generate Unstake message
void stakingUnstake(uint64_t index, const Data& payload);

/// Function to generate Withdraw message
void stakingWithdraw(uint64_t index, const Data& payload);

/// Function to generate Restake message
void stakingRestake(uint64_t index, uint32_t duration, bool autoStake,const Data& payload);

/// Function to generate ChangeCandidate message
void stakingChangeCandidate(const Data& candidate, uint64_t index,const Data& payload);

/// Function to generate Transfer message
void stakingTransfer(const Data& candidate, uint64_t index,const Data& payload);

/// Function to generate candidate register message
void candidateRegister(const CandidateBasicInfo& candidate,const Data& amount,uint32_t duration,, bool autoStake, const Data& ownerAddress,const Data& payload);

} // namespace TW::IoTeX
