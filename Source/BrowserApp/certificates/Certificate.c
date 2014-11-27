/*
   main function:
      get single certificate information
      output single certificate information
  author:luyue
*/

#include "Certificate.h"

#include "openssl/x509v3.h"
#include "openssl/pem.h"

static const char *exname[] = 
{
   "X509v3 Basic Constraints",
   "X509v3 Key Usage", 
   "X509v3 Certificate Policies",
   "X509v3 CRL Distribution Points",
   "X509v3 Extended Key Usage",
   "Authority Information Access",
   "X509v3 Authority Key Identifier",
   "Netscape Cert Type",
   "X509v3 Subject Key Identifier",
   "X509v3 Subject Alternative Name",
   "X509v3 Issuer Alternative Name",
   "Netscape Comment",
   "X509v3 Private Key Usage Period",
   "X509v3 Name Constraints",
   "X509v3 Policy Mappings"
};

static const BIT_STRING_BITNAME reason_flags[] = {
{0, "Unused", "unused"},
{1, "Key Compromise", "keyCompromise"},
{2, "CA Compromise", "CACompromise"},
{3, "Affiliation Changed", "affiliationChanged"},
{4, "Superseded", "superseded"},
{5, "Cessation Of Operation", "cessationOfOperation"},
{6, "Certificate Hold", "certificateHold"},
{7, "Privilege Withdrawn", "privilegeWithdrawn"},
{8, "AA Compromise", "AACompromise"},
{-1, NULL, NULL}
};

static const unsigned char char_type[] = {
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
120, 0, 1,40, 0, 0, 0,16,16,16, 0,25,25,16,16,16,
16,16,16,16,16,16,16,16,16,16,16, 9, 9,16, 9,16,
 0,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,16,16,16, 0, 1, 0, 0, 0,
 0,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,16,16,16, 0, 0, 0, 0, 2
};

static const signed char tag2nbyte[] = {
        -1, -1, -1, -1, -1,     /* 0-4 */
        -1, -1, -1, -1, -1,     /* 5-9 */
        -1, -1, 0, -1,          /* 10-13 */
        -1, -1, -1, -1,         /* 15-17 */
        -1, 1, 1,               /* 18-20 */
        -1, 1, 1, 1,            /* 21-24 */
        -1, 1, -1,              /* 25-27 */
        4, -1, 2                /* 28-30 */
};

static const char *mon[12]=
{
   "Jan","Feb","Mar","Apr","May","Jun",
   "Jul","Aug","Sep","Oct","Nov","Dec"
};

struct Certificate *certificate;

//certificate segment value
GtkTreeIter   iter_v;

//save tree(certificate segmemt)
GtkWidget *tree_s;

//save certificate chain info
char *ce_chain_value;
char *ce_chain_value1;
char *ce_chain_value2;

char *root_cerpath="./Source/BrowserApp/resources/assets/root_certificate/";

//ASN1_Time to SIZE_T_Time
static char * asn1_gettime(ASN1_TIME* time,char *date)
{
   const char*str;
   int year,mon,day;

   str=(const char*) time->data;
   if (time->type==V_ASN1_UTCTIME) 
   {
      year=(str[0]-'0')*10+(str[1]-'0');
      if(year<50)
      {
         year+=2000;
      }
      else year+=1900;
      mon=((str[2]-'0')*10+(str[3]-'0'));
      day=(str[4]-'0')*10+(str[5]-'0');
      sprintf(date,"%d年%02d月%02d日",year,mon,day);
   }
   else if(time->type==V_ASN1_GENERALIZEDTIME) 
   {
      year=(str[0]-'0')*1000+(str[1]-'0')*100+(str[2]-'0')*10+(str[3]-'0');
      mon=(str[4]-'0')*10+(str[5]-'0');
      day=(str[6]-'0')*10+(str[7]-'0');
      sprintf(date,"%d年%02d月%02d日",year,mon,day);   
   }
   return date;
}

//X509_ALGOR* convert to char*
static void x509_algor( X509_ALGOR *sigalg,char **sig_alg)
{
    char buf[80];
    int i;

    i=i2t_ASN1_OBJECT(buf,sizeof buf,sigalg->algorithm);
    if (i > 0)
    {
       *sig_alg = malloc(i + 1);
       i2t_ASN1_OBJECT(*sig_alg,i + 1,sigalg->algorithm);
    }
}

//ASN1_STRING* convert to char*
static void x509_asn1_string(ASN1_STRING *sig,char **signature) 
{
   unsigned char *s;
   int i, n;
   char tmpdata[4];

   n=sig->length;
   s=sig->data;
   *signature = (char *)malloc(n*3+1+n/18);
   for (i=0; i<n; i++)
   {     
      sprintf(tmpdata,"%02x ",s[i]);
      if(i==0)
         strcpy(*signature,tmpdata);
      else
      {
         if ((i%18) == 0)
           strcat(*signature,"\n");  
         strcat(*signature,tmpdata);
      }
   }
}

static void update_buflen(const BIGNUM *b, size_t *pbuflen)
{
   size_t i;
   if (!b) return;
   if (*pbuflen < (i = (size_t)BN_num_bytes(b)))
      *pbuflen = i;
}

// RSA* convert to char*
static void x509_get_pkey_value(RSA *rsa,char **pkey_value)
{
   unsigned char *m=NULL;
   int mod_len = 0;
   size_t buf_len=0;
   char tmpdata[32];
   int n,i;

   update_buflen(rsa->n, &buf_len);
   update_buflen(rsa->e, &buf_len);  
   m=(unsigned char *)malloc(buf_len+10);
   mod_len = BN_num_bits(rsa->n);
   m[0]=0;
   n=BN_bn2bin(rsa->n,&m[1]);
   *pkey_value = (char *)malloc(sizeof(tmpdata)*3+n*3+n/18);
   sprintf(tmpdata,"Private-Key: (%d bit)\n",mod_len);
   strcpy(*pkey_value,tmpdata);
   strcat(*pkey_value,"Modulus:\n");
   if (m[1] & 0x80)
      n++;
   else    
      m++;
   for (i=1; i<n; i++)
   {     
      sprintf(tmpdata,"%02x%s",m[i],((i+1) == n)?"\n":":");
      strcat(*pkey_value,tmpdata);
      if ((i%18) == 0)
        strcat(*pkey_value,"\n");  
   }
   free(m);
   m = NULL; 
   sprintf(tmpdata,"Exponent: %s%lu (%s0x%lx)",(BN_is_negative(rsa->e))?"-":"",
                        (unsigned long)(rsa->e)->d[0],(BN_is_negative(rsa->e))?"-":"",
                        (unsigned long)(rsa->e)->d[0]);
   strcat(*pkey_value,tmpdata);  
}

typedef int char_io(char *value, const void *buf, int len);

static int send_bio_char(char *value, const void *buf, int len)
{
   int i;
   if(!value) return 1;
   if(strlen(buf) < len)
      return 0;
   else if(strlen(buf) > len)
   {
      for(i=0;i<strlen(buf)-len;i++)
         strcat(value," ");       
   }
   strcat(value,buf);
   return 1;
}

static int do_indent(char_io *io_ch, char *value, int indent)
{
   int i;
   for(i = 0; i < indent; i++)
     if(!io_ch(value, " ", 1)) 
        return 0;
   return 1;
}

static int do_esc_char1(unsigned long c, unsigned char flags, char *do_quotes, char_io *io_ch, char *arg)
{
   unsigned char chflgs, chtmp;
   char tmphex[HEX_SIZE(long)+3];

   if(c > 0xffffffffL)
      return -1;
   if(c > 0xffff) 
   {
      sprintf(tmphex, "%d\\W%08lX",sizeof(tmphex), c);
      if(!io_ch(arg, tmphex, 10)) 
         return -1;
      return 10;
   }
   if(c > 0xff) 
   {
      sprintf(tmphex, "%d\\U%04lX",sizeof(tmphex), c);
      if(!io_ch(arg, tmphex, 6)) 
         return -1;
      return 6;
   }
   chtmp = (unsigned char)c;
   if(chtmp > 0x7f) 
      chflgs = flags & ASN1_STRFLGS_ESC_MSB;
   else 
      chflgs = char_type[chtmp] & flags;
   if(chflgs & CHARTYPE_BS_ESC) 
   {
      /* If we don't escape with quotes, signal we need quotes */
      if(chflgs & ASN1_STRFLGS_ESC_QUOTE) 
      {
         if(do_quotes) 
            *do_quotes = 1;
         if(!io_ch(arg, &chtmp, 1)) 
            return -1;
         return 1;
      }
      if(!io_ch(arg, "\\", 1)) 
         return -1;
      if(!io_ch(arg, &chtmp, 1)) 
         return -1;
      return 2;
   }
   if(chflgs & (ASN1_STRFLGS_ESC_CTRL|ASN1_STRFLGS_ESC_MSB)) 
   {
      sprintf(tmphex,"%s\\%02X","11", chtmp);
      if(!io_ch(arg, tmphex, 3)) 
         return -1;
      return 3;
   }
   /* If we get this far and do any escaping at all must escape 
    * the escape character itself: backslash.
    */
   if (chtmp == '\\' && flags & ESC_FLAGS) 
   {
      if(!io_ch(arg, "\\\\", 2)) 
         return -1;
      return 2;
   }
   if(!io_ch(arg, &chtmp, 1)) 
      return -1;
   return 1;
}

static int do_buf1(unsigned char *buf, int buflen,
                        int type, unsigned char flags, char *quotes, char_io *io_ch, char *arg)
{
   int i, outlen, len;
   unsigned char orflags, *p, *q;
   unsigned long c;
   p = buf;
   q = buf + buflen;
   outlen = 0;
   while(p != q) 
   {
      if(p == buf && flags & ASN1_STRFLGS_ESC_2253) 
         orflags = CHARTYPE_FIRST_ESC_2253;
      else 
         orflags = 0;
      switch(type & BUF_TYPE_WIDTH_MASK) 
      {
         case 4:
            c = ((unsigned long)*p++) << 24;
            c |= ((unsigned long)*p++) << 16;
            c |= ((unsigned long)*p++) << 8;
            c |= *p++;
            break;
         case 2:
            c = ((unsigned long)*p++) << 8;
            c |= *p++;
            break;
         case 1:
            c = *p++;
            break;
         case 0:
            i = UTF8_getc(p, buflen, &c);
            if(i < 0) 
               return -1;    /* Invalid UTF8String */
            p += i;
            break;
         default:
            return -1;      /* invalid width */
      }
      if (p == q && flags & ASN1_STRFLGS_ESC_2253) 
         orflags = CHARTYPE_LAST_ESC_2253;
      if(type & BUF_TYPE_CONVUTF8) 
      {
         unsigned char utfbuf[6];
         int utflen;
         utflen = UTF8_putc(utfbuf, sizeof utfbuf, c);
         for(i = 0; i < utflen; i++) 
         {
            /* We don't need to worry about setting orflags correctly
             * because if utflen==1 its value will be correct anyway 
             * otherwise each character will be > 0x7f and so the 
             * character will never be escaped on first and last.
             */
             len = do_esc_char1(utfbuf[i], (unsigned char)(flags | orflags), quotes, io_ch, arg);
             if(len < 0) 
                return -1;
             outlen += len;
         }
      } 
      else 
      {
         len = do_esc_char1(c, (unsigned char)(flags | orflags), quotes, io_ch, arg);
         if(len < 0) 
            return -1;
         outlen += len;
     }
  }
  return outlen;
}

