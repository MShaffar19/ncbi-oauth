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

#ifndef _hpp_ncbi_oauth_jwt_
#define _hpp_ncbi_oauth_jwt_

#ifndef _hpp_ncbi_oauth_json_
#include <ncbi/json.hpp>
#endif

#include <string>

namespace ncbi
{
    class JWTFactory;
    class JWTFixture_BasicConstruction;
    
    typedef std :: string StringOrURI;
    
    class JWT
    {
    public:
        
        // registered claims
        void setIssuer ( const StringOrURI & iss );
        void setSubject ( const StringOrURI & sub );
        void addAudience ( const StringOrURI & aud );
        void setDuration ( long long int dur_seconds );
        void setNotBefore ( long long int nbf );

        // user interface for managing claims
        void addClaim ( const std :: string & name, JSONValue * value );
        const JSONValue & getClaim ( const std :: string & name ) const;
        
        // C++ assignment
        JWT & operator = ( const JWT & jwt );
        JWT ( const JWT & jwt );
        
        ~JWT ();
        
    private:
        
        static void verifyStringOrURI ( const std :: string & str );
        
        JWT ( JSONObject * payload );

        JSONObject * payload;

        friend class JWTFactory;
        friend class JWTFixture_BasicConstruction;
    };

    class JWTFactory
    {
    public:

        // make a new, more or less empty JWT object
        JWT make () const;
        
        // decode a JWT
        JWT decode ( const std :: string & jwt ) const;
        
        // create a JWS from the claims set
        std :: string sign ( const JWT & claims_set ) const;

        // registered claim factory parameters
        void setIssuer ( const StringOrURI & iss );
        void setSubject ( const StringOrURI & sub );
        void addAudience ( const StringOrURI & aud );
        void setDuration ( long long int dur_seconds );
        void setNotBefore ( long long int nbf_seconds );
        
        // sign and verify keys
        void setSigningKeys ( const std :: string & sign, const std :: string & verify );

        // copy construction
        JWTFactory & operator = ( const JWTFactory & fact );
        JWTFactory ( const JWTFactory & fact );
        
        // create a standard factory
        JWTFactory ();
        ~ JWTFactory ();

    private:
        
        // make a new identifier
        std :: string newJTI () const;
        
        // return timestamp in seconds since epoch
        static long long int now ();

        std :: string sign_key;
        std :: string verify_key;
        std :: string iss;
        std :: string sub;
        std :: vector < std :: string > aud;
        long long duration;
        long long not_before;

        static std :: atomic < unsigned long long > id_seq;
    };
}

#endif /* _hpp_ncbi_oauth_jwt_ */
