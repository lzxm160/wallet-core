// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/IoTeX.pb.h"
#include <TrustWalletCore/TWData.h>
namespace TW::IoTeX {

// /// Function to generate Stake message
// void stakingStake(const Data& candidate, uint64_t stakeDuration, bool nonDecay, const Data&
// dataIn, Data& dataOut);

// /// Function to generate Unstake message
// void stakingUnstake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

// /// Function to generate Withdraw message
// void stakingWithdraw(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

// /// Function to generate AddStake message
// void stakingAddStake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut);

// /// Function to generate MoveStake message
// void stakingMoveStake(uint64_t pyggIndex, const Data& candidate, const Data& dataIn, Data&
// dataOut); struct TWIoTeXStaking;

/// Function to generate Create message
const char* stakingCreate(const char* candidate, const char* amount, uint32_t duration,
                          bool autoStake, const char* payload);

/// Function to generate AddDeposit message
const char* stakingAddDeposit(uint64_t index, const char* amount, const char* payload);

/// Function to generate Unstake message
const char* stakingUnstake(uint64_t index, const char* payload);

/// Function to generate Withdraw message
const char* stakingWithdraw(uint64_t index, const char* payload);

/// Function to generate Restake message
const char* stakingRestake(uint64_t index, uint32_t duration, bool autoStake, const char* payload);

/// Function to generate ChangeCandidate message
const char* stakingChangeCandidate(uint64_t index, const char* candidate, const char* payload);

/// Function to generate Transfer message
const char* stakingTransfer(uint64_t index, const char* voterAddress, const char* payload);

/// Function to generate candidate register message
const char* candidateRegister(const char* name, const char* operatorAddress,
                              const char* rewardAddress, const char* amount, uint32_t duration,
                              bool autoStake, const char* ownerAddress, const char* payload);

/// Function to generate candidate update message
const char* candidateUpdate(const char* name, const char* operatorAddress,
                            const char* rewardAddress);

} // namespace TW::IoTeX
