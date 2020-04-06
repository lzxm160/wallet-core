// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Hash.h"
#include "HexCoding.h"
#include "IoTeX/Staking.h"
#include "PrivateKey.h"

using namespace TW;
using namespace TW::IoTeX;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto signer = Signer(input);
    return signer.build();
}

Data Signer::sign() const {
    auto key = PrivateKey(input.privatekey());
    return key.sign(hash(), TWCurveSECP256k1);
}

Proto::SigningOutput Signer::build() const {
    auto signedAction = Proto::Action();
    signedAction.mutable_core()->MergeFrom(action);
    auto key = PrivateKey(input.privatekey());
    auto pk = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended).bytes;
    signedAction.set_senderpubkey(pk.data(), pk.size());
    auto sig = key.sign(hash(), TWCurveSECP256k1);
    signedAction.set_signature(sig.data(), sig.size());

    auto output = Proto::SigningOutput();
    auto serialized = signedAction.SerializeAsString();
    output.set_encoded(serialized);
    auto h = Hash::keccak256(serialized);
    output.set_hash(h.data(), h.size());
    return output;
}

Data Signer::hash() const {
    return Hash::keccak256(action.SerializeAsString());
}

void Signer::toActionCore() {
    action.set_version(input.version());
    action.set_nonce(input.nonce());
    action.set_gaslimit(input.gaslimit());
    action.set_gasprice(input.gasprice());
    auto staking = input.staking();
    switch (staking.message_case()) {
    case Proto::Staking::kStakeCreate: {
        auto& stake = staking.stakecreate();
        auto ss = new IoTeX::Proto::Staking_StakeCreate();
        ss->CopyFrom(stake);
        action.set_allocated_stakecreate(ss);
        return;
    }
    case kStakeUnstake: {
        auto& unstake = staking.stakeunstake();
        auto ss = new IoTeX::Proto::Staking_StakeReclaim();
        ss->CopyFrom(unstake);
        action.set_allocated_stakeunstake(ss);
        return;
    }
    case kStakeWithdraw: {
        auto& withdraw = staking.stakewithdraw();
        auto ss = new IoTeX::Proto::Staking_StakeReclaim();
        ss->CopyFrom(withdraw);
        action.set_allocated_stakewithdraw(ss);
        return;
    }
    case kStakeAddDeposit: {
        auto& adddeposit = staking.stakeadddeposit();
        auto ss = new IoTeX::Proto::Staking_StakeAddDeposit();
        ss->CopyFrom(adddeposit);
        action.set_allocated_stakeadddeposit(ss);
        return;
    }
    case kStakeRestake: {
        auto& restake = staking.stakerestake();
        auto ss = new IoTeX::Proto::Staking_StakeRestake();
        ss->CopyFrom(restake);
        action.set_allocated_stakerestake(ss);
        return;
    }
    case kStakeChangeCandidate: {
        auto& changecandidate = staking.stakechangecandidate();
        auto ss = new IoTeX::Proto::Staking_StakeChangeCandidate();
        ss->CopyFrom(changecandidate);
        action.set_allocated_stakechangecandidate(ss);
        return;
    }
    case kStakeTransferOwnership: {
        auto& transfer = staking.staketransferownership();
        auto ss = new IoTeX::Proto::Staking_StakeTransferOwnership();
        ss->CopyFrom(transfer);
        action.set_allocated_staketransferownership(ss);
        return;
    }
    case kCandidateRegister: {
        auto& candidateregister = staking.candidateregister();

        auto cbi = new IoTeX::Proto::Staking_CandidateBasicInfo();
        cbi->CopyFrom(candidateregister.candidate());

        auto ss = new IoTeX::Proto::Staking_CandidateRegister();
        ss->set_allocated_candidate(cbi);
        ss->CopyFrom(candidateregister);
        action.set_allocated_candidateregister(ss);
        return;
    }
    case kCandidateUpdate: {
        auto& candidateupdate = staking.candidateupdate();
        auto cbi = new IoTeX::Proto::Staking_CandidateBasicInfo();
        cbi->CopyFrom(candidateupdate);
        action.set_allocated_candidateupdate(cbi);
        return;
    }
    case MESSAGE_NOT_SET: {
        break;
    }
    }
    action.ParseFromString(input.SerializeAsString());
    action.DiscardUnknownFields();
}
