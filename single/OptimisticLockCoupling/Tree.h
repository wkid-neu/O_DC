//
// Created by florian on 18.11.15.
//

#ifndef ART_OPTIMISTICLOCK_COUPLING_N_H
#define ART_OPTIMISTICLOCK_COUPLING_N_H
#include "N.h"

using namespace ART;

namespace ART_OLC {

    class Tree {
    public:
        using LoadKeyFunction = void (*)(TID tid, Key &key);

    private:
        N *const root;

        TID checkKey(const TID tid, const Key &k) const;

        LoadKeyFunction loadKey;

        Epoche epoche{256};

    public:
        enum class CheckPrefixResult : uint8_t {
            Match,
            NoMatch,
            OptimisticMatch
        };

        enum class CheckPrefixPessimisticResult : uint8_t {
            Match,
            NoMatch,
        };

        enum class PCCompareResults : uint8_t {
            Smaller,
            Equal,
            Bigger,
        };
        enum class PCEqualsResults : uint8_t {
            BothMatch,
            Contained,
            NoMatch
        };
        static CheckPrefixResult checkPrefix(N* n, const Key &k, uint32_t &level);

        static CheckPrefixPessimisticResult checkPrefixPessimistic(N *n, const Key &k, uint32_t &level,
                                                                   uint8_t &nonMatchingKey,
                                                                   Prefix &nonMatchingPrefix,
                                                                   LoadKeyFunction loadKey, bool &needRestart);

        static PCCompareResults checkPrefixCompare(const N* n, const Key &k, uint8_t fillKey, uint32_t &level, LoadKeyFunction loadKey, bool &needRestart);

        static PCEqualsResults checkPrefixEquals(const N* n, uint32_t &level, const Key &start, const Key &end, LoadKeyFunction loadKey, bool &needRestart);
        
        //add four function

        static N* findFirst(N* n,uint8_t keyByte,bool ss,uint32_t &level,const Key &k,
                           bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);

        static void findBoundary(N *node,const Key &k,uint32_t level,N *&first,N *&second,
                            bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);

        static N* minimum(N* node, bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);
        static N* maximum(N* node, bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);
        //static void findBoundary(N *node,const Key &k,uint32_t level,N *&first,N *&second);
        //static N* findFirst(N* n,uint8_t keyByte,bool ss,uint32_t &level,const Key &k);

    public:

        Tree(LoadKeyFunction loadKey);

        Tree(const Tree &) = delete;

        Tree(Tree &&t) : root(t.root), loadKey(t.loadKey) { }

        ~Tree();

        ThreadInfo getThreadInfo();

        TID lookup(const Key &k, ThreadInfo &threadEpocheInfo) const;

        TID lookup2(const Key &k) const;


        // N* findFirst(N* n,uint8_t keyByte,bool ss,uint32_t &level,const Key &k,
        //                    bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);

        // void findBoundary(N *node,const Key &k,uint32_t level,N *&first,N *&second,
        //                     bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);
        
        // //N* minimum(N* node,ThreadInfo &threadEpocheInfo);
        // N* minimum(N* node, bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);
        // N* maximum(N* node, bool &needRestart, uint64_t v, ThreadInfo &threadEpocheInfo);
        
    
        //N* Tree::LookupLT(const Key &k,ThreadInfo &threadInfo) const;

        N* LookupLT(const Key &k, N *&first,N *&second,ThreadInfo &threadInfo, uint64_t kk) const;
        //N* LookupLT(const Key &k, N *&first,N *&second,ThreadInfo &threadInfo) const;

        bool lookupRange(const Key &start, const Key &end, Key &continueKey, TID result[], std::size_t resultLen,
                         std::size_t &resultCount, ThreadInfo &threadEpocheInfo) const;

        void insert(const Key &k, TID tid, ThreadInfo &epocheInfo);

        void remove(const Key &k, TID tid, ThreadInfo &epocheInfo);
    };
}
#endif //ART_OPTIMISTICLOCK_COUPLING_N_H

