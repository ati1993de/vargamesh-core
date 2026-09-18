// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Copyright (c) 2026 VargaMesh developers
// Distributed under the MIT software license.

#include <primitives/pureheader.h>

#include <hash.h>

uint256 CPureBlockHeader::GetHash() const
{
    return (HashWriter{} << *this).GetHash();
}
