/*
 * Copyright (C) 2011 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef Certificate_h
#define Certificate_h

#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <webkit2/webkit2.h>
#include <gtk/gtk.h>


#define TIMELENGTH           64
#define SERIALNUMBERLENGTH   64
#define FINGTERPRINTLENGTH  128
#define VERSIONLENGTH         3
#define CERTITLELENGTH      256
#define CHAIN_MAX_SIZE        3
#define DER_MAX_SIZE       1024*8

#define HEX_SIZE(type)		(sizeof(type)*2)
#define CHARTYPE_BS_ESC         (ASN1_STRFLGS_ESC_2253 | CHARTYPE_FIRST_ESC_2253 | CHARTYPE_LAST_ESC_2253)
#define ESC_FLAGS (ASN1_STRFLGS_ESC_2253 | \
                  ASN1_STRFLGS_ESC_QUOTE | \
                  ASN1_STRFLGS_ESC_CTRL | \
                  ASN1_STRFLGS_ESC_MSB)
#define BUF_TYPE_WIDTH_MASK     0x7
#define BUF_TYPE_CONVUTF8       0x8
#define FN_WIDTH_LN     25
#define FN_WIDTH_SN     10

//save single certificate detail information
struct Certificate
{
   char version[VERSIONLENGTH];  
   struct Issued_By *by;
   struct Issued_To *to; 
   struct Expand *exinfo;
   char serialNumber[SERIALNUMBERLENGTH];
   char sha1_t[FINGTERPRINTLENGTH]; 
   char md5_t[FINGTERPRINTLENGTH];
   char startDate[TIMELENGTH];
   char endDate[TIMELENGTH];
   char *sig_alg;
   char *signature;
   char *pkey_alg;
   char *pkey_value;
};
struct Issued_By
{
   char *commonName;
   char *countryName;
   char *localityName;
   char *provinceName;
   char *organizationName;
   char *organizationalUnitName;
   char *organizationalUnitName1;
};

struct Issued_To
{
   char *commonName;
   char *countryName;
   char *localityName;
   char *provinceName;
   char *organizationName;
   char *organizationalUnitName;
   char *organizationalUnitName1;
};

struct Expand
{
   char *basicConst;
   char *keyUsage;
   char *certPolicies;
   char *crlDist;
   char *exkeyUsage;
   char *infoAccess;
   char *autokeyIdentifier;
   char *netscertType;
   char *subkeyIdentifier;
   char *subaltName;
   char *issaltName;
   char *netsComment;
   char *prikeyPeriod;
   char *nameConstraints;
   char *polMappings;
   char *oid;
};

//get single certificate information
void get_certificate_data(const char *certificateData);

//display single certificate detail information
void display_certificate_info(void);

#endif
