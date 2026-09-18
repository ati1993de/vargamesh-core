// Copyright (c) 2013-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <clientversion.h>
#include <key.h>
#include <key_io.h>
#include <streams.h>
#include <test/util/setup_common.h>
#include <util/strencodings.h>

#include <string>
#include <vector>

namespace {

struct TestDerivation {
    std::string pub;
    std::string prv;
    unsigned int nChild;
};

struct TestVector {
    std::string strHexMaster;
    std::vector<TestDerivation> vDerive;

    explicit TestVector(std::string strHexMasterIn) : strHexMaster(strHexMasterIn) {}

    TestVector& operator()(std::string pub, std::string prv, unsigned int nChild) {
        vDerive.emplace_back();
        TestDerivation &der = vDerive.back();
        der.pub = pub;
        der.prv = prv;
        der.nChild = nChild;
        return *this;
    }
};

TestVector test1 =
  TestVector("000102030405060708090a0b0c0d0e0f")
    ("VMPULNojPHWzovqgVmf1qEoKeZHF6FRjw92A5mTRCQK9YGRoN6EwGSCDbJoNT95kXnb3sUCCf9C4xseMJZfKdHQSkgHBtz3Kv8uWx7ZS8huTArL",
     "VMPRKy5LrGteHtAqP9dDKA3zpsJGjN9R3sDzi5Gd1e2iTcr8CdpsWWa7JiWHczM1CLQEVthWDYRY7ftMHd9mZ8TAJTzbRBQUBLb73p7BDnoHA8Q",
     0x80000000)
    ("VMPULR59NzpTuBxfeNnj2zfxXbPMR4z6KDWAmNtKAVhGWTd5jNY7gfjJiT9nEQeTH6VasgUmXNjVhWiSGxdTaK8ppZjv6cqaqSojU3gzSQfjuLR",
     "VMPRL1LkqzC7P9HpXkkvWuvdhuQP4BhmRwi1PghWyjQqRp3QZv83vk7CRrrhQFhLc3nCCwbCaqkQW2jE1PBVCijM6Y57V45cRJHzkpk2ersBRWp",
     1)
    ("VMPULTFGanNMHtqjUpqAmFZq8LH1USyCKFEThhZ9kmU5T6RHxv3FQU9cg241eV72h3CvqQ6pJoBbp7CusGNH3eJiWDZrPpgSK5p16nDmjKUu5dS",
     "VMPRL3Wt3mjzmrAtNCoNFApWJeJ37ZgsRySJL1NMa1BeNSqcoTdBeYXWPRkvpJtQCyZEf2LtPfLqajge9KhiL4XKshZYzq3LV44rRd4QBoKHRoY",
     0x80000002)
    ("VMPULVrYdKCCzmh9LzdG1qyCVLK9hbqianpT3Enq16mEcxnjNmENDDz3aTbwCWHs8ueFBvCYvBADC3mtKbWZkjrFZfigyieaQ2sgpU8upXnvctu",
     "VMPRL68A6JZrUj2JENbTVmDsfeLBLiZPhX2HfYc2pLUoYKD4DJpJTJMwHsJrNM72BD4jAVNGz8Hm19WYNXSTv5SWzo1DMSbtdzJo2tocpTFkHy2",
     2)
    ("VMPULY5wTkKABFN8RU6yi7BHNqUTnR8TDP8eUig8G4HhwQMLjrrjGzgQn34rsqHFYhnyfEKj4BeAEnfzt1rZr3HTHGBwtWCmEDmWqA3kyDLBoKM",
     "VMPRL8MYvjgofChHJr5BC2RxZ9VVRXr8L7LV72VL5J1GrkmfaQSfX54JVSmn3fWUdsUAEHVyc3XKuxxhqiLv7VysCBbFkggzC58WBsGnKsgTzqh",
     1000000000)
    ("VMPULZohwRvQJNkKwotRpek1gYUTaehm2swgGy4gWLfr9UjqSzG7oQw539DvWfCCPxsh8vcGZXZxmzNbUSFawtnJ8XM4m3cr2hM9e7fgDHEoX5g",
     "VMPRLA5KQRJ3nL5UqBrdJZzgrrVVDmRS9c9WuGstKaPR4qAAHXr43VJxkYvqgXHgYnbEqguEPQWGm1VpqtMHeZzbK9Eo6BTDJytDUCq954hpTVF",
     0);

TestVector test2 =
  TestVector("fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542")
    ("VMPULNojPHWzovqgVPALwPsc6m11LSaAcmGymMF9VM3yWeYff5G8SayA2CxxoFAjF2x1n2RzZorEMx9c2MPYkUKbtbH8kvEnkdQBGVEYQeg7rtp",
     "VMPRKy5LrGteHtAqNm8YRK8HH522yZHqjVUpPf4MJamYRzxzVcr4gfM3jcfsy484NqxKjwSvxpszPmvYP9ssbYqruqKMsbUiKgVqd4jQakxukAT",
     0)
    ("VMPULS5Uf1CxHwfm1eke5BFVAZQXhQKRKjmGKWzWfA5Rb4QdM3i71rKEB3YDNi8d37xSzLvAS2qym2zaBWpEdd22EBwR2xyTeyYZPyMCqtrrU6P",
     "VMPRL2M67zabmtzuu2iqZ6WALsRZLX36STy6wpoiUPnzWQpxBbJ3Fvh7tTF8YZPs1uoA9bgkrqHipwLfjfWh9qLur4UksmtmiAfakUdxwzckmYL",
     0xFFFFFFFF)
    ("VMPULTEXucEUfqqsBWxpVdF8du6669YKsDvxQg53aYzCEoB1oLwZdjjYP7Koz6NjnrZYmrqi5shb4FRArBgaQXW8xLQ3a3bX1hoaGMcoggnSED1",
     "VMPRL3W9Nbc89oB24tw1yYVopD77jGFzyx8o2ytFPnhmA9bLdtXVsp7S6X2j9usqkAXe3q1edrVMswqnAuFTtCx3RMvKJW87UgPqYFyViT7QGQ4",
     1)
    ("VMPULW3WKdDQqV94H8dxZHPhbr2KRgQpyQrDj7ZuRceXYgXTUAoiwFGFH1iBptZsajFqN3JFvWnphNizvywbKRw6DfrxAKoNEFEnC8Hkw6nki9e",
     "VMPRL6K7ncb4KSUDAWcA3CeNnA3M4o8W6944MRP7ErN6U2wnJiPfBKe8zRR6zi5TmsjLMnNMP9ZkHVWzVc2cw6URRqjcSGevKnNE24QEC7w3yaE",
     0xFFFFFFFE)
    ("VMPULXDYEb9mSt6ma5sZHmDEyMiWLQtC2PUikpHnxLLwPSnhvX8f2vCFoY5HTYhNn6nHKUwtj1rXjz3rad6z1K3bw2ZQSE7rnYLC4MNLU31aNRa",
     "VMPRL7V9haXQvqRvTTqkmgTv9fjXyXbs97gZP86zma4WJoD2m4ibGza9WwnCdQorQJ7cuMf3gyLjfs6aJj7zT3Nv7gQnGDCETzPrMeQkFqMHVVt",
     2)
    ("VMPULYaaC2NGy4R1KM4T1qLvx76UjURJGiPoQyugN7hS2EeHziUECZ81z72HXwmeug8yqYA7zwMwdh4r54AUqQjWFHQz5PwwiucvjeZX8gLXFVR",
     "VMPRL8rBf1jvT1kACj2eVkbc8R7WNb8yPSbe3HitBMQzwb4cqG4ASdVuhWjChpUrZKCgwfWmyiyZ7xwGMH1WBMk9Af9W54hjXQXa41kJLPkARCY",
     0);

TestVector test3 =
  TestVector("4b381541583be4423346c643850da4b320e46a87ae3d2a4e6da11eba819cd4acba45d239319ac14f863b8d5ab5a0d0c64d2e8a1e7d1457df2e5a3c51c73235be")
    ("VMPULNojPHWzovqgUScWTnNQJkG3D5uGNdSFUh1hte5XnLdTdKVQ6jD7yUCVHEhi4dXex7BrPojEkcyTTf2TbBN3JvwGtA91VBr24CkEw9Fon8w",
     "VMPRKy5LrGteHtAqMpahwhd5V4H4rCcwVMe66zpuhso6hh3nTs5LLob1gsuQT3rEKsBepe7gXka6JCfFjoVBpyzHhJpcPt3MX7qounPmEe1x7AF",
      0x80000000)
    ("VMPULRAw8FM8VJD2fyhugvhuN11huSWeXUr9tBFehsmNieYptLcrgDyz3vzCaxYDnz8Q9UghAmUybaAx67xoXRJDtBDDZG5u33LpYP79bZwhc9A",
     "VMPRL1SYbEimyFYBZMg7AqxaYK2jYZEKeD3zWV4rX7Uwdzy9itCnvJMsmLh7kpCafpWaGnm6YhMxwwHiPM8LQCa3T52Eq1xQGH7d2oMRC5Tn41S",
      0);

TestVector test4 =
  TestVector("3ddd5602285899a946114506157c7997e5444528f3003f6134712147db19b678")
    ("VMPULNojPHWzovqgWW85JWjD7VivqKVSHsqzipi7GNm7esU9UDJjJdotBgkayMxP657iXeKQYUxRhPkCGXob1PNxPnoBeaS63VopxoRU1wDY1RQ",
     "VMPRKy5LrGteHtAqPt6GnRytHojxUSD7Qc3qM8XK5cUgaDtUJktfYiBmu6TW9D8MLbaqQ75JdwTEEXs2Np7UV5FCn8Uzu7uPZzjUBvFuzkmQY3M",
     0x80000000)
    ("VMPULRxqmfdfBgx89P9Cvqez7tphHFNi1BNYU8KmcLw6JKY7xkMqryEcKjRw8xE97hUqwk5mVXM3JKgSoTVeiAqMV6rDgts3vJuD1byWpPwrFhw",
     "VMPRL2ETEf1JfeHH2m7QQkufJCqivN6P7uaP6S8yRaefDfxSoHwn73cW398rJm3bPQmCN66kAWdTYyy7KUJa6SuqnmMvkWD4mvBUBBHijedraE4",
     0x80000001)
    ("VMPULU6XRf2fhQzipXzr57ZtDmQYTBYnW75M84NbP6THxWdZjqgMjTiKYrPp2tzfxt7XXXh8PJH9kn2RmwpZAmeWCPnb2gi6t3P9t2LQstEaH95",
     "VMPRL4N8teQKBNKshuy3Z2pZQ5Ra6JGTcqHBkNBoCLArss3taPGHyY6DGG6jCj1gSoo2SrVrM7TRKd4z2S8HVNry59q9zqv5sxRo2rqQqHaqEAJ",
     0);

const std::vector<std::string> TEST5 = {
    "VMPULNojPHWzovqgURZULGT7ntje8ApAhXpYg335kWN74CsDEyyU1nrdCTf9zT8LGevobt2HFr9v9nysSHycvFm1Kqowzf7ddodx5LbWZyjrNMV",
    "VMPRKy5LrGteHtAqMoXfpBhnyCkfmHXqpG2PJLrHZk5fyZHY5XZQFsEWusN5AWJUgB86JgfZv8o3kT2d8JucmtzsTPFUDLtZz267pcPevxPRYVp",
    "VMPULNojPHWzovqgURZULGT7ntje8ApAhXpYg335kWN74CsDEyyU1nrdCTf9zauJyaiw1ajBo8Ujn7TsFaNiaiDsvfBT9DQ4cQPY3xd4YRfPTZX",
    "VMPRKy5LrGteHtAqMoXfpBhnyCkfmHXqpG2PJLrHZk5fyZHY5XZQFsEWusN5AWfb4qob9ogRx2cRuTJiRiWWv8chLdJmw7B4BomfyutJukX2ptw",
    "VMPULNojPHWzovqgURZULGT7ntje8ApAhXpYg335kWN74CsDEyyU1nrdCTf9zV55StNqTJhWPfjd4NbsPcaPqNNiyYepnJBjsxKr4zbu4XQUDTe",
    "VMPRKy5LrGteHtAqMoXfpBhnyCkfmHXqpG2PJLrHZk5fyZHY5XZQFsEWusN5AQqMY9TVbXekYZsKBiSiZkiCAnmYPWn9aBxjTMhyzws9RrEfg56",
    "VMPRKy5mg1Njgyd8S1dbUSxr9eAuKiQLZYkXncuNG87y72uWGrgpPBZBNKV8awQRTmLWNgx3iRZcbEPLbQa63UgMpcqCEZdcFJuLv4YUhc2rsRT",
    "VMPULNpAD216D2HyYdfPzXiAyL9sgbgfSpYhAK6AStQQBgVBSK6t97BHeunDR941gmNUo3gr4uwMwCkPw9EVEH8b8E24sTrZSibEoRp8L49EzeN",
    "VMPRKy5LrGteMgBzDTWvxSFJEzSHC99QM7Swh3WoLBCVUuLSGgNWb6RmVZQJtZtv9BfwG1A7V9tjHmaem7Nx1MUZaRQVaQU9K85m376vPs83fJr",
    "VMPULNojPHWzsirqL5YjUWzd4gRFZ2RjEPF74jhbWwUvZYv7S8naM23sn9hPimYWNBhugMtuqeGUdjwi6r3MC9vnt2bNDJh6WXmevUNa2KSow3e",
    "DMwo58pR1QLEFihHiXPVykYB6fJmsTeHvyTp7hRThAtCX8CvYzgPcn8XnmdfHGMQzT7ayAmfo4z3gY5KfbrZWZ6St24UVf2Qgo6oujFktLHdHY4",
    "DMwo58pR1QLEFihHiXPVykYB6fJmsTeHvyTp7hRThAtCX8CvYzgPcn8XnmdfHPmHJiEDXkTiJTVV9rHEBUem2mwVbbNfvT2MTcAqj3nesx8uBf9",
    "VMPRKy5LrGteHtAqMoXfpBhnyCkfmHXqpG2PJLrHZk5fyZHY5XZQFsEWusN5ANtcMv1Tk6yXQkHcH8picS7RFg9pjowGnYtdDD261Hrkuqfh3Pb",
    "VMPRKy5LrGteHtAqMoXfpBhnyCkfmHXqpG2PJLrHZk5fyZHY5XZQFsEWusN5AQqMY9TVbXekYZsKBiSiZki7dvQdrVXN7UhkCVzfJeqme87urSv",
    "VMPULNojPHWzovqgURZULGT7ntje8ApAhXpYg335kWN74CsDEyyU1nrdCTf9zX1pd7psJjNjXVKKxxDsLwBAkUzSdFVhZwFr871k4ecHYa3Fk6Y",
    "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHL"
};

void RunTest(const TestVector& test)
{
    std::vector<std::byte> seed{ParseHex<std::byte>(test.strHexMaster)};
    CExtKey key;
    CExtPubKey pubkey;
    key.SetSeed(seed);
    pubkey = key.Neuter();
    for (const TestDerivation &derive : test.vDerive) {
        unsigned char data[74];
        key.Encode(data);
        pubkey.Encode(data);

        // Test private key
        BOOST_CHECK(EncodeExtKey(key) == derive.prv);
        BOOST_CHECK(DecodeExtKey(derive.prv) == key); //ensure a base58 decoded key also matches

        // Test public key
        BOOST_CHECK(EncodeExtPubKey(pubkey) == derive.pub);
        BOOST_CHECK(DecodeExtPubKey(derive.pub) == pubkey); //ensure a base58 decoded pubkey also matches

        // Derive new keys
        CExtKey keyNew;
        BOOST_CHECK(key.Derive(keyNew, derive.nChild));
        CExtPubKey pubkeyNew = keyNew.Neuter();
        if (!(derive.nChild & 0x80000000)) {
            // Compare with public derivation
            CExtPubKey pubkeyNew2;
            BOOST_CHECK(pubkey.Derive(pubkeyNew2, derive.nChild));
            BOOST_CHECK(pubkeyNew == pubkeyNew2);
        }
        key = keyNew;
        pubkey = pubkeyNew;
    }
}

}  // namespace

