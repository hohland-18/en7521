/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.20 $ of : mfd-data-get.m2c,v $ 
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/snmp-usm-dh-objects-mib/usmDHUserKeyTable/usmDHUserKeyTable_data_get.c#1 $
 */
/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <openssl/dh.h>

/*
 * include our parent header 
 */
#define NEED_USMDH_FUNCTIONS
#include "usmDHUserKeyTable.h"

DH             *
usmDHGetUserDHptr(struct usmUser *user, int for_auth_key)
{
    DH             *dh, *dh_params;
    void          **theptr;

    if (user == NULL)
        return NULL;

    if (for_auth_key == 1)
        theptr = &user->usmDHUserAuthKeyChange;
    else
        theptr = &user->usmDHUserPrivKeyChange;

    if (!*theptr) {
        /*
         * copy the system parameters to the local ones 
         */
        dh = DH_new();
        if (!dh)
            return NULL;
        dh_params = get_dh_params();
        if (!dh_params)
            return NULL;
        dh->g = BN_dup(dh_params->g);
        dh->p = BN_dup(dh_params->p);
        if (!dh->g || !dh->p)
            return NULL;
        DH_generate_key(dh);
        *theptr = dh;
    } else {
        dh = (DH *) * theptr;
    }
    return dh;
}

int
usmDHGetUserKeyChange(struct usmUser *user, int for_auth_key,
                      char **keyobj, size_t *keyobj_len)
{
    DH             *dh;

    dh = usmDHGetUserDHptr(user, for_auth_key);

    if (!dh) {
        snmp_log(LOG_ERR, "ack...  shouldn't get here: %x %d\n",
                 user, for_auth_key);
        return MFD_ERROR;
    }

    *keyobj_len = BN_num_bytes(dh->pub_key);
    *keyobj = malloc(*keyobj_len);
    BN_bn2bin(dh->pub_key, *keyobj);

    return MFD_SUCCESS;
}

/** @ingroup data_access 
 * @defgroup data_get data_get: Routines to get data
 *
 * TODO:230:M: Implement usmDHUserKeyTable get routines.
 * TODO:240:M: Implement usmDHUserKeyTable mapping routines (if any).
 *
 * These routine are used to get the value for individual objects. The
 * row context is passed, along with a pointer to the memory where the
 * value should be copied.
 *
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table usmDHUserKeyTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * SNMP-USM-DH-OBJECTS-MIB::usmDHUserKeyTable is subid 2 of usmDHPublicObjects.
 * Its status is Current.
 * OID: .1.3.6.1.3.101.1.1.2, length: 9
 */

/*
 * ---------------------------------------------------------------------
 * * TODO:200:r: Implement usmDHUserKeyTable data context functions.
 */
/*
 * usmDHUserKeyTable_allocate_data
 *
 * Purpose: create new usmDHUserKeyTable_data.
 */
usmDHUserKeyTable_data *
usmDHUserKeyTable_allocate_data(void)
{
    /*
     * TODO:201:r: |-> allocate memory for the usmDHUserKeyTable data context.
     */
    usmDHUserKeyTable_data *rtn =
        SNMP_MALLOC_TYPEDEF(usmDHUserKeyTable_data);

    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserKeyTable_allocate_data", "called\n"));

    if (NULL == rtn) {
        snmp_log(LOG_ERR, "unable to malloc memory for new "
                 "usmDHUserKeyTable_data.\n");
    }
    /*
     * not real user, not in a list. mark for testing
     */
    rtn->next = (struct usmUser *) -1;
    rtn->prev = (struct usmUser *) -1;

    return rtn;
}                               /* usmDHUserKeyTable_allocate_data */

/*
 * usmDHUserKeyTable_release_data
 *
 * Purpose: release usmDHUserKeyTable data.
 */
void
usmDHUserKeyTable_release_data(usmDHUserKeyTable_data * data)
{
    struct usmUser *user = data;

    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserKeyTable_release_data",
                "called\n"));

    netsnmp_assert(user->next == (struct usmUser *) -1);
    netsnmp_assert(user->prev == (struct usmUser *) -1);

    /*
     * TODO:202:r: |-> release memory for the usmDHUserKeyTable data context.
     */
    if (user) {
        SNMP_FREE(user->authKey);
        SNMP_FREE(user->privKey);
    }

    free(data);
}                               /* usmDHUserKeyTable_release_data */



/**
 * set mib index(es)
 *
 * @param tbl_idx mib index structure
 * @param usmUserEngineID_val_ptr
 * @param usmUserEngineID_val_ptr_len
 * @param usmUserName_val_ptr
 * @param usmUserName_val_ptr_len
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This convenience function is useful for setting all the MIB index
 *  components with a single function call. It is assume that the C values
 *  have already been mapped from their native/rawformat to the MIB format.
 */
