/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This file has been generated, so don't modify it by hand !!

#ifndef __CHECK_SINM_HXX__
#define __CHECK_SINM_HXX__

#include "TIType.hxx"

namespace analysis
{
    inline static TIType check_sinm(GVN & gvn, const TIType & in0)
    {
        switch (in0.type)
        {
        case TIType::EMPTY :
        {
            return in0;
        }
        case TIType::COMPLEX :
        {
            if (in0.cols == in0.rows)
            {
                return in0;
            }
            return TIType(gvn);
        }
        case TIType::DOUBLE :
        {
            if (in0.cols == in0.rows)
            {
                return in0;
            }
            return TIType(gvn);
        }
        default :
            return TIType(gvn);
        }
    }

} // namespace analysis

#endif // __CHECK_sinm_HXX__