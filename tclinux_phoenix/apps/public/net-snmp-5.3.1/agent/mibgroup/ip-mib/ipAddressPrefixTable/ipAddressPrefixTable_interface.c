/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.67 $ of : mfd-interface.m2c,v $ 
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/ip-mib/ipAddressPrefixTable/ipAddressPrefixTable_interface.c#1 $
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */

/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header 
 */
#include "ipAddressPrefixTable.h"


#include <net-snmp/agent/table_container.h>
#include <net-snmp/library/container.h>

#include "ipAddressPrefixTable_interface.h"

#include <ctype.h>

/**********************************************************************
 **********************************************************************
 ***
 *** Table ipAddressPrefixTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * IP-MIB::ipAddressPrefixTable is subid 32 of ip.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.4.32, length: 8
 */
typedef struct ipAddressPrefixTable_interface_ctx_s {

    netsnmp_container *container;
    netsnmp_cache  *cache;

    ipAddressPrefixTable_registration *user_ctx;

    netsnmp_table_registration_info tbl_info;

    netsnmp_baby_steps_access_methods access_multiplexer;

} ipAddressPrefixTable_interface_ctx;

static ipAddressPrefixTable_interface_ctx ipAddressPrefixTable_if_ctx;

static void
                _ipAddressPrefixTable_container_init(ipAddressPrefixTable_interface_ctx *
                                                     if_ctx);
static void
                _ipAddressPrefixTable_container_shutdown(ipAddressPrefixTable_interface_ctx
                                                         * if_ctx);


netsnmp_container *
ipAddressPrefixTable_container_get(void)
{
    return ipAddressPrefixTable_if_ctx.container;
}

ipAddressPrefixTable_registration *
ipAddressPrefixTable_registration_get(void)
{
    return ipAddressPrefixTable_if_ctx.user_ctx;
}

ipAddressPrefixTable_registration *
ipAddressPrefixTable_registration_set(ipAddressPrefixTable_registration *
                                      newreg)
{
    ipAddressPrefixTable_registration *old =
        ipAddressPrefixTable_if_ctx.user_ctx;
    ipAddressPrefixTable_if_ctx.user_ctx = newreg;
    return old;
}

int
ipAddressPrefixTable_container_size(void)
{
    return CONTAINER_SIZE(ipAddressPrefixTable_if_ctx.container);
}

/*
 * mfd multiplexer modes
 */
static Netsnmp_Node_Handler _mfd_ipAddressPrefixTable_pre_request;
static Netsnmp_Node_Handler _mfd_ipAddressPrefixTable_post_request;
static Netsnmp_Node_Handler _mfd_ipAddressPrefixTable_object_lookup;
static Netsnmp_Node_Handler _mfd_ipAddressPrefixTable_get_values;
/**
 * @internal
 * Initialize the table ipAddressPrefixTable 
 *    (Define its contents and how it's structured)
 */
