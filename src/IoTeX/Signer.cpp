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
    auto signedAction = iotextypes::Action();
    signedAction.mutable_core()->MergeFrom(action);
    auto key = PrivateKey(input.privatekey());
    auto pk = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended).bytes;
    signedAction.set_senderpubkey(pk.data(), pk.size());
    auto sig = key.sign(hash(), TWCurveSECP256k1);
    signedAction.set_signature(sig.data(), sig.size());

    auto output = IoTeX::Proto::SigningOutput();
    auto serialized = signedAction.SerializeAsString();
    output.set_encoded(serialized);
    auto h = Hash::keccak256(serialized);
    output.set_hash(h.data(), h.size());
    return output;
}

Data Signer::hash() const {
    return Hash::keccak256(action.SerializeAsString());
}

static Data encodeStaking(const Proto::Staking& staking) {
    Data encoded;
    if (staking.has_stakecreate()) {
        auto& stake = staking.stakecreate();
        stakingStake(TW::data(stake.candidate()), stake.duration(), stake.nondecay(),
                     TW::data(stake.data()), encoded);
    } else if (staking.has_stakeunstake()) {
        auto& unstake = staking.stakeunstake();
    } else if (staking.has_stakewithdraw()) {
        auto& withdraw = staking.stakewithdraw();
    } else if (staking.has_stakeadddeposit()) {

    } else if (staking.has_stakerestake()) {
    } else if (staking.has_stakechangecandidate()) {
    } else if (staking.has_staketransferownership()) {
    } else if (staking.has_candidateregister()) {
    } else if (staking.has_candidateupdate()) {
    }
    return encoded;
}
void Signer::toActionCore() {
    if (input.has_staking()) {
        action.set_version(input.version());
        action.set_nonce(input.nonce());
        action.set_gaslimit(input.gaslimit());
        action.set_gasprice(input.gasprice());
        auto& staking = input.staking();
        auto encoded = encodeStaking(staking);
        auto& execution = *action.mutable_execution();
        execution.set_amount(staking.amount());
        execution.set_contract(staking.contract());
        execution.set_data(encoded.data(), encoded.size());
    } else {
        // ActionCore is almost same as SigningInput, missing field privateKey = 5;
        action.ParseFromString(input.SerializeAsString());
        action.DiscardUnknownFields();
    }
}
