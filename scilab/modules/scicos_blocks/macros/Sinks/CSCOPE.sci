//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=CSCOPE(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs

        model=arg1.model;
        //dstate=model.in
        while %t do
            [ok,clrs,win,wpos,wdim,ymin,ymax,per,N,heritance,nom,exprs]=scicos_getvalue(..
            msprintf(_("Set %s block parameters"), "CSCOPE"),..
            [_("Curve style: Color>0 | mark<0");
            _("Output window number (-1 for automatic)");
            _("Output window position");
            _("Output window sizes");
            "Ymin";
            "Ymax";
            _("Refresh period");
            _("Buffer size");
            _("Accept herited events 0/1");
            _("Name of Scope (label&Id)")],..
            list("vec",8,"vec",1,"vec",-1,"vec",-1,"vec",1,..
            "vec",1,"vec",1,"vec",1,"vec",1,"str",1),..
            exprs)
            if ~ok then
                break,
            end //user cancel modification
            mess=[]
            if size(wpos,"*")<>0 &size(wpos,"*")<>2 then
                mess=[mess ; _("''Window position'' must be [] or a 2 vector") ; " "]
            end
            if size(wdim,"*")<>0 &size(wdim,"*")<>2 then
                mess=[mess ; _("''Window sizes'' must be [] or a 2 vector") ; " "]
            end
            if win<-1 then
                mess=[mess ; _("The Window number must be >= -1") ; " "]
            end
            if per<=0 then
                mess=[mess ; _("The Refresh period must be > 0") ; " "]
            end
            if N<2 then
                mess=[mess ; _("The Buffer size must be >= 2") ; " "]
            end
            if ymin>=ymax then
                mess=[mess ; _("Ymax > Ymin is required") ; " "]
            end
            if ~or(heritance==[0 1]) then
                mess=[mess ; _("''Accept herited events'' must be 0 or 1") ; " "]
            end
            if mess<>[] then
                message([_("Some specified values are inconsistent:") ;
                         " " ; mess])
            else
                [model,graphics,ok]=set_io(model,graphics,list([-1 1],1),list(),ones(1-heritance,1),[])
                if wpos==[] then
                    wpos=[-1;-1];
                end
                if wdim==[] then
                    wdim=[-1;-1];
                end
                rpar=[0;ymin;ymax;per]
                ipar=[win;1;N;clrs(:);wpos(:);wdim(:)]
                //if prod(size(dstate))<>(8+1)*N+1 then dstate=-eye((8+1)*N+1,1),end
                //model.dstate=[];
                model.rpar=rpar;
                model.ipar=ipar
                model.evtin=ones(1-heritance,1)
                model.label=nom;
                graphics.id=nom
                graphics.exprs=exprs;
                graphics.style = "CSCOPE;verticalLabelPosition=bottom;" + ..
                   "verticalAlign=top;spacing=2;displayedLabel=" + nom
                x.graphics=graphics;
                x.model=model
                break
            end
        end

    case "define" then
        win=-1;
        wdim=[600;400]
        wpos=[-1;-1]
        clrs=[1;3;5;7;9;11;13;15];
        N=20;
        ymin=-15;
        ymax=+15;
        per=30;

        model=scicos_model()
        model.sim=list("cscope",4)
        model.in=-1
        model.in2=1
        model.evtin=1
        model.rpar=[0;ymin;ymax;per]
        model.ipar=[win;1;N;clrs;wpos;wdim]
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[strcat(string(clrs)," ");
        string(win);
        sci2exp([]);
        sci2exp(wdim);
        string(ymin);
        string(ymax);
        string(per);
        string(N);
        string(0)';
        emptystr()]; //label-id
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
