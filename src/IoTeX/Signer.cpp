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
        // action.set_allocated_stakecreate(
        //     mutable_stakecreate()->::TW::IoTeX::Proto::Staking_StakeCreate::MergeFrom(
        //         staking.stakecreate()));
        auto& stake = staking.stakecreate();
        auto ss = new IoTeX::Proto::Staking_StakeCreate();
        ss->CopyFrom(stake);
        action.set_allocated_stakecreate(ss);
        return;
    }
        // case kStakeUnstake: {
        //     mutable_stakeunstake()->::TW::IoTeX::Proto::Staking_StakeReclaim::MergeFrom(
        //         from.stakeunstake());
        //     break;
        // }
        // case kStakeWithdraw: {
        //     mutable_stakewithdraw()->::TW::IoTeX::Proto::Staking_StakeReclaim::MergeFrom(
        //         from.stakewithdraw());
        //     break;
        // }
        // case kStakeAddDeposit: {
        //     mutable_stakeadddeposit()->::TW::IoTeX::Proto::Staking_StakeAddDeposit::MergeFrom(
        //         from.stakeadddeposit());
        //     break;
        // }
        // case kStakeRestake: {
        //     mutable_stakerestake()->::TW::IoTeX::Proto::Staking_StakeRestake::MergeFrom(
        //         from.stakerestake());
        //     break;
        // }
        // case kStakeChangeCandidate: {
        //     mutable_stakechangecandidate()->::TW::IoTeX::Proto::Staking_StakeChangeCandidate::MergeFrom(
        //         from.stakechangecandidate());
        //     break;
        // }
        // case kStakeTransferOwnership: {
        //     mutable_staketransferownership()
        //         ->::TW::IoTeX::Proto::Staking_StakeTransferOwnership::MergeFrom(
        //             from.staketransferownership());
        //     break;
        // }
        // case kCandidateRegister: {
        //     mutable_candidateregister()->::TW::IoTeX::Proto::Staking_CandidateRegister::MergeFrom(
        //         from.candidateregister());
        //     break;
        // }
        // case kCandidateUpdate: {
        //     mutable_candidateupdate()->::TW::IoTeX::Proto::Staking_CandidateBasicInfo::MergeFrom(
        //         from.candidateupdate());
        //     break;
        // }
        // case MESSAGE_NOT_SET: {
        //     break;
        // }
    }
    // if (staking.has_stakecreate()) {
    //     auto stake = staking.release_stakerestake();
    // auto ss = new IoTeX::Proto::Staking_StakeCreate();
    // ss->set_candidatename(stake.candidatename());
    // ss->set_stakedamount(stake.stakedamount());
    // ss->set_stakedduration(stake.stakedduration());
    // ss->set_autostake(stake.autostake());
    // ss->set_payload(stake.payload());
    //     action.set_allocated_stakecreate(stake);
    //     return;
    // }
    if (staking.has_stakeunstake()) {
        auto& unstake = staking.stakeunstake();
        auto ss = new IoTeX::Proto::Staking_StakeReclaim();
        ss->set_bucketindex(unstake.bucketindex());
        ss->set_payload(unstake.payload());
        action.set_allocated_stakeunstake(ss);
        return;
    }
    if (staking.has_stakewithdraw()) {
        auto& withdraw = staking.stakewithdraw();
        auto ss = new IoTeX::Proto::Staking_StakeReclaim();
        ss->set_bucketindex(withdraw.bucketindex());
        ss->set_payload(withdraw.payload());
        action.set_allocated_stakewithdraw(ss);
        return;
    }
    if (staking.has_stakeadddeposit()) {
        auto& adddeposit = staking.stakeadddeposit();
        auto ss = new IoTeX::Proto::Staking_StakeAddDeposit();
        ss->set_bucketindex(adddeposit.bucketindex());
        ss->set_amount(adddeposit.amount());
        ss->set_payload(adddeposit.payload());
        action.set_allocated_stakeadddeposit(ss);
        return;
    }
    if (staking.has_stakerestake()) {
        auto& restake = staking.stakerestake();

        auto ss = new IoTeX::Proto::Staking_StakeRestake();
        ss->set_bucketindex(restake.bucketindex());
        ss->set_stakedduration(restake.stakedduration());
        ss->set_autostake(restake.autostake());
        ss->set_payload(restake.payload());
        action.set_allocated_stakerestake(ss);
        return;
    }
    if (staking.has_stakechangecandidate()) {
        auto& changecandidate = staking.stakechangecandidate();
        auto ss = new IoTeX::Proto::Staking_StakeChangeCandidate();
        ss->set_bucketindex(changecandidate.bucketindex());
        ss->set_candidatename(changecandidate.candidatename());
        ss->set_payload(changecandidate.payload());
        action.set_allocated_stakechangecandidate(ss);
        return;
    }
    if (staking.has_staketransferownership()) {
        auto& transfer = staking.staketransferownership();
        auto ss = new IoTeX::Proto::Staking_StakeTransferOwnership();
        ss->set_bucketindex(transfer.bucketindex());
        ss->set_voteraddress(transfer.voteraddress());
        ss->set_payload(transfer.payload());
        action.set_allocated_staketransferownership(ss);
        return;
    }
    if (staking.has_candidateregister()) {
        auto& candidateregister = staking.candidateregister();

        auto cbi = new IoTeX::Proto::Staking_CandidateBasicInfo();
        cbi->set_name(candidateregister.candidate().name());
        cbi->set_operatoraddress(candidateregister.candidate().operatoraddress());
        cbi->set_rewardaddress(candidateregister.candidate().rewardaddress());

        auto ss = new IoTeX::Proto::Staking_CandidateRegister();
        ss->set_allocated_candidate(cbi);
        ss->set_stakedamount(candidateregister.stakedamount());
        ss->set_stakedduration(candidateregister.stakedduration());
        ss->set_autostake(candidateregister.autostake());
        ss->set_owneraddress(candidateregister.owneraddress());
        ss->set_payload(candidateregister.payload());
        action.set_allocated_candidateregister(ss);
        return;
    }
    if (staking.has_candidateupdate()) {
        auto& candidateupdate = staking.candidateupdate();
        auto cbi = new IoTeX::Proto::Staking_CandidateBasicInfo();
        cbi->set_name(candidateupdate.name());
        cbi->set_operatoraddress(candidateupdate.operatoraddress());
        cbi->set_rewardaddress(candidateupdate.rewardaddress());
        action.set_allocated_candidateupdate(cbi);
        return;
    }
    action.ParseFromString(input.SerializeAsString());
    action.DiscardUnknownFields();
}
