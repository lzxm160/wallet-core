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

// static Data encodeStaking(const Proto::Staking& staking) {
//     Data encoded;
//     switch (1) {
//     case staking.has_stakecreate():
//         auto& stake = staking.stakecreate();
//         encoded =
//             stakingCreate(TW::data(stake.candidatename()), TW::data(stake.stakedamount()),
//                           stake.stakedduration(), stake.autoStake(), TW::data(stake.payload()));
//         break;
//     case staking.has_stakeunstake():
//         auto& unstake = staking.stakeunstake();
//         encoded = stakingUnstake(unstake.bucketindex(), TW::data(unstake.payload()));
//         break;
//     case staking.has_stakewithdraw():
//         auto& withdraw = staking.stakewithdraw();
//         encoded = stakingWithdraw(withdraw.bucketindex(), TW::data(withdraw.payload()));
//         break;
//     case staking.has_stakeadddeposit():
//         auto& adddeposit = staking.stakeadddeposit();
//         encoded = stakingAddDeposit(adddeposit.bucketindex(), TW::data(adddeposit.payload()));
//         break;
//     case staking.has_stakerestake():
//         auto& restake = staking.stakerestake();
//         encoded = stakingRestake(restake.bucketindex(), restake.stakedduration(),
//                                  restake.autostake(), TW::data(restake.payload()));
//         break;
//     case staking.has_stakechangecandidate():
//         auto& changecandidate = staking.stakerestake();
//         encoded = stakingRestake(changecandidate.bucketindex(), changecandidate.stakedduration(),
//                                  changecandidate.autostake(),
//                                  TW::data(changecandidate.payload()));
//         break;
//     case staking.has_staketransferownership():
//         auto& transfer = staking.staketransferownership();
//         encoded = stakingTransfer(transfer.bucketindex(), TW::data(transfer.voteraddress()),
//                                   TW::data(transfer.payload()));
//         break;
//     case staking.has_candidateregister():
//         auto& candidateregister = staking.candidateregister();
//         encoded = candidateRegister(TW::data(candidateregister.name()),
//         TW::data(candidateregister.operatoraddress()),
//                                     TW::data(candidateregister.rewardaddress()),
//                                     TW::data(candidateregister.stakedamount())),candidateregister.stakedduration(),
//                                    candidateregister.autostake(),
//                                    TW::data(candidateregister.owneraddress()),
//                                    TW::data(candidateregister.payload()));
//         break;
//     case staking.has_candidateupdate():
//         auto& candidateupdate = staking.candidateupdate();
//         encoded = candidateUpdate(TW::data(candidateupdate.name()),
//                                   TW::data(candidateupdate.operatoraddress()),
//                                   TW::data(candidateupdate.rewardaddress()));
//         break;
//     }
//     return encoded;
// }
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
        auto sc = iotextypes::StakeCreate();
        sc.set_candidatename(stake.candidatename());
        sc.set_stakedamount(stake.stakedamount());
        sc.set_stakedduration(stake.stakedduration());
        sc.set_autostake(stake.autostake());
        sc.set_payload(stake.payload());
        action.set_allocated_stakecreate(&sc);
        return;
    }
    if (has_stakeunstake) {
        auto& unstake = staking.stakeunstake();
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
// auto encoded = encodeStaking(staking);
// auto& execution = *action.mutable_execution();
// execution.set_amount(staking.amount());
// execution.set_contract(staking.contract());
// execution.set_data(encoded.data(), encoded.size());
// action.DiscardUnknownFields();

// if (input.has_staking()) {
//     action.set_version(input.version());
//     action.set_nonce(input.nonce());
//     action.set_gaslimit(input.gaslimit());
//     action.set_gasprice(input.gasprice());
//     auto& staking = input.staking();
//     auto encoded = encodeStaking(staking);
//     auto& execution = *action.mutable_execution();
//     execution.set_amount(staking.amount());
//     execution.set_contract(staking.contract());
//     execution.set_data(encoded.data(), encoded.size());
// } else {
//     // ActionCore is almost same as SigningInput, missing field privateKey = 5;
//     action.ParseFromString(input.SerializeAsString());
//     action.DiscardUnknownFields();
// }
// }