int
usmDHUserKeyTable_indexes_set_tbl_idx(usmDHUserKeyTable_mib_index *
                                      tbl_idx,
                                      char *usmUserEngineID_val_ptr,
                                      size_t usmUserEngineID_val_ptr_len,
                                      char *usmUserName_val_ptr,
                                      size_t usmUserName_val_ptr_len)
{
    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserKeyTable_indexes_set_tbl_idx", "called\n"));

    /*
     * usmUserEngineID(1)/SnmpEngineID/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h 
     */
    tbl_idx->usmUserEngineID_len = sizeof(tbl_idx->usmUserEngineID) / sizeof(tbl_idx->usmUserEngineID[0]);      /* max length */
    /** WARNING: this code might not work for struct usmUser */
    /*
     * make sure there is enough space for usmUserEngineID data
     */
    if ((NULL == tbl_idx->usmUserEngineID) ||
        (tbl_idx->usmUserEngineID_len < (usmUserEngineID_val_ptr_len))) {
        snmp_log(LOG_ERR, "not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->usmUserEngineID_len = usmUserEngineID_val_ptr_len;
    memcpy(tbl_idx->usmUserEngineID, usmUserEngineID_val_ptr,
           usmUserEngineID_val_ptr_len *
           sizeof(usmUserEngineID_val_ptr[0]));

    /*
     * usmUserName(2)/SnmpAdminString/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H 
     */
    tbl_idx->usmUserName_len = sizeof(tbl_idx->usmUserName) / sizeof(tbl_idx->usmUserName[0]);  /* max length */

    /** WARNING: this code might not work for struct usmUser */
    /*
     * make sure there is enough space for usmUserName data
     */
    if ((NULL == tbl_idx->usmUserName) ||
        (tbl_idx->usmUserName_len < (usmUserName_val_ptr_len))) {
        snmp_log(LOG_ERR, "not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->usmUserName_len = usmUserName_val_ptr_len;
    memcpy(tbl_idx->usmUserName, usmUserName_val_ptr,
           usmUserName_val_ptr_len * sizeof(usmUserName_val_ptr[0]));


    return MFD_SUCCESS;
}                               /* usmDHUserKeyTable_indexes_set_tbl_idx */

/**
 * @internal
 * set row context indexes
 *
 * @param reqreq_ctx the row context that needs updated indexes
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This function sets the mib indexs, then updates the oid indexs
 *  from the mib index.
 */
int
usmDHUserKeyTable_indexes_set(usmDHUserKeyTable_rowreq_ctx * rowreq_ctx,
                              char *usmUserEngineID_val_ptr,
                              size_t usmUserEngineID_val_ptr_len,
                              char *usmUserName_val_ptr,
                              size_t usmUserName_val_ptr_len)
{
    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserKeyTable_indexes_set",
                "called\n"));

    if (MFD_SUCCESS !=
        usmDHUserKeyTable_indexes_set_tbl_idx(&rowreq_ctx->tbl_idx,
                                              usmUserEngineID_val_ptr,
                                              usmUserEngineID_val_ptr_len,
                                              usmUserName_val_ptr,
                                              usmUserName_val_ptr_len))
        return MFD_ERROR;

    /*
     * convert mib index to oid index
     */
    rowreq_ctx->oid_idx.len = sizeof(rowreq_ctx->oid_tmp) / sizeof(oid);
    if (0 != usmDHUserKeyTable_index_to_oid(&rowreq_ctx->oid_idx,
                                            &rowreq_ctx->tbl_idx)) {
        return MFD_ERROR;
    }

    return MFD_SUCCESS;
}                               /* usmDHUserKeyTable_indexes_set */


/*---------------------------------------------------------------------
 * SNMP-USM-DH-OBJECTS-MIB::usmDHUserKeyEntry.usmDHUserAuthKeyChange
 * usmDHUserAuthKeyChange is subid 1 of usmDHUserKeyEntry.
 * Its status is Current, and its access level is Create.
 * OID: .1.3.6.1.3.101.1.1.2.1.1
 * Description:
The object used to change any given user's Authentication Key
    using a Diffie-Hellman key exchange.

    The right-most n bits of the shared secret 'sk', where 'n' is the
    number of bits required for the protocol defined by
    usmUserAuthProtocol, are installed as the operational
    authentication key for this row after a successful SET.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 *
 * Its syntax is DHKeyChange (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.
 */
/**
 * Extract the current value of the usmDHUserAuthKeyChange data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param usmDHUserAuthKeyChange_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param usmDHUserAuthKeyChange_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by usmDHUserAuthKeyChange.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*usmDHUserAuthKeyChange_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update usmDHUserAuthKeyChange_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
usmDHUserAuthKeyChange_get(usmDHUserKeyTable_rowreq_ctx * rowreq_ctx,
                           char **usmDHUserAuthKeyChange_val_ptr_ptr,
                           size_t *usmDHUserAuthKeyChange_val_ptr_len_ptr)
{
   /** we should have a non-NULL pointer and enough storage */
    netsnmp_assert((NULL != usmDHUserAuthKeyChange_val_ptr_ptr)
                   && (NULL != *usmDHUserAuthKeyChange_val_ptr_ptr));
    netsnmp_assert(NULL != usmDHUserAuthKeyChange_val_ptr_len_ptr);


    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserAuthKeyChange_get",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:231:o: |-> Extract the current value of the usmDHUserAuthKeyChange data.
     * copy (* usmDHUserAuthKeyChange_val_ptr_ptr ) data and (* usmDHUserAuthKeyChange_val_ptr_len_ptr ) from rowreq_ctx->data
     */
    if (!rowreq_ctx || !usmDHUserAuthKeyChange_val_ptr_len_ptr ||
        !usmDHUserAuthKeyChange_val_ptr_ptr ||
        !*usmDHUserAuthKeyChange_val_ptr_ptr) {
        return MFD_ERROR;
    }

    return usmDHGetUserKeyChange(rowreq_ctx->data, 1,
                                 usmDHUserAuthKeyChange_val_ptr_ptr,
                                 usmDHUserAuthKeyChange_val_ptr_len_ptr);
}                               /* usmDHUserAuthKeyChange_get */

/*---------------------------------------------------------------------
 * SNMP-USM-DH-OBJECTS-MIB::usmDHUserKeyEntry.usmDHUserOwnAuthKeyChange
 * usmDHUserOwnAuthKeyChange is subid 2 of usmDHUserKeyEntry.
 * Its status is Current, and its access level is Create.
 * OID: .1.3.6.1.3.101.1.1.2.1.2
 * Description:
The object used to change the agents own Authentication Key
    using a Diffie-Hellman key exchange.

    The right-most n bits of the shared secret 'sk', where 'n' is the
    number of bits required for the protocol defined by
    usmUserAuthProtocol, are installed as the operational
    authentication key for this row after a successful SET.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 *
 * Its syntax is DHKeyChange (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.
 */
/**
 * Extract the current value of the usmDHUserOwnAuthKeyChange data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param usmDHUserOwnAuthKeyChange_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param usmDHUserOwnAuthKeyChange_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by usmDHUserOwnAuthKeyChange.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*usmDHUserOwnAuthKeyChange_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update usmDHUserOwnAuthKeyChange_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
usmDHUserOwnAuthKeyChange_get(usmDHUserKeyTable_rowreq_ctx * rowreq_ctx,
                              char **usmDHUserOwnAuthKeyChange_val_ptr_ptr,
                              size_t
                              *usmDHUserOwnAuthKeyChange_val_ptr_len_ptr)
{
   /** we should have a non-NULL pointer and enough storage */
    netsnmp_assert((NULL != usmDHUserOwnAuthKeyChange_val_ptr_ptr)
                   && (NULL != *usmDHUserOwnAuthKeyChange_val_ptr_ptr));
    netsnmp_assert(NULL != usmDHUserOwnAuthKeyChange_val_ptr_len_ptr);


    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserOwnAuthKeyChange_get",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:231:o: |-> Extract the current value of the usmDHUserOwnAuthKeyChange data.
     * copy (* usmDHUserOwnAuthKeyChange_val_ptr_ptr ) data and (* usmDHUserOwnAuthKeyChange_val_ptr_len_ptr ) from rowreq_ctx->data
     */
    if (!rowreq_ctx || !usmDHUserOwnAuthKeyChange_val_ptr_len_ptr ||
        !usmDHUserOwnAuthKeyChange_val_ptr_ptr ||
        !*usmDHUserOwnAuthKeyChange_val_ptr_ptr) {
        return MFD_ERROR;
    }

    return usmDHGetUserKeyChange(rowreq_ctx->data, 1,
                                 usmDHUserOwnAuthKeyChange_val_ptr_ptr,
                                 usmDHUserOwnAuthKeyChange_val_ptr_len_ptr);
}                               /* usmDHUserOwnAuthKeyChange_get */