static int do_hex_dump1(char_io *io_ch, char *arg, unsigned char *buf, int buflen)
{
   static const char hexdig[] = "0123456789ABCDEF";
   unsigned char *p, *q;
   char hextmp[2];
   if(arg) 
   {
      p = buf;
      q = buf + buflen;
      while(p != q) 
      {
         hextmp[0] = hexdig[*p >> 4];
         hextmp[1] = hexdig[*p & 0xf];
         if(!io_ch(arg, hextmp, 2)) 
            return -1;
         p++;
      }
   }
   return buflen << 1;
}

static int do_dump1(unsigned long lflags, char_io *io_ch, char *arg, ASN1_STRING *str)
{
   /* Placing the ASN1_STRING in a temp ASN1_TYPE allows
    * the DER encoding to readily obtained
    */
   ASN1_TYPE t;
   unsigned char *der_buf=NULL, *p=NULL;
   int outlen, der_len;

   if(!io_ch(arg, "#", 1)) 
      return -1;
   /* If we don't dump DER encoding just dump content octets */
   if(!(lflags & ASN1_STRFLGS_DUMP_DER)) 
   {
      outlen = do_hex_dump1(io_ch, arg, str->data, str->length);
      if(outlen < 0) 
         return -1;
      return outlen + 1;
   }
   t.type = str->type;
   t.value.ptr = (char *)str;
   der_len = i2d_ASN1_TYPE(&t, NULL);
   der_buf = OPENSSL_malloc(der_len);
   if(!der_buf) 
      return -1;
   p = der_buf;
   i2d_ASN1_TYPE(&t, &p);
   outlen = do_hex_dump1(io_ch, arg, der_buf, der_len);
   OPENSSL_free(der_buf);
   p=NULL;
   if(outlen < 0) 
      return -1;
   return outlen + 1;
}

static int do_ex(char_io *io_ch, char *arg, unsigned long lflags, ASN1_STRING *str)
{
   int outlen, len;
   int type;
   char quotes;
   unsigned char flags;

   quotes = 0;
   /* Keep a copy of escape flags */
   flags = (unsigned char)(lflags & ESC_FLAGS);
   type = str->type;
   outlen = 0;
   if(lflags & ASN1_STRFLGS_SHOW_TYPE) 
   {
      const char *tagname;
      tagname = ASN1_tag2str(type);
      outlen += strlen(tagname);
      if(!io_ch(arg, tagname, outlen) || !io_ch(arg, ":", 1)) 
         return -1;
      outlen++;
   }
   /* Decide what to do with type, either dump content or display it */

   /* Dump everything */
   if(lflags & ASN1_STRFLGS_DUMP_ALL) 
      type = -1;
   /* Ignore the string type */
   else if(lflags & ASN1_STRFLGS_IGNORE_TYPE) 
      type = 1;
   else 
   {
      /* Else determine width based on type */
         if((type > 0) && (type < 31)) 
            type = tag2nbyte[type];
         else 
            type = -1;
         if((type == -1) && !(lflags & ASN1_STRFLGS_DUMP_UNKNOWN)) 
            type = 1;
   }
   if(type == -1) 
   {
      len = do_dump1(lflags, io_ch, arg, str);
      if(len < 0) 
         return -1;
      outlen += len;
      return outlen;
   }
   if(lflags & ASN1_STRFLGS_UTF8_CONVERT) 
   {
      /* Note: if string is UTF8 and we want
       * to convert to UTF8 then we just interpret
       * it as 1 byte per character to avoid converting
       * twice.
       */
       if(!type) 
          type = 1;
       else 
          type |= BUF_TYPE_CONVUTF8;
   }
   len = do_buf1(str->data, str->length, type, flags, &quotes, io_ch, NULL);
   if(len < 0) 
      return -1;
   outlen += len;
   if(quotes) 
      outlen += 2;
   if(!arg) 
      return outlen;
   if(quotes && !io_ch(arg, "\"", 1)) 
      return -1;
   if(do_buf1(str->data, str->length, type, flags, NULL, io_ch, arg) < 0)
      return -1;
   if(quotes && !io_ch(arg, "\"", 1)) 
      return -1;
   return outlen;
}

static void get_name_ex(char_io *io_ch, char *value, X509_NAME *n,int indent,unsigned long flags)
{
   int i, prev = -1, orflags, cnt;
   int fn_opt, fn_nid;
   ASN1_OBJECT *fn;
   ASN1_STRING *val;
   X509_NAME_ENTRY *ent;
   char objtmp[80];
   const char *objbuf;
   int len;
   char *sep_dn, *sep_mv, *sep_eq;
   int sep_dn_len, sep_mv_len, sep_eq_len;

   if(!do_indent(io_ch, value, indent)) 
     return ;
   switch (flags & XN_FLAG_SEP_MASK)
   {
      case XN_FLAG_SEP_MULTILINE:
         sep_dn = "\n";
         sep_dn_len = 1;
         sep_mv = " + ";
         sep_mv_len = 3;
         break;
      case XN_FLAG_SEP_COMMA_PLUS:
         sep_dn = ",";
         sep_dn_len = 1;
         sep_mv = "+";
         sep_mv_len = 1;
         indent = 0;
         break;
      case XN_FLAG_SEP_CPLUS_SPC:
         sep_dn = ", ";
         sep_dn_len = 2;
         sep_mv = " + ";
         sep_mv_len = 3;
         indent = 0;
         break;
      case XN_FLAG_SEP_SPLUS_SPC:
         sep_dn = "; ";
         sep_dn_len = 2;
         sep_mv = " + ";
         sep_mv_len = 3;
         indent = 0;
         break;
      default:
         return;
   }
   if(flags & XN_FLAG_SPC_EQ) 
   {
      sep_eq = " = ";
      sep_eq_len = 3;
   } 
   else 
   {
      sep_eq = "=";
      sep_eq_len = 1;
   }
   fn_opt = flags & XN_FLAG_FN_MASK;
   cnt = X509_NAME_entry_count(n);
   for(i = 0; i < cnt; i++) 
   {
      if(flags & XN_FLAG_DN_REV)
         ent = X509_NAME_get_entry(n, cnt - i - 1);
      else 
         ent = X509_NAME_get_entry(n, i);
      if(prev != -1) 
      {
         if(prev == ent->set) 
         {
            if(!io_ch(value, sep_mv, sep_mv_len)) 
               return ;
         } 
         else 
         {
            if(!io_ch(value, sep_dn, sep_dn_len)) 
               return ;
            if(!do_indent(io_ch, value, indent)) 
               return ;
         }
      }
      prev = ent->set;
      fn = X509_NAME_ENTRY_get_object(ent);
      val = X509_NAME_ENTRY_get_data(ent);
      fn_nid = OBJ_obj2nid(fn);
      if(fn_opt != XN_FLAG_FN_NONE) 
      {
         int objlen, fld_len;
         if((fn_opt == XN_FLAG_FN_OID) || (fn_nid==NID_undef) ) 
         {
            OBJ_obj2txt(objtmp, sizeof objtmp, fn, 1);
            fld_len = 0;
            objbuf = objtmp;
         } 
         else 
         {
            if(fn_opt == XN_FLAG_FN_SN) 
            {
               fld_len = FN_WIDTH_SN;
               objbuf = OBJ_nid2sn(fn_nid);
            } 
            else if(fn_opt == XN_FLAG_FN_LN) 
            {
               fld_len = FN_WIDTH_LN;
               objbuf = OBJ_nid2ln(fn_nid);
            } 
            else 
            {
               fld_len = 0; 
               objbuf = "";
            }
         }
         objlen = strlen(objbuf);
         if(!io_ch(value, objbuf, objlen)) 
            return ;
         if ((objlen < fld_len) && (flags & XN_FLAG_FN_ALIGN)) 
         {
            if (!do_indent(io_ch, value, fld_len - objlen)) 
               return ;
         }
         if(!io_ch(value, sep_eq, sep_eq_len)) 
            return ;
      }
      /* If the field name is unknown then fix up the DER dump
      * flag. We might want to limit this further so it will
      * DER dump on anything other than a few 'standard' fields.
      */
      if((fn_nid == NID_undef) && (flags & XN_FLAG_DUMP_UNKNOWN_FIELDS))
         orflags = ASN1_STRFLGS_DUMP_ALL;
      else 
         orflags = 0;
      len = do_ex(io_ch, value, flags | orflags, val);
      if(len < 0) 
         return ;
   }  
}

static void x509_name_get_ex(char *value, X509_NAME *nm,int indent, unsigned long flags)
{
   //if(flags == XN_FLAG_COMPAT)
     // X509_NAME_print(out, nm, indent);
    get_name_ex(send_bio_char, value, nm,indent ,flags);
}