void
    _ipAddressPrefixTable_initialize_interface
    (ipAddressPrefixTable_registration * reg_ptr, u_long flags)
{
    netsnmp_baby_steps_access_methods *access_multiplexer =
        &ipAddressPrefixTable_if_ctx.access_multiplexer;
    netsnmp_table_registration_info *tbl_info =
        &ipAddressPrefixTable_if_ctx.tbl_info;
    netsnmp_handler_registration *reginfo;
    netsnmp_mib_handler *handler;
    int             mfd_modes = 0;

    DEBUGMSGTL(("internal:ipAddressPrefixTable:_ipAddressPrefixTable_initialize_interface", "called\n"));


    /*************************************************
     *
     * save interface context for ipAddressPrefixTable
     */
    /*
     * Setting up the table's definition
     */
    netsnmp_table_helper_add_indexes(tbl_info, ASN_INTEGER,
                                               /** index: ipAddressPrefixIfIndex */
                                     ASN_INTEGER,
                                               /** index: ipAddressPrefixType */
                                     ASN_OCTET_STR,
                                                 /** index: ipAddressPrefixPrefix */
                                     ASN_UNSIGNED,
                                                /** index: ipAddressPrefixLength */
                                     0);

    /*
     * Define the minimum and maximum accessible columns.  This
     * optimizes retrival. 
     */
    tbl_info->min_column = IPADDRESSPREFIXTABLE_MIN_COL;
    tbl_info->max_column = IPADDRESSPREFIXTABLE_MAX_COL;

    /*
     * save users context
     */
    ipAddressPrefixTable_if_ctx.user_ctx = reg_ptr;

    /*
     * call data access initialization code
     */
    ipAddressPrefixTable_init_data(reg_ptr);

    /*
     * set up the container
     */
    _ipAddressPrefixTable_container_init(&ipAddressPrefixTable_if_ctx);
    if (NULL == ipAddressPrefixTable_if_ctx.container) {
        snmp_log(LOG_ERR,
                 "could not initialize container for ipAddressPrefixTable\n");
        return;
    }

    /*
     * access_multiplexer: REQUIRED wrapper for get request handling
     */
    access_multiplexer->object_lookup =
        _mfd_ipAddressPrefixTable_object_lookup;
    access_multiplexer->get_values = _mfd_ipAddressPrefixTable_get_values;

    /*
     * no wrappers yet
     */
    access_multiplexer->pre_request =
        _mfd_ipAddressPrefixTable_pre_request;
    access_multiplexer->post_request =
        _mfd_ipAddressPrefixTable_post_request;


    /*************************************************
     *
     * Create a registration, save our reg data, register table.
     */
    DEBUGMSGTL(("ipAddressPrefixTable:init_ipAddressPrefixTable",
                "Registering ipAddressPrefixTable as a mibs-for-dummies table.\n"));
    handler =
        netsnmp_baby_steps_access_multiplexer_get(access_multiplexer);
    reginfo =
        netsnmp_handler_registration_create("ipAddressPrefixTable",
                                            handler,
                                            ipAddressPrefixTable_oid,
                                            ipAddressPrefixTable_oid_size,
                                            HANDLER_CAN_BABY_STEP |
                                            HANDLER_CAN_RONLY);
    if (NULL == reginfo) {
        snmp_log(LOG_ERR,
                 "error registering table ipAddressPrefixTable\n");
        return;
    }
    reginfo->my_reg_void = &ipAddressPrefixTable_if_ctx;

    /*************************************************
     *
     * set up baby steps handler, create it and inject it
     */
    if (access_multiplexer->object_lookup)
        mfd_modes |= BABY_STEP_OBJECT_LOOKUP;
    if (access_multiplexer->set_values)
        mfd_modes |= BABY_STEP_SET_VALUES;
    if (access_multiplexer->irreversible_commit)
        mfd_modes |= BABY_STEP_IRREVERSIBLE_COMMIT;
    if (access_multiplexer->object_syntax_checks)
        mfd_modes |= BABY_STEP_CHECK_OBJECT;

    if (access_multiplexer->pre_request)
        mfd_modes |= BABY_STEP_PRE_REQUEST;
    if (access_multiplexer->post_request)
        mfd_modes |= BABY_STEP_POST_REQUEST;

    if (access_multiplexer->undo_setup)
        mfd_modes |= BABY_STEP_UNDO_SETUP;
    if (access_multiplexer->undo_cleanup)
        mfd_modes |= BABY_STEP_UNDO_CLEANUP;
    if (access_multiplexer->undo_sets)
        mfd_modes |= BABY_STEP_UNDO_SETS;

    if (access_multiplexer->row_creation)
        mfd_modes |= BABY_STEP_ROW_CREATE;
    if (access_multiplexer->consistency_checks)
        mfd_modes |= BABY_STEP_CHECK_CONSISTENCY;
    if (access_multiplexer->commit)
        mfd_modes |= BABY_STEP_COMMIT;
    if (access_multiplexer->undo_commit)
        mfd_modes |= BABY_STEP_UNDO_COMMIT;

    handler = netsnmp_baby_steps_handler_get(mfd_modes);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject row_merge helper with prefix rootoid_len + 2 (entry.col)
     */
    handler = netsnmp_get_row_merge_handler(reginfo->rootoid_len + 2);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject container_table helper
     */
    handler =
        netsnmp_container_table_handler_get(tbl_info,
                                            ipAddressPrefixTable_if_ctx.
                                            container,
                                            TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject cache helper
     */
    if (NULL != ipAddressPrefixTable_if_ctx.cache) {
        handler =
            netsnmp_cache_handler_get(ipAddressPrefixTable_if_ctx.cache);
        netsnmp_inject_handler(reginfo, handler);
    }

    /*
     * register table
     */
    netsnmp_register_table(reginfo, tbl_info);

}                               /* _ipAddressPrefixTable_initialize_interface */

/**
 * @internal
 * Shutdown the table ipAddressPrefixTable
 */
void
_ipAddressPrefixTable_shutdown_interface(ipAddressPrefixTable_registration
                                         * reg_ptr)
{
    /*
     * shutdown the container
     */
    _ipAddressPrefixTable_container_shutdown(&ipAddressPrefixTable_if_ctx);
}

void
ipAddressPrefixTable_valid_columns_set(netsnmp_column_info *vc)
{
    ipAddressPrefixTable_if_ctx.tbl_info.valid_columns = vc;
}                               /* ipAddressPrefixTable_valid_columns_set */

/**
 * @internal
 * convert the index component stored in the context to an oid
 */
int
ipAddressPrefixTable_index_to_oid(netsnmp_index * oid_idx,
                                  ipAddressPrefixTable_mib_index * mib_idx)
{
    int             err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /*
     * ipAddressPrefixIfIndex(1)/InterfaceIndex/ASN_INTEGER/long(long)//l/a/w/e/R/d/H
     */
    netsnmp_variable_list var_ipAddressPrefixIfIndex;
    /*
     * ipAddressPrefixType(2)/InetAddressType/ASN_INTEGER/long(u_long)//l/a/w/E/r/d/h
     */
    netsnmp_variable_list var_ipAddressPrefixType;
    /*
     * ipAddressPrefixPrefix(3)/InetAddress/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h
     */
    netsnmp_variable_list var_ipAddressPrefixPrefix;
    /*
     * ipAddressPrefixLength(4)/InetAddressPrefixLength/ASN_UNSIGNED/u_long(u_long)//l/a/w/e/R/d/H
     */
    netsnmp_variable_list var_ipAddressPrefixLength;

    /*
     * set up varbinds
     */
    memset(&var_ipAddressPrefixIfIndex, 0x00,
           sizeof(var_ipAddressPrefixIfIndex));
    var_ipAddressPrefixIfIndex.type = ASN_INTEGER;
    memset(&var_ipAddressPrefixType, 0x00,
           sizeof(var_ipAddressPrefixType));
    var_ipAddressPrefixType.type = ASN_INTEGER;
    memset(&var_ipAddressPrefixPrefix, 0x00,
           sizeof(var_ipAddressPrefixPrefix));
    var_ipAddressPrefixPrefix.type = ASN_OCTET_STR;
    memset(&var_ipAddressPrefixLength, 0x00,
           sizeof(var_ipAddressPrefixLength));
    var_ipAddressPrefixLength.type = ASN_UNSIGNED;

    /*
     * chain temp index varbinds together
     */
    var_ipAddressPrefixIfIndex.next_variable = &var_ipAddressPrefixType;
    var_ipAddressPrefixType.next_variable = &var_ipAddressPrefixPrefix;
    var_ipAddressPrefixPrefix.next_variable = &var_ipAddressPrefixLength;
    var_ipAddressPrefixLength.next_variable = NULL;


    DEBUGMSGTL(("verbose:ipAddressPrefixTable:ipAddressPrefixTable_index_to_oid", "called\n"));

    /*
     * ipAddressPrefixIfIndex(1)/InterfaceIndex/ASN_INTEGER/long(long)//l/a/w/e/R/d/H 
     */
    snmp_set_var_value(&var_ipAddressPrefixIfIndex,
                       (u_char *) & mib_idx->ipAddressPrefixIfIndex,
                       sizeof(mib_idx->ipAddressPrefixIfIndex));

    /*
     * ipAddressPrefixType(2)/InetAddressType/ASN_INTEGER/long(u_long)//l/a/w/E/r/d/h 
     */
    snmp_set_var_value(&var_ipAddressPrefixType,
                       (u_char *) & mib_idx->ipAddressPrefixType,
                       sizeof(mib_idx->ipAddressPrefixType));

    /*
     * ipAddressPrefixPrefix(3)/InetAddress/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h 
     */
    snmp_set_var_value(&var_ipAddressPrefixPrefix,
                       (u_char *) & mib_idx->ipAddressPrefixPrefix,
                       mib_idx->ipAddressPrefixPrefix_len *
                       sizeof(mib_idx->ipAddressPrefixPrefix[0]));

    /*
     * ipAddressPrefixLength(4)/InetAddressPrefixLength/ASN_UNSIGNED/u_long(u_long)//l/a/w/e/R/d/H 
     */
    snmp_set_var_value(&var_ipAddressPrefixLength,
                       (u_char *) & mib_idx->ipAddressPrefixLength,
                       sizeof(mib_idx->ipAddressPrefixLength));


    err = build_oid_noalloc(oid_idx->oids, oid_idx->len, &oid_idx->len,
                            NULL, 0, &var_ipAddressPrefixIfIndex);
    if (err)
        snmp_log(LOG_ERR, "error %d converting index to oid\n", err);

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers(&var_ipAddressPrefixIfIndex);

    return err;
}                               /* ipAddressPrefixTable_index_to_oid */

/**
 * extract ipAddressPrefixTable indexes from a netsnmp_index
 *
 * @retval SNMP_ERR_NOERROR  : no error
 * @retval SNMP_ERR_GENERR   : error
 */
int
ipAddressPrefixTable_index_from_oid(netsnmp_index * oid_idx,
                                    ipAddressPrefixTable_mib_index *
                                    mib_idx)
{
    int             err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /*
     * ipAddressPrefixIfIndex(1)/InterfaceIndex/ASN_INTEGER/long(long)//l/a/w/e/R/d/H
     */
    netsnmp_variable_list var_ipAddressPrefixIfIndex;
    /*
     * ipAddressPrefixType(2)/InetAddressType/ASN_INTEGER/long(u_long)//l/a/w/E/r/d/h
     */
    netsnmp_variable_list var_ipAddressPrefixType;
    /*
     * ipAddressPrefixPrefix(3)/InetAddress/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h
     */
    netsnmp_variable_list var_ipAddressPrefixPrefix;
    /*
     * ipAddressPrefixLength(4)/InetAddressPrefixLength/ASN_UNSIGNED/u_long(u_long)//l/a/w/e/R/d/H
     */
    netsnmp_variable_list var_ipAddressPrefixLength;

    /*
     * set up varbinds
     */
    memset(&var_ipAddressPrefixIfIndex, 0x00,
           sizeof(var_ipAddressPrefixIfIndex));
    var_ipAddressPrefixIfIndex.type = ASN_INTEGER;
    memset(&var_ipAddressPrefixType, 0x00,
           sizeof(var_ipAddressPrefixType));
    var_ipAddressPrefixType.type = ASN_INTEGER;
    memset(&var_ipAddressPrefixPrefix, 0x00,
           sizeof(var_ipAddressPrefixPrefix));
    var_ipAddressPrefixPrefix.type = ASN_OCTET_STR;
    memset(&var_ipAddressPrefixLength, 0x00,
           sizeof(var_ipAddressPrefixLength));
    var_ipAddressPrefixLength.type = ASN_UNSIGNED;

    /*
     * chain temp index varbinds together
     */
    var_ipAddressPrefixIfIndex.next_variable = &var_ipAddressPrefixType;
    var_ipAddressPrefixType.next_variable = &var_ipAddressPrefixPrefix;
    var_ipAddressPrefixPrefix.next_variable = &var_ipAddressPrefixLength;
    var_ipAddressPrefixLength.next_variable = NULL;


    DEBUGMSGTL(("verbose:ipAddressPrefixTable:ipAddressPrefixTable_index_from_oid", "called\n"));

    /*
     * parse the oid into the individual index components
     */
    err = parse_oid_indexes(oid_idx->oids, oid_idx->len,
                            &var_ipAddressPrefixIfIndex);
    if (err == SNMP_ERR_NOERROR) {
        /*
         * copy out values
         */
        mib_idx->ipAddressPrefixIfIndex =
            *((long *) var_ipAddressPrefixIfIndex.val.string);
        mib_idx->ipAddressPrefixType =
            *((u_long *) var_ipAddressPrefixType.val.string);
        /*
         * NOTE: val_len is in bytes, ipAddressPrefixPrefix_len might not be
         */
        if (var_ipAddressPrefixPrefix.val_len >
            sizeof(mib_idx->ipAddressPrefixPrefix))
            err = SNMP_ERR_GENERR;
        else {
            memcpy(mib_idx->ipAddressPrefixPrefix,
                   var_ipAddressPrefixPrefix.val.string,
                   var_ipAddressPrefixPrefix.val_len);
            mib_idx->ipAddressPrefixPrefix_len =
                var_ipAddressPrefixPrefix.val_len /
                sizeof(mib_idx->ipAddressPrefixPrefix[0]);
        }
        mib_idx->ipAddressPrefixLength =
            *((u_long *) var_ipAddressPrefixLength.val.string);


    }

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers(&var_ipAddressPrefixIfIndex);

    return err;
}                               /* ipAddressPrefixTable_index_from_oid */


/*
 *********************************************************************
 * @internal
 * allocate resources for a ipAddressPrefixTable_rowreq_ctx
 */
ipAddressPrefixTable_rowreq_ctx *
ipAddressPrefixTable_allocate_rowreq_ctx(void *user_init_ctx)
{
    ipAddressPrefixTable_rowreq_ctx *rowreq_ctx =
        SNMP_MALLOC_TYPEDEF(ipAddressPrefixTable_rowreq_ctx);

    DEBUGMSGTL(("internal:ipAddressPrefixTable:ipAddressPrefixTable_allocate_rowreq_ctx", "called\n"));

    if (NULL == rowreq_ctx) {
        snmp_log(LOG_ERR, "Couldn't allocate memory for a "
                 "ipAddressPrefixTable_rowreq_ctx.\n");
    }

    rowreq_ctx->oid_idx.oids = rowreq_ctx->oid_tmp;

    rowreq_ctx->ipAddressPrefixTable_data_list = NULL;

    /*
     * if we allocated data, call init routine
     */
    if (!(rowreq_ctx->rowreq_flags & MFD_ROW_DATA_FROM_USER)) {
        if (SNMPERR_SUCCESS !=
            ipAddressPrefixTable_rowreq_ctx_init(rowreq_ctx,
                                                 user_init_ctx)) {
            ipAddressPrefixTable_release_rowreq_ctx(rowreq_ctx);
            rowreq_ctx = NULL;
        }
    }

    return rowreq_ctx;
}                               /* ipAddressPrefixTable_allocate_rowreq_ctx */

/*
 * @internal
 * release resources for a ipAddressPrefixTable_rowreq_ctx
 */
void
ipAddressPrefixTable_release_rowreq_ctx(ipAddressPrefixTable_rowreq_ctx *
                                        rowreq_ctx)
{
    DEBUGMSGTL(("internal:ipAddressPrefixTable:ipAddressPrefixTable_release_rowreq_ctx", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    ipAddressPrefixTable_rowreq_ctx_cleanup(rowreq_ctx);

    /*
     * free index oid pointer
     */
    if (rowreq_ctx->oid_idx.oids != rowreq_ctx->oid_tmp)
        free(rowreq_ctx->oid_idx.oids);

    SNMP_FREE(rowreq_ctx);
}                               /* ipAddressPrefixTable_release_rowreq_ctx */

/**
 * @internal
 * wrapper
 */
static int
_mfd_ipAddressPrefixTable_pre_request(netsnmp_mib_handler *handler, netsnmp_handler_registration
                                      *reginfo, netsnmp_agent_request_info
                                      *agtreq_info,
                                      netsnmp_request_info *requests)
{
    int             rc;

    DEBUGMSGTL(("internal:ipAddressPrefixTable:_mfd_ipAddressPrefixTable_pre_request", "called\n"));

    if (1 != netsnmp_row_merge_status_first(reginfo, agtreq_info)) {
        DEBUGMSGTL(("internal:ipAddressPrefixTable",
                    "skipping additional pre_request\n"));
        return SNMP_ERR_NOERROR;
    }

    rc = ipAddressPrefixTable_pre_request(ipAddressPrefixTable_if_ctx.
                                          user_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("ipAddressPrefixTable", "error %d from "
                    "ipAddressPrefixTable_pre_request\n", rc));
        netsnmp_request_set_error_all(requests, SNMP_VALIDATE_ERR(rc));
    }

    return SNMP_ERR_NOERROR;
}                               /* _mfd_ipAddressPrefixTable_pre_request */

/**
 * @internal
 * wrapper
 */
static int
_mfd_ipAddressPrefixTable_post_request(netsnmp_mib_handler *handler, netsnmp_handler_registration
                                       *reginfo, netsnmp_agent_request_info
                                       *agtreq_info,
                                       netsnmp_request_info *requests)
{
    ipAddressPrefixTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_row_extract(requests);
    int             rc, packet_rc;

    DEBUGMSGTL(("internal:ipAddressPrefixTable:_mfd_ipAddressPrefixTable_post_request", "called\n"));

    /*
     * release row context, if deleted
     */
    if (rowreq_ctx && (rowreq_ctx->rowreq_flags & MFD_ROW_DELETED))
        ipAddressPrefixTable_release_rowreq_ctx(rowreq_ctx);

    /*
     * wait for last call before calling user
     */
    if (1 != netsnmp_row_merge_status_last(reginfo, agtreq_info)) {
        DEBUGMSGTL(("internal:ipAddressPrefixTable",
                    "waiting for last post_request\n"));
        return SNMP_ERR_NOERROR;
    }

    packet_rc = netsnmp_check_all_requests_error(agtreq_info->asp, 0);
    rc = ipAddressPrefixTable_post_request(ipAddressPrefixTable_if_ctx.
                                           user_ctx, packet_rc);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("ipAddressPrefixTable", "error %d from "
                    "ipAddressPrefixTable_post_request\n", rc));
    }

    return SNMP_ERR_NOERROR;
}                               /* _mfd_ipAddressPrefixTable_post_request */

