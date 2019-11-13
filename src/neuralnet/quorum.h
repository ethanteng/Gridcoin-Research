#pragma once

#include <string>

class CBlockIndex;

namespace NN {

class Claim;
class QuorumHash;

//!
//! \brief Produces, stores, and validates superblocks.
//!
//! The quorum system enables the Gridcoin network to arrive at a consensus on
//! the daily superblocks. The legacy strategy for historical blocks follows a
//! protocol wherein nodes vote for superblocks. After switching to version 11
//! blocks, nodes create and validate superblocks from scraper convergences.
//!
//! This class is designed as a facade that provides an interface for the rest
//! of the application. It consumes information from newly-connected blocks to
//! update the superblock data for research reward calculations.
//!
//! THREAD SAFETY: The quorum system interacts closely with pointers to blocks
//! in the chain index. Always lock cs_main before calling its methods.
//!
class Quorum
{
public:
    //!
    //! \brief Determine whether the provided address participates in the
    //! quorum consensus at the specified time.
    //!
    //! \param address Default wallet address of a node in the network.
    //! \param time    Timestamp to check participation at.
    //!
    //! \return \c true if the address matches the subset of addrsses that
    //! particpate in the quorum on the day of the year of \p time.
    //!
    static bool Participating(const std::string& address, const int64_t time);

    //!
    //! \brief Get the hash of the pending superblock with the greatest vote
    //! weight.
    //!
    //! \param pindex Provides context about the chain tip used to calculate
    //! quorum vote weights.
    //!
    //! \return Quorum hash of the most popular superblock or an invalid hash
    //! when no nodes voted in the current superblock cycle.
    //!
    static QuorumHash FindPopularHash(const CBlockIndex* const pindex);

    //!
    //! \brief Store a node's vote for a pending superblock in the cache.
    //!
    //! \param quorum_hash Hash of the superblock the node voted for.
    //! \param grc_address Default wallet address of the node.
    //! \param pindex      The block that contains the vote to store.
    //!
    static void RecordVote(
        const QuorumHash quorum_hash,
        const std::string& grc_address,
        const CBlockIndex* const pindex);

    //!
    //! \brief Remove a node's vote for a pending superblock from the cache.
    //!
    //! \param pindex The block that contains the vote to remove.
    //!
    static void ForgetVote(const CBlockIndex* const pindex);

    //!
    //! \brief Validate a superblock published to the network for the day.
    //!
    //! \param claim  Contains the superblock data staked in a block.
    //! \param pindex Provides context for the block containing the superblock.
    //!
    static bool ValidateSuperblockClaim(
        const Claim& claim,
        const CBlockIndex* const pindex);
};
}