static void get_gens(char *value, STACK_OF(GENERAL_NAME) *gens)
{
   int i,j;
   unsigned char *p=NULL;
   GENERAL_NAME *gen;
   char buf[80];
   char *tmp=NULL;
   for (i = 0; i < sk_GENERAL_NAME_num(gens); i++)
   {
      gen = sk_GENERAL_NAME_value(gens, i);
      switch (gen->type)
      {
         case GEN_OTHERNAME:
            strcat(value, "  othername:<unsupported>");
            break;
         case GEN_X400:
            strcat(value, "   X400Name:<unsupported>");
            break;
         case GEN_EDIPARTY:
            strcat(value, "  EdiPartyName:<unsupported>");
            break;
         case GEN_EMAIL:
            strcat(value, "  email:");
            strcat(value, gen->d.ia5->data);
            break;
         case GEN_DNS:
            strcat(value, "  DNS:");
            strcat(value, gen->d.ia5->data);
            break;
         case GEN_URI:
            strcat(value, "  URI:");
            strcat(value, gen->d.ia5->data);
            break;
         case GEN_DIRNAME:
            strcat(value, "  DirName: ");
            x509_name_get_ex(value, gen->d.dirn,0,XN_FLAG_ONELINE);
            break;
         case GEN_IPADD:
            p = gen->d.ip->data;
            if(gen->d.ip->length == 4)
            {
               sprintf(buf,"  IP Address:%d.%d.%d.%d",p[0], p[1], p[2], p[3]);
               strcat(value, buf);
            }
            else if(gen->d.ip->length == 16)
            {
               strcat(value, "  IP Address");
               for (j = 0; j < 8; j++)
               {
                  sprintf(buf,":%X", p[0] << 8 | p[1]);    
                  strcat(value, buf);
                  p += 2;
               }
            }
            else
            {
               strcat(value, "  IP Address:<invalid>");  
               break;
            }
            break;
         case GEN_RID:
            strcat(value, "  Registered ID"); 
            j=i2t_ASN1_OBJECT(buf,sizeof buf,gen->d.rid);
            if (j > 0)
            {
               tmp = malloc(j + 1);
               i2t_ASN1_OBJECT(tmp,j + 1,gen->d.rid);
               strcat(value, tmp);
               free(tmp);
               tmp = NULL;
            }    
            break;
      }            
      strcat(value, "\n");
      p=NULL;
   }                
}

static void get_dispoint(char *value,DIST_POINT_NAME *dpn)
{
   X509_NAME ntmp;

   if (dpn->type == 0)
   {
      strcat(value,"Full Name:");
      strcat(value,"\n");
      get_gens(value,dpn->name.fullname);      
   }
   else
   {
      ntmp.entries = dpn->name.relativename;
      strcat(value,"Relative Name:");
      strcat(value,"\n");
      x509_name_get_ex(value, &ntmp,0, XN_FLAG_ONELINE);
      strcat(value,"\n");    
   }   
}

static void get_reasons( char *value, const char *rname,ASN1_BIT_STRING *rflags)
{
    int first = 1;
    const BIT_STRING_BITNAME *pbn;

    strcat(value,rname);
    for (pbn = reason_flags; pbn->lname; pbn++)
    {
       if (ASN1_BIT_STRING_get_bit(rflags, pbn->bitnum))
       {
          if (first)
             first = 0;
          else
             strcat(value,", ");
          strcat(value,pbn->lname);    
       }
    }   
    if (first)
       strcat(value,"<EMPTY>");
    strcat(value,"\n");
}

static void get_generalizedtime(char *value, const ASN1_GENERALIZEDTIME *tm)
{
   char *v=NULL;
   int gmt=0;
   int i;
   int y=0,M=0,d=0,h=0,m=0,s=0;
   char *f = NULL;
   int f_len = 0;
   char tmp[80];

   i=tm->length;
   v=(char *)tm->data;

   if (i < 12)
   { 
      strcat(value,"Bad time value");
      return;
   }
   if (v[i-1] == 'Z') 
      gmt=1;
   for (i=0; i<12; i++)
      if ((v[i] > '9') || (v[i] < '0')) 
      {
         strcat(value,"Bad time value");
         return;   
      }
   y= (v[0]-'0')*1000+(v[1]-'0')*100 + (v[2]-'0')*10+(v[3]-'0');
   M= (v[4]-'0')*10+(v[5]-'0');
   if ((M > 12) || (M < 1)) 
   {
      strcat(value,"Bad time value");
      return; 
   }   
   d= (v[6]-'0')*10+(v[7]-'0');
   h= (v[8]-'0')*10+(v[9]-'0');
   m=  (v[10]-'0')*10+(v[11]-'0');
   if (tm->length >= 14 &&(v[12] >= '0') && (v[12] <= '9') &&(v[13] >= '0') && (v[13] <= '9'))
   {
      s=  (v[12]-'0')*10+(v[13]-'0');
      /* Check for fractions of seconds. */
      if (tm->length >= 15 && v[14] == '.')
      {
         int l = tm->length;
         f = &v[14];     /* The decimal point. */
         f_len = 1;
         while (14 + f_len < l && f[f_len] >= '0' && f[f_len] <= '9')
            ++f_len;
      }
   }
   sprintf(tmp,"%s %2d %02d:%02d:%02d%.*s %d%s",mon[M-1],d,h,m,s,f_len,f,y,(gmt)?" GMT":"");       
   strcat(value,tmp);
   memset(tmp,0,sizeof(tmp));
   v=NULL;
   f=NULL;   
}

static void get_pkey_usage_period( PKEY_USAGE_PERIOD *usage,char *value)
{
   if(usage->notBefore) 
   {
      strcat(value,"Not Before: ");     
      get_generalizedtime(value, usage->notBefore);
      if(usage->notAfter) 
         strcat(value,", ");
   }
   if(usage->notAfter) 
   {
      strcat(value,"Not After: ");
      get_generalizedtime(value, usage->notAfter);
   }
}

static void get_crldp( void *pcrldp,char *value)
{
   STACK_OF(DIST_POINT) *crld = pcrldp;
   DIST_POINT *point;
   int i;

   for(i = 0; i < sk_DIST_POINT_num(crld); i++)
   {
      point = sk_DIST_POINT_value(crld, i);
      if(point->distpoint)
         get_dispoint(value,point->distpoint);
      if(point->reasons)     
         get_reasons(value, "Reasons:", point->reasons);
      if(point->CRLissuer)
      {
         strcat(value,"CRL Issuer:");
         strcat(value,"\n");
         get_gens(value, point->CRLissuer);
      }
   }
}

static void get_certpol(STACK_OF(POLICYINFO) *ext_str,char *value)
{
   POLICYINFO *pinfo;
   POLICYQUALINFO *qualinfo;
   USERNOTICE *notice;
   ASN1_INTEGER *num;
   int i,j,n;
   char buf[80],*tmp=NULL;
   
   for(i = 0; i < sk_POLICYINFO_num(ext_str); i++) 
   {
      pinfo = sk_POLICYINFO_value(ext_str, i);
      strcat(value, "Policy: ");
      j=i2t_ASN1_OBJECT(buf,sizeof buf,pinfo->policyid);
      if (j > 0)
      {
         tmp = malloc(j + 1);
         i2t_ASN1_OBJECT(tmp,j + 1,pinfo->policyid);
         strcat(value, tmp);
         free(tmp);
         tmp = NULL;
      }
      if(pinfo->qualifiers)
      {
         strcat(value,"\n");
         for(j = 0; j < sk_POLICYQUALINFO_num(pinfo->qualifiers); j++) 
         {
            qualinfo = sk_POLICYQUALINFO_value(pinfo->qualifiers, j);
            switch(OBJ_obj2nid(qualinfo->pqualid))
            {
               case NID_id_qt_cps:
                  strcat(value, "  CPS: ");
                  strcat(value,qualinfo->d.cpsuri->data);
                  strcat(value,"\n");
                  break;
               case NID_id_qt_unotice:
                  strcat(value, "  User Notice: ");
                  notice = qualinfo->d.usernotice;
                  if(notice->noticeref)
                  {
                     strcat(value,"\n");
                     strcat(value,"    Organization:");
                     strcat(value,notice->noticeref->organization->data);
                     strcat(value,"\n");
                     strcat(value,"    Number:");
                     for(n = 0; n < sk_ASN1_INTEGER_num(notice->noticeref->noticenos); n++) 
                     {
                        num = sk_ASN1_INTEGER_value(notice->noticeref->noticenos, n);
                        if(n) strcat(value,", ");
                        tmp = i2s_ASN1_INTEGER(NULL, num);
                        strcat(value,tmp);
                        tmp = NULL;                
                     }
                     strcat(value,"\n");
                  }
                  if(notice->exptext)
                  {
                     strcat(value,"\n");
                     strcat(value,"      Explicit Text:");
                     strcat(value,notice->exptext->data);
                     strcat(value,"\n");
                  }                    
                  break;
               default:
                  strcat(value, "  Unknown Qualifier: ");
                  n=i2t_ASN1_OBJECT(buf,sizeof buf,qualinfo->pqualid);
                  if (n > 0)
                  {
                     tmp = malloc(n + 1);
                     i2t_ASN1_OBJECT(tmp,n + 1,qualinfo->pqualid);
                     strcat(value, tmp);
                     free(tmp);
                     tmp = NULL;                        
                  }
                  break;
            }
         }
      }
   }
}

static void get_nc_ipadd(char *value, ASN1_OCTET_STRING *ip)
{
   int i, len;
   unsigned char *p;
   char tmp[80];
   p = ip->data;
   len = ip->length;
   strcat(value,"IP:");
   if(len == 8)
   {
      sprintf(tmp,"%d.%d.%d.%d/%d.%d.%d.%d",p[0], p[1], p[2], p[3],p[4], p[5], p[6], p[7]);
      strcat(value,tmp);
   }
   else if(len == 32)
   {
      for (i = 0; i < 16; i++)
      {
         sprintf(tmp,"%X",p[0] << 8 | p[1]);
         strcat(value,tmp);
         p += 2;
         if (i == 7)
            strcat(value,"/");
         else if (i != 15)
            strcat(value,":");
      }
   }
   else
      strcat(value,"IP Address:<invalid>");
   memset(tmp,0,sizeof(tmp));
}

static void get_general_name(char *value, GENERAL_NAME *gen)
{
   unsigned char *p;
   int i,j;
   char tmp[80];
   char *p1;

   switch (gen->type)
   {
      case GEN_OTHERNAME:
         strcat(value,"othername:<unsupported>");
         break;
      case GEN_X400:
         strcat(value,"X400Name:<unsupported>");
         break;
      case GEN_EDIPARTY:
         /* Maybe fix this: it is supported now */
         strcat(value,"EdiPartyName:<unsupported>");
         break;
      case GEN_EMAIL:
         sprintf(tmp,"email:%s",gen->d.ia5->data);
         strcat(value,tmp);
         break;
      case GEN_DNS:
         sprintf(tmp,"DNS:%s",gen->d.ia5->data);
         strcat(value,tmp); 
         break;
      case GEN_URI:
         sprintf(tmp,"URI:%s",gen->d.ia5->data);
         strcat(value,tmp); 
         break;
      case GEN_DIRNAME:
         strcat(value,"DirName: ");
         x509_name_get_ex(value, gen->d.dirn, 0, XN_FLAG_ONELINE);
         break;
      case GEN_IPADD:
         p = gen->d.ip->data;
         if(gen->d.ip->length == 4)
         {
            sprintf(tmp,"IP Address:%d.%d.%d.%d",p[0], p[1], p[2], p[3]);
            strcat(value,tmp);
         }
         else if(gen->d.ip->length == 16)
         {
            strcat(value,"IP Address");
            for (i = 0; i < 8; i++)
            {
               sprintf(tmp,":%X",p[0] << 8 | p[1]);
               strcat(value,tmp);
               p += 2;
            }
            strcat(value,"\n");
         }
         else
            strcat(value,"IP Address:<invalid>");
         break;
      case GEN_RID:
         strcat(value,"Registered ID");
         j=i2t_ASN1_OBJECT(tmp,sizeof tmp,gen->d.rid);
         if (j > 0)
         {
            p1 = malloc(j + 1);
            i2t_ASN1_OBJECT(p1,j + 1,gen->d.rid);
            strcat(value,p1);
            free(p1);
            p1 = NULL;
         }
         break;
   }
   memset(tmp,0,sizeof(tmp));
   p=NULL;
}

