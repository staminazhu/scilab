/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();atanh(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();atanh(a);toc
*/
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_atanh(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn = NULL;
    types::Double* pDblOut = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "atanh", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "atanh", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        pDblIn = in[0]->getAs<types::Double>();
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

        double* pInR = pDblIn->get();
        double* pOutR = pDblOut->get();
        int size = pDblIn->getSize();
        if (pDblIn->isComplex())
        {
            double* pInI = pDblIn->getImg();
            double* pOutI = pDblOut->getImg();

            for (int i = 0; i < size; i++)
            {
                //zcoss(-pInI[i], pInR[i], &pOutR[i], &pOutI[i]);
                std::complex<double> c(pInR[i], pInI[i]);
                std::complex<double> d = std::atanh(c);
                pOutR[i] = d.real();
                pOutI[i] = d.imag();
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                pOutR[i] = std::atanh(pInR[i]);
            }
        }

        out.push_back(pDblOut);
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_atanh";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/