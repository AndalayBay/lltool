

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jun 23 16:34:10 2005
 */
/* Compiler settings for .\_MTParserInfoFile.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MTParserInfoFile_h__
#define __MTParserInfoFile_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMTParserInfoFile_FWD_DEFINED__
#define __IMTParserInfoFile_FWD_DEFINED__
typedef interface IMTParserInfoFile IMTParserInfoFile;
#endif 	/* __IMTParserInfoFile_FWD_DEFINED__ */


#ifndef __CATLMTParserInfoFile_FWD_DEFINED__
#define __CATLMTParserInfoFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class CATLMTParserInfoFile CATLMTParserInfoFile;
#else
typedef struct CATLMTParserInfoFile CATLMTParserInfoFile;
#endif /* __cplusplus */

#endif 	/* __CATLMTParserInfoFile_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf__MTParserInfoFile_0000 */
/* [local] */ 

typedef /* [helpstring][version][uuid] */  DECLSPEC_UUID("9D7C0E15-9513-4884-9F47-BFCFA847AFCF") struct SATLFuncInfo
    {
    /* [helpstring] */ BSTR symbol;
    } 	SATLFuncInfo;



extern RPC_IF_HANDLE __MIDL_itf__MTParserInfoFile_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__MTParserInfoFile_0000_v0_0_s_ifspec;

#ifndef __IMTParserInfoFile_INTERFACE_DEFINED__
#define __IMTParserInfoFile_INTERFACE_DEFINED__

/* interface IMTParserInfoFile */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMTParserInfoFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9D7C0E16-9513-4884-9F47-BFCFA847AFCF")
    IMTParserInfoFile : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE load( 
            /* [in] */ BSTR fileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getNbLocales( 
            /* [retval][out] */ ULONG *pNbLocales) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getLocale( 
            /* [in] */ ULONG id,
            /* [retval][out] */ BSTR *pLocale) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setLocale( 
            /* [in] */ BSTR locale) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getNbFuncs( 
            /* [retval][out] */ ULONG *pNbFuncs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getFunc( 
            /* [in] */ ULONG id,
            /* [retval][out] */ SATLFuncInfo *pFunc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMTParserInfoFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMTParserInfoFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMTParserInfoFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMTParserInfoFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMTParserInfoFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMTParserInfoFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMTParserInfoFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMTParserInfoFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *load )( 
            IMTParserInfoFile * This,
            /* [in] */ BSTR fileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getNbLocales )( 
            IMTParserInfoFile * This,
            /* [retval][out] */ ULONG *pNbLocales);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getLocale )( 
            IMTParserInfoFile * This,
            /* [in] */ ULONG id,
            /* [retval][out] */ BSTR *pLocale);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setLocale )( 
            IMTParserInfoFile * This,
            /* [in] */ BSTR locale);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getNbFuncs )( 
            IMTParserInfoFile * This,
            /* [retval][out] */ ULONG *pNbFuncs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getFunc )( 
            IMTParserInfoFile * This,
            /* [in] */ ULONG id,
            /* [retval][out] */ SATLFuncInfo *pFunc);
        
        END_INTERFACE
    } IMTParserInfoFileVtbl;

    interface IMTParserInfoFile
    {
        CONST_VTBL struct IMTParserInfoFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMTParserInfoFile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMTParserInfoFile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMTParserInfoFile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMTParserInfoFile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMTParserInfoFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMTParserInfoFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMTParserInfoFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMTParserInfoFile_load(This,fileName)	\
    (This)->lpVtbl -> load(This,fileName)

#define IMTParserInfoFile_getNbLocales(This,pNbLocales)	\
    (This)->lpVtbl -> getNbLocales(This,pNbLocales)

#define IMTParserInfoFile_getLocale(This,id,pLocale)	\
    (This)->lpVtbl -> getLocale(This,id,pLocale)

#define IMTParserInfoFile_setLocale(This,locale)	\
    (This)->lpVtbl -> setLocale(This,locale)

#define IMTParserInfoFile_getNbFuncs(This,pNbFuncs)	\
    (This)->lpVtbl -> getNbFuncs(This,pNbFuncs)

#define IMTParserInfoFile_getFunc(This,id,pFunc)	\
    (This)->lpVtbl -> getFunc(This,id,pFunc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMTParserInfoFile_load_Proxy( 
    IMTParserInfoFile * This,
    /* [in] */ BSTR fileName);


void __RPC_STUB IMTParserInfoFile_load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMTParserInfoFile_getNbLocales_Proxy( 
    IMTParserInfoFile * This,
    /* [retval][out] */ ULONG *pNbLocales);


void __RPC_STUB IMTParserInfoFile_getNbLocales_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMTParserInfoFile_getLocale_Proxy( 
    IMTParserInfoFile * This,
    /* [in] */ ULONG id,
    /* [retval][out] */ BSTR *pLocale);


void __RPC_STUB IMTParserInfoFile_getLocale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMTParserInfoFile_setLocale_Proxy( 
    IMTParserInfoFile * This,
    /* [in] */ BSTR locale);


void __RPC_STUB IMTParserInfoFile_setLocale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMTParserInfoFile_getNbFuncs_Proxy( 
    IMTParserInfoFile * This,
    /* [retval][out] */ ULONG *pNbFuncs);


void __RPC_STUB IMTParserInfoFile_getNbFuncs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMTParserInfoFile_getFunc_Proxy( 
    IMTParserInfoFile * This,
    /* [in] */ ULONG id,
    /* [retval][out] */ SATLFuncInfo *pFunc);


void __RPC_STUB IMTParserInfoFile_getFunc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMTParserInfoFile_INTERFACE_DEFINED__ */



#ifndef __MTParserInfoFile_LIBRARY_DEFINED__
#define __MTParserInfoFile_LIBRARY_DEFINED__

/* library MTParserInfoFile */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_MTParserInfoFile;

EXTERN_C const CLSID CLSID_CATLMTParserInfoFile;

#ifdef __cplusplus

class DECLSPEC_UUID("B521F2A0-5D67-4164-A04B-32D636DB8F9F")
CATLMTParserInfoFile;
#endif
#endif /* __MTParserInfoFile_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