/*---------------------------------------------------------------------
 * SNMP-USM-DH-OBJECTS-MIB::usmDHUserKeyEntry.usmDHUserPrivKeyChange
 * usmDHUserPrivKeyChange is subid 3 of usmDHUserKeyEntry.
 * Its status is Current, and its access level is Create.
 * OID: .1.3.6.1.3.101.1.1.2.1.3
 * Description:
The object used to change any given user's Privacy Key using
    a Diffie-Hellman key exchange.

    The right-most n bits of the shared secret 'sk', where 'n' is the
    number of bits required for the protocol defined by
    usmUserPrivProtocol, are installed as the operational privacy key
    for this row after a successful SET.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 *
 * Its syntax is DHKeyChange (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.
 */
/**
 * Extract the current value of the usmDHUserPrivKeyChange data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param usmDHUserPrivKeyChange_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param usmDHUserPrivKeyChange_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by usmDHUserPrivKeyChange.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*usmDHUserPrivKeyChange_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update usmDHUserPrivKeyChange_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
usmDHUserPrivKeyChange_get(usmDHUserKeyTable_rowreq_ctx * rowreq_ctx,
                           char **usmDHUserPrivKeyChange_val_ptr_ptr,
                           size_t *usmDHUserPrivKeyChange_val_ptr_len_ptr)
{
   /** we should have a non-NULL pointer and enough storage */
    netsnmp_assert((NULL != usmDHUserPrivKeyChange_val_ptr_ptr)
                   && (NULL != *usmDHUserPrivKeyChange_val_ptr_ptr));
    netsnmp_assert(NULL != usmDHUserPrivKeyChange_val_ptr_len_ptr);


    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserPrivKeyChange_get",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:231:o: |-> Extract the current value of the usmDHUserPrivKeyChange data.
     * copy (* usmDHUserPrivKeyChange_val_ptr_ptr ) data and (* usmDHUserPrivKeyChange_val_ptr_len_ptr ) from rowreq_ctx->data
     */
    if (!rowreq_ctx || !usmDHUserPrivKeyChange_val_ptr_len_ptr ||
        !usmDHUserPrivKeyChange_val_ptr_ptr ||
        !*usmDHUserPrivKeyChange_val_ptr_ptr) {
        return MFD_ERROR;
    }

    return usmDHGetUserKeyChange(rowreq_ctx->data, 0,
                                 usmDHUserPrivKeyChange_val_ptr_ptr,
                                 usmDHUserPrivKeyChange_val_ptr_len_ptr);
}                               /* usmDHUserPrivKeyChange_get */

