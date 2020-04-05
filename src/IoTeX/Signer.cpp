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
    const Proto::Staking& staking = input.staking();
    bool has_stakecreate = staking.has_stakecreate();
    bool has_stakeunstake = staking.has_stakeunstake();
    bool has_stakewithdraw = staking.has_stakewithdraw();
    bool has_stakeadddeposit = staking.has_stakeadddeposit();
    bool has_stakerestake = staking.has_stakerestake();
    bool has_stakechangecandidate = staking.has_stakechangecandidate();
    bool has_staketransferownership = staking.has_staketransferownership();
    bool has_candidateregister = staking.has_candidateregister();
    bool has_candidateupdate = staking.has_candidateupdate();
    if (has_stakecreate) {
        auto& stake = staking.stakecreate();
        auto ss = new IoTeX::Proto::Staking_StakeCreate();
        ss->set_candidatename(stake.candidatename());
        ss->set_stakedamount(stake.stakedamount());
        ss->set_stakedduration(stake.stakedduration());
        ss->set_autostake(stake.autostake());
        ss->set_payload(stake.payload());
        action.set_allocated_stakecreate(ss);
        return;
    }
    if (has_stakeunstake) {
        auto& unstake = staking.stakeunstake();
        auto ss = new IoTeX::Proto::Staking_StakeReclaim();
        ss->set_bucketindex(unstake.bucketindex());
        ss->set_payload(unstake.payload());
        action.set_allocated_stakeunstake(ss);
        return;
    }
    if (has_stakewithdraw) {
        auto& withdraw = staking.stakewithdraw();
        return;
    }
    if (has_stakeadddeposit) {
        auto& adddeposit = staking.stakeadddeposit();
        return;
    }
    if (has_stakerestake) {
        auto& restake = staking.stakerestake();
        return;
    }
    if (has_stakechangecandidate) {
        auto& changecandidate = staking.stakerestake();
        return;
    }
    if (has_staketransferownership) {
        auto& transfer = staking.staketransferownership();
        return;
    }
    if (has_candidateregister) {
        auto& candidateregister = staking.candidateregister();
        return;
    }
    if (has_candidateupdate) {
        auto& candidateupdate = staking.candidateupdate();
        return;
    }
    action.ParseFromString(input.SerializeAsString());
    action.DiscardUnknownFields();
}
