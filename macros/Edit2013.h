// -*- mode: c++ -*- 
//____________________________________________________________________ 
//  
// $Id$ 
// Author: Shota TAKAHASHI <shotakaha@gmail.com>
// Update: 2013-03-01 19:52:28+0900
// Copyright: 2013 (C) Shota TAKAHASHI
//
#ifndef ROOT_Edit2013
#define ROOT_Edit2013
#ifndef ROOT_TObject
#include "TObject.h"
#endif

class Edit2013 : public TObject
{
private:
public:
    Edit2013();
    virtual ~Edit2013() {}
    void GenerateDummy(const char* ofn);
    void ConvertText2Root(const char* ifn, const char* ofn);
    void ReadData(const char* ifn);
    void Display();
    ClassDef(Edit2013,0) //DOCUMENT ME
};

#endif
//____________________________________________________________________ 
//  
// EOF
//
#ifndef EDIT2013_H
#define EDIT2013_H



#endif