/*---------------------------------------------------------------------
 * SNMP-USM-DH-OBJECTS-MIB::usmDHUserKeyEntry.usmDHUserOwnPrivKeyChange
 * usmDHUserOwnPrivKeyChange is subid 4 of usmDHUserKeyEntry.
 * Its status is Current, and its access level is Create.
 * OID: .1.3.6.1.3.101.1.1.2.1.4
 * Description:
The object used to change the agent's own Privacy Key using a
    Diffie-Hellman key exchange.

    The right-most n bits of the shared secret 'sk', where 'n' is the
    number of bits required for the protocol defined by
    usmUserPrivProtocol, are installed as the operational privacy key
    for this row after a successful SET.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 *
 * Its syntax is DHKeyChange (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.
 */
/**
 * Extract the current value of the usmDHUserOwnPrivKeyChange data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param usmDHUserOwnPrivKeyChange_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param usmDHUserOwnPrivKeyChange_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by usmDHUserOwnPrivKeyChange.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*usmDHUserOwnPrivKeyChange_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update usmDHUserOwnPrivKeyChange_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
usmDHUserOwnPrivKeyChange_get(usmDHUserKeyTable_rowreq_ctx * rowreq_ctx,
                              char **usmDHUserOwnPrivKeyChange_val_ptr_ptr,
                              size_t
                              *usmDHUserOwnPrivKeyChange_val_ptr_len_ptr)
{
   /** we should have a non-NULL pointer and enough storage */
    netsnmp_assert((NULL != usmDHUserOwnPrivKeyChange_val_ptr_ptr)
                   && (NULL != *usmDHUserOwnPrivKeyChange_val_ptr_ptr));
    netsnmp_assert(NULL != usmDHUserOwnPrivKeyChange_val_ptr_len_ptr);


    DEBUGMSGTL(("verbose:usmDHUserKeyTable:usmDHUserOwnPrivKeyChange_get",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:231:o: |-> Extract the current value of the usmDHUserOwnPrivKeyChange data.
     * copy (* usmDHUserOwnPrivKeyChange_val_ptr_ptr ) data and (* usmDHUserOwnPrivKeyChange_val_ptr_len_ptr ) from rowreq_ctx->data
     */
    if (!rowreq_ctx || !usmDHUserOwnPrivKeyChange_val_ptr_len_ptr ||
        !usmDHUserOwnPrivKeyChange_val_ptr_ptr ||
        !*usmDHUserOwnPrivKeyChange_val_ptr_ptr) {
        return MFD_ERROR;
    }
  
    return usmDHGetUserKeyChange(rowreq_ctx->data, 0,
                                 usmDHUserOwnPrivKeyChange_val_ptr_ptr,
                                 usmDHUserOwnPrivKeyChange_val_ptr_len_ptr);
}                               /* usmDHUserOwnPrivKeyChange_get */



/** @} */