/**
 * @internal
 * wrapper
 */
static int
_mfd_ipAddressPrefixTable_object_lookup(netsnmp_mib_handler *handler, netsnmp_handler_registration
                                        *reginfo, netsnmp_agent_request_info
                                        *agtreq_info,
                                        netsnmp_request_info *requests)
{
    int             rc = SNMP_ERR_NOERROR;
    ipAddressPrefixTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_row_extract(requests);

    DEBUGMSGTL(("internal:ipAddressPrefixTable:_mfd_ipAddressPrefixTable_object_lookup", "called\n"));

    /*
     * get our context from mfd
     * ipAddressPrefixTable_interface_ctx *if_ctx =
     *             (ipAddressPrefixTable_interface_ctx *)reginfo->my_reg_void;
     */

    if (NULL == rowreq_ctx) {
        rc = SNMP_ERR_NOCREATION;
    }

    if (MFD_SUCCESS != rc)
        netsnmp_request_set_error_all(requests, rc);
    else
        ipAddressPrefixTable_row_prep(rowreq_ctx);

    return SNMP_VALIDATE_ERR(rc);
}                               /* _mfd_ipAddressPrefixTable_object_lookup */

/***********************************************************************
 *
 * GET processing
 *
 ***********************************************************************/
/*
 * @internal
 * Retrieve the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_ipAddressPrefixTable_get_column(ipAddressPrefixTable_rowreq_ctx *
                                 rowreq_ctx, netsnmp_variable_list * var,
                                 int column)
{
    int             rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:ipAddressPrefixTable:_mfd_ipAddressPrefixTable_get_column", "called for %d\n", column));


    netsnmp_assert(NULL != rowreq_ctx);

    switch (column) {

        /*
         * ipAddressPrefixOrigin(5)/IpAddressPrefixOriginTC/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h 
         */
    case COLUMN_IPADDRESSPREFIXORIGIN:
        var->val_len = sizeof(u_long);
        var->type = ASN_INTEGER;
        rc = ipAddressPrefixOrigin_get(rowreq_ctx,
                                       (u_long *) var->val.string);
        break;

        /*
         * ipAddressPrefixOnLinkFlag(6)/TruthValue/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h 
         */
    case COLUMN_IPADDRESSPREFIXONLINKFLAG:
        var->val_len = sizeof(u_long);
        var->type = ASN_INTEGER;
        rc = ipAddressPrefixOnLinkFlag_get(rowreq_ctx,
                                           (u_long *) var->val.string);
        break;

        /*
         * ipAddressPrefixAutonomousFlag(7)/TruthValue/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h 
         */
    case COLUMN_IPADDRESSPREFIXAUTONOMOUSFLAG:
        var->val_len = sizeof(u_long);
        var->type = ASN_INTEGER;
        rc = ipAddressPrefixAutonomousFlag_get(rowreq_ctx,
                                               (u_long *) var->val.string);
        break;

        /*
         * ipAddressPrefixAdvPreferredLifetime(8)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IPADDRESSPREFIXADVPREFERREDLIFETIME:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipAddressPrefixAdvPreferredLifetime_get(rowreq_ctx,
                                                     (u_long *) var->val.
                                                     string);
        break;

        /*
         * ipAddressPrefixAdvValidLifetime(9)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IPADDRESSPREFIXADVVALIDLIFETIME:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipAddressPrefixAdvValidLifetime_get(rowreq_ctx,
                                                 (u_long *) var->val.
                                                 string);
        break;

    default:
        snmp_log(LOG_ERR,
                 "unknown column %d in _ipAddressPrefixTable_get_column\n",
                 column);
        break;
    }

    return rc;
}                               /* _ipAddressPrefixTable_get_column */