static void get_i2r_name_constraints( STACK_OF(GENERAL_SUBTREE) *trees,char *value,char *name)
{
   GENERAL_SUBTREE *tree;
   int i;

   if (sk_GENERAL_SUBTREE_num(trees) > 0)
   {
      strcat(value,name);
      strcat(value,"\n");
   }
   for(i = 0; i < sk_GENERAL_SUBTREE_num(trees); i++)
   {
      tree = sk_GENERAL_SUBTREE_value(trees, i);
      strcat(value,"  ");
      if (tree->base->type == GEN_IPADD)
         get_nc_ipadd(value, tree->base->d.ip);
      else
         get_general_name(value, tree->base);
      strcat(value,"\n");
   }

}

static void get_name_constraints(void *ext_str,char *value)
{
     NAME_CONSTRAINTS *ncons = ext_str;
     get_i2r_name_constraints(ncons->permittedSubtrees,value,"Permitted:");
     get_i2r_name_constraints(ncons->excludedSubtrees,value,"Excluded:");

}

//get expand value
static char* get_expand_value(X509_EXTENSION *ext, char *value,char *name)
{
   void *ext_str = NULL;
   const unsigned char *p;
   const X509V3_EXT_METHOD *method;
   STACK_OF(CONF_VALUE) *nval = NULL;
   CONF_VALUE *val;
   int i,j;

   strcpy(value,"\n");
   j=X509_EXTENSION_get_critical(ext);
   if(j != 0)
   {
      strcat(value,"critical");
      strcat(value,"\n");
   }  
   if(!(method = X509V3_EXT_get(ext)))
      return NULL;
   p = ext->value->data;
   if(method->it) 
      ext_str = ASN1_item_d2i(NULL, &p, ext->value->length, ASN1_ITEM_ptr(method->it));
   else 
      ext_str = method->d2i(NULL, &p, ext->value->length);
   if(!ext_str)
      return NULL;
   if(method->i2v)
   {
      nval = method->i2v(method, ext_str, NULL);
      for(i = 0; i < sk_CONF_VALUE_num(nval); i++) 
      {
         val = sk_CONF_VALUE_value(nval, i);
         if(!val->name)
         {
            if(i==0)
            {
               strcat(value,val->value);
               strcat(value,"\n");
            }
            else
            {               
               strcat(value,val->value);
               strcat(value,"\n");
            }
         }
         else if(!val->value)
         {
            if(i==0)
            {
               strcat(value,val->name);
               strcat(value,"\n");
            }
            else
            {
               strcat(value,val->name);
               strcat(value,"\n");
            }
         }      
         else 
         {
            strcat(value,val->name);
            strcat(value,":");
            strcat(value,val->value);
            strcat(value,"\n");
         }
      }
   }
   else if(method->i2s) 
      strcat(value,method->i2s(method, ext_str));
   else if(method->i2r) 
   {
      if(strcmp(name,"X509v3 CRL Distribution Points")==0)
         get_crldp(ext_str,value);
      else if (strcmp(name,"X509v3 Certificate Policies")==0)
         get_certpol(ext_str,value);
      else if (strcmp(name,"X509v3 Private Key Usage Period")==0)
         get_pkey_usage_period(ext_str,value); 
      else if (strcmp(name,"X509v3 Name Constraints")==0)
         get_name_constraints(ext_str,value);    
   }
   return value;
}
       
static void get_expand_info(STACK_OF(X509_EXTENSION) *exts)
{
   int i,j;
   char buf[80];
   ASN1_OBJECT *obj;
   X509_EXTENSION *ex;
   char *name=NULL;
   char tmp_value[1024];
   char *expand_value=NULL;

   for (i=0; i<sk_X509_EXTENSION_num(exts); i++)
   {
      ex=sk_X509_EXTENSION_value(exts, i);
      obj=X509_EXTENSION_get_object(ex);
      j=i2t_ASN1_OBJECT(buf,sizeof buf,obj);
      if (j > 0)
      {
         name = malloc(j + 1);
         i2t_ASN1_OBJECT(name,j + 1,obj);
      }
      expand_value = get_expand_value(ex,tmp_value,name);
      if(!expand_value)
      {
         certificate->exinfo->oid=(char *)malloc(strlen(name)+1); 
         strcpy(certificate->exinfo->oid,name);
      }
      else if(strlen(expand_value))
      {
         for (j = 0; j < 15; j++) 
         {
            if (strcmp(name,exname[j])==0) 
            {
               switch (j) 
               {
                  case 0:
                     certificate->exinfo->basicConst=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->basicConst,expand_value);
                     break;
                  case 1:
                     certificate->exinfo->keyUsage=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->keyUsage,expand_value);
                     break;                 
                  case 2:
                     certificate->exinfo->certPolicies=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->certPolicies,expand_value);
                     break;
                  case 3:
                     certificate->exinfo->crlDist=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->crlDist,expand_value);
                     break;
                  case 4:
                     certificate->exinfo->exkeyUsage=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->exkeyUsage,expand_value);
                     break;
                  case 5:
                     certificate->exinfo->infoAccess=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->infoAccess,expand_value);
                     break;
                  case 6:
                     certificate->exinfo->autokeyIdentifier=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->autokeyIdentifier,expand_value);
                     break;
                  case 7:
                     certificate->exinfo->netscertType=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->netscertType,expand_value);
                     break;
                  case 8:
                     certificate->exinfo->subkeyIdentifier=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->subkeyIdentifier,expand_value);
                     break;
                  case 9:
                     certificate->exinfo->subaltName=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->subaltName,expand_value);
                     break;
                  case 10:
                     certificate->exinfo->issaltName=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->issaltName,expand_value);
                     break;
                  case 11:
                     certificate->exinfo->netsComment=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->netsComment,expand_value);
                     break;
                  case 12:
                     certificate->exinfo->prikeyPeriod=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->prikeyPeriod,expand_value);
                     break;
                  case 13:
                     certificate->exinfo->nameConstraints=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->nameConstraints,expand_value);
                     break;
                  case 14:
                     certificate->exinfo->polMappings=(char *)malloc(strlen(expand_value)+1); 
                     strcpy(certificate->exinfo->polMappings,expand_value);
                     break;
               }
            }
         }         
         memset(tmp_value,0,sizeof(tmp_value));
      }
      free(name);
      name = NULL;
   }   
}

