/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.hxx"
#include "ConvertSciVarToMatVar.hxx"

extern "C"
{
#include <string.h>
#include "api_scilab.h"
#include "sci_types.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "localization.h"
}

matvar_t *GetCharVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{
    types::GatewayStruct* pGS = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pGS->m_pIn;

    if (in[iVar - 1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: String matrix expected.\n"), "GetCharVariable");
        return NULL;
    }

    types::String* pStr = in[iVar - 1]->getAs<types::String>();

    return GetCharMatVar(pStr, name);
}

matvar_t* GetCharMatVar(types::String* pStr, const char* name)
{
    int Dims = pStr->getDims();
    int* pDims = pStr->getDimsArray();
    matvar_t * pMatVarOut = NULL;
    int* piLen = NULL;

    if (Dims > 2)
    {
        Scierror(999, _("%s: Row array of strings saving is not implemented.\n"), "GetCharMatVar");
        return NULL;
    }

    if (pDims[1] != 1)
    {
        Scierror(999, _("%s: Row array of strings saving is not implemented.\n"), "GetCharMatVar");
        return NULL;
    }

    char* pcName = wide_string_to_UTF8(pStr->get(0));
    int iLen = strlen(pcName);
    FREE(pcName);

    for (int i = 1; i < pStr->getSize(); ++i)
    {
        pcName = wide_string_to_UTF8(pStr->get(i));
        if (iLen != strlen(pcName))
        {
            Scierror(999, _("%s: Column array of strings with different lengths saving is not implemented.\n"), "GetCharMatVar");
            FREE(pcName);
            return NULL;
        }
        FREE(pcName);
    }

    size_t* psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (psize_t == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetCharMatVar");
        return NULL;
    }

    /* Reorder characters */
    char* pstMatData = (char*)MALLOC(sizeof(char) * pDims[0] * iLen);
    for (int i = 0; i < pDims[0]; ++i)
    {
        pcName = wide_string_to_UTF8(pStr->get(i));
        for (int j = 0; j < iLen; ++j)
        {
            pstMatData[i + j * pDims[0]] = pcName[j];
        }
        FREE(pcName);
    }


    /* Save the variable */
    psize_t[0] = pDims[0];
    psize_t[1] = iLen;

    pMatVarOut = Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, Dims, psize_t, pstMatData, 0);

    FREE(pstMatData);
    FREE(psize_t);

    return pMatVarOut;
}