int
_mfd_ipAddressPrefixTable_get_values(netsnmp_mib_handler *handler,
                                     netsnmp_handler_registration *reginfo,
                                     netsnmp_agent_request_info
                                     *agtreq_info,
                                     netsnmp_request_info *requests)
{
    ipAddressPrefixTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_row_extract(requests);
    netsnmp_table_request_info *tri;
    u_char         *old_string;
    void            (*dataFreeHook) (void *);
    int             rc;

    DEBUGMSGTL(("internal:ipAddressPrefixTable:_mfd_ipAddressPrefixTable_get_values", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    for (; requests; requests = requests->next) {
        /*
         * save old pointer, so we can free it if replaced
         */
        old_string = requests->requestvb->val.string;
        dataFreeHook = requests->requestvb->dataFreeHook;
        if (NULL == requests->requestvb->val.string) {
            requests->requestvb->val.string = requests->requestvb->buf;
            requests->requestvb->val_len =
                sizeof(requests->requestvb->buf);
        } else if (requests->requestvb->buf ==
                   requests->requestvb->val.string) {
            if (requests->requestvb->val_len !=
                sizeof(requests->requestvb->buf))
                requests->requestvb->val_len =
                    sizeof(requests->requestvb->buf);
        }

        /*
         * get column data
         */
        tri = netsnmp_extract_table_info(requests);
        if (NULL == tri)
            continue;

        rc = _ipAddressPrefixTable_get_column(rowreq_ctx,
                                              requests->requestvb,
                                              tri->colnum);
        if (rc) {
            if (MFD_SKIP == rc) {
                requests->requestvb->type = SNMP_NOSUCHINSTANCE;
                rc = SNMP_ERR_NOERROR;
            }
        } else if (NULL == requests->requestvb->val.string) {
            snmp_log(LOG_ERR, "NULL varbind data pointer!\n");
            rc = SNMP_ERR_GENERR;
        }
        if (rc)
            netsnmp_request_set_error(requests, SNMP_VALIDATE_ERR(rc));

        /*
         * if the buffer wasn't used previously for the old data (i.e. it
         * was allcoated memory)  and the get routine replaced the pointer,
         * we need to free the previous pointer.
         */
        if (old_string && (old_string != requests->requestvb->buf) &&
            (requests->requestvb->val.string != old_string)) {
            if (dataFreeHook)
                (*dataFreeHook) (old_string);
            else
                free(old_string);
        }
    }                           /* for results */

    return SNMP_ERR_NOERROR;
}                               /* _mfd_ipAddressPrefixTable_get_values */


/***********************************************************************
 *
 * SET processing
 *
 ***********************************************************************/

/*
 * SET PROCESSING NOT APPLICABLE (per MIB or user setting)
 */
/***********************************************************************
 *
 * DATA ACCESS
 *
 ***********************************************************************/
static void     _container_free(netsnmp_container *container);

/**
 * @internal
 */
static int
_cache_load(netsnmp_cache * cache, void *vmagic)
{
    DEBUGMSGTL(("internal:ipAddressPrefixTable:_cache_load", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache for ipAddressPrefixTable_cache_load\n");
        return -1;
    }

    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));

    /*
     * call user code
     */
    return ipAddressPrefixTable_container_load((netsnmp_container *)
                                               cache->magic);
}                               /* _cache_load */

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache * cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:ipAddressPrefixTable:_cache_free", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache in ipAddressPrefixTable_cache_free\n");
        return;
    }

    container = (netsnmp_container *) cache->magic;

    _container_free(container);
}                               /* _cache_free */

