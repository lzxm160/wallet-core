// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWIoTeXStaking;

/// Function to generate Stake message
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWIoTeXStakingCreate(TWData* _Nonnull candidate, TWData* _Nonnull amount,uint32_t duration, bool autoStake, TWData* _Nonnull payload);

/// Function to generate AddDeposit message
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWIoTeXStakingAddDeposit(uint64_t index, TWData* _Nonnull amount,TWData* _Nonnull payload);

/// Function to generate Unstake message
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWIoTeXStakingUnstake(uint64_t index, TWData* _Nonnull payload);

/// Function to generate Withdraw message
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWIoTeXStakingWithdraw(uint64_t index, TWData* _Nonnull payload);

/// Function to generate Restake message
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWIoTeXStakingRestake(uint64_t index, uint32_t duration, bool autoStake,TWData* _Nonnull payload);

/// Function to generate Move message
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWIoTeXStakingMove(uint64_t index, TWData* _Nonnull name,TWData* _Nonnull payload);

TW_EXTERN_C_END