//get single certificate information
void get_certificate_data(const char *certificateData)
{
   BIO *bio_mem=NULL;
   X509 *x509=NULL;
   int i,Nid,entriesNum;

   X509_NAME_ENTRY *name_entry;
   X509_NAME *issuer,*subject;
   ASN1_INTEGER *serialnum;
   char tmpdata[3];
   char date[TIMELENGTH];
   unsigned int fingerNum;
   unsigned char md[EVP_MAX_MD_SIZE];
   const EVP_MD *fdig=NULL;
   EVP_PKEY *pkey;

   if(!ce_chain_value)
   {
      ce_chain_value = (char *)malloc(strlen(certificateData)+1);
      strcpy(ce_chain_value,certificateData);
   }
   bio_mem = BIO_new(BIO_s_mem());
   BIO_puts(bio_mem, certificateData);
   x509 = PEM_read_bio_X509(bio_mem, NULL, NULL, NULL);
   BIO_free(bio_mem);
  
   certificate = (struct Certificate *)malloc(sizeof(struct Certificate));
   memset(certificate,0,sizeof(*certificate));
  
   certificate->by = (struct Issued_By *)malloc(sizeof(struct Issued_By));
   memset(certificate->by,0,sizeof(*(certificate->by)));

   certificate->to = (struct Issued_To *)malloc(sizeof(struct Issued_To));
   memset(certificate->to,0,sizeof(*(certificate->to)));
  
   //version
   sprintf(certificate->version,"%ld",X509_get_version(x509)+1);  

  //issure
   issuer = X509_get_issuer_name(x509);
   entriesNum = sk_X509_NAME_ENTRY_num(issuer->entries);
   for(i=0;i<entriesNum;i++)
   {
       name_entry = sk_X509_NAME_ENTRY_value(issuer->entries,i);
       Nid = OBJ_obj2nid(name_entry->object);
       switch(Nid)
       {
          case NID_countryName:
            certificate->by->countryName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->by->countryName,name_entry->value->data);
            break;
          case NID_stateOrProvinceName: 
            certificate->by->provinceName=(char *)malloc(strlen(name_entry->value->data)+1); 
            strcpy(certificate->by->provinceName,name_entry->value->data);
            break;
          case NID_localityName:
            certificate->by->localityName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->by->localityName,name_entry->value->data);
            break;
          case NID_organizationName:
            certificate->by->organizationName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->by->organizationName,name_entry->value->data);
            break;
          case NID_organizationalUnitName:
            if(certificate->by->organizationalUnitName==NULL)
            {
               certificate->by->organizationalUnitName=(char *)malloc(strlen(name_entry->value->data)+1);
               strcpy(certificate->by->organizationalUnitName,name_entry->value->data);
            }
            else
            {
               certificate->by->organizationalUnitName1=(char *)malloc(strlen(name_entry->value->data)+1);
               strcpy(certificate->by->organizationalUnitName1,name_entry->value->data);
            }
            break;
          case NID_commonName:
            certificate->by->commonName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->by->commonName,name_entry->value->data);
            break;
      }
   }
  
  //subject
   subject = X509_get_subject_name(x509);
   entriesNum = sk_X509_NAME_ENTRY_num(subject->entries);
   for(i=0;i<entriesNum;i++)
   {
       name_entry = sk_X509_NAME_ENTRY_value(subject->entries,i);
       Nid = OBJ_obj2nid(name_entry->object);
       switch(Nid)
       {
          case NID_countryName:
            certificate->to->countryName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->to->countryName,name_entry->value->data);
            break;
          case NID_stateOrProvinceName: 
            certificate->to->provinceName=(char *)malloc(strlen(name_entry->value->data)+1); 
            strcpy(certificate->to->provinceName,name_entry->value->data);
            break;
          case NID_localityName:
            certificate->to->localityName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->to->localityName,name_entry->value->data);
            break;
          case NID_organizationName:
            certificate->to->organizationName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->to->organizationName,name_entry->value->data);
            break;
          case NID_organizationalUnitName:
            if(certificate->to->organizationalUnitName==NULL)
            {
               certificate->to->organizationalUnitName=(char *)malloc(strlen(name_entry->value->data)+1);
               strcpy(certificate->to->organizationalUnitName,name_entry->value->data);
            }
            else
            {
               certificate->to->organizationalUnitName1=(char *)malloc(strlen(name_entry->value->data)+1);
               strcpy(certificate->to->organizationalUnitName1,name_entry->value->data);
            }
            break;
          case NID_commonName:
            certificate->to->commonName=(char *)malloc(strlen(name_entry->value->data)+1);
            strcpy(certificate->to->commonName,name_entry->value->data);
            break;
       }
    }

    //serial number 
    serialnum = X509_get_serialNumber(x509);
    for(i=0; i<serialnum->length; i++) 
    {
        sprintf(tmpdata,"%02x",serialnum->data[i]);
        
        if(i==0)
        {
           strcpy(certificate->serialNumber,tmpdata);
        }
        else
        {
           strcat(certificate->serialNumber,":");
           strcat(certificate->serialNumber,tmpdata);
        }
    }
 
    //period of validity
    strcpy(certificate->startDate,asn1_gettime(X509_get_notBefore(x509),date));
    strcpy(certificate->endDate,asn1_gettime(X509_get_notAfter(x509),date));

    //finger:sha1 information
    fdig = EVP_sha1();
    X509_digest(x509,fdig,md,&fingerNum);
    for (i=0; i<(int)fingerNum; i++)
    { 
        sprintf(tmpdata,"%02x",md[i]);
        if(i==0)
        {
           strcpy(certificate->sha1_t,tmpdata);
        }
        else
        {
           strcat(certificate->sha1_t,":");
           strcat(certificate->sha1_t,tmpdata);
        }          
    }

    //finger:md5 information
    fdig = EVP_md5();
    X509_digest(x509,fdig,md,&fingerNum);
    for (i=0; i<(int)fingerNum; i++)
    { 
        sprintf(tmpdata,"%02x",md[i]);
        if(i==0)
        {
           strcpy(certificate->md5_t,tmpdata);
        }
        else
        {
           strcat(certificate->md5_t,":");
           strcat(certificate->md5_t,tmpdata);
        }          
    }
    
    //Signature
    x509_algor(x509->sig_alg,&certificate->sig_alg);
    x509_asn1_string(x509->signature,&certificate->signature);

   //public key
   x509_algor(x509->cert_info->key->algor,&certificate->pkey_alg);   
   pkey=X509_get_pubkey(x509);
   if(strcmp(certificate->pkey_alg,"rsaEncryption")==0)
      x509_get_pkey_value(pkey->pkey.rsa,&certificate->pkey_value);
   
   //expand info   
   certificate->exinfo = (struct Expand *)malloc(sizeof(struct Expand));
   memset(certificate->exinfo,0,sizeof(*(certificate->exinfo)));
   get_expand_info(x509->cert_info->extensions); 
 
   X509_free(x509);  
}

static void free_certificate_memeory()
{
   free(certificate->by->commonName);
   free(certificate->by->countryName);
   free(certificate->by->localityName);
   free(certificate->by->provinceName);
   free(certificate->by->organizationName);
   free(certificate->by->organizationalUnitName);
   free(certificate->by->organizationalUnitName1);
   memset(certificate->by,0,sizeof(*(certificate->by)));

   free(certificate->to->commonName);
   free(certificate->to->countryName);
   free(certificate->to->localityName);
   free(certificate->to->provinceName);
   free(certificate->to->organizationName);
   free(certificate->to->organizationalUnitName);
   free(certificate->to->organizationalUnitName1);
   memset(certificate->to,0,sizeof(*(certificate->to)));

   free(certificate->exinfo->basicConst);
   free(certificate->exinfo->keyUsage);
   free(certificate->exinfo->certPolicies);
   free(certificate->exinfo->crlDist);
   free(certificate->exinfo->exkeyUsage);
   free(certificate->exinfo->infoAccess);
   free(certificate->exinfo->autokeyIdentifier);
   free(certificate->exinfo->netscertType);
   free(certificate->exinfo->subkeyIdentifier);
   free(certificate->exinfo->subaltName);
   free(certificate->exinfo->issaltName);
   free(certificate->exinfo->netsComment);
   free(certificate->exinfo->nameConstraints);
   free(certificate->exinfo->prikeyPeriod);
   free(certificate->exinfo->polMappings);
   free(certificate->exinfo->oid);
   memset(certificate->exinfo,0,sizeof(*(certificate->exinfo)));

   free(certificate->sig_alg);
   free(certificate->signature);
   free(certificate->pkey_alg);
   free(certificate->pkey_value);
   memset(certificate,0,sizeof(*certificate));
   free(certificate);
   certificate=NULL;
}

static void check_certificate_verfity(char **value)
{
   BIO *bio_mem=NULL;
   X509_STORE_CTX *ctx=NULL;
   X509_STORE *certchain=NULL;   
   X509 *cert=NULL;
   X509 *rootcert=NULL;
   int x509verify;
   long ncode;

   bio_mem=BIO_new(BIO_s_mem());
   BIO_puts(bio_mem,ce_chain_value);
   cert=PEM_read_bio_X509(bio_mem, NULL, NULL, NULL);
   BIO_free(bio_mem);
   certchain = X509_STORE_new();
   if(ce_chain_value1)
   {
      bio_mem=BIO_new(BIO_s_mem());
      BIO_puts(bio_mem,ce_chain_value1);
      rootcert=PEM_read_bio_X509(bio_mem, NULL, NULL, NULL);    
      BIO_free(bio_mem);
      X509_STORE_add_cert(certchain,rootcert);      
      X509_free(rootcert);
   }
   else
      X509_STORE_add_cert(certchain,cert);
   if(ce_chain_value2)
   {
      bio_mem=BIO_new(BIO_s_mem());
      BIO_puts(bio_mem,ce_chain_value2);
      rootcert=PEM_read_bio_X509(bio_mem, NULL, NULL, NULL);
      BIO_free(bio_mem);     
      X509_STORE_add_cert(certchain,rootcert);
      X509_free(rootcert);
   }
            
   ctx=X509_STORE_CTX_new();  
   X509_STORE_CTX_init(ctx,certchain,cert,NULL);
   x509verify=X509_verify_cert(ctx);
   if(x509verify!=1)
   {
      ncode=X509_STORE_CTX_get_error(ctx);
     *value=(char *)X509_verify_cert_error_string(ncode);
      if(strcmp(*value,"certificate signature failure")==0)
         *value=NULL;
   }
   if(ctx!=NULL)
      X509_STORE_CTX_free(ctx);
   if(certchain!=NULL)
      X509_STORE_free(certchain);
   X509_free(cert); 
}

