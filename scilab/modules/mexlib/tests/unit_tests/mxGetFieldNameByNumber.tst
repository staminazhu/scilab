// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetFieldNameByNumber mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(['#include ""mex.h""';
       'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
       '{';
       '    int fieldnum = mxGetScalar(prhs[1]);';
       '    char *fieldname = mxGetFieldNameByNumber(prhs[0], fieldnum);';
       '    plhs[0] = mxCreateString(fieldname != NULL ? fieldname : """");';
       '}'],'mexGetFieldNameByNumber.c');
ilib_mex_build('libmextest',['getFieldNameByNumber','mexGetFieldNameByNumber','cmex'], 'mexGetFieldNameByNumber.c',[],'Makelib','','','');
exec('loader.sce');

s = struct("a", 1, "b", 2);
if getFieldNameByNumber(s, 0) <> "a" then pause end
if getFieldNameByNumber(s, 1) <> "b" then pause end
if getFieldNameByNumber(s, 2) <> "" then pause end