/**
 * @internal
 */
static void
_container_item_free(ipAddressPrefixTable_rowreq_ctx * rowreq_ctx,
                     void *context)
{
    DEBUGMSGTL(("internal:ipAddressPrefixTable:_container_item_free",
                "called\n"));

    if (NULL == rowreq_ctx)
        return;

    ipAddressPrefixTable_release_rowreq_ctx(rowreq_ctx);
}                               /* _container_item_free */

/**
 * @internal
 */
static void
_container_free(netsnmp_container *container)
{
    DEBUGMSGTL(("internal:ipAddressPrefixTable:_container_free",
                "called\n"));

    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "invalid container in ipAddressPrefixTable_container_free\n");
        return;
    }

    /*
     * call user code
     */
    ipAddressPrefixTable_container_free(container);

    /*
     * free all items. inefficient, but easy.
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *) _container_item_free,
                    NULL);
}                               /* _container_free */

/**
 * @internal
 * initialize the container with functions or wrappers
 */
void
_ipAddressPrefixTable_container_init(ipAddressPrefixTable_interface_ctx *
                                     if_ctx)
{
    DEBUGMSGTL(("internal:ipAddressPrefixTable:_ipAddressPrefixTable_container_init", "called\n"));

    /*
     * cache init
     */
    if_ctx->cache = netsnmp_cache_create(30,    /* timeout in seconds */
                                         _cache_load, _cache_free,
                                         ipAddressPrefixTable_oid,
                                         ipAddressPrefixTable_oid_size);

    if (NULL == if_ctx->cache) {
        snmp_log(LOG_ERR,
                 "error creating cache for ipAddressPrefixTable\n");
        return;
    }

    if_ctx->cache->flags = NETSNMP_CACHE_DONT_INVALIDATE_ON_SET;

    ipAddressPrefixTable_container_init(&if_ctx->container, if_ctx->cache);
    if (NULL == if_ctx->container)
        if_ctx->container =
            netsnmp_container_find("ipAddressPrefixTable:table_container");
    if (NULL == if_ctx->container) {
        snmp_log(LOG_ERR, "error creating container in "
                 "ipAddressPrefixTable_container_init\n");
        return;
    }

    if (NULL != if_ctx->cache)
        if_ctx->cache->magic = (void *) if_ctx->container;
}                               /* _ipAddressPrefixTable_container_init */