static void get_certificate_chain(char **value)
{
   int i,j;
   char *toname=NULL;
   char *byname=NULL;
   char *path=NULL;
   DIR *dir;
   BIO *b=NULL;
   BIO *out=NULL;
   X509 * x=NULL;
   X509_NAME *issuer,*subject;
   int entriesNum,Nid;
   X509_NAME_ENTRY *name_entry;
   struct dirent *ptr;
   FILE *fp;
   int file_size;
   char *der=NULL;
   char pem[DER_MAX_SIZE];
   char *p=NULL;

   if(!certificate->to->commonName)
   {
      if(certificate->to->organizationalUnitName)
      {
         value[0]=(char *)malloc(strlen(certificate->to->organizationalUnitName)+1);
         strcpy(value[0],certificate->to->organizationalUnitName);
      }
      else
      {
         value[0]=(char *)malloc(strlen(certificate->to->organizationName)+1);
         strcpy(value[0],certificate->to->organizationName);    
      }
      return;
   }
   else
   {
      toname=(char *)malloc(strlen(certificate->to->commonName)+1);
      strcpy(toname,certificate->to->commonName);
      byname=(char *)malloc(strlen(certificate->by->commonName)+1);
      strcpy(byname,certificate->by->commonName);
      for(i=0;i<CHAIN_MAX_SIZE;i++)
      {
         value[i]=(char *)malloc(strlen(toname) +1);
         strcpy(value[i],toname);
         free(toname);
         toname=NULL;
         if(strcmp(value[i],byname)==0)
         {
            free(byname);
            byname=NULL;
            break;
         }
         dir=opendir(root_cerpath);
         while((ptr=readdir(dir))!=NULL)
         {
            if((strcmp(ptr->d_name,".")==0) || (strcmp(ptr->d_name,"..")==0))
               continue;
            path=(char *)malloc(strlen(ptr->d_name)+strlen(root_cerpath)+1);
            strcpy(path,root_cerpath);
            strcat(path,ptr->d_name);
            b=BIO_new_file(path,"r");   
            x=PEM_read_bio_X509(b,NULL,NULL,NULL);
            BIO_free(b);
            if(!x)
            {
               fp=fopen(path,"r");
               fseek(fp,0,SEEK_END);   
               file_size=ftell(fp);
               fseek(fp,0,SEEK_SET);
               der=(char *)malloc(file_size);
               j=fread(der,file_size,1,fp);
               if(!j)
               {                 
                  free(der);
                  der=NULL;
                  return;
               }
               x=d2i_X509(NULL,(const unsigned char ** )&der,file_size);
               fclose(fp);
               if(!x)
               { 
                  //only support der or pem
                  free(der);
                  der=NULL;
                  X509_free(x);
                  return;
               }
            }
            subject=X509_get_subject_name(x);
            entriesNum=sk_X509_NAME_ENTRY_num(subject->entries);
            for(j=0;j<entriesNum;j++)
            {
               name_entry=sk_X509_NAME_ENTRY_value(subject->entries,j);
               Nid=OBJ_obj2nid(name_entry->object);
               if(Nid==NID_commonName)
               { 
                  toname=(char *)malloc(strlen(name_entry->value->data)+1);
                  strcpy(toname,name_entry->value->data);    
                  break;
               }
            }        
            if(!toname)
            { 
               X509_free(x);
               free(path);
               path=NULL;
               continue;
            }
            if(strcmp(byname,toname)==0)
            {
               issuer=X509_get_issuer_name(x); 
               entriesNum=sk_X509_NAME_ENTRY_num(issuer->entries);
               for(j=0;j<entriesNum;j++)
               {
                  name_entry=sk_X509_NAME_ENTRY_value(issuer->entries,j);
                  Nid=OBJ_obj2nid(name_entry->object);
                  if(Nid==NID_commonName)
                  {
                     byname=(char *)malloc(strlen(name_entry->value->data)+1);
                     strcpy(byname,name_entry->value->data);
                     break;
                  }
               }
               X509_free(x);
               break;
            }
            else
            {
               X509_free(x);
               free(toname);
               toname=NULL;
               free(path);
               path=NULL;
            }            
         }
         if(path)
         {
            //store certificate information
            b=BIO_new_file(path,"r");   
            x=PEM_read_bio_X509(b,NULL,NULL,NULL);
            if(x)
            {
               fp=fopen(path,"r");
               fseek(fp,0,SEEK_END);   
               file_size=ftell(fp);
               fseek(fp,0,SEEK_SET);
               if(!ce_chain_value1)
               {
                  ce_chain_value1=(char *)malloc(file_size * sizeof(char));
                  j=fread(ce_chain_value1,file_size,sizeof(char),fp);
                  if(!j)
                  {
                     free(ce_chain_value1);
                     ce_chain_value1=NULL;
                  }
               }
               else
               {
                  ce_chain_value2=(char *)malloc(file_size * sizeof(char));
                  j=fread(ce_chain_value2,file_size,sizeof(char),fp);
                  if(!j)
                  {
                     free(ce_chain_value1);
                     ce_chain_value1=NULL;
                  }               
               }
            }
            else
            {
               b=BIO_new_file(path,"r");
               x=d2i_X509_bio(b,NULL);
               BIO *out = BIO_new(BIO_s_mem());
               PEM_write_bio_X509(out,x);
               BIO_read(out,pem,DER_MAX_SIZE);
               BIO_free(out);
               p=pem;
               for(j=0;;p++,j++)
               {
                  if(*p=='E'&&strncmp(p,"END CERTIFICATE",strlen("END CERTIFICATE"))==0)
                  break;
               }
               if(!ce_chain_value1)
               {
                  ce_chain_value1=(char *)malloc(j+1+strlen("END CERTIFICATE-----"));
                  memcpy(ce_chain_value1,pem,j+strlen("END CERTIFICATE-----"));
                  strcat(ce_chain_value1,"\0");
                  memset(pem,0,sizeof(pem));
               }
               else
               {
                  ce_chain_value2=(char *)malloc(j+1+strlen("END CERTIFICATE-----"));
                  memcpy(ce_chain_value2,pem,j+strlen("END CERTIFICATE-----"));
                  strcat(ce_chain_value2,"\0");
                  memset(pem,0,sizeof(pem));                  
               }
            }
            BIO_free(b);
            X509_free(x);
            free(path);
            path=NULL;
            fclose(fp);
         }
         if(!toname)
         {
            if(byname)
            {
               free(byname);
               byname=NULL;
               break;
            }
         }
      }
   }
}

static void get_tree_store(GtkTreeStore *tree_store)
{
   GtkTreeIter  iters;
   GtkTreeIter  iters_child,iters_child1,iters_child2;
   char tmp[80];

   gtk_tree_store_clear(tree_store);
   gtk_tree_store_append(tree_store, &iters, NULL);
   if(certificate->to->commonName)
      gtk_tree_store_set(tree_store, &iters,0,certificate->to->commonName,-1);
   else if(certificate->to->organizationalUnitName)
      gtk_tree_store_set(tree_store, &iters ,0,certificate->to->organizationalUnitName,-1);
   else
      gtk_tree_store_set(tree_store, &iters ,0,certificate->to->organizationName,-1); 
   gtk_tree_store_append(tree_store, &iters_child, &iters);
   gtk_tree_store_set(tree_store, &iters_child ,0,"证书",-1);   
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"版本",-1);
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"序列号",-1);  
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"颁发者",-1);   
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"有效日期",-1);
   gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
   gtk_tree_store_set(tree_store, &iters_child2 ,0,"不早于",-1);
   gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
   gtk_tree_store_set(tree_store, &iters_child2 ,0,"不晚于",-1);
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"主题",-1);
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"公共密钥信息",-1);
   gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
   gtk_tree_store_set(tree_store, &iters_child2 ,0,"公共密钥算法",-1);
   gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
   gtk_tree_store_set(tree_store, &iters_child2 ,0,"公共密钥值",-1);
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"扩展",-1);
   if(certificate->exinfo->basicConst)
   {
      gtk_tree_store_append(tree_store,&iters_child2,&iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书基本约束",-1);
   }
   if(certificate->exinfo->keyUsage)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书密钥使用",-1);
   }
   if(certificate->exinfo->certPolicies)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书方针",-1);
   }
   if(certificate->exinfo->crlDist)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"CRL分布点",-1);
   }
   if(certificate->exinfo->exkeyUsage)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书扩展密钥使用",-1);
   }
   if(certificate->exinfo->infoAccess)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"授权信息访问",-1);
   }
   if(certificate->exinfo->autokeyIdentifier)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书授权密钥身份",-1);
   }
   if(certificate->exinfo->netscertType)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"Netscape证书类型",-1);
   }
   if(certificate->exinfo->subkeyIdentifier)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书标题密钥身份",-1);
   }
   if(certificate->exinfo->subaltName)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书主题可选名",-1);
   }
   if(certificate->exinfo->issaltName)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书颁发可选名",-1);
   }
   if(certificate->exinfo->netsComment)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"Netscape证书说明",-1);
   }
   if(certificate->exinfo->prikeyPeriod)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"私有密钥有效期",-1);
   }
   if(certificate->exinfo->nameConstraints)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书名称约束",-1);
   }
   if(certificate->exinfo->polMappings)
   {
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,"证书方针映射",-1);
   }
   if(certificate->exinfo->oid)
   {
      sprintf(tmp,"标识符%s",certificate->exinfo->oid);
      gtk_tree_store_append(tree_store, &iters_child2, &iters_child1);
      gtk_tree_store_set(tree_store, &iters_child2 ,0,tmp,-1);
   }
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"证书签名算法",-1);
   gtk_tree_store_append(tree_store, &iters_child1, &iters_child);
   gtk_tree_store_set(tree_store, &iters_child1 ,0,"证书签名值",-1);
   gtk_tree_view_expand_all((GtkTreeView *)tree_s);
   memset(tmp,0,sizeof(tmp));
}

static void get_name(char *value,char **name)
{
   X509 * x=NULL;
   BIO *b=NULL;
   X509_NAME *subject;
   int entriesNum,Nid,i;
   X509_NAME_ENTRY *name_entry;

   b=BIO_new(BIO_s_mem());
   BIO_puts(b,value);
   x=PEM_read_bio_X509(b, NULL, NULL, NULL);
   BIO_free(b);         
   subject=X509_get_subject_name(x);
   entriesNum=sk_X509_NAME_ENTRY_num(subject->entries);
   for(i=0;i<entriesNum;i++)
   {
      name_entry=sk_X509_NAME_ENTRY_value(subject->entries,i);
      Nid=OBJ_obj2nid(name_entry->object);
      if(Nid==NID_commonName)
      { 
         *name=(char *)malloc(strlen(name_entry->value->data)+1);
         strcpy(*name,name_entry->value->data);    
         break;
      }
   }
   X509_free(x);
}

static void on_changed(GtkWidget *widget, GtkTreeStore *tree_store) 
{
   GtkTreeIter iter;
   GtkTreeModel *model;
   char *value; 
   char *name=NULL;

   if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
   {
      gtk_tree_model_get(model, &iter, 0, &value,  -1);
      if((certificate->to->commonName) && (strcmp(value,certificate->to->commonName)!=0))
      {
         get_name(ce_chain_value,&name);     
         if((name) && (strcmp(value,name)==0))
         {
            free_certificate_memeory();
            get_certificate_data(ce_chain_value);
            get_tree_store(tree_store);
         }
         else
         {  
            if(name)
            {
               free(name);
               name=NULL;
            } 
            get_name(ce_chain_value1,&name); 
            if((name) && (strcmp(value,name)==0))
            {
               free_certificate_memeory();
               get_certificate_data(ce_chain_value1);
               get_tree_store(tree_store);
            }
            else
            {  
               if(name)
               {
                  free(name);
                  name=NULL;
               }  
               get_name(ce_chain_value2,&name);        
               if((name) && (strcmp(value,name)==0))
               {
                  free_certificate_memeory();
                  get_certificate_data(ce_chain_value2);
                  get_tree_store(tree_store);
               }                  
            }
         }
         free(name);
         name=NULL;
      }
      value=NULL;   
   }
}

