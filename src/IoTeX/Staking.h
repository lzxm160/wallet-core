// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/IoTeX.pb.h"
#include <TrustWalletCore/TWData.h>
namespace TW::IoTeX {

/// Function to generate Create message
TWData* _Nullable stakingCreate(TWData* _Nonnull candidate, TWData* _Nonnull amount,
                                uint32_t duration, bool autoStake, TWData* _Nonnull payload);

/// Function to generate AddDeposit message
TWData* _Nullable stakingAddDeposit(uint64_t index, TWData* _Nonnull amount,
                                    TWData* _Nonnull payload);

/// Function to generate Unstake message
TWData* _Nullable stakingUnstake(uint64_t index, TWData* _Nonnull payload);

/// Function to generate Withdraw message
TWData* _Nullable stakingWithdraw(uint64_t index, TWData* _Nonnull payload);

/// Function to generate Restake message
TWData* _Nullable stakingRestake(uint64_t index, uint32_t duration, bool autoStake,
                                 TWData* _Nonnull payload);

/// Function to generate ChangeCandidate message
TWData* _Nullable stakingChangeCandidate(uint64_t index, TWData* _Nonnull candidate,
                                         TWData* _Nonnull payload);

/// Function to generate Transfer message
TWData* _Nullable stakingTransfer(uint64_t index, TWData* _Nonnull voterAddress,
                                  TWData* _Nonnull payload);

/// Function to generate candidate register message
TWData* _Nullable candidateRegister(TWData* _Nonnull name, TWData* _Nonnull operatorAddress,
                                    TWData* _Nonnull rewardAddress, TWData* _Nonnull amount,
                                    uint32_t duration, bool autoStake,
                                    TWData* _Nonnull ownerAddress, TWData* _Nonnull payload);

/// Function to generate candidate update message
TWData* _Nullable candidateUpdate(TWData* _Nonnull name, TWData* _Nonnull operatorAddress,
                                  TWData* _Nonnull rewardAddress);

} // namespace TW::IoTeX