BOOST_FIXTURE_TEST_SUITE(bip32_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(bip32_test1) {
    RunTest(test1);
}

BOOST_AUTO_TEST_CASE(bip32_test2) {
    RunTest(test2);
}

BOOST_AUTO_TEST_CASE(bip32_test3) {
    RunTest(test3);
}

BOOST_AUTO_TEST_CASE(bip32_test4) {
    RunTest(test4);
}

BOOST_AUTO_TEST_CASE(bip32_test5) {
    for (const auto& str : TEST5) {
        auto dec_extkey = DecodeExtKey(str);
        auto dec_extpubkey = DecodeExtPubKey(str);
        BOOST_CHECK_MESSAGE(!dec_extkey.key.IsValid(), "Decoding '" + str + "' as xprv should fail");
        BOOST_CHECK_MESSAGE(!dec_extpubkey.pubkey.IsValid(), "Decoding '" + str + "' as xpub should fail");
    }
}

BOOST_AUTO_TEST_CASE(bip32_max_depth) {
    CExtKey key_parent{DecodeExtKey(test1.vDerive[0].prv)}, key_child;
    CExtPubKey pubkey_parent{DecodeExtPubKey(test1.vDerive[0].pub)}, pubkey_child;

    // We can derive up to the 255th depth..
    for (auto i = 0; i++ < 255;) {
        BOOST_CHECK(key_parent.Derive(key_child, 0));
        std::swap(key_parent, key_child);
        BOOST_CHECK(pubkey_parent.Derive(pubkey_child, 0));
        std::swap(pubkey_parent, pubkey_child);
    }

    // But trying to derive a non-existent 256th depth will fail!
    BOOST_CHECK(key_parent.nDepth == 255 && pubkey_parent.nDepth == 255);
    BOOST_CHECK(!key_parent.Derive(key_child, 0));
    BOOST_CHECK(!pubkey_parent.Derive(pubkey_child, 0));
}


BOOST_AUTO_TEST_CASE(vargamesh_mainnet_extended_key_identity)
{
    std::vector<std::byte> seed{
        ParseHex<std::byte>("000102030405060708090a0b0c0d0e0f")
    };

    CExtKey private_key;
    private_key.SetSeed(seed);

    CExtPubKey public_key{private_key.Neuter()};

    const std::string encoded_private{
        EncodeExtKey(private_key)
    };

    const std::string encoded_public{
        EncodeExtPubKey(public_key)
    };

    BOOST_CHECK(
        encoded_private.starts_with("VMPR")
    );

    BOOST_CHECK(
        encoded_public.starts_with("VMPU")
    );

    BOOST_CHECK(
        DecodeExtKey(encoded_private).key.IsValid()
    );

    BOOST_CHECK(
        DecodeExtPubKey(encoded_public).pubkey.IsValid()
    );

    BOOST_CHECK_EQUAL(
        EncodeExtKey(
            DecodeExtKey(encoded_private)
        ),
        encoded_private
    );

    BOOST_CHECK_EQUAL(
        EncodeExtPubKey(
            DecodeExtPubKey(encoded_public)
        ),
        encoded_public
    );

    // Bitcoin-mainnet BIP32 encodings must not be
    // accepted as VargaMesh-mainnet extended keys.
    BOOST_CHECK(
        !DecodeExtKey(
            "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi"
        ).key.IsValid()
    );

    BOOST_CHECK(
        !DecodeExtPubKey(
            "xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8"
        ).pubkey.IsValid()
    );
}

BOOST_AUTO_TEST_SUITE_END()