static void on_changed1(GtkWidget *widget, GtkTreeStore *tree_store) 
{
   GtkTreeIter iter;
   GtkTreeModel *model;
   char *value; 
   char tmp_info[1024];


   if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) 
   {
      gtk_tree_model_get(model, &iter, 0, &value,  -1);
      if(strcmp(value,"版本")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->version,-1);
      else if(strcmp(value,"序列号")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->serialNumber,-1);
      else if(strcmp(value,"颁发者")==0)
      {
         strcpy(tmp_info,"\n");
         if(certificate->by->commonName)
         {
            strcat(tmp_info,"CN = ");
            strcat(tmp_info,certificate->by->commonName);
            strcat(tmp_info,"\n");
         }  
         if(certificate->by->countryName && g_locale_to_utf8(certificate->by->countryName,-1,0,0,0))
         {
            strcat(tmp_info,"C = ");
            strcat(tmp_info,certificate->by->countryName);
            strcat(tmp_info,"\n");
         }
         if(certificate->by->localityName && g_locale_to_utf8(certificate->by->localityName,-1,0,0,0))
         {
            strcat(tmp_info,"L = ");
            strcat(tmp_info,certificate->by->localityName);
            strcat(tmp_info,"\n");
         }   
         if(certificate->by->provinceName && g_locale_to_utf8(certificate->by->provinceName,-1,0,0,0))
         {
            strcat(tmp_info,"ST = ");
            strcat(tmp_info,certificate->by->provinceName);
            strcat(tmp_info,"\n");
         }
         if(certificate->by->organizationName && g_locale_to_utf8(certificate->by->organizationName,-1,0,0,0))
         {
            strcat(tmp_info,"O = ");
            strcat(tmp_info,certificate->by->organizationName);
            strcat(tmp_info,"\n");
         }
         if(certificate->by->organizationalUnitName && g_locale_to_utf8(certificate->by->organizationalUnitName,-1,0,0,0))
         {
            strcat(tmp_info,"OU = ");
            strcat(tmp_info,certificate->by->organizationalUnitName);
            strcat(tmp_info,"\n");
         }
         if(certificate->by->organizationalUnitName1 && g_locale_to_utf8(certificate->by->organizationalUnitName1,-1,0,0,0))
         {
            strcat(tmp_info,"OU = ");
            strcat(tmp_info,certificate->by->organizationalUnitName1);
            strcat(tmp_info,"\n");
         }
         gtk_tree_store_set (tree_store, &iter_v ,0,tmp_info,-1);
         tmp_info[0]='\0';      
      }
      else if(strcmp(value,"不早于")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->startDate,-1);
      else if(strcmp(value,"不晚于")==0)
        gtk_tree_store_set (tree_store, &iter_v ,0,certificate->endDate,-1);
      else if(strcmp(value,"主题")==0)
      {
         strcpy(tmp_info,"\n");
         if(certificate->to->commonName)
         {
            strcat(tmp_info,"CN = ");
            strcat(tmp_info,certificate->to->commonName);
            strcat(tmp_info,"\n");
         }  
         if(certificate->to->countryName && g_locale_to_utf8(certificate->to->countryName,-1,0,0,0))
         {
            strcat(tmp_info,"C = ");
            strcat(tmp_info,certificate->to->countryName);
            strcat(tmp_info,"\n");
         }
         if(certificate->to->localityName && g_locale_to_utf8(certificate->to->localityName,-1,0,0,0))
         {
            strcat(tmp_info,"L = ");
            strcat(tmp_info,certificate->to->localityName);
            strcat(tmp_info,"\n");
         }   
         if(certificate->to->provinceName && g_locale_to_utf8(certificate->to->provinceName,-1,0,0,0))
         {
            strcat(tmp_info,"ST = ");
            strcat(tmp_info,certificate->to->provinceName);
            strcat(tmp_info,"\n");
         }
         if(certificate->to->organizationName && g_locale_to_utf8(certificate->to->organizationName,-1,0,0,0))
         {
            strcat(tmp_info,"O = ");
            strcat(tmp_info,certificate->to->organizationName);
            strcat(tmp_info,"\n");
         }
         if(certificate->to->organizationalUnitName && g_locale_to_utf8(certificate->to->organizationalUnitName,-1,0,0,0))
         {
            strcat(tmp_info,"OU = ");
            strcat(tmp_info,certificate->to->organizationalUnitName);
            strcat(tmp_info,"\n");
         }
         if(certificate->to->organizationalUnitName1 && g_locale_to_utf8(certificate->to->organizationalUnitName1,-1,0,0,0))
         {
            strcat(tmp_info,"OU = ");
            strcat(tmp_info,certificate->to->organizationalUnitName1);
            strcat(tmp_info,"\n");
         }
         gtk_tree_store_set (tree_store, &iter_v ,0,tmp_info,-1);
         memset(tmp_info,0,sizeof(tmp_info));      
      }
      else if(strcmp(value,"证书签名算法")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->sig_alg,-1);
      else if(strcmp(value,"证书签名值")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->signature,-1);
      else if(strcmp(value,"公共密钥算法")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->pkey_alg,-1);
      else if(strcmp(value,"公共密钥值")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->pkey_value,-1);
      else if(strcmp(value,"证书基本约束")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->basicConst,-1);
      else if(strcmp(value,"证书密钥使用")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->keyUsage,-1);
      else if(strcmp(value,"证书方针")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->certPolicies,-1);
      else if(strcmp(value,"CRL分布点")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->crlDist,-1);
      else if(strcmp(value,"证书扩展密钥使用")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->exkeyUsage,-1);
      else if(strcmp(value,"授权信息访问")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->infoAccess,-1);
      else if(strcmp(value,"证书授权密钥身份")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->autokeyIdentifier,-1);
      else if(strcmp(value,"Netscape证书类型")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->netscertType,-1);
      else if(strcmp(value,"证书标题密钥身份")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->subkeyIdentifier,-1);
      else if(strcmp(value,"证书主题可选名")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->subaltName,-1);
      else if(strcmp(value,"证书颁发可选名")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->issaltName,-1);    
      else if(strcmp(value,"Netscape证书说明")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->netsComment,-1); 
      else if(strcmp(value,"私有密钥有效期")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->prikeyPeriod,-1);
      else if(strcmp(value,"证书名称约束")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->nameConstraints,-1);
      else if(strcmp(value,"证书方针映射")==0)
         gtk_tree_store_set (tree_store, &iter_v ,0,certificate->exinfo->polMappings,-1);
      else 
         gtk_tree_store_set (tree_store, &iter_v ,0,NULL,-1); 
      value=NULL;
   }
}

static void export_certificates(GtkWidget *widget) 
{
   GtkWidget *dialog,*dialog1;
   GtkFileFilter *filter;
   gint result,result1;
   char *pathname=NULL;
   char *pretermitname=NULL;
   char *filevalue=NULL;
   GTlsCertificate* certificate1=NULL;
   GByteArray *certificateData=NULL;
   int i;

   dialog=gtk_file_chooser_dialog_new ("保存证书至文件", NULL,
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                         NULL);

   filter=gtk_file_filter_new();
   gtk_file_filter_set_name (filter, "X.509 证书(PEM)");
   gtk_file_filter_add_pattern(filter,"*.[Pp][Ee][Mm]");
   gtk_file_filter_add_pattern(filter,"*.[Cc][Rr][Tt]");
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog),filter);

   filter=gtk_file_filter_new();
   gtk_file_filter_set_name (filter, "X.509 含链证书(PEM)");
   gtk_file_filter_add_pattern(filter,"*.[Pp][Ee][Mm]");
   gtk_file_filter_add_pattern(filter,"*.[Cc][Rr][Tt]");
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog),filter);
 
   filter=gtk_file_filter_new();
   gtk_file_filter_set_name (filter, "X.509 证书(DER)");
   gtk_file_filter_add_pattern(filter,"*.[Dd][Ee][Rr]");
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog),filter);

   filter=gtk_file_filter_new();
   gtk_file_filter_set_name (filter, "X.509 证书(PKCS#7)");
   gtk_file_filter_add_pattern(filter,"*.[Pp][77][Cc]");
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog),filter);

   filter=gtk_file_filter_new(); 
   gtk_file_filter_set_name (filter, "X.509 含链证书(PKCS#7)");
   gtk_file_filter_add_pattern(filter,"*.[Pp][77][Cc]");
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog),filter); 

   filter=gtk_file_filter_new();
   gtk_file_filter_set_name (filter, "所有文件");
   gtk_file_filter_add_pattern(filter,"*");
   gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog),filter);

   gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),"~");

   //set pretermit file name
   if(!certificate->to->commonName)
   {
      if(certificate->to->organizationalUnitName)
      {
         pretermitname=(char *)malloc(strlen(certificate->to->organizationalUnitName)+1);
         strcpy(pretermitname,certificate->to->organizationalUnitName);
      }
      else
      {
         pretermitname=(char *)malloc(strlen(certificate->to->organizationName)+1);
         strcpy(pretermitname,certificate->to->organizationName);
      }
   }
   else
      get_name(ce_chain_value,&pretermitname);     
   gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER (dialog),pretermitname);

   while(1)
   {
      result=gtk_dialog_run (GTK_DIALOG (dialog));
      if (result==GTK_RESPONSE_ACCEPT)  
      {  
         filter=gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(dialog));
         pathname=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
         if(g_file_test(pathname,G_FILE_TEST_EXISTS))
         {
           dialog1=gtk_message_dialog_new((GtkWindow *)dialog,
                              GTK_DIALOG_MODAL,
                              GTK_MESSAGE_QUESTION,
                              GTK_BUTTONS_YES_NO,
                              "文件名已经存在，是否想要替换?\n替换该文件将覆盖其中的内容.");
            result1=gtk_dialog_run(GTK_DIALOG (dialog1));
            if(result1==GTK_RESPONSE_YES)
            {
               gtk_widget_destroy (dialog1);
               break;
            }
            else
            {
               pathname=NULL;
               gtk_widget_destroy (dialog1);
               continue; 
            }
         }
         else
            break;
      }
      else
      {
         free(pretermitname);
         pretermitname=NULL;
         gtk_widget_destroy (dialog);
         return;
      }
   }
   if(strcmp(gtk_file_filter_get_name(filter),"X.509 证书(PEM)")==0 ||
      strcmp(gtk_file_filter_get_name(filter),"所有文件")==0)
      g_file_set_contents(pathname,ce_chain_value,strlen(ce_chain_value),NULL);
   else if(strcmp(gtk_file_filter_get_name(filter),"X.509 含链证书(PEM)")==0)
   {
      if(!ce_chain_value1)
      {
         filevalue=(char *)malloc(strlen(ce_chain_value)+1);
         strcpy(filevalue,ce_chain_value);
      }
      else if(!ce_chain_value2)
      {
         filevalue=(char *)malloc(strlen(ce_chain_value)+strlen(ce_chain_value1)+1);
         strcpy(filevalue,ce_chain_value);
         strcat(filevalue,ce_chain_value1);           
      }
      else
      {
         filevalue=(char *)malloc(strlen(ce_chain_value)+strlen(ce_chain_value1)+strlen(ce_chain_value2)+1);
         strcpy(filevalue,ce_chain_value);
         strcat(filevalue,ce_chain_value1);
         strcat(filevalue,ce_chain_value2);   
      }
      g_file_set_contents(pathname,filevalue,strlen(filevalue),NULL);
      free(filevalue);
      filevalue=NULL;
   }
   else if(strcmp(gtk_file_filter_get_name(filter),"X.509 证书(DER)")==0)
   {
      certificate1=g_tls_certificate_new_from_pem(
           ce_chain_value, strlen(ce_chain_value)+1, NULL);
      g_object_get(G_OBJECT(certificate1), "certificate", &certificateData, NULL);
      filevalue=(char *)malloc(certificateData->len+1);
      for(i=0;i<(int)certificateData->len;i++)
         filevalue[i]=(char)certificateData->data[i];
      filevalue[i]='\0';
      g_file_set_contents(pathname,filevalue,certificateData->len,NULL);
      free(filevalue);
      filevalue=NULL;
      pathname=NULL;
      certificate1=NULL;
      certificateData=NULL;
   }
   free(pretermitname);
   pretermitname=NULL;
   gtk_widget_destroy (dialog);
}

