/*==============================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 */

#ifndef _hpp_ncbi_oauth_jwa_
#define _hpp_ncbi_oauth_jwa_

#include <map>
#include <set>

namespace ncbi
{
    // JSON Web Algorithms - RFC 7518

    class JWAKeyHolder
    {
    public:
        
        const std :: string & algorithm () const;
        const std :: string & name () const;

        virtual ~ JWAKeyHolder ();
        
    protected:

        JWAKeyHolder ( const std :: string & alg,
            const std :: string & name, const std :: string & key );

        std :: string alg;
        std :: string nam;
        std :: string key;
    };

    class JWASigner : public JWAKeyHolder
    {
    public:

        virtual std :: string sign ( const void * data, size_t bytes ) const = 0;
        virtual JWASigner * clone () const = 0;
        
    protected:

        JWASigner ( const std :: string & alg,
            const std :: string & name, const std :: string & key );
    };

    class JWAVerifier : public JWAKeyHolder
    {
    public:

        virtual void verify ( const void * data, size_t bytes, const std :: string & signature ) const = 0;
        virtual JWAVerifier * clone () const = 0;

    protected:

        JWAVerifier ( const std :: string & alg,
            const std :: string & name, const std :: string & key );
    };

    struct JWASignerFact
    {
        virtual JWASigner * make ( const std :: string & alg,
            const std :: string & name, const std :: string & key ) const = 0;
        virtual ~ JWASignerFact ();
    };

    struct JWAVerifierFact
    {
        virtual JWAVerifier * make ( const std :: string & alg,
            const std :: string & name, const std :: string & key ) const = 0;
        virtual ~ JWAVerifierFact ();
    };
    
    class JWAFactory
    {
    public:

        JWASigner * makeSigner ( const std :: string & alg,
            const std :: string & name, const std :: string & key ) const;
        JWAVerifier * makeVerifier ( const std :: string & alg,
            const std :: string & name, const std :: string & key ) const;

        void registerSignerFact ( const std :: string & alg, JWASignerFact * fact );
        void registerVerifierFact ( const std :: string & alg, JWAVerifierFact * fact );

        JWAFactory ();
        ~ JWAFactory ();
        
    private:

        void makeMaps ();

        struct Maps
        {
            Maps ();
            ~ Maps ();
            
            std :: set < std :: string > sign_accept;
            std :: map < std :: string, JWASignerFact * > signer_facts;
            std :: map < std :: string, JWAVerifierFact * > verifier_facts;

        } * maps;

    };

    extern JWAFactory gJWAFactory;

    // functions that can be used to cause inclusion of an algorithm
    // i.e. to keep it from being dead-stripped by the linker
    void includeJWA_none ( bool always_false );

}

#endif /* _hpp_ncbi_oauth_jws_ */