/**
 * @internal
 * shutdown the container with functions or wrappers
 */
void
_ipAddressPrefixTable_container_shutdown(ipAddressPrefixTable_interface_ctx
                                         * if_ctx)
{
    DEBUGMSGTL(("internal:ipAddressPrefixTable:_ipAddressPrefixTable_container_shutdown", "called\n"));

    ipAddressPrefixTable_container_shutdown(if_ctx->container);

    _container_free(if_ctx->container);

}                               /* _ipAddressPrefixTable_container_shutdown */


ipAddressPrefixTable_rowreq_ctx *
ipAddressPrefixTable_row_find_by_mib_index(ipAddressPrefixTable_mib_index *
                                           mib_idx)
{
    ipAddressPrefixTable_rowreq_ctx *rowreq_ctx;
    oid             oid_tmp[MAX_OID_LEN];
    netsnmp_index   oid_idx;
    int             rc;

    /*
     * set up storage for OID
     */
    oid_idx.oids = oid_tmp;
    oid_idx.len = sizeof(oid_tmp) / sizeof(oid);

    /*
     * convert
     */
    rc = ipAddressPrefixTable_index_to_oid(&oid_idx, mib_idx);
    if (MFD_SUCCESS != rc)
        return NULL;

    rowreq_ctx =
        CONTAINER_FIND(ipAddressPrefixTable_if_ctx.container, &oid_idx);

    return rowreq_ctx;
}