static int close_window()
{
   free_certificate_memeory();
   free(ce_chain_value);
   ce_chain_value=NULL;
   free(ce_chain_value1);
   ce_chain_value1=NULL;
   free(ce_chain_value2);
   ce_chain_value2=NULL;
   return false;
}

//display single certificate information
void display_certificate_info()
{
   char title[CERTITLELENGTH];
   GtkBuilder *builder;
   GtkLabel *label;
   GtkButton *export_button;
   GtkWindow *window;
   GtkTreeSelection *selection,*selection1;
   GtkWidget *fixed2;
   GtkWidget *tree,*tree2;
   GtkTreeStore *tree_store,*tree_store1,*tree_store2;
   GtkWidget *scrolledwindow, *scrolledwindow1;
   GtkTreeIter   iter;
   GtkTreeIter   iter_child,iter_child1;
   GtkCellRenderer *renderer;
   GtkTreeViewColumn *column;
   char *verfity_value=NULL;
   char *certificate_chain[CHAIN_MAX_SIZE]={NULL,NULL,NULL};

   builder=gtk_builder_new();
   gtk_builder_add_from_file(builder,"./Source/BrowserApp/resources/layout/single-certificate.glade", NULL);

   //set title
   window=(GtkWindow *)GTK_WIDGET(gtk_builder_get_object(builder,"window"));
   strcpy(title,"证书查看器：");
   if(certificate->to->commonName)
   {
      if(strcmp(certificate->to->commonName,certificate->by->commonName) ==0)
      {
         strcat(title,"Builtin Object Token:");
         strcat(title,certificate->to->commonName);
      }
      else 
         strcat(title,certificate->to->commonName);
   }   
   else if (certificate->to->organizationalUnitName)
   {
      strcat(title,"Builtin Object Token:");
      strcat(title,certificate->to->organizationalUnitName);
   }
   else
   {
      strcat(title,"Builtin Object Token:");
      strcat(title,certificate->to->organizationName);      
   }
   gtk_window_set_title(window,title);
   memset(title,0,CERTITLELENGTH);   
   
   //get certificate chain
   get_certificate_chain(certificate_chain);

   //check certificate verfity
   check_certificate_verfity(&verfity_value);

   //output basic information
   label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"verfity"));   
   if(verfity_value)
   {
       sprintf(title,"此证书没有通过认证,没有通过的原因为:%s",verfity_value);
       gtk_label_set_text(label,title);
       memset(title,0,CERTITLELENGTH);
       verfity_value=NULL;
   }
   if(certificate->to->commonName)
   {
      label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"ToCommonName"));
      gtk_label_set_text(label,certificate->to->commonName);
   }
   if(certificate->to->organizationName)
   {
      label =  (GtkLabel *)GTK_WIDGET (gtk_builder_get_object (builder, "ToOrgName"));
      gtk_label_set_text(label,g_locale_to_utf8(certificate->to->organizationName,-1,0,0,0));
   }
   if(certificate->to->organizationalUnitName)
   {
      label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"ToOrgUnitName"));
      gtk_label_set_text(label,g_locale_to_utf8(certificate->to->organizationalUnitName,-1,0,0,0));
   }
   label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"SerialNumber"));
   gtk_label_set_text(label,certificate->serialNumber);
   if(certificate->by->commonName)
   {
      label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"ByCommonName"));
      gtk_label_set_text(label,certificate->by->commonName);
   }
   if(certificate->by->organizationName)
   {
      label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"ByOrgName"));
      gtk_label_set_text(label,g_locale_to_utf8(certificate->by->organizationName,-1,0,0,0));
   }
   if(certificate->by->organizationalUnitName)
   {
      label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"ByOrgUnitName"));
      gtk_label_set_text(label,g_locale_to_utf8(certificate->by->organizationalUnitName,-1,0,0,0));
   }
   label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"StartDate"));
   gtk_label_set_text(label,certificate->startDate);
   label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"EndDate"));
   gtk_label_set_text(label,certificate->endDate);
   label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"Sha1"));
   gtk_label_set_text(label,certificate->sha1_t);
   label=(GtkLabel *)GTK_WIDGET(gtk_builder_get_object(builder,"Md5"));
   gtk_label_set_text(label,certificate->md5_t);

   //output detail infomation 
   fixed2=GTK_WIDGET(gtk_builder_get_object(builder,"fixed2"));;
   scrolledwindow=GTK_WIDGET(gtk_builder_get_object(builder,"scrolledwindow"));
   scrolledwindow1=GTK_WIDGET(gtk_builder_get_object(builder,"scrolledwindow1"));
   tree=gtk_tree_view_new();
   tree_store=gtk_tree_store_new(1,G_TYPE_STRING);  
   if(certificate_chain[CHAIN_MAX_SIZE-1])
   {
      gtk_tree_store_append(tree_store, &iter, NULL); 
      gtk_tree_store_set(tree_store, &iter,0,certificate_chain[CHAIN_MAX_SIZE-1],-1);
      free(certificate_chain[CHAIN_MAX_SIZE-1]);
      certificate_chain[CHAIN_MAX_SIZE-1]=NULL;
      gtk_tree_store_append(tree_store,&iter_child,&iter);
      gtk_tree_store_set(tree_store,&iter_child,0,certificate_chain[CHAIN_MAX_SIZE-2],-1);
      free(certificate_chain[CHAIN_MAX_SIZE-2]);
      certificate_chain[CHAIN_MAX_SIZE-2]=NULL;
      gtk_tree_store_append(tree_store,&iter_child1,&iter_child);
      gtk_tree_store_set(tree_store, &iter_child1,0,certificate_chain[CHAIN_MAX_SIZE-3],-1);
      free(certificate_chain[CHAIN_MAX_SIZE-3]);
      certificate_chain[CHAIN_MAX_SIZE-3]=NULL;
   }    
   else if(!certificate_chain[CHAIN_MAX_SIZE-1] && certificate_chain[CHAIN_MAX_SIZE-2])
   {
      gtk_tree_store_append(tree_store,&iter,NULL); 
      gtk_tree_store_set(tree_store, &iter,0,certificate_chain[CHAIN_MAX_SIZE-2],-1);
      free(certificate_chain[CHAIN_MAX_SIZE-2]);
      certificate_chain[CHAIN_MAX_SIZE-2]=NULL;
      gtk_tree_store_append(tree_store, &iter_child,&iter);
      gtk_tree_store_set(tree_store,&iter_child,0,certificate_chain[CHAIN_MAX_SIZE-3],-1);
      free(certificate_chain[CHAIN_MAX_SIZE-3]);
      certificate_chain[CHAIN_MAX_SIZE-3]=NULL;
   }
   else
   {
      gtk_tree_store_append(tree_store, &iter, NULL); 
      gtk_tree_store_set(tree_store,&iter,0,certificate_chain[CHAIN_MAX_SIZE-3],-1);
      free(certificate_chain[CHAIN_MAX_SIZE-3]);
      certificate_chain[CHAIN_MAX_SIZE-3]=NULL; 
   }
  
   gtk_tree_view_set_model(GTK_TREE_VIEW(tree),GTK_TREE_MODEL(tree_store));
   g_object_unref(G_OBJECT (tree_store));
   renderer=gtk_cell_renderer_text_new();        
   column=gtk_tree_view_column_new_with_attributes("证书结构",renderer,"text",0,NULL);
   
   gtk_tree_view_append_column(GTK_TREE_VIEW(tree),column);
   gtk_tree_view_expand_all((GtkTreeView *)tree);
   gtk_container_add(GTK_CONTAINER (fixed2),(GtkWidget *)tree);
   gtk_widget_set_size_request(tree,630,100);
   gtk_widget_show(tree);

   tree_s=gtk_tree_view_new();
   tree_store1=gtk_tree_store_new(1,G_TYPE_STRING);
   get_tree_store(tree_store1);
   gtk_tree_view_set_model(GTK_TREE_VIEW(tree_s),GTK_TREE_MODEL(tree_store1));
   g_object_unref(G_OBJECT(tree_store1));
   renderer=gtk_cell_renderer_text_new();
   column=gtk_tree_view_column_new_with_attributes("证书字段",renderer,"text",0,NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW (tree_s),column);
   gtk_container_add (GTK_CONTAINER (scrolledwindow),(GtkWidget *)tree_s);
   gtk_tree_view_expand_all((GtkTreeView *)tree_s);
   gtk_widget_show(tree_s);

   tree2=gtk_tree_view_new();
   tree_store2=gtk_tree_store_new(1,G_TYPE_STRING);  
   gtk_tree_store_append(tree_store2, &iter_v,NULL);
   gtk_tree_store_set(tree_store2,&iter_v,0,NULL,-1);    
   gtk_tree_view_set_model(GTK_TREE_VIEW(tree2),GTK_TREE_MODEL(tree_store2));
   g_object_unref(G_OBJECT (tree_store2));
   renderer=gtk_cell_renderer_text_new ();        
   column=gtk_tree_view_column_new_with_attributes("字段值",renderer,"text",0,NULL);
   
   gtk_tree_view_append_column(GTK_TREE_VIEW(tree2),column);
   gtk_container_add(GTK_CONTAINER(scrolledwindow1),(GtkWidget *)tree2);
   gtk_widget_show(tree2);
  
   selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(tree)); 
   g_signal_connect(selection,"changed", G_CALLBACK(on_changed),tree_store1);
   gtk_builder_connect_signals(builder, NULL);

   selection1=gtk_tree_view_get_selection(GTK_TREE_VIEW (tree_s)); 
   g_signal_connect(selection1,"changed",G_CALLBACK(on_changed1),tree_store2);

   //export certificate information
   export_button=(GtkButton *)GTK_WIDGET(gtk_builder_get_object (builder,"button2"));
   g_signal_connect(export_button,"clicked",G_CALLBACK(export_certificates),NULL);   

   gtk_builder_connect_signals(builder,NULL);
   g_object_unref(G_OBJECT(builder));
  
   //close window
   g_signal_connect(G_OBJECT(window),"delete_event", G_CALLBACK(close_window),NULL);
}

//display certificate manager information
void display_certificatemanager_info()
{
}